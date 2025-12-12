#include "Scene.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Color.h"
#include "Object.h"
#include "Ray.h"
#include "Material.h"
#include "Random.h"

#include <iostream>

void Scene::Render(Framebuffer& framebuffer, const Camera& camera, int numSamples) {
    const float maxDistance = 100.0f;

    for (int y = 0; y < framebuffer.height; y++) {
        for (int x = 0; x < framebuffer.width; x++) {
            color3_t color{ 0.0f };

            for (int s = 0; s < numSamples; s++) {
                glm::vec2 pixel{ static_cast<float>(x), static_cast<float>(y) };
                pixel += glm::vec2(random::getReal(0.0f, 1.0f), random::getReal(0.0f, 1.0f));

                glm::vec2 uv = pixel / glm::vec2(framebuffer.width, framebuffer.height);

                Ray ray = camera.GetRay(uv);

                color += Trace(ray, 0.001f, maxDistance, 5);
            }

            color /= static_cast<float>(numSamples);

            color = glm::sqrt(color);

            framebuffer.DrawPoint(x, y, ColorConvert(color));
        }
    }
}


void Scene::AddObject(std::unique_ptr<Object> object) {
	objects.push_back(std::move(object));
}

color3_t Scene::Trace(const Ray& ray, float minDistance, float maxDistance, int maxDepth) {

    if (maxDepth == 0) return color3_t(0.0f, 0.0f, 0.0f);

    bool rayHit = false;
    float closestDistance = maxDistance;
    raycastHit_t raycastHit;

    for (auto& object : objects) {
        raycastHit_t hit;
        if (object->Hit(ray, minDistance, closestDistance, hit)) {
            rayHit = true;
            closestDistance = hit.distance;
            raycastHit = hit;
        }
    }

    if (rayHit) {
        color3_t attenuation;
        Ray scattered;
        if (raycastHit.material->Scatter(ray, raycastHit, attenuation, scattered)) {
            return attenuation * Trace(scattered, minDistance, maxDistance, maxDepth - 1);
        }
        else {
            return raycastHit.material->GetEmissive();
        }
    }

    float t = glm::clamp(ray.direction.y * 0.5f + 0.5f, 0.0f, 1.0f);
    color3_t sky = glm::mix(skyBottom, skyTop, t);
    return sky;
}