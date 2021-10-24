#pragma once

#include <core/scene.hpp>

namespace SyncX {

struct Loader {
    Loader(Scene* sc);

    void Execute(const char* filename) const;

    Scene* m_Scene;
};

}   // namespace SyncX