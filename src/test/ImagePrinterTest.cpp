#include <math/vector.hpp>
#include <core/io_interface.hpp>

#include <iostream>
#include <fstream>

int main(int argc, char** argv) {
    uint32_t width = std::atoi(argv[1]), height = std::atoi(argv[2]);
    SyncX::ImagePrinter printer2("imagetest.ppm", width, height);
    printer2.Init(nullptr);
    std::ofstream ppm("imagetest2.ppm", std::ios::out);
    ppm << "P3\n" << printer2.m_Width << ' ' << printer2.m_Height << "\n255\n";

    for (int i = 0; i < printer2.m_Height; ++i) {
        for (int j = 0; j < printer2.m_Width; ++j) {
            auto r = double(i) / (printer2.m_Height-1);
            auto g = double(j) / (printer2.m_Width-1);
            auto b = 0.25;
            //std::cout << r << ' ' << g << ' ' << b << std::endl;
            ppm << (int)(r * 255.0) << ' ' << (int)(g * 255.0) << ' ' << (int)(b * 255.0) << '\n';
            printer2.m_Framebuffer[printer2.GetIndex(i, j)] = SyncX::Vector4f(r, g, b, 1.f);
        }
    }
    printer2.Display();
    return 0;
}