#define MIPMAP_DEBUG_INFO
#include <core/texture.hpp>

#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char** argv) {
    SyncX::Texture t("texture.png");
    
    uint32_t width = t.m_Width, height = t.m_Height;
    std::string l[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    for (uint32_t level = 0; level <= t.m_MipmapMaxLevel; ++level) {
        std::string filename = "mipmap_level" + l[level] + ".ppm";
        //uint32_t capcity = t.layer_compacity[level];
        uint8_t* starter = &(t.m_Data[t.m_MipmapOffset[level]]);
        uint32_t bytes = 0;
        std::ofstream ppm(filename, std::ios::out);
        ppm << "P3\n" << width << ' ' << height << "\n255\n";
        while (bytes != t.layer_compacity[level]) {
            ppm << (int)starter[0] << ' ' << (int)starter[1] << ' ' << (int)starter[2] << '\n';
            starter += t.m_Channels;
            bytes += t.m_Channels;
        }
        width /= 2;
        height /= 2;
        ppm.close();
    }

    return 0;
}
