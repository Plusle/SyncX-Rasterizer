#include <core/loader.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace SyncX {
    Loader::Loader(Scene* sc) : m_Scene(sc) {}

    void Loader::Execute(const char* filename) const {
        int32_t length = std::strlen(filename);
        if (length <= 4 || std::strcmp(&filename[length - 4], ".obj") != 0) {
            std::cerr << "Model file has an illegal file name" << std::endl;
            std::exit(1);
        }

        std::ifstream model(filename, std::ios::in);
        if (!model.is_open()) {
            std::cerr << "Unable to open model file \"" << filename << "\"." << std::endl;
            std::exit(1);
        }

        auto& verts = m_Scene->m_Vertrices;
        auto& faces = m_Scene->m_Triangles;
        std::vector<Vector3f> tmp_pos;
        std::vector<Vector3f> tmp_norm;
        std::vector<Vector2f> tmp_uv;
        std::string line_buf;
        char trash;


        int count = 0;

        auto jump_to_next_line = [](std::string& line) -> bool {
            if (!line.compare(0, 1, "#") || !line.compare(0, 6, "mtllib") || line.empty())
                return true;
            return false;
        };

        while (!model.eof()) {
            std::getline(model, line_buf);
            std::istringstream iss(line_buf.c_str());
            if (jump_to_next_line(line_buf)) continue;
            
            if (!line_buf.compare(0, 2, "v ")) {
                float f1, f2, f3;
                iss >> trash >> f1 >> f2 >> f3;
                tmp_pos.emplace_back(f1, f2, f3);
            } else if (!line_buf.compare(0, 3, "vn ")) {
                float f1, f2, f3;
                iss >> trash >> trash >> f1 >> f2 >> f3;
                tmp_norm.emplace_back(f1, f2, f3);
            } else if (!line_buf.compare(0, 3, "vt ")) {
                float f1, f2;
                iss >> trash >> trash >> f1 >> f2;
                tmp_uv.emplace_back(f1, f2);
            } else if (!line_buf.compare(0, 2, "f ")) {
                std::string v1, v2, v3;
                iss >> trash >> v1 >> v2 >> v3;
            }

            //if (count++ > 5) break;
        }

        assert(tmp_pos.size() == tmp_norm.size() && tmp_pos.size() == tmp_uv.size());

        for (auto i = 0; i < tmp_pos.size(); ++i) {
            Vertex v;
            v.position = tmp_pos[i];
            v.normal = tmp_norm[i];
            v.uv = tmp_uv[i];
            verts.push_back(v);
        }

        assert(tmp_pos.size() == verts.size());

#if 0
        for (const auto& v : m_Scene->m_Vertrices) {
            std::cout << v.position;
        }
        std::cout << std::flush;
#endif
        std::cout << "\nDone.\n";

    }



}   // namespace SyncX