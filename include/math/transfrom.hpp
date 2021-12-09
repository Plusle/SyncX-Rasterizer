#pragma once

#include <math/vector.hpp>
#include <math/matrix.hpp>

namespace SyncX {

struct Transform {
	Matrix4f model, view, project;

	Vector4f operator*(const Vector4f& v) const {
		return project * view * model * v;
	}
};

}
