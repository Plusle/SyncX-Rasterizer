#pragma once

#include <math/vector.hpp>
#include <math/linear.hpp>

namespace SyncX {

    enum class SampleApproximation { Linear, Nearest };

    class Texture {
    public:
        Texture(const char* filename, bool inverse = false);
        ~Texture();

        Vector3f GetColor(float u, float v, SampleApproximation method = SampleApproximation::Linear) const;
        Vector3f Get(int i, int j) const;

        int m_Width, m_Height, m_Channels;

    private:
        void GenerateMipmap() const;
        Vector3f GetColorLinear(float u, float v) const;
        Vector3f GetColorNearest(float u, float v) const;

        unsigned char* m_Data;
    };

}