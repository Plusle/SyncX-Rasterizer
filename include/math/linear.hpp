#pragma once

#include <math/matrix.hpp>
#include <cmath>

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
    auto& l1 = axis.x;
    auto& l2 = axis.y;
    auto& l3 = axis.z;
    Matrix4<T> rotation;
    rotation[0][0] = coso + l1 * l1 * (1 - coso);
    rotation[0][1] = l1 * l2 * (1 - coso) - l3 * sino;
    rotation[0][2] = l1 * l3 * (1 - coso) + l2 * sino;
    rotation[1][0] = l2 * l1 * (1 - coso) + l3 * sino;
    rotation[1][1] = coso + l2 * l2 * (1 - coso);
    rotation[1][2] = l2 * l3 * (1 - coso) - l1 * sino;
    rotation[2][0] = l3 * l1 * (1 - coso) - l2 * sino;
    rotation[2][1] = l3 * l2 * (1 - coso) + l1 * sino;
    rotation[2][2] = coso + l3 * l3 * (1 - coso);
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

template <typename T>
inline Matrix4<T> MakeModelMatrix(const Vector3<T>& axis, float radians, const Vector3<T>& translate) {
    return MakeTranslateMatrix(translate) * MakeRotateMatrix(axis, radians);
}

template <typename T>
inline Matrix4<T> MakeViewMatrix(const Vector3<T>& pos, const Vector3<T>& dir, const Vector3<T>& up) {
    Matrix4<T> view;
    for (int i = 0; i < 4; ++i)
        view[i][i] = 1;
    auto d = dir / dir.Length();
    auto u = up / up.Length();
    auto r = cross(d, u);
    for (int i = 0; i < 3; ++i) {
        view[0][i] = r[i];
        view[1][i] = u[i];
        view[2][i] = d[i];
    }
    return view * MakeTranslateMatrix(-pos);
}

template <typename T>
inline Matrix4<T> MakePrespectiveMatrix(float zFar, float zNear, float aspect, float yfov) {
    Matrix4<T> proj;

    float half_theta = 0.5 * yfov / 180 * MY_PI;
    float h = 2 * zNear * std::tan(half_theta);
    float w = h * aspect;
    float& n = zNear, &f = zFar;

    proj[0][0] = 2 * n / w;
    proj[1][1] = 2 * n / h;
    proj[2][2] = -(f + n) / (f - n);
    proj[2][3] = -2 * n * f / (f - n);
    proj[3][2] = -1;

    return proj;
}
}   // namespace SyncX