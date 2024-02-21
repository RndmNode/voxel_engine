#pragma once

#include "renderer.h"

class Texture
{
private:
    unsigned int m_RendererID;
    std::string m_FilePath;
    unsigned char* m_LocalBuffer;
    int m_Width, m_Height, m_BPP;
public:
    Texture(std::string& path);
    ~Texture();

    void Bind(unsigned int slot = 0) const;
    void Unbind();

    inline int GetWidth() { return m_Width; }
    inline int GetHeight() { return m_Height; }
};