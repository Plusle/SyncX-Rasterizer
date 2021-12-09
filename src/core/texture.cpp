#include <core/texture.hpp>
#include <math/linear.hpp>

#include <cstring>
#include <iostream>
#include <numeric>
#include <string>

#define STB_IMAGE_IMPLEMENTATION
#include <external/stb_image.h>

namespace SyncX {
    Texture::Texture(const char* filename, bool inverse, bool mipmap) {
        int32_t required_components = GetChannels(filename);
        m_Data = stbi_load(filename, &m_Width, &m_Height, &m_Channels, required_components);
        if (m_Data == nullptr) {
            std::cerr << "Unable to load texture \"" << filename << "\" by stb_image, exit" << std::endl;
            std::exit(1);
        }

        layer_compacity = std::vector<uint32_t>();

        if (mipmap) GenerateMipmap();
    }

    Texture::~Texture() {
        STBI_FREE(m_Data);
    }

    Vector3f Texture::Get(int32_t i, int32_t j) const {
        constexpr float deno = 1 / 255.0;
        int32_t idx = (i * m_Width + j) * m_Channels;
        float r = deno * m_Data[idx];
        float g = deno * m_Data[idx + 1];
        float b = deno * m_Data[idx + 2];
        return { r, g, b };
    }

    int32_t Texture::GetChannels(const char* filename) const {
        const char* ext = &filename[std::strlen(filename) - 3];
        if (!std::strcmp(ext, "png")) return 4;
        else if (!std::strcmp(ext, "jpg")) return 3;
        return 3;
    }

