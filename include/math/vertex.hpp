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
        Vector4f pos_in_ndc;
        Vector3f pos_in_world;
        Vector2f pos_in_viewport;
        Vector3f normal;
        Vector2f uv;
        // Vector4f tangent;
        // Vector4f bitangent;
    };

    struct Fragment {
        Vector4f pos_in_ndc;
        Vector3f pos_in_world;
        Vector3f normal;
        Vector2f uv;
        Vector2i window_pos;
        // Vector4f tangent;
        // Vector4f bitangent;
        Vector4f color;
    };

    //using Fragment = Pixel;

}   // namespace SyncX