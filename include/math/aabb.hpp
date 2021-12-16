#pragma once

#include <math/vertex.hpp>

namespace SyncX {

struct AABB {
	AABB(const Vertex* v0, const Vertex* v1, const Vertex* v2, int32_t width, int32_t height) 
		: m_Width(width), m_Height(height), v({ v0, v1, v2 }) {}

	void Traverse(std::vector<Fragment>)

	int32_t m_Width, m_Height;
	const Vertex* v[3];
};

}   // namespace SyncX