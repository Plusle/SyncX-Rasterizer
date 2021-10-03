#include <core/displayer.hpp>

#include <iostream>
#include <fstream>

namespace SyncX {

void ImagePrinter::Display(Renderer* r) const {
    std::ofstream ppm(m_Filename, std::ios::out);
    if (!ppm.is_open()) {
        std::cerr << "Unable to create file." << std::endl;
        std::exit(1);
    }

#if 0   
    // This is the primitive test from rtweekend
    int image_height = 600, image_width = 800;
    ppm << "P3\n" << image_width << ' ' 
                  << image_height << "\n255\n";
    

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            auto r = double(i) / (image_width-1);
            auto g = double(j) / (image_height-1);
            auto b = 0.25;

            int ir = static_cast<int>(255.999 * r);
            int ig = static_cast<int>(255.999 * g);
            int ib = static_cast<int>(255.999 * b);

            ppm << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::cerr << "\nDone.\n";

#else
    // And this is the really part
    // Switch to 1 after Render() is done
    #if 0
        r->Render();
    #endif
    ppm << "P3\n" << r->m_Scene->m_Width << ' ' 
                  << r->m_Scene->m_Height << "\n255\n";
    
    int count = 0, width = r->m_Scene->m_Width;
    // for (const Vector3f& color : r->m_Scene->m_Framebuffer) {
    auto& framebuffer = r->m_Scene->m_Framebuffer;
    for (int i = r->m_Scene->m_Height - 1; i >= 0; --i) {    
        for (int j = 0; j < r->m_Scene->m_Width; ++j) {
            std::cerr << "\rScanlines Remaining: " << i << ' ' << std::flush;
            // std::cout << color.x << ' ' << color.y << ' ' << color.z << std::endl;
            int ir = static_cast<int>(255.999 * framebuffer[r->m_Scene->GetIndex(i, j)].x);
            int ig = static_cast<int>(255.999 * framebuffer[r->m_Scene->GetIndex(i, j)].y);
            int ib = static_cast<int>(255.999 * framebuffer[r->m_Scene->GetIndex(i, j)].z);
            ppm << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    // }
    std::cerr << "\nDone.\n";
#endif

    ppm.close();
}

void Terminal::Display(Renderer* r) const {

} 

}   // namespace SyncX