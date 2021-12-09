#pragma once

#include <math/vertex.hpp>
#include <core/object.hpp>
#include <core/texture.hpp>

#include <vector>
#include <limits>

namespace SyncX {

    class Scene {
    public:
        Scene() {}
        friend class Loader;
        friend class Renderer;

#if 1
        std::vector<RawVertex>& GetVertices() { return m_Vertices; }
        std::vector<Triangle>& GetTriangles() { return m_Faces; }
        std::vector<Texture*>& GetTextures() { return m_Textures; }
#endif

    private:
        std::vector<RawVertex> m_Vertices;
        std::vector<Triangle> m_Faces;
        std::vector<Texture*> m_Textures;
        std::vector<Model> m_Models;
    };

    struct RTScene {};

}   // namespace SyncX