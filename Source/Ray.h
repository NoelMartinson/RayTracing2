#pragma once
#include "Color.h"


#include <memory>
#include <glm/vec3.hpp>

using glm::vec3;

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    Ray() = default;
    Ray(const glm::vec3& origin, const glm::vec3& direction)
        : origin(origin), direction(direction) {
    }

    glm::vec3 at(float t) const { return origin + t * direction; }

    glm::vec3 operator*(float t) const { return at(t); }
};

struct raycastHit_t
{
    glm::vec3 point;     
    glm::vec3 normal;     
    float distance = 0;   

    std::shared_ptr<class Material> material;
};
