#include <iostream>
#include <fstream>

#define MIPMAP_DEBUG_INFO
#include <core/io_interface.hpp>
#include <core/texture.hpp>

namespace SyncX {
struct DebugImagePrinter : IODevice {
    DebugImagePrinter() {
        m_Height = 600; m_Width = 800;
    }
    virtual void Display() override;
};

void DebugImagePrinter::Display() {
    std::ofstream ppm("TexPrototype.ppm", std::ios::out);
    if (!ppm.is_open()) {
        std::cerr << "Unable to create file." << std::endl;
        std::exit(1);
    }

    Texture t("texture.png", false, false);
              
    ppm << "P3\n" << t.m_Width << ' ' 
                  << t.m_Height << "\n255\n";

    m_Width = t.m_Width;
    m_Height = t.m_Height;
    Init();

    for (int i = 0; i < m_Height; ++i) {
        for (int j = 0; j < m_Width; ++j) {
            auto rgb = t.Get(i, j);
            m_Framebuffer[GetIndex(i, j)] = Vector4f(rgb.x, rgb.y, rgb.z, 1.0f);
        }
    }

    for (int i = 0; i < m_Height; ++i) {
        std::cerr << "\rScanlines No." << i << ' ' << std::flush;
        for (int j = 0; j < m_Width; ++j) {
            auto rgb = m_Framebuffer[GetIndex(i, j)];
            int r = rgb.x * 255.0;
            int g = rgb.y * 255.0;
            int b = rgb.z * 255.0;
            ppm << r << ' ' << g << ' ' << b << '\n';
        }
    }
    std::cerr << "\nDone.\n";

    ppm.close();
}

}

int main(int argc, char** argv) {    
    SyncX::DebugImagePrinter ipr;
    ipr.Display();

    return 0;
}