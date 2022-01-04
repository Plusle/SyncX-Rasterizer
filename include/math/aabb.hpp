#pragma once

#include <math/vertex.hpp>
#include <math/linear.hpp>

#include <vector>


#if 1
#include <iostream>
#define DEBUG_INFO
#endif
// The actual works of rasterization are done in here with the help of some intepolation functions



namespace SyncX {

struct AABB {
	AABB(const Vertex* v0, const Vertex* v1, const Vertex* v2, int32_t width, int32_t height) 
		: m_Width(width), m_Height(height) {
		v[0] = v0; v[1] = v1; v[2] = v2;
#ifdef DEBUG_INFO
		std::cout << "New aabb" << std::endl;
#endif
	}

	void Traverse(std::vector<Fragment>& fragments);

private:
	int32_t m_Width, m_Height;
	const Vertex* v[3];
};

void AABB::Traverse(std::vector<Fragment>& fragments) {
	// In this stage, all vertices have been transformed to screen coordinate.
	// x [0, width), y [0, height), z [-1, 1]
	// First, determining x_min, y_min, x_max, y_max
	constexpr float kPlaceholder = 1.0f;
	int32_t xmin = std::numeric_limits<int32_t>::max();
	int32_t ymin = std::numeric_limits<int32_t>::max();
	int32_t xmax = 0;
	int32_t ymax = 0;
	for (const auto* vertex : v) {
		if (vertex->position.x < xmin) xmin = vertex->position.x;
		if (vertex->position.x > xmax) xmax = vertex->position.x;
		if (vertex->position.y < ymin) ymin = vertex->position.y;
		if (vertex->position.y > ymax) ymax = vertex->position.y;
	}
	if (xmax >= m_Width) xmax = m_Width - 1;
	if (ymax >= m_Height) ymax = m_Height - 1;

	// This is a preparation for checking insideness
	Vector3f vp[3];
	for (auto i = 0; i < 3; ++i) {
		vp[i].x = v[i]->position.x;
		vp[i].y = v[i]->position.y;
		vp[i].z = kPlaceholder;
	}
	Vector3f f0 = cross(vp[1], vp[0]);
	Vector3f f1 = cross(vp[2], vp[1]);
	Vector3f f2 = cross(vp[0], vp[2]);

	// Then, for each pixel in [x_min, x_max] x [y_min, y_max]
	for (auto x = xmin; x <= xmax; ++x) {
		for (auto y = ymin; y <= ymax; ++y) {
			// determining if this pixel is inside the face.
			Vector3f p(x + 0.5, y + 0.5, kPlaceholder);
			if (dot(p, f0) * dot(f0, v[2]->position.toVec3()) > 0
			 && dot(p, f1) * dot(f1, v[0]->position.toVec3()) > 0
			 && dot(p, f2) * dot(f2, v[1]->position.toVec3()) > 0) {
				// If inside, using barycentric coordinate to interpolate the value for this pixel
				// 
				// compute barycentric coordinate
				auto [alpha, beta, gamma] = ComputeBarycentricCoordinate(p, vp);
				float w_reciprocal = 1.0 / (alpha * v[0]->position.w + beta * v[1]->position.w + gamma * v[2]->position.w);
				//                                                 ^                         ^                          ^
				// recall that w has been setted to 1/w after mvp transform
				// this follows the OpenGL convention.
				
				// Then intepolation
				Fragment frag;
				frag.position = Vector4f(x + 0.5, y + 0.5, kPlaceholder, kPlaceholder);
				frag.position.z = BarycentricIntepolation<float>(alpha, beta, gamma, 
								    v[0]->position.z, v[1]->position.z, v[2]->position.z, w_reciprocal);
				frag.normal = BarycentricIntepolation<Vector3f>(alpha, beta, gamma, 
									v[0]->normal, v[1]->normal, v[2]->normal, w_reciprocal);
				frag.uv = BarycentricIntepolation<Vector2f>(alpha, beta, gamma, 
									v[0]->uv, v[1]->uv, v[2]->uv, w_reciprocal);
				frag.window_pos = Vector2i(x, y);
				fragments.push_back(frag);
			}
		}
	}

}


}   // namespace SyncX