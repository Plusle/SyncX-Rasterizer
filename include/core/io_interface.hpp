#pragma once 

#include <core/renderer.hpp>

#include <string>

namespace SyncX {

struct IODevice {
    IODevice() = default;
    IODevice(uint32_t width, uint32_t height);
    virtual void Display(Renderer* r) = 0; 
    void Init();
    void ClearBuffer();
    uint32_t GetIndex(int32_t i, int32_t j) const;

    uint32_t m_Width, m_Height;
    std::vector<Vector4f> m_Framebuffer;
    std::vector<float> m_Zbuffer;
};

struct ImagePrinter : IODevice {
    ImagePrinter(const char* filename, uint32_t width, uint32_t height);
    void DebugImageInit();
    virtual void Display(Renderer* r) override;
    std::string m_Filename;
};

struct Win32Platform : IODevice {
    Win32Platform(const char* title, uint32_t width, uint32_t height);
    virtual void Display(Renderer* r) override;
    std::string m_Title;
};

}   // namespace SyncX