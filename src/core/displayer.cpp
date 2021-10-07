#include <core/displayer.hpp>

#include <iostream>
#include <fstream>

namespace SyncX {

void DebugImagePrinter::Display(Renderer* r) const {
    std::ofstream ppm("test_output.ppm", std::ios::out);
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

void ImagePrinter::Display(Renderer* r) const {
    std::ofstream ppm(m_Filename, std::ios::out);
    if (!ppm.is_open()) {
        std::cerr << "Unable to create file." << std::endl;
        std::exit(1);
    }

    // Switch to 1 after Render() is done
    #if 0
        r->Render();
    #endif
    ppm << "P3\n" << r->m_Scene->m_Width << ' ' 
                  << r->m_Scene->m_Height << "\n255\n";
    
    auto& framebuffer = r->m_Scene->m_Framebuffer;
    for (int i = r->m_Scene->m_Height - 1; i >= 0; --i) {    
        std::cerr << "\rScanlines Remaining: " << i << ' ' << std::flush;
        for (int j = 0; j < r->m_Scene->m_Width; ++j) {
            int ir = static_cast<int>(255.999 * framebuffer[r->m_Scene->GetIndex(i, j)].x);
            int ig = static_cast<int>(255.999 * framebuffer[r->m_Scene->GetIndex(i, j)].y);
            int ib = static_cast<int>(255.999 * framebuffer[r->m_Scene->GetIndex(i, j)].z);
            ppm << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::cerr << "\nDone.\n";

    ppm.close();
}

void Terminal::Display(Renderer* r) const {

} 

}   // namespace SyncX