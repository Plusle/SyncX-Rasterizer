#pragma once

#include <core/scene.hpp>
#include <core/pipeline.hpp>

#include <vector>


namespace SyncX {

class Renderer {
public:
	Renderer(Scene* sc, std::vector<Vector4f>* framebuffer, std::vector<float>* zbuffer, uint32_t width, uint32_t height);
	~Renderer();

	void Render(Model* model, const Transform& MVP);

private:
	Pipeline* m_Pipeline;
};

}