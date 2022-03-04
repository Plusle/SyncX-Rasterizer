#pragma once

#include <math/vertex.hpp>
#include <math/linear.hpp>

#include <vector>


#if 0
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
	void DrawFrame(std::vector<Fragment>& fragments);

private:
	int32_t m_Width, m_Height;
	const Vertex* v[3];

	void DrawLine(const Vertex& v1, const Vertex& v2, std::vector<Fragment>& fragments);
};

//void DrawLine(const Vertex& v1, const Vertex& v2, std::vector<Fragment>& fragments) {
//    auto x1 = v1.
//    auto y1 = begin.y();
//    auto x2 = end.x();
//    auto y2 = end.y();
//
//    int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
//
//    dx = x2 - x1;
//    dy = y2 - y1;
//    dx1 = fabs(dx);
//    dy1 = fabs(dy);
//    px = 2 * dy1 - dx1;
//    py = 2 * dx1 - dy1;
//
//    if (dy1 <= dx1) {
//        if (dx >= 0) {
//            x = x1;
//            y = y1;
//            xe = x2;
//        } else {
//            x = x2;
//            y = y2;
//            xe = x1;
//        }
//        Eigen::Vector2i point = Eigen::Vector2i(x, y);
//        set_pixel(point, line_color);
//        for (i = 0; x < xe; i++) {
//            x = x + 1;
//            if (px < 0) {
//                px = px + 2 * dy1;
//            } else {
//                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
//                    y = y + 1;
//                } else {
//                    y = y - 1;
//                }
//                px = px + 2 * (dy1 - dx1);
//            }
//            //            delay(0);
//            Eigen::Vector2i point = Eigen::Vector2i(x, y);
//            set_pixel(point, line_color);
//        }
//    } else {
//        if (dy >= 0) {
//            x = x1;
//            y = y1;
//            ye = y2;
//        } else {
//            x = x2;
//            y = y2;
//            ye = y1;
//        }
//        Eigen::Vector2i point = Eigen::Vector2i(x, y);
//        set_pixel(point, line_color);
//        for (i = 0; y < ye; i++) {
//            y = y + 1;
//            if (py <= 0) {
//                py = py + 2 * dx1;
//            } else {
//                if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
//                    x = x + 1;
//                } else {
//                    x = x - 1;
//                }
//                py = py + 2 * (dx1 - dy1);
//            }
//            //            delay(0);
//            Eigen::Vector2i point = Eigen::Vector2i(x, y);
//            set_pixel(point, line_color);
//        }
//    }
//}
//
//void AABB::DrawFrame(std::vector<Fragment>& fragments) {
//	DrawLine(*(v[0]), *(v[1]), fragments);
//	DrawLine(*(v[1]), *(v[2]), fragments);
//	DrawLine(*(v[2]), *(v[0]), fragments);
//}

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
		if (vertex->pos_in_viewport.x < xmin) xmin = vertex->pos_in_viewport.x;
		if (vertex->pos_in_viewport.x > xmax) xmax = vertex->pos_in_viewport.x;
		if (vertex->pos_in_viewport.y < ymin) ymin = vertex->pos_in_viewport.y;
		if (vertex->pos_in_viewport.y > ymax) ymax = vertex->pos_in_viewport.y;
	}
	if (xmin < 0) xmin = 0;
	if (ymin < 0) ymin = 0;
	if (xmax >= m_Width) xmax = m_Width - 1;
	if (ymax >= m_Height) ymax = m_Height - 1;

	// This is a preparation for checking insideness
	Vector3f vp[3];
	for (auto i = 0; i < 3; ++i) {
		vp[i].x = v[i]->pos_in_viewport.x;
		vp[i].y = v[i]->pos_in_viewport.y;
		vp[i].z = kPlaceholder;
	}
	Vector3f f0 = cross(vp[1], vp[0]);
	Vector3f f1 = cross(vp[2], vp[1]);
	Vector3f f2 = cross(vp[0], vp[2]);

	Vector3f v0(v[0]->pos_in_viewport.x, v[0]->pos_in_viewport.y, kPlaceholder);
	Vector3f v1(v[1]->pos_in_viewport.x, v[1]->pos_in_viewport.y, kPlaceholder);
	Vector3f v2(v[2]->pos_in_viewport.x, v[2]->pos_in_viewport.y, kPlaceholder);

	// Then, for each pixel in [x_min, x_max] x [y_min, y_max]
	for (auto x = xmin; x <= xmax; ++x) {
		for (auto y = ymin; y <= ymax; ++y) {
			// determining if this pixel is inside the face.
			Vector3f p(x + 0.5, y + 0.5, kPlaceholder);
			if (dot(p, f0) * dot(f0, v2) > 0
			 && dot(p, f1) * dot(f1, v0) > 0
			 && dot(p, f2) * dot(f2, v1) > 0) {
				// If inside, using barycentric coordinate to interpolate the value for this pixel
				// 
				// compute barycentric coordinate
				auto [alpha, beta, gamma] = ComputeBarycentricCoordinate(p, vp);
				float w_reciprocal = 1.0 / (alpha * v[0]->pos_in_ndc.w + beta * v[1]->pos_in_ndc.w + gamma * v[2]->pos_in_ndc.w);
				//                                                 ^                         ^                          ^
				// recall that w has been setted to 1/w after mvp transform
				// this follows the OpenGL convention.
				
				// Intepolation
				float wr0 = v[0]->pos_in_ndc.w;
				float wr1 = v[1]->pos_in_ndc.w;
				float wr2 = v[2]->pos_in_ndc.w;

				Fragment frag;

				frag.pos_in_world = BarycentricIntepolation(alpha, beta, gamma, wr0, wr1, wr2, v[0]->pos_in_world, v[1]->pos_in_world, v[2]->pos_in_world);
				frag.pos_in_ndc = BarycentricIntepolation(alpha, beta, gamma, wr0, wr1, wr2, v[0]->pos_in_ndc, v[1]->pos_in_ndc, v[2]->pos_in_ndc);

				// Since I keep using right hand system, z needs to be inverted to fit "depth value grows along with distance from view point"
				frag.pos_in_ndc.z *= -1;
				frag.normal = BarycentricIntepolation(alpha, beta, gamma, wr0, wr1, wr2, v[0]->normal, v[1]->normal, v[2]->normal);
				frag.uv = BarycentricIntepolation(alpha, beta, gamma, wr0, wr1, wr2, v[0]->uv, v[1]->uv, v[2]->uv);	

				// I hate various conventions, I love standard. Maybe make a switch later.
				frag.uv.y = 1 - frag.uv.y;

				frag.window_pos = Vector2i(x, y);
				fragments.push_back(frag);
			}
		}
	}

}


}   // namespace SyncX