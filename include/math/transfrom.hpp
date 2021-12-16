#pragma once

#include <math/linear.hpp>

namespace SyncX {

struct Transform {
    Transform() : model(Matrix4f::Identity()), view(Matrix4f::Identity()), project(Matrix4f::Identity()) {}


    void SetModelMatrix(const Vector3f& axis, float radians, const Vector3f& translate) {
        auto step = radians > 0 ? MY_PI : -MY_PI;
        while (std::abs(radians) < MY_PI)
            radians -= step;
        if (std::abs(radians) > epsilon)
            model = MakeRotateMatrix(axis, radians) * MakeTranslateMatrix(translate);
        else
            model = MakeTranslateMatrix(translate);
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
        project = Matrix4f::Identity();

        float half_theta = 0.5 * yfov / 180 * MY_PI;
        float h = 2 * zNear * std::tan(half_theta);
        float w = h * aspect;
        float& n = zNear, & f = zFar;

        project[0][0] = 2 * n / w;
        project[1][1] = 2 * n / h;
        project[2][2] = -(f + n) / (f - n);
        project[2][3] = -2 * n * f / (f - n);
        project[3][2] = -1;
    }

	Vector4f operator*(const Vector4f& v) const {
		return project * view * model * v;
	}

	Matrix4f model, view, project;
private:
    constexpr static double epsilon = 1e-5;
};

}
