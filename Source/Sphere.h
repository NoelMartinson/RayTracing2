#pragma once
#include "Object.h"
#include "Ray.h"
#include <glm/glm.hpp>

class Sphere : public Object {
public:
    Sphere() = default;

    Sphere(const Transform& transform, float radius, std::shared_ptr<Material> material)
        : Object(transform, material), radius(radius) {
    }

    bool Hit(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) override;

public:
    float radius{ 0 };
};
