#include <iostream>
#include <fstream>

#include <core/displayer.hpp>

namespace SyncX {
struct DebugImagePrinter : IODevice {
    virtual void Display(Renderer* r) const override;
};

void DebugImagePrinter::Display(Renderer* r) const {
    std::ofstream ppm("TexturePrototypeTest.ppm", std::ios::out);
    if (!ppm.is_open()) {
        std::cerr << "Unable to create file." << std::endl;
        std::exit(1);
    }

    Texture t("texture.png", false);

    // std::cout << t.m_Width << ' ' << t.m_Height << ' ' << t.m_Channels;

    ppm << "P3\n" << t.m_Width << ' ' 
                  << t.m_Height << "\n255\n";

    for (int i = 0; i < t.m_Width; ++i) {
        std::cerr << "\rScanlines No." << i << ' ' << std::flush;
        for (int j = 0; j < t.m_Height; ++j) {
            auto rgb = t.Get(i, j);
            int r = static_cast<int>(rgb.x * 255.0);
            int g = static_cast<int>(rgb.y * 255.0);
            int b = static_cast<int>(rgb.z * 255.0);
            ppm << r << ' ' << g << ' ' << b << '\n';
        }
    }
    std::cerr << "\nDone.\n";

    ppm.close();

}

}

int main(int argc, char** argv) {    
    SyncX::DebugImagePrinter ipr;
    ipr.Display(nullptr);

    return 0;
}