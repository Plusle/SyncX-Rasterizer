#include <core/io_interface.hpp>

#include <iostream>
#include <fstream>
#include <limits>

namespace SyncX {

IODevice::IODevice(uint32_t width, uint32_t height) 
        : m_Width(width), m_Height(height), m_Renderer(nullptr) {}

IODevice::~IODevice() {
    if (m_Renderer != nullptr) delete m_Renderer;
}

void IODevice::Init(Scene* sc) {
    m_Framebuffer = std::vector<Vector4f>(m_Width * m_Height, Vector4f(0.f, 0.f, 0.f, 0.f));
    m_Zbuffer = std::vector<float>(m_Width * m_Height, std::numeric_limits<float>::max());
    m_Scene = sc;
    m_Renderer = new Renderer(sc, &m_Framebuffer, &m_Zbuffer, m_Width, m_Height);
}

void IODevice::ClearBuffer() {
    m_Framebuffer.assign(m_Width * m_Height, Vector4f(0.f, 0.f, 0.f, 0.f));
    m_Zbuffer.assign(m_Width * m_Height, std::numeric_limits<float>::max());
}

uint32_t IODevice::GetIndex(int32_t i, int32_t j) const {
    return i * m_Width + j;
}

ImagePrinter::ImagePrinter(const char* filename, uint32_t width, uint32_t height) 
                : m_Filename(filename), IODevice(width, height) {}

ImagePrinter::~ImagePrinter() {}

void ImagePrinter::DebugImageInit() {
    for (uint32_t i = 0; i < m_Height; ++i) {
        for (uint32_t j = 0; j < m_Width; ++j) {
            Vector4f color(
                static_cast<float>(i) / (m_Height - 1),
                static_cast<float>(j) / (m_Width - 1),
                0.25f, 1.f
            );
        }
    }
}

void ImagePrinter::Display() {
    std::ofstream ppm(m_Filename, std::ios::out);
    if (!ppm.is_open()) {
        std::cerr << "Unable to create file." << std::endl;
        std::exit(1);
    }


    // This is just for debugging
    // Rewrite this part after pipeline is done
    //DebugImageInit();

    for (auto& model : m_Scene->GetModels()) {
        m_Renderer->Render(&model);
    }

#if 0
    ppm << "P3\n" << m_Width << ' ' << m_Height << "\n255\n";
    
    for (int i = 0; i < m_Height; ++i) {    
        std::cerr << "\rScanlines No." << i << ' ' << std::flush;
        for (int j = 0; j < m_Width; ++j) {
            int ir = static_cast<int>(255.999 * m_Framebuffer[GetIndex(i, j)].x);
            int ig = static_cast<int>(255.999 * m_Framebuffer[GetIndex(i, j)].y);
            int ib = static_cast<int>(255.999 * m_Framebuffer[GetIndex(i, j)].z);
            ppm << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
    std::cerr << "\nDone.\n";
#endif

    ppm.close();
}

Win32Platform::Win32Platform(const char* title, uint32_t width, uint32_t height) 
                : m_Title(title), IODevice(width, height) {}

Win32Platform::~Win32Platform() {}

void Win32Platform::Display() {

} 

}   // namespace SyncX