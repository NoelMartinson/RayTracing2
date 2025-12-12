#include "Framebuffer.h"
#include "Renderer.h"
#include <iostream>
#include <algorithm>

Framebuffer::Framebuffer(const Renderer& renderer, int width, int height)
{
    this->width = width;
    this->height = height;
    this->pitch = width * sizeof(SDL_Color);

    texture = SDL_CreateTexture(
        renderer.renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        width, height
    );

    if (!texture)
    {
        std::cerr << "Error creating SDL texture: " << SDL_GetError() << std::endl;
    }

    buffer.resize(width * height);

    accumulation.resize(width * height, glm::vec3(0.0f));

    sampleCount = 0;
}

Framebuffer::~Framebuffer()
{
    SDL_DestroyTexture(texture);
}

void Framebuffer::ResetAccumulation()
{
    std::fill(accumulation.begin(), accumulation.end(), glm::vec3(0.0f));
    sampleCount = 0;
}

void Framebuffer::WritePixel(int x, int y, const glm::vec3& hdrColor)
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;

    int index = y * width + x;

    accumulation[index] += hdrColor;

    glm::vec3 avg = accumulation[index] / float(sampleCount);

    avg = glm::clamp(avg, glm::vec3(0.0f), glm::vec3(1.0f));

    avg = glm::pow(avg, glm::vec3(1.0f / 2.2f));

    SDL_Color c;
    c.r = static_cast<Uint8>(avg.r * 255.0f);
    c.g = static_cast<Uint8>(avg.g * 255.0f);
    c.b = static_cast<Uint8>(avg.b * 255.0f);
    c.a = 255;

    buffer[index] = c;
}

void Framebuffer::Update()
{
    SDL_UpdateTexture(texture, nullptr, buffer.data(), pitch);
}

void Framebuffer::Clear(const SDL_Color& color)
{
    std::fill(buffer.begin(), buffer.end(), color);
}

void Framebuffer::DrawPoint(int x, int y, const SDL_Color& color)
{
    if (x < 0 || x >= width || y < 0 || y >= height) {
        std::cerr << "pixel out of bounds - x: " << x << " y: " << y << ".\n";
        return;
    }

    buffer[x + y * width] = color;
}
