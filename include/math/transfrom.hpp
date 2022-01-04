#pragma once

#include <math/linear.hpp>
#include <iostream>

namespace SyncX {

struct Transform {
    Transform() : model(Matrix4f::Identity()), view(Matrix4f::Identity()), projection(Matrix4f::Identity()) {}


    void SetModelMatrix(const Vector3f& axis, float radians, const Vector3f& translate) {
        model = MakeRotateMatrix(axis, radians) * MakeTranslateMatrix(translate);
    }

    void SetViewMatrix(const Vector3f& pos, const Vector3f& dir, const Vector3f& up) {
        view = Matrix4f::Identity();
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
        view = view * MakeTranslateMatrix(-pos);
    }

    void SetPrespectiveMatrix(float zNear, float zFar, float aspect, float yfov) {
        projection = Matrix4f::Identity();

        float half_theta = 0.5 * yfov / 180 * MY_PI;
        float h = 2 * zNear * std::tan(half_theta);
        float w = h * aspect;
        float& n = zNear, & f = zFar;

        projection[0][0] = 2 * n / w;
        projection[1][1] = 2 * n / h;
        projection[2][2] = -(f + n) / (f - n);
        projection[2][3] = -2 * n * f / (f - n);
        projection[3][2] = -1;
    }

	Vector4f operator*(const Vector4f& v) const {
		return projection * view * model * v;
	}

	Matrix4f model, view, projection;
private:
    constexpr static double epsilon = 1e-5;
};

}
