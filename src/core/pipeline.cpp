#include <core/pipeline.hpp>
#include <math/aabb.hpp>
#include <algorithm>

namespace SyncX {

Pipeline::Pipeline(Scene* sc, std::vector<Vector4f>* framebuffer, std::vector<float>* zbuffer, uint32_t width, uint32_t height)
	: m_Scene(sc),  m_Object(nullptr), m_Framebuffer(framebuffer), m_ZBuffer(zbuffer) {}


void Pipeline::Initialize(Model* model) {
	m_Object = model;
}

void Pipeline::VertexProcess(const Transform& t) {
	auto& vert_from = m_Object->m_VertexFrom;
	auto& vert_to   = m_Object->m_VertexTo;
	auto vertices = m_Scene->GetVertices().cbegin();

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
	std::transform(vertices + vert_from, vertices + vert_to, std::back_inserter(m_VertexStream), MVPTrans);

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
	std::transform(faces + face_from, faces + face_to, std::back_inserter(m_FaceStream), remove_offset);
}

void Pipeline::ClippingCulling() {
	// Culling
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
		vertex.position.x *= width;
		vertex.position.y *= height;
	}
}

void Pipeline::Rasterization() {
	for (const auto& face : m_FaceStream) {
		AABB box(&m_VertexStream[face.v1], &m_VertexStream[face.v2], &m_VertexStream[face.v3], m_Width, m_Height);
		box.Traverse(m_Fragments);
	}
}

// Find that this need to be implemented inside rasterization
//void Pipeline::MXAASetting() {
//
//}

void Pipeline::PreDepthTest() {

}

void Pipeline::FragmentShading() {

}

void Pipeline::Blending() {

}

}   // namespace SyncX