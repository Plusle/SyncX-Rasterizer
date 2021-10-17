#pragma once

#include <math/vertex.hpp>
#include <math/triangle.hpp>

#include <vector>
#include <limits>

namespace SyncX {

    struct Scene {
        Scene() {}

        std::vector<Vertex> m_Vertrices;
        std::vector<Triangle> m_Triangles;
    };

    struct RTScene {};

}   // namespace SyncX