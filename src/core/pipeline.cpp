#include <core/pipeline.hpp>
#include <math/aabb.hpp>
#include <algorithm>

#if 0
#define DEBUG_INFO
#include <iostream>
#endif

namespace SyncX {

Pipeline::Pipeline(Scene* sc, std::vector<Vector4f>* framebuffer, std::vector<float>* zbuffer, uint32_t width, uint32_t height)
	: m_Scene(sc),  m_Object(nullptr), m_Framebuffer(framebuffer), m_ZBuffer(zbuffer), m_Width(width), m_Height(height) {}


void Pipeline::Initialize(Model* model) {
	m_Object = model;
}

void Pipeline::VertexProcess(const Transform& t) {
	std::cout << "Beginning Vertex Processing" << std::endl;
	auto& vert_from = m_Object->m_VertexFrom;
	auto& vert_to   = m_Object->m_VertexTo;
	auto  vertices  = m_Scene->GetVertices().cbegin();

#ifdef DEBUG_INFO
	std::cout << m_Scene->GetVertices().size() << " vertices" << std::endl;

	for (const auto& v : m_Scene->GetVertices()) {
		std::cout << v.position << std::endl;
	}
#endif

	//--------------------------------------------------------------------------------
	// This part has some bugs, maybe in mvp
	auto MVPTrans = [&t](const RawVertex& v) -> Vertex {
		Vertex re;
		re.position = t * Vector4f(v.position.x, v.position.y, v.position.z, 1.0);
		re.position.w = 1 / re.position.w;
		re.position.x *= re.position.w; 
		re.position.y *= re.position.w;
		re.position.z *= re.position.w;
		//re.normal = t * Vector4f(v.normal.x, v.normal.y, v.normal.z, 0.0);
		//re.uv = v.uv;
		return re;
	};
	//--------------------------------------------------------------------------------
	std::transform(vertices + vert_from, vertices + vert_to, std::back_inserter(m_VertexStream), MVPTrans);

#ifdef DEBUG_INFO
	for (const auto& v : m_VertexStream) {
		std::cout << v.position << std::endl;
	}
#endif

	auto faces = m_Scene->GetTriangles().cbegin();
	auto& face_from = m_Object->m_FaceFrom;
	auto& face_to = m_Object->m_FaceTo;
	auto remove_offset = [&vert_from](const Triangle& t) -> Triangle {
		Triangle re = t;
		re.v1 -= vert_from;
		re.v2 -= vert_from;
		re.v3 -= vert_from;
		return re;
	};
#ifdef DEBUG_INFO
	std::cout << m_Scene->GetTriangles().size() << " faces" << std::endl;
#endif

	std::transform(faces + face_from, faces + face_to, std::back_inserter(m_FaceStream), remove_offset);

#ifdef DEBUG_INFO
	std::cout << "End Vertex Processing" << std::endl;
#endif
}

void Pipeline::ClippingCulling() {
	// Culling
#ifdef DEBUG_INFO
	std::cout << "Begin clipping" << std::endl;
#endif
	for (auto& face : m_FaceStream) {
		const auto& v1 = m_VertexStream[face.v1];
		const auto& v2 = m_VertexStream[face.v2];
		const auto& v3 = m_VertexStream[face.v3];
		
		// discrimination here

		if (/* if counterclockwise, which means discrimnation is */ true) {
			face.culling = true;
		}
	}

	// How to do clipping
}

void Pipeline::Viewport() {
	auto width = m_Width - 1;
	auto height = m_Height - 1;
	for (auto& vertex : m_VertexStream) {
		vertex.position.x = (vertex.position.x + 1.0f) / 2 * width;
		vertex.position.y = (vertex.position.y + 1.0f) / 2 * height;
#ifdef DEBUG_INFO
		std::cout << vertex.position << std::endl;
#endif
	}
}

void Pipeline::Rasterization() {
	for (const auto& face : m_FaceStream) {
		AABB box(&m_VertexStream[face.v1], &m_VertexStream[face.v2], &m_VertexStream[face.v3], m_Width, m_Height);
		box.Traverse(m_Fragments);
	}
}

void Pipeline::FragmentShading() {
	std::cout << "Fragments: " << m_Fragments.size() << std::endl;
	size_t i = 0;
	for (auto& fragment : this->m_Fragments) {
		if (fragment.window_pos.x >= m_Width  ||
			fragment.window_pos.y >= m_Height ||
			fragment.window_pos.x < 0 || fragment.window_pos.y < 0) {
#if 0
			std::cout << frag.window_pos << std::endl;
#endif
			++i; continue;
		}
		fragment.color = Vector4f(1.f, 1.f, 0.f, 1.f);

	}
	std::cout << "Illegal pixel:" << i << std::endl;
}

void Pipeline::DepthTest() {
	for (const auto& fragment : m_Fragments) {
		auto j = fragment.window_pos.x;
		auto i = fragment.window_pos.y;
		if ((*m_ZBuffer)[GetIndex(i, j)] > fragment.position.z) {
			(*m_ZBuffer)[GetIndex(i, j)] = fragment.position.z;
			(*m_Framebuffer)[GetIndex(i, j)] = fragment.color;
		}
	}
}

}   // namespace SyncX