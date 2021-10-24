#pragma once

#include <math/vertex.hpp>
#include <math/triangle.hpp>
#include <core/texture.hpp>

#include <vector>
#include <limits>

namespace SyncX {

    class Scene {
    public:
        Scene() {}
        friend class Loader;

#if 1
        std::vector<Vertex>& GetVertrices() { return m_Vertrices; }
        std::vector<Triangle>& GetTriangles() { return m_Triangles; }
        std::vector<Texture*>& GetTextures() { return m_Textures; }
#endif

    private:
        std::vector<Vertex> m_Vertrices;
        std::vector<Triangle> m_Triangles;
        std::vector<Texture*> m_Textures;
    };

    struct RTScene {};

}   // namespace SyncX