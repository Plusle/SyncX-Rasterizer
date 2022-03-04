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
	m_VertexStream.clear();
	m_FaceStream.clear();
	std::cout << "VStream: " << m_VertexStream.size() << " FaceStream: " << m_FaceStream.size() << std::endl;
}

void Pipeline::VertexProcess(const Transform& t) {
	std::cout << "Begin Vertex Processing" << std::endl;
	auto& vert_from = m_Object->m_VertexFrom;
	auto& vert_to   = m_Object->m_VertexTo;
	auto  vertices  = m_Scene->GetVertices().cbegin();
	std::cout << "Vertices from " << vert_from << " to " << vert_to << std::endl;

#ifdef DEBUG_INFO
	std::cout << m_Scene->GetVertices().size() << " vertices" << std::endl;

	for (const auto& v : m_Scene->GetVertices()) {
		std::cout << v.position << std::endl;
	}
#endif

	//--------------------------------------------------------------------------------
	// This part has some bugs, maybe in mvp

	// From RawVertex to Vertex, notice the difference between them
	float xmin = std::numeric_limits<float>::max(), ymin = std::numeric_limits<float>::max();
	float xmax = 0, ymax = 0;

	auto MVPTrans = [&t, &xmin, &ymin, &xmax, &ymax](const RawVertex& v) -> Vertex {
		Vertex re;
		re.pos_in_ndc = t * Vector4f(v.position.x, v.position.y, v.position.z, 1.0f);
		re.pos_in_ndc.w = 1 / re.pos_in_ndc.w;
		re.pos_in_ndc.x *= re.pos_in_ndc.w;
		re.pos_in_ndc.y *= re.pos_in_ndc.w;
		re.pos_in_ndc.z *= re.pos_in_ndc.w;
		re.pos_in_world = (t.model * Vector4f(v.position.x, v.position.y, v.position.z, 1.0f)).toVec3();
		re.pos_in_viewport = Vector2f();
		re.normal = MakeMatrix3from4(MakeTranspose(MakeInverse(t.model))) * v.normal;
		re.uv = v.uv;
		if (v.uv.x < xmin) xmin = v.uv.x;
		if (v.uv.x > xmax) xmax = v.uv.x;
		if (v.uv.y < ymin) ymin = v.uv.y;
		if (v.uv.y > ymax) ymax = v.uv.y;
		return re;
	};
	//--------------------------------------------------------------------------------
	std::transform(vertices + vert_from, vertices + vert_to, std::back_inserter(m_VertexStream), MVPTrans);

	std::cout << "UV Range: (" << xmin << ", " << ymin << ")     (" << xmax << ", " << ymax << ")" << std::endl;
#ifdef DEBUG_INFO
	for (const auto& v : m_VertexStream) {
		std::cout << v.position << std::endl;
	}
#endif

	auto faces = m_Scene->GetTriangles().cbegin();
	auto& face_from = m_Object->m_FaceFrom;
	auto& face_to = m_Object->m_FaceTo;
	std::cout << "Faces from " << face_from << " to " << face_to << std::endl;
	auto remove_offset = [&vert_from](const Triangle& t) -> Triangle {		
		Triangle tri = t;
		tri.v1 -= vert_from;
		tri.v2 -= vert_from;
		tri.v3 -= vert_from;
		return tri;
	};
#ifdef DEBUG_INFO
	std::cout << m_Scene->GetTriangles().size() << " faces" << std::endl;
#endif

	std::transform(faces + face_from, faces + face_to, std::back_inserter(m_FaceStream), remove_offset);

#if 1
	std::cout << "End Vertex Processing" << std::endl;
#endif
}

void Pipeline::ClippingCulling() {
	// Culling
#ifdef DEBUG_INFO
	std::cout << "Begin clipping" << std::endl;
#endif
	int count = 0;
	for (auto& face : m_FaceStream) {
		const auto& v1 = m_VertexStream[face.v1].pos_in_ndc.toVec3();
		const auto& v2 = m_VertexStream[face.v2].pos_in_ndc.toVec3();
		const auto& v3 = m_VertexStream[face.v3].pos_in_ndc.toVec3();
		face.culling = false;
		// discrimination here
		auto face_normal = cross(v2 - v1, v3 - v2);
		if (dot(face_normal, Vector3f(0, 0, -1)) < 0) {
			face.culling = true;
			++count;
		}
	}

	std::cout << "Culled faces: " << count << std::endl;

	//// How to do clipping
}

