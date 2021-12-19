#pragma once

#include <math/vertex.hpp>
#include <math/linear.hpp>

#include <vector>

// The actual works of rasterization are done in here with the help of some intepolation functions



namespace SyncX {

struct AABB {
	AABB(const Vertex* v0, const Vertex* v1, const Vertex* v2, int32_t width, int32_t height) 
		: m_Width(width), m_Height(height), v({ v0, v1, v2 }) {}

	void Traverse(std::vector<Fragment>& fragments);

private:
	int32_t m_Width, m_Height;
	const Vertex* v[3];
};

void AABB::Traverse(std::vector<Fragment>& fragmens) {
	// In this stage, all vertices have been transformed to screen coordinate.
	// x [0, width), y [0, height), z [-1, 1]
	// First, determining x_min, y_min, x_max, y_max
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

	// This is a preparation for checking insideness
	Vector3f vp[3];
	for (auto i = 0; i < 3; ++i) {
		vp[i].x = v[i]->position.x;
		vp[i].y = v[i]->position.y;
		vp[i].z = 1.0;
	}
	Vector3f f0 = cross(vp[1], vp[0]);
	Vector3f f1 = cross(vp[2], vp[1]);
	Vector3f f2 = cross(vp[0], vp[2]);

	// Then, for each pixel in [x_min, x_max] x [y_min, y_max]
	for (auto x = xmin; x <= xmax; ++x) {
		for (auto y = ymin; y <= ymax; ++y) {
			// determining if this pixel is inside the face.
			Vector3f p(x, y, 1.0);
			if (dot(p, f0) * dot(f0, v[2]->position) > 0
			 && dot(p, f1) * dot(f1, v[0]->position) > 0
			 && dot(p, f2) * dot(f2, v[1]->position) > 0) {
				// If inside, using barycentric coordinate to interpolate the value for this pixel
				// 
				// compute barycentric coordinate
				// auto [alpha, beta, gamma] = ComputeBarycentricCoordinate(x, y, v);
				// float w_reciprocal = 1.0 / (alpha * v0.w + beta * v1.w + gamma * v2.w); 
				//                                   ^             ^              ^
				// recall that w has been setted to 1/w after mvp transform
				// this follows the OpenGL convention.
				
				// Then intepolation
				// Fragment frag;
				// frag.position = (x, y, 0.0);
				// frag.position.z = interpolation<float>(alpha, beta, gamma, z0, z1, z2, w_reciprocal);
				// frag.normal = interpolation<Vector3f>(alpha, beta, gamma, n0, n1, n2, w_reciprocal);
				// frag.uv = interpolation<Vector2f>(alpha, beta, gamma, uv0, uv1, uv2, w_reciprocal);
				// 
				// fragments.push_back(frag);
			}
		}
	}

}


}   // namespace SyncX