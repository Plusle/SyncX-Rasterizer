#pragma once

#include <math/vector.hpp>

#include <vector>
#include <tuple>

namespace SyncX {

    enum class SampleApproximation { Linear, Nearest };

    class Texture {
    public:
        Texture(const char* filename, bool inverse = false, bool mipmap = true);
        ~Texture();

        Vector3f Get(int32_t i, int32_t j) const;

        int32_t m_Width, m_Height, m_Channels;

#ifdef MIPMAP_DEBUG_INFO
        std::vector<uint32_t> layer_compacity;
#endif
        uint8_t* m_Data;
        int32_t m_MipmapMaxLevel;
        std::vector<uint32_t> m_MipmapOffset;

    private:
        int32_t GetChannels(const char* filename) const;
        void GenerateMipmap();
        bool HasMipmap() const;
        std::tuple<int32_t, int32_t> GetInterpolationDirection(const Vector2f& uv) const;
        Vector4f GetTexel(int32_t i, int32_t j, int32_t width, int32_t height, int32_t level) const;
        Vector4f GetColorBilinear(const Vector2f& uv, int32_t level = 0) const;
        Vector4f GetColorTrilinear(const Vector2f& uv, const Vector2f& uv_x, const Vector2f& uv_y) const;
        Vector4f GetColorNearest(const Vector2f& uv) const;



    };

}