void Pipeline::Viewport() {
	auto width = m_Width - 1;
	auto height = m_Height - 1;
	for (auto& vertex : m_VertexStream) {
		vertex.pos_in_viewport.x = (vertex.pos_in_ndc.x + 1.0f) / 2 * width;
		vertex.pos_in_viewport.y = (vertex.pos_in_ndc.y + 1.0f) / 2 * height;
#ifdef DEBUG_INFO
		std::cout << vertex.position << std::endl;
#endif
	}
	std::cout << "Viewport transform finished" << std::endl;
}

void Pipeline::Rasterization() {
	auto i = 0;
	std::cout << m_FaceStream.size() << " faces need to be rasterized" << std::endl;
	for (const auto& face : m_FaceStream) {
		if (face.culling) continue;
		AABB box(&m_VertexStream[face.v1], &m_VertexStream[face.v2], &m_VertexStream[face.v3], m_Width, m_Height);
		box.Traverse(m_Fragments);
	}
	std::cout << "Finished rasterization" << std::endl;
}

void Pipeline::FragmentShading() {
	std::cout << "Fragments: " << m_Fragments.size() << std::endl;
	size_t i = 0;

	Texture t("./obj/Mary.png");

	float xmin = 1, xmax = 0, ymin = 1, ymax = 0;

	for (auto& fragment : this->m_Fragments) {
		// In case some illegal fragments exist.
		if (fragment.window_pos.x >= m_Width  ||
			fragment.window_pos.y >= m_Height ||
			fragment.window_pos.x < 0 || fragment.window_pos.y < 0) {
#if 0
			std::cout << frag.window_pos << std::endl;
#endif
			++i; continue;
		}

		if (fragment.uv.x < xmin) xmin = fragment.uv.x;
		if (fragment.uv.x > xmax) xmax = fragment.uv.x;
		if (fragment.uv.y < ymin) ymin = fragment.uv.y;
		if (fragment.uv.y > ymax) ymax = fragment.uv.y;


		// Fragment shader begins from here
		Vector3f color = dot(fragment.normal.Normalized(), Vector3f(0, 0, -1)) < 0 ? t.GetColorNearest(fragment.uv).toVec3() : Vector3f(0.3f, 0.3f, 0.3f);
		//Vector3f color = dot(fragment.normal.Normalized(), Vector3f(0, 0, -1)) < 0 ? Vector3f(1.0f, 0.5f, 0.3f) : Vector3f(0.3f, 0.3f, 0.3f);

		Vector3f light_pos = Vector3f(100.0f, 0.0f, 100.0f); // Spotlight
		Vector3f light_col = Vector3f(1.0f, 1.0f, 1.0f);
		Vector3f view_pos = Vector3f(0, 0, 10);

		Vector3f ambient = Vector3f(0.1f, 0.1f, 0.1f) * color;

		Vector3f normal = fragment.normal.Normalized();
		Vector3f light_dir = (light_pos - fragment.pos_in_world).Normalized();
		float diff = std::max(dot(normal, light_dir), 0.0f);
		Vector3f diffuse = Vector3f(0.5f, 0.5f, 0.5f) * color * diff;

		Vector3f view_dir = (view_pos - fragment.pos_in_world).Normalized();
		auto reflection = [](const Vector3f& a, const Vector3f& n) -> Vector3f {
			Vector3f u = a - dot(a, n) * n;
			return a - 2.0f * u;
		};
		Vector3f reflect = reflection(view_dir, normal).Normalized();
		float spec = std::pow(std::max(dot(view_dir, reflect), 0.0f), 32.0f);
		Vector3f specular = Vector3f(1.0f, 1.0f, 1.0f) * color * spec;

		Vector3f final = ambient + diffuse + specular;
		fragment.color = Vector4f(
			clamp(final.x, 0.0f, 1.0f),
			clamp(final.y, 0.0f, 1.0f),
			clamp(final.z, 0.0f, 1.0f), 1.0f
		);
	}
	std::cout << "Illegal pixel:" << i << std::endl;
	std::cout << "UV range:" << xmin << ", " << ymin << "    " << xmax << ", " << ymax << std::endl;
}

void Pipeline::DepthTest() {
	for (const auto& fragment : m_Fragments) {
		auto j = fragment.window_pos.x;
		auto i = fragment.window_pos.y;
		if ((*m_ZBuffer)[GetIndex(i, j)] > fragment.pos_in_ndc.z) {
			(*m_ZBuffer)[GetIndex(i, j)] = fragment.pos_in_ndc.z;
			(*m_Framebuffer)[GetIndex(i, j)] = fragment.color;
		}
	}
}

}   // namespace SyncX