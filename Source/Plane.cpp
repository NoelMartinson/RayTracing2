#include "Plane.h"

bool Plane::Hit(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) {
    float t;

    glm::vec3 center = transform.position;
    glm::vec3 normal = transform.up();        

    if (!Raycast(ray, center, normal, minDistance, maxDistance, t))
        return false;

    // fill hit record
    raycastHit.distance = t;
    raycastHit.point = ray.origin + t * ray.direction;
    raycastHit.normal = normal;
    raycastHit.material = material; 

    return true;
}


bool Plane::Raycast(const Ray& ray, const glm::vec3& planePoint, const glm::vec3& planeNormal,
    float minDistance, float maxDistance, float& t)
{
    float denominator = glm::dot(ray.direction, planeNormal);

    if (std::abs(denominator) < glm::epsilon<float>()) {
        return false;
    }

    t = glm::dot(planePoint - ray.origin, planeNormal) / denominator;

    return t >= minDistance && t <= maxDistance;
}

