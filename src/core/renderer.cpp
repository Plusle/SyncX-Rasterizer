#include <core/renderer.hpp>

#include <iostream>

namespace SyncX {

Renderer::Renderer(Scene* sc, std::vector<Vector4f>* framebuffer, std::vector<float>* zbuffer, uint32_t width, uint32_t height)
	: clipping(false), prezbuffer(false) {
	m_Pipeline = new Pipeline(sc, framebuffer, zbuffer, width, height);
}

Renderer::~Renderer() {
	delete m_Pipeline;
}

void Renderer::Render(Model* model, const Transform& MVP) {
	std::cout << "Begin to render" << std::endl;
	m_Pipeline->Initialize(model);
	m_Pipeline->VertexProcess(MVP);
	//if (clipping) 
		m_Pipeline->ClippingCulling();
	m_Pipeline->Viewport();
	m_Pipeline->Rasterization();
	m_Pipeline->FragmentShading();
	m_Pipeline->DepthTest();
	std::cout << "\n\n";
}

}