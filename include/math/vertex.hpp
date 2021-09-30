#pragma once

#include <math/vector.hpp>

namespace SyncX {

    struct RawDatas {
        Vector3f position;
        Vector3f normal;
        Vector2f uv;
        Vector3f tangent;
        Vector3f bitangent;
    };

    using Vertex = RawDatas;
    using Pixel = RawDatas;

}   // namespace SyncX