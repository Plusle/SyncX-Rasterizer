#include <core/loader.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <core/benchmark.hpp>

namespace SyncX {
    static void GetFaceVertrices(const char* line, Triangle& t) {
        
    }

    Loader::Loader(Scene* sc) : m_Scene(sc) {}

    void Loader::Execute(const char* filename) const {
        int32_t length = std::strlen(filename);
        if (length <= 4 || std::strcmp(&filename[length - 4], ".obj") != 0) {
            std::cerr << "Model file has an illegal file name" << std::endl;
            std::exit(1);
        }

        std::ifstream obj(filename, std::ios::in);
        if (!obj.is_open()) {
            std::cerr << "Unable to open model file \"" << filename << "\"." << std::endl;
            std::exit(1);
        }

        auto& verts = m_Scene->m_Vertices;
        auto& faces = m_Scene->m_Faces;
        auto vert_offset = verts.size();
        auto face_offset = faces.size();
        std::vector<Vector3f> positions;
        std::vector<Vector3f> normals;
        std::vector<Vector2f> tex_coords;
        std::string line_buf;
        char trash;

        auto jump_to_next_line = [](std::string& line) -> bool {
            if (!line.compare(0, 1, "#") || !line.compare(0, 6, "mtllib") || line.empty())
                return true;
            return false;
        };

        std::cout << "Begin to load obj file: " << filename << ". There are " << vert_offset << " vertices in the buffer" << std::endl;
        Timer timer;
        timer.start();

        uint32_t face_count = 0;
        while (!obj.eof()) {
            std::getline(obj, line_buf);
            std::istringstream iss(line_buf.c_str());
            if (jump_to_next_line(line_buf)) continue;
            
            if (!line_buf.compare(0, 2, "v ")) {
                float f1, f2, f3;
                iss >> trash >> f1 >> f2 >> f3;
                positions.emplace_back(f1, f2, f3);
            } else if (!line_buf.compare(0, 3, "vn ")) {
                float f1, f2, f3;
                iss >> trash >> trash >> f1 >> f2 >> f3;
                normals.emplace_back(f1, f2, f3);
            } else if (!line_buf.compare(0, 3, "vt ")) {
                float f1, f2;
                iss >> trash >> trash >> f1 >> f2;
                tex_coords.emplace_back(f1, f2);
            } else if (!line_buf.compare(0, 2, "f ")) {
                float v1, vt1, vn1,
                      v2, vt2, vn2,
                      v3, vt3, vn3;
                iss >> trash >> v1 >> trash >> vt1 >> trash >> vn1
                             >> v2 >> trash >> vt2 >> trash >> vn2
                             >> v3 >> trash >> vt3 >> trash >> vn3;
                Triangle tri;
                tri.v1 = v1 + vert_offset;
                tri.v2 = v2 + vert_offset;
                tri.v3 = v3 + vert_offset;
                //tri.t = ?
                
                faces.push_back(tri);
                ++face_count;
                //GetFaceVertrices(line_buf.c_str(), tri);
            }

            //if (count++ > 5) break;
        }

        assert(positions.size() == normals.size() && positions.size() == tex_coords.size());

        for (auto i = 0; i < positions.size(); ++i) {
            RawVertex v;
            v.position = positions[i];
            v.normal = normals[i];
            v.uv = tex_coords[i];
            verts.push_back(v);
        }
        assert(positions.size() == verts.size() - vert_offset);

#if 0
        for (const auto& v : m_Scene->m_Vertrices) {
            std::cout << v.position;
        }
        std::cout << std::flush;
#endif

        Model model;
        model.m_FaceFrom = face_offset;
        model.m_FaceTo = faces.size();
        model.m_VertexFrom = vert_offset;
        model.m_VertexTo = verts.size();
        m_Scene->m_Models.push_back(model);

        timer.end();

        std::cout << "Done.\n";
        std::cout << "Used " << timer.elapsed() << " ms to load " << filename << std::endl;
        std::cout << "This model has " << positions.size() << " vertrices, " << face_count << " faces.\n"
                  << "This Scene has " << verts.size() << " vertrices, " << faces.size() << " faces.\n";
        std::cout << "---------------------------------------------------------------------------------\n";
    }





}   // namespace SyncX