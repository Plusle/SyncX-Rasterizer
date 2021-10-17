#include <math/vector.hpp>
#include <core/io_interface.hpp>

#include <iostream>

int main(int argc, char** argv) {
    // SyncX::ImagePrinter printer("test.ppm");
    // printer.Display(nullptr);

    SyncX::Scene sc;
    uint32_t width = std::atoi(argv[1]), height = std::atoi(argv[2]);
    SyncX::Renderer renderer(&sc);
    SyncX::ImagePrinter printer2("test2.ppm", width, height);
        for (int i = 0; i < printer2.m_Height; ++i) {
        for (int j = 0; j < printer2.m_Width; ++j) {
            auto r = double(i) / (printer2.m_Width-1);
            auto g = double(j) / (printer2.m_Height-1);
            auto b = 0.25;
            // std::cout << r << ' ' << g << ' ' << b << std::endl;
            printer2.m_Framebuffer[printer2.GetIndex(i, j)] = SyncX::Vector4f(r, g, b, 1.f);
        }
    }
    printer2.Display(&renderer);
    return 0;
}