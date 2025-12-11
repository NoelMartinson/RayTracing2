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
            color3_t color{ 0.0f }; // accumulator for the pixel

            // --- MULTI-SAMPLING LOOP ---
            for (int s = 0; s < numSamples; s++) {
                // pixel coordinates with random offset
                glm::vec2 pixel{ static_cast<float>(x), static_cast<float>(y) };
                pixel += glm::vec2(random::getReal(0.0f, 1.0f), random::getReal(0.0f, 1.0f));

                // normalize to 0..1
                glm::vec2 uv = pixel / glm::vec2(framebuffer.width, framebuffer.height);

                // Remove y-flip if camera appears upside down
                // uv.y = 1.0f - uv.y; // <-- commented out to fix upside-down issue

                // generate ray from camera
                Ray ray = camera.GetRay(uv);

                // accumulate traced color
                color += Trace(ray, 0.001f, maxDistance, 5);
            }

            // average color over all samples
            color /= static_cast<float>(numSamples);

            // optional gamma correction (gamma 2.0)
            color = glm::sqrt(color);

            // write color to framebuffer
            framebuffer.DrawPoint(x, y, ColorConvert(color));
        }
    }
}


void Scene::AddObject(std::unique_ptr<Object> object) {
	// add object to objects vector
	objects.push_back(std::move(object));
}

color3_t Scene::Trace(const Ray& ray, float minDistance, float maxDistance, int maxDepth) {

    // reached max depth (bounce) return black color
    if (maxDepth == 0) return color3_t(0.0f, 0.0f, 0.0f);

    bool rayHit = false;
    float closestDistance = maxDistance;
    raycastHit_t raycastHit;

    // check if scene objects are hit by the ray
    for (auto& object : objects) {
        raycastHit_t hit;
        if (object->Hit(ray, minDistance, closestDistance, hit)) {
            rayHit = true;
            closestDistance = hit.distance;
            raycastHit = hit;
        }
    }

    // check if ray hit object
    if (rayHit) {
        color3_t attenuation;
        Ray scattered;
        // get raycast hit matereial, get material color and scattered ray 
        if (raycastHit.material->Scatter(ray, raycastHit, attenuation, scattered)) {
            // trace scattered ray, final color will be the product of all the material colors
            return attenuation * Trace(scattered, minDistance, maxDistance, maxDepth - 1);
        }
        else {
            return raycastHit.material->GetEmissive();
        }
    }

    // no ray hit, draw sky colors based on the ray y position
    color3_t sky = glm::mix(skyTop, skyBottom, ray.direction.y);
    return sky;
}