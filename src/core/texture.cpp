#include <core/texture.hpp>

#include <cstring>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <external/stb_image.h>

namespace SyncX {
    Texture::Texture(const char* filename, bool inverse) {
        int c_len = std::strlen(filename), idx = c_len;
        while (--idx >= 0) 
            if (filename[idx] == '.') break;
        char* ext = new char[c_len - idx];
        std::strcpy(ext, &filename[idx]);
        
        int w, h, ch, req;
        if (!strcmp(ext, "jpg")) req = 3;
        else if (!strcmp(ext, "png")) req = 4;
        else req = 1;
        m_Data = stbi_load(filename, &w, &h, &ch, req);
        if (m_Data == nullptr) {
            std::cerr << "Unable to load texture " << filename << " by stb_image, exit" << std::endl;
            std::exit(1);
        }
    }

    Texture::~Texture() {
        STBI_FREE(m_Data);
    }

    Vector3f GetColor(float u, float v) {
#ifndef FORMAL_VERSION
        return { 255.0f * u, 255.0f * v, 255.0f };
#endif
        
    }


}   // namesapce SyncX