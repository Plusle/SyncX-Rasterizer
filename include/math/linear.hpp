#pragma once

#include <math/matrix.hpp>

namespace SyncX {
    struct Transform {

        Matrix4f m_Model, m_View, m_Projection;
    };


    template <typename T>
    inline T lerp(const T& v1, const T& v2, float r) {
        return v1 * (1 - r) + v2 * r;
    }
}   // namespace SyncX