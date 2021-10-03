#pragma once

#include <math/vector.hpp>

namespace SyncX {

    enum class SampleApproximation { Linear, Nearest };

    class Texture {
    public:
        Texture(const char* filename, bool inverse = false);
        ~Texture();

        Vector3f GetColor(float u, float v, SampleApproximation method = SampleApproximation::Linear) const;

    private:
        unsigned char* m_Data;
        int m_Width, m_Height;
    };

}