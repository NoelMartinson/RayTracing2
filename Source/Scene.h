#pragma once
#include "Color.h"
#include "Object.h"
#include <vector>
#include <memory>

class Framebuffer;
class Camera;

class Scene {
public:
    Scene() = default;

    void Render(Framebuffer& framebuffer, const Camera& camera, int numSamples = 10);
    void AddObject(std::unique_ptr<Object> object);
    color3_t Trace(const struct Ray& ray, float minDistance, float maxDistance, int maxDepth = 5);

public:
    std::vector<std::unique_ptr<Object>> objects;

    color3_t skyBottom{ 0.1f, 0.2f, 0.4f };
    color3_t skyTop{ 0.5f, 0.7f, 1.0f };
};
