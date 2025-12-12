#pragma once
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <vector>

class Framebuffer
{
public:
    Framebuffer(const class Renderer& renderer, int width, int height);
    ~Framebuffer();

    void Update();

    void Clear(const SDL_Color& color = { 0, 0, 0, 255 });

    void DrawPoint(int x, int y, const SDL_Color& color);

    void WritePixel(int x, int y, const glm::vec3& hdrColor);

    void ResetAccumulation();

public:
    int width{ 0 };
    int height{ 0 };
    int pitch{ 0 };

    SDL_Texture* texture{ nullptr };

    std::vector<SDL_Color> buffer;

    std::vector<glm::vec3> accumulation;

    int sampleCount = 0;
};
