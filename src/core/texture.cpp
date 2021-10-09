#define MIPMAP_DEBUG_INFO
#include <core/texture.hpp>
#include <math/linear.hpp>

#include <cstring>
#include <iostream>
#include <numeric>
#include <string>

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

    Texture::Texture(const char* filename, bool inverse, bool mipmap) {
        // int c_len = std::strlen(filename), idx = c_len;
        // while (--idx >= 0) 
        //     if (filename[idx] == '.') break;
        // char* ext = new char[c_len - idx];
        // std::strcpy(ext, &filename[idx]);

        std::string name(filename);
        const char* ext = name.substr(name.length() - 3, 3).c_str();
        
        constexpr int required_components = 4;
        if (!strcmp(ext, "jpg")) m_Channels = 3;
        else if (!strcmp(ext, "png")) m_Channels = 4;
        else m_Channels = 1;
        m_Data = stbi_load(filename, &m_Width, &m_Height, &m_Channels, required_components);
        if (m_Data == nullptr) {
            std::cerr << "Unable to load texture \"" << filename << "\" by stb_image, exit" << std::endl;
            std::exit(1);
        }
#ifdef MIPMAP_DEBUG_INFO
        layer_compacity = std::vector<uint32_t>();
#endif

#if 1
        if (mipmap) GenerateMipmap();
#endif
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

    void Texture::GenerateMipmap() {
        m_MipmapMaxLevel = -1;
#ifndef MIPMAP_DEBUG_INFO
        std::vector<uint32_t> layer_compacity;
#endif
        for (uint32_t width = m_Width, height = m_Height; 
                width != 1 && height != 1; 
                ++m_MipmapMaxLevel, width >>= 1, height >>= 1) {
            layer_compacity.push_back(m_Channels * width * height);
        }

        if (m_MipmapMaxLevel == -1) return;

        uint32_t byte_sum = std::accumulate(layer_compacity.cbegin(), layer_compacity.cend(), 0);

        m_MipmapOffset = std::vector<uint32_t>(m_MipmapMaxLevel + 1, 0);
        for (uint32_t level = 1; level <= m_MipmapMaxLevel; ++level) {
            m_MipmapOffset[level] = m_MipmapOffset[level - 1] + layer_compacity[level - 1];
        }
        
        uint8_t* mipmap_data = new uint8_t[byte_sum];
        //--------------------------------------------------------------
        std::memcpy(mipmap_data, m_Data, layer_compacity[0]);
        for (uint32_t level = 1, src_width = m_Width, src_height = m_Height;
             level <= m_MipmapMaxLevel; 
             ++level, src_height >>= 1, src_width >>= 1) {
#ifdef MIPMAP_DEBUG_INFO
            std::cerr << "Begin to generating mipmap level " << level << ".\n" << std::flush;
#endif
            uint8_t* current_src = &mipmap_data[m_MipmapOffset[level - 1]];
            uint8_t* current_dst = &mipmap_data[m_MipmapOffset[level]];
            for (uint32_t i = 0; i < src_height - src_height % 2; i += 2) {
                for (uint32_t j = 0; j < src_width - src_width % 2; j += 2) {
// #ifdef MIPMAP_DEBUG_INFO
//             std::cout << "Creating pixel row " << i << " col " << j << ".\n" << std::flush;
// #endif   
                    uint8_t* tl = &current_src[i * src_width * m_Channels + j * m_Channels];
                    uint8_t* tr = tl + m_Channels;
                    uint8_t* bl = tl + src_width * m_Channels;
                    uint8_t* br = bl + m_Channels;
                    for (uint32_t k = 0; k < m_Channels; ++k) {
                        current_dst[k] = (tl[k] + tr[k] + bl[k] + br[k]) / 4;
                    }
                    current_dst += m_Channels;
                }
            }
            assert(current_dst == &mipmap_data[m_MipmapOffset[level] + layer_compacity[level]]);
        }
        //--------------------------------------------------------------

        STBI_FREE(m_Data);
        m_Data = mipmap_data;
    }

    bool Texture::HasMipmap() const {
        return m_MipmapMaxLevel != -1;
    }

    Vector3f Texture::GetColorLinear(float u, float v) const {
        return { 0.f, 0.f, 0.f };
    }

    Vector3f Texture::GetColorNearest(float u, float v) const {
        uint32_t x = clamp<uint32_t>(u * (float)m_Width,  0, m_Width - 1);
        uint32_t y = clamp<uint32_t>(v * (float)m_Height, 0, m_Height - 1);

        return Get(y, x);
    }


}   // namesapce SyncX