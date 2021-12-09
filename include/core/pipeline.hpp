#pragma once

#include <math/matrix.hpp>
#include <math/transfrom.hpp>
#include <core/scene.hpp>
#include <core/io_interface.hpp>

#include <atomic>

namespace SyncX {

class Pipeline {
public:
	Pipeline(Scene* sc, IODevice* device, const Model& model) : m_Scene(sc), m_Device(device), m_Object(model) {}
	
	// step 1: copy faces to face buffer, remove offset
	// then copy all corresponding vertices after transforming them by MVP matrix
	void VertexProcess(const Transform& t);

	// step 2: clipping & face culling
	void ClippingCulling();

	// step 3: rasterization
	void Rasterization();

	//// step 3.1: MXAA
	//void MXAASetting();

	// step 3.9: Early depth test
	void PreDepthTest();

	// step 4: Fragment Shading
	void FragmentShading();

	// step 5: Depth test & blending
	void Blending();

private:
	Scene* m_Scene;
	IODevice* m_Device;
	const Model& m_Object;

	std::vector<Vertex> m_VertexStream;
	std::vector<Triangle> m_FaceStream;
	std::vector<Fragment> m_Fragments;

};

}   // namespace SyncX