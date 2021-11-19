#pragma once

#include <cstdint>
#include <vector>

namespace SyncX {
	struct Primitive {};
	struct Point : Primitive {};
	struct Line : Primitive {};

	struct Triangle : Primitive {
		uint32_t v1, v2, v3;
		uint32_t vt1, vt2, vt3;
		uint32_t vn1, vn2, vn3;
	};

	

	struct Model {
		std::vector<uint32_t> m_FaceIndices;
	};

}	// namespace SyncX