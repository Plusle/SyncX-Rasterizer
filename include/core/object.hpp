#pragma once

#include <cstdint>
#include <vector>

//#include <core/texture.hpp>

namespace SyncX {
	struct Primitive {};
	struct Point : Primitive {};
	struct Line : Primitive {};

	struct Triangle : Primitive {
		uint32_t v1, v2, v3;
		bool culling = false;
		//uint32_t vt1, vt2, vt3;
		//uint32_t vn1, vn2, vn3;
		Triangle(uint32_t v1, uint32_t v2, uint32_t v3) : v1(v1), v2(v2), v3(v3) {}
	};

	

	struct Model {
		//std::vector<uint32_t> m_FaceIndices;
		uint32_t m_FaceFrom, m_FaceTo;
		uint32_t m_VertexFrom, m_VertexTo;
		
	};

}	// namespace SyncX