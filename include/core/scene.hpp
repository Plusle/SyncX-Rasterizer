#pragma once

#include <math/vertex.hpp>
#include <math/triangle.hpp>

#include <vector>
#include <limits>

namespace SyncX {

    struct Scene {
        Scene(size_t w, size_t h) : m_Width(w), m_Height(h) {
            m_Framebuffer = std::vector<Vector3f>(w * h, Vector3f());
            m_Zbuffer = std::vector<float>(w * h, std::numeric_limits<float>::max());
        }

    #ifndef IMG_REVERSE_TOP_BOTTOM
        size_t GetIndex(size_t i, size_t j) { return i * m_Width + j; }
    #else
        size_t GetIndex(size_t i, size_t j) { return (m_Height - 1 - i) * m_Width + j; }
    #endif

        const size_t m_Width, m_Height;
        std::vector<Vertex> m_Vertrices;
        std::vector<Triangle> m_Triangles;
        std::vector<Vector3f> m_Framebuffer;
        std::vector<float> m_Zbuffer;
    };

}   // namespace SyncX