    void Texture::GenerateMipmap() {
        m_MipmapMaxLevel = -1;
        for (int32_t width = m_Width, height = m_Height; 
                width != 1 && height != 1; 
                ++m_MipmapMaxLevel, width >>= 1, height >>= 1) {
            layer_compacity.push_back(m_Channels * width * height);
        }

        if (m_MipmapMaxLevel == -1) return;

        uint32_t byte_sum = std::accumulate(layer_compacity.cbegin(), layer_compacity.cend(), 0);

        m_MipmapOffset = std::vector<uint32_t>(m_MipmapMaxLevel + 1, 0);
        for (int32_t level = 1; level <= m_MipmapMaxLevel; ++level) {
            m_MipmapOffset[level] = m_MipmapOffset[level - 1] + layer_compacity[level - 1];
        }
        
        uint8_t* mipmap_data = new uint8_t[byte_sum];
        //--------------------------------------------------------------
        std::memcpy(mipmap_data, m_Data, layer_compacity[0]);
        for (int32_t level = 1, src_width = m_Width, src_height = m_Height;
             level <= m_MipmapMaxLevel; 
             ++level, src_height >>= 1, src_width >>= 1) {

            uint8_t* current_src = &mipmap_data[m_MipmapOffset[level - 1]];
            uint8_t* current_dst = &mipmap_data[m_MipmapOffset[level]];
            for (int32_t i = 0; i < src_height - src_height % 2; i += 2) {
                for (int32_t j = 0; j < src_width - src_width % 2; j += 2) {
                    uint8_t* tl = &current_src[i * src_width * m_Channels + j * m_Channels];
                    uint8_t* tr = tl + m_Channels;
                    uint8_t* bl = tl + src_width * m_Channels;
                    uint8_t* br = bl + m_Channels;
                    for (int32_t k = 0; k < m_Channels; ++k) {
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

    std::tuple<int32_t, int32_t> Texture::GetInterpolationDirection(const Vector2f& uv) const {
        int32_t horizontal = uv.x - std::floor(uv.x) < 0.5f ? -1 : 1;
        int32_t vertical = uv.y - std::floor(uv.y) < 0.5 ? 1 : -1;
        return std::make_tuple(vertical, horizontal);
    }

    Vector4f Texture::GetTexel(int32_t i, int32_t j, int32_t width, int32_t height, int32_t level) const {
        uint8_t* texel = m_Data + m_MipmapOffset[level];
        texel += i * width * m_Channels + j * m_Channels;
        float r = static_cast<float>(texel[0]);
        float g = static_cast<float>(texel[1]);
        float b = static_cast<float>(texel[2]);
        float a = 1.0f;
        if (m_Channels == 4)
            a = static_cast<float>(texel[3]);
        return { r, g, b, a };
    }

    Vector4f Texture::GetColorBilinear(const Vector2f& uv, int32_t level) const {
        int32_t width = m_Width, height = m_Height;
        if (HasMipmap()) {
            int32_t i = level;
            while (i != 0) {
                width /= 2; height /= 2;
            }
        }

        float col = static_cast<float>(width) * uv.x;
        float row = static_cast<float>(height) * uv.y;

        auto [ vertical, horizontal ] = GetInterpolationDirection(uv);
        float v_coeff = row - std::floor(row) + (vertical == -1 ? 0.5f : -0.5f);
        float h_coeff = col - std::floor(col) + (horizontal == -1 ? 0.5f : -0.5f);

        int32_t i0 = clamp<int32_t>(static_cast<int32_t>(row), 0, height - 1);
        int32_t j0 = clamp<int32_t>(static_cast<int32_t>(col), 0, width - 1);
        int32_t i1 = clamp<int32_t>(i0 + vertical, 0, width - 1);
        int32_t j1 = clamp<int32_t>(j0 + horizontal, 0, height - 1);

        Vector4f color_tl = GetTexel(i0, j0, width, height, level);
        Vector4f color_tr = GetTexel(i0, j1, width, height, level);
        Vector4f color_bl = GetTexel(i1, j0, width, height, level);
        Vector4f color_br = GetTexel(i1, j1, width, height, level);
        
        if (vertical == -1) {
            std::swap(color_tl, color_bl);
            std::swap(color_tr, color_br);
        }

        if (horizontal == -1) {
            std::swap(color_tl, color_tr);
            std::swap(color_bl, color_br);
        }

        Vector4f color_top = lerp(color_tl, color_tr, h_coeff);
        Vector4f color_bot = lerp(color_bl, color_br, h_coeff);
        Vector4f bilinear_color = lerp(color_top, color_bot, v_coeff);

        return bilinear_color;
    }

    // Calling this function need dx and dy to determine mipmap level.
    // Conventionally, this application would try to assign right and bottom pixel, firstly.
    // But if anyone doesn't exist, then try to assign opposite pixel. If still doesn't exist, assign itself then.
    // That means:
    // dx = right pixel exists ? right pixel : (left pixel exists ? left pixel : itself)
    // dy = top pixel exists ? top pixel : (bottom pixel exists? bottom pixel : itself)
    // More details will be introduced in shader code
    Vector4f Texture::GetColorTrilinear(const Vector2f& uv, const Vector2f& duv_dx, const Vector2f& duv_dy) const {
        int32_t u0 = uv.x * (float)m_Width, v0 = uv.y * (float)m_Height;
        int32_t ux = duv_dx.x * (float)m_Width, vx = duv_dx.y * (float)m_Height;
        int32_t uy = duv_dy.x * (float)m_Width, vy = duv_dy.y * (float)m_Height;
        float L = std::max(std::sqrt((ux - u0) * (ux - u0) + (vx - v0) * (vx - v0)), 
                           std::sqrt((uy - u0) * (uy - u0) + (vy - v0) * (vy - v0)));
        float D = std::log2f(L);
        float level1 = std::floor(D);
        float level2 = std::ceil(D);
        float coeff  = D - level1;
        
        Vector4f trilinear_color = lerp(GetColorBilinear(uv, level1), GetColorBilinear(uv, level2), coeff);
        return trilinear_color;
    }

    Vector4f Texture::GetColorNearest(const Vector2f& uv) const {
        uint32_t x = clamp<uint32_t>(uv.x * (float)m_Width,  0, m_Width - 1);
        uint32_t y = clamp<uint32_t>(uv.y * (float)m_Height, 0, m_Height - 1);
        Vector3f color = Get(y, x);
        return Vector4f(color.x, color.y, color.z, 1.0f);
    }


}   // namesapce SyncX