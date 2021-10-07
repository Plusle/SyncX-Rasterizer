#pragma once 

#include <core/renderer.hpp>

#include <string>

namespace SyncX {

struct IODevice {
    virtual void Display(Renderer* r) const = 0; 
};

struct DebugImagePrinter : IODevice {
    virtual void Display(Renderer* r) const override;
};

struct ImagePrinter : IODevice {
    ImagePrinter(const char* filename) : m_Filename(filename) {}
    virtual void Display(Renderer* r) const override;
    std::string m_Filename;
};

struct Terminal : IODevice {
    Terminal(const char* title) : m_Title(title) {}
    virtual void Display(Renderer* r) const override;
    std::string m_Title;
};

}   // namespace SyncX