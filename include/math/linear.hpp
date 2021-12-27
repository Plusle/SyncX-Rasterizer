#pragma once

#include <math/matrix.hpp>
#include <math/vertex.hpp>
#include <cmath>

#include <utility>

#define MY_PI 3.1415927

namespace SyncX {

template <typename T>
inline T lerp(const T& v1, const T& v2, float r) {
    return v1 * (1 - r) + v2 * r;
}

template <typename T>
inline T clamp(const T& v, const T& min, const T& max) {
    return v < min ? min : (max < v ? max : v); 
}

template <typename T>
inline Matrix4<T> MakeRotateMatrix(const Vector3<T>& axis, float radians) {
    auto coso = static_cast<float>(std::cos(radians));
    auto sino = static_cast<float>(std::sin(radians));
    auto& L1 = axis.x;
    auto& L2 = axis.y;
    auto& L3 = axis.z;
    Matrix4<T> rotation;
    rotation[0][0] = coso + L1 * L1 * (1 - coso);
    rotation[0][1] = L1 * L2 * (1 - coso) - L3 * sino;
    rotation[0][2] = L1 * L3 * (1 - coso) + L2 * sino;
    rotation[1][0] = L2 * L1 * (1 - coso) + L3 * sino;
    rotation[1][1] = coso + L2 * L2 * (1 - coso);
    rotation[1][2] = L2 * L3 * (1 - coso) - L1 * sino;
    rotation[2][0] = L3 * L1 * (1 - coso) - L2 * sino;
    rotation[2][1] = L3 * L2 * (1 - coso) + L1 * sino;
    rotation[2][2] = coso + L3 * L3 * (1 - coso);
    rotation[3][3] = 1.0;
    return rotation;
}

template <typename T>
inline Matrix4<T> MakeTranslateMatrix(const Vector3<T>& translate) {
    Matrix4<T> translation;
    for (int i = 0; i < 4; ++i)
        translation[i][i] = 1;
    for (int i = 0; i < 3; ++i)
        translation[i][3] = translate[i];
    return translation;
}

inline std::tuple<float, float, float> 
ComputeBarycentricCoordinate(const Vector3f& p, const Vector3f* v) {
    auto x = p.x;
    auto y = p.y;
    float alpha = (x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * y + v[1].x * v[2].y - v[2].x * v[1].y) / (v[0].x * (v[1].y - v[2].y) + (v[2].x - v[1].x) * v[0].y + v[1].x * v[2].y - v[2].x * v[1].y);
    float beta  = (x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * y + v[2].x * v[0].y - v[0].x * v[2].y) / (v[1].x * (v[2].y - v[0].y) + (v[0].x - v[2].x) * v[1].y + v[2].x * v[0].y - v[0].x * v[2].y);
    float gamma = (x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * y + v[0].x * v[1].y - v[1].x * v[0].y) / (v[2].x * (v[0].y - v[1].y) + (v[1].x - v[0].x) * v[2].y + v[0].x * v[1].y - v[1].x * v[0].y);
    return std::make_tuple(alpha, beta, gamma);
}

template <typename T>
inline T BarycentricIntepolation(float alpha, float beta, float gamma, const T& t0, const T& t1, const T& t2, float w_reciprocal) {
    return (alpha * w_reciprocal * t0 
          + beta  * w_reciprocal * t1 
          + gamma * w_reciprocal * t2) * w_reciprocal;
}

}   // namespace SyncX