#pragma once

#include <core/scene.hpp>
#include <core/pipeline.hpp>

namespace SyncX {

struct Renderer {
    Renderer(Scene* sc) : m_Scene(sc) {}

    void Render() const;

    Scene* m_Scene;
};


}   // namespace SyncX