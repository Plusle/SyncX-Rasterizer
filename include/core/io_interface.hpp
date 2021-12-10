#pragma once 


#include <string>
#include <vector>

#include <math/vector.hpp>
#include <core/scene.hpp>
#include <core/renderer.hpp>

namespace SyncX {

struct IODevice {
    IODevice() = default;
    IODevice(uint32_t width, uint32_t height);
    virtual ~IODevice();
    virtual void Display() = 0; 
    void Init(Scene* sc);
    void ClearBuffer();
    uint32_t GetIndex(int32_t i, int32_t j) const;

    uint32_t m_Width, m_Height;
    std::vector<Vector4f> m_Framebuffer;
    std::vector<float> m_Zbuffer;
    Scene* m_Scene;
    Renderer* m_Renderer;
};

struct ImagePrinter : IODevice {
    ImagePrinter(const char* filename, uint32_t width, uint32_t height);
    virtual ~ImagePrinter() override;
    void DebugImageInit();
    virtual void Display() override;
    std::string m_Filename;
};

struct Win32Platform : IODevice {
    Win32Platform(const char* title, uint32_t width, uint32_t height);
    virtual ~Win32Platform() override;
    virtual void Display() override;
    std::string m_Title;
};

}   // namespace SyncX