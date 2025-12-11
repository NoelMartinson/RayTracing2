#include "Plane.h"

bool Plane::Hit(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) {
    float t;

    // Plane center position and normal (assuming you store a Transform)
    glm::vec3 center = transform.position;       // plane point
    glm::vec3 normal = transform.up();           // plane normal

    // check ray-plane intersection
    if (!Raycast(ray, center, normal, minDistance, maxDistance, t))
        return false;

    // fill hit record
    raycastHit.distance = t;
    raycastHit.point = ray.origin + t * ray.direction;
    raycastHit.normal = normal;
    raycastHit.material = material; // your material pointer

    return true;
}


bool Plane::Raycast(const Ray& ray, const glm::vec3& planePoint, const glm::vec3& planeNormal,
    float minDistance, float maxDistance, float& t)
{
    float denominator = glm::dot(ray.direction, planeNormal);

    // ray is parallel to plane
    if (std::abs(denominator) < glm::epsilon<float>()) {
        return false;
    }

    // t = (planePoint - rayOrigin) · planeNormal / (rayDirection · planeNormal)
    t = glm::dot(planePoint - ray.origin, planeNormal) / denominator;

    // check distance bounds
    return t >= minDistance && t <= maxDistance;
}

