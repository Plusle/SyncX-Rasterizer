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

    template <typename T>
    inline T clamp(const T& v, const T& min, const T& max) {
        return v < min ? min : (max < v ? max : v); 
    }
}   // namespace SyncX