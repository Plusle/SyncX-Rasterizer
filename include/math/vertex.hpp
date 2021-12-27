#pragma once

#include <math/vector.hpp>

namespace SyncX {

    struct RawVertex {
        Vector3f position;
        Vector3f normal;
        Vector2f uv;
        // Vector3f tangent;
        // Vector3f bitangent;
    };

    struct Vertex {
        Vector4f position;
        Vector3f normal;
        Vector2f uv;
        // Vector4f tangent;
        // Vector4f bitangent;
    };

    struct Pixel {
        Vector4f position;
        Vector3f normal;
        Vector2f uv;
        Vector2i window_pos;
        // Vector4f tangent;
        // Vector4f bitangent;
    };

    using Fragment = Pixel;

}   // namespace SyncX