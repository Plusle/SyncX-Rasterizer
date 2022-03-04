#include <core/loader.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include <core/benchmark.hpp>

namespace SyncX {
    namespace misc {
        // Note:
        // Several vertices can share same attributes. For example, four vertices of 
        // rectangle should have same normal. For this consideration, indices of attributes 
        // should be temporary stored in structure below whithin IO operation. After IO is over, 
        // loader will traverse the vector of this structure to construct all vertices.
        struct FaceIndices {
            size_t v0, v1, v2, vt0, vt1, vt2, vn0, vn1, vn2;

            // Attribute index starts from 1
            void translate() {
                --v0; --v1; --v2;
                --vt0; --vt1; --vt2;
                --vn0; --vn1; --vn2;
            }
        };
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
        std::vector<misc::FaceIndices> indices;
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

        while (!obj.eof()) {
            std::getline(obj, line_buf);
            if (jump_to_next_line(line_buf)) continue;
            std::istringstream iss(line_buf.c_str());
            
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
                indices.push_back(misc::FaceIndices());
                auto& fi = indices.back();
                iss >> trash >> fi.v0 >> trash >> fi.vt0 >> trash >> fi.vn0
                             >> fi.v1 >> trash >> fi.vt1 >> trash >> fi.vn1
                             >> fi.v2 >> trash >> fi.vt2 >> trash >> fi.vn2;
                // Attribute index starts from 1
                fi.translate();
            }
        }

        verts.reserve(verts.size() + positions.size());
        std::vector<bool> constructed(positions.size(), false);
        for (auto i = 0; i != positions.size(); ++i) {
            verts.emplace_back(RawVertex());
        }

        faces.reserve(faces.size() + indices.size());
        for (auto i = 0; i != indices.size(); ++i) {
            const auto& fi = indices[i];
            if (constructed[fi.v0] == false) {
                verts[fi.v0 + vert_offset].position = positions[fi.v0];
                verts[fi.v0 + vert_offset].normal = normals[fi.vn0];
                verts[fi.v0 + vert_offset].uv = tex_coords[fi.vt0];
                constructed[fi.v0] = true;
            }
            if (constructed[fi.v1] == false) {
                verts[fi.v1 + vert_offset].position = positions[fi.v1];
                verts[fi.v1 + vert_offset].normal = normals[fi.vn1];
                verts[fi.v1 + vert_offset].uv = tex_coords[fi.vt1];
                constructed[fi.v1] = true;
            }
            if (constructed[fi.v2] == false) {
                verts[fi.v2 + vert_offset].position = positions[fi.v2];
                verts[fi.v2 + vert_offset].normal = normals[fi.vn2];
                verts[fi.v2 + vert_offset].uv = tex_coords[fi.vt2];
                constructed[fi.v2] = true;
            }      
            faces.emplace_back(Triangle(fi.v0 + vert_offset, fi.v1 + vert_offset, fi.v2 + vert_offset));
        }

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
        std::cout << m_Scene->m_Models.size() << " models within the Scene" << std::endl;
        std::cout << "Current model has " << positions.size() << " vertrices, " << model.m_FaceTo - model.m_FaceFrom << " faces.\n"
                  << "Scene has " << verts.size() << " vertrices, " << faces.size() << " faces.\n";
        std::cout << "Vertex from " << model.m_VertexFrom << " to " << model.m_VertexTo << "\n";
        std::cout << "Vertex offset = " << vert_offset << "\n";
        //for (auto i = face_offset; i < faces.size(); ++i) {
        //    if (faces[i].v1 < vert_offset || faces[i].v2 < vert_offset || faces[i].v3 < vert_offset) {
        //        std::cout << "Illegal exist" << std::endl;
        //    }
        //}

        float xmin = std::numeric_limits<float>::max(), ymin = std::numeric_limits<float>::max();
        float xmax = 0, ymax = 0;
        for (const auto& uv : tex_coords) {
            if (uv.x < xmin) xmin = uv.x;
            if (uv.x > xmax) xmax = uv.x;
            if (uv.y < ymin) ymin = uv.y;
            if (uv.y > ymax) ymax = uv.y;
        }

        std::cout << "UV Range: (" << xmin << ", " << ymin << ")     (" << xmax << ", " << ymax << ")" << std::endl;


        std::cout << "---------------------------------------------------------------------------------\n";
    }





}   // namespace SyncX