#pragma once

#include <math/vector.hpp>

#include <vector>

namespace SyncX {

    enum class SampleApproximation { Linear, Nearest };

    class Texture {
    public:
        Texture(const char* filename, bool inverse = false, bool mipmap = true);
        ~Texture();

        Vector3f GetColor(float u, float v, SampleApproximation method = SampleApproximation::Linear) const;
        Vector3f Get(int i, int j) const;

        int m_Width, m_Height, m_Channels;

#ifdef MIPMAP_DEBUG_INFO
        std::vector<uint32_t> layer_compacity;
#endif

    private:
        void GenerateMipmap();
        bool HasMipmap() const;
        Vector3f GetColorLinear(float u, float v) const;
        Vector3f GetColorNearest(float u, float v) const;

        uint8_t* m_Data;
        int32_t m_MipmapMaxLevel;
        std::vector<uint32_t> m_MipmapOffset;

    };

}