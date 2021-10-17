#pragma once

#include <core/scene.hpp>
#include <core/pipeline.hpp>

#include <vector>

namespace SyncX {

struct Renderer {
    Renderer(Scene* sc) : m_Scene(sc) {}

    void Render(std::vector<Vector4f>& framebuffer, uint32_t width, uint32_t height) const;

    Scene* m_Scene;
};

}   // namespace SyncX