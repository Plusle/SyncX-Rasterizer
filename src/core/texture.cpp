#include <core/texture.hpp>
#include <math/linear.hpp>

#include <cstring>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <external/stb_image.h>

namespace SyncX {
    // Note:
    // The meanings of uv/xy and ij are different
    // In xy-/uv-manner, x/u is corresponded to horizontal axis, y/v is corresponded to vertical axis.
    // In other word, xy-/uv-manner treats the object as an image
    // In ij-manner, i represents the i-th row, j represents the j-th column. 
    // In oterh word, ij-manner treats the object as a matrix
    // In conclusion, if you have a xy and want to use it to get sample on texture 
    // by a function which parameters are ij, you should invoke it like:
    // 
    //          func(y, x, ...)
    //
    // and vise versa.

    Texture::Texture(const char* filename, bool inverse) {
        int c_len = std::strlen(filename), idx = c_len;
        while (--idx >= 0) 
            if (filename[idx] == '.') break;
        char* ext = new char[c_len - idx];
        std::strcpy(ext, &filename[idx]);
        
        constexpr int required_components = 4;
        if (!strcmp(ext, "jpg")) m_Channels = 3;
        else if (!strcmp(ext, "png")) m_Channels = 4;
        else m_Channels = 1;
        m_Data = stbi_load(filename, &m_Width, &m_Height, &m_Channels, required_components);
        if (m_Data == nullptr) {
            std::cerr << "Unable to load texture \"" << filename << "\" by stb_image, exit" << std::endl;
            std::exit(1);
        }
    }

    Texture::~Texture() {
        STBI_FREE(m_Data);
    }

    Vector3f Texture::Get(int i, int j) const {
        constexpr float deno = 1 / 255.0;
        int idx = (i * m_Width + j) * m_Channels;
        float r = deno * m_Data[idx];
        float g = deno * m_Data[idx + 1];
        float b = deno * m_Data[idx + 2];
        return { r, g, b };
    }

    Vector3f Texture::GetColor(float u, float v, SampleApproximation mode) const {
#if 1
        return { 0.5f, 0.5f, 0.5f };
#else
        switch(mode) {
            case SampleApproximation::Linear:
                return GetColorLinear(u, v);
            case SampleApproximation::Nearest:
                return GetColorNearest(u, v);
            default:
                assert(false); return { 0.f, 0.f, 0.f };
        }
#endif
    }

    Vector3f Texture::GetColorLinear(float u, float v) const {
        return { 0.f, 0.f, 0.f };
    }

    Vector3f Texture::GetColorNearest(float u, float v) const {
        size_t x = clamp<size_t>(u * (float)m_Width,  0, m_Width - 1);
        size_t y = clamp<size_t>(v * (float)m_Height, 0, m_Height - 1);

        return Get(y, x);
    }


}   // namesapce SyncX