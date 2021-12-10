#include <core/renderer.hpp>

namespace SyncX {

Renderer::Renderer(Scene* sc, std::vector<Vector4f>* framebuffer, std::vector<float>* zbuffer, uint32_t width, uint32_t height) {
	m_Pipeline = new Pipeline(sc, framebuffer, zbuffer, width, height);
}

Renderer::~Renderer() {
	delete m_Pipeline;
}

void Renderer::Render(Model* model, const Transform& MVP) {
	m_Pipeline->Initialize(model);
	///.....
}

}