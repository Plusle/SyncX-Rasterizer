#pragma once

#include <math/matrix.hpp>
#include <math/transfrom.hpp>
#include <core/scene.hpp>


namespace SyncX {

class Pipeline {
public:
	Pipeline(Scene* sc, std::vector<Vector4f>* framebuffer, std::vector<float>* zbuffer, uint32_t width, uint32_t height);

	void Initialize(Model* model);

	// step 1: copy faces to face buffer, remove offset
	// then copy all corresponding vertices after transforming them by MVP matrix
	void VertexProcess(const Transform& t);

	// step 2: clipping & face culling
	void ClippingCulling();

	// step 2.9: Viewport transformation
	void Viewport();

	// step 3: rasterization
	void Rasterization();

	// step 4: Fragment Shading
	void FragmentShading();

	// step 5: Depth test & blending
	void DepthTest();

private:
	Scene* m_Scene;
	const Model* m_Object;
	std::vector<Vector4f>* m_Framebuffer;
	std::vector<float>* m_ZBuffer;
	uint32_t m_Width, m_Height;

	std::vector<Vertex> m_VertexStream;
	std::vector<Triangle> m_FaceStream;
	std::vector<Fragment> m_Fragments;

	uint32_t GetIndex(uint32_t i, uint32_t j) { return m_Width * i + j; }
};

}   // namespace SyncX