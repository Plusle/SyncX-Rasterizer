#include <math/vector.hpp>
#include <core/displayer.hpp>

#include <iostream>

int main(int argc, char** argv) {
    // SyncX::ImagePrinter printer("test.ppm");
    // printer.Display(nullptr);

    SyncX::Scene sc(std::atoi(argv[1]), std::atoi(argv[2]));
    for (int i = 0; i < sc.m_Height; ++i) {
        for (int j = 0; j < sc.m_Width; ++j) {
            auto r = double(i) / (sc.m_Width-1);
            auto g = double(j) / (sc.m_Height-1);
            auto b = 0.25;
            // std::cout << r << ' ' << g << ' ' << b << std::endl;
            sc.m_Framebuffer[sc.GetIndex(i, j)] = SyncX::Vector3f(r, g, b);
        }
    }
    SyncX::Renderer renderer(&sc);
    SyncX::ImagePrinter printer2("test2.ppm");
    printer2.Display(&renderer);
    return 0;
}