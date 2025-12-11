#include "Sphere.h"
#include "Material.h"

bool Sphere::Hit(const Ray& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit) {
    glm::vec3 oc = ray.origin - transform.position;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.0f * glm::dot(ray.direction, oc);
    float c = glm::dot(oc, oc) - (radius * radius);

    float discriminant = (b * b) - 4.0f * a * c;

    if (discriminant >= 0.0f) {
        float sqrtDisc = sqrt(discriminant);

        float t = (-b - sqrtDisc) / (2.0f * a);
        if (t > minDistance && t < maxDistance) {
            raycastHit.distance = t;
            raycastHit.point = ray.origin + t * ray.direction;
            raycastHit.normal = glm::normalize(raycastHit.point - transform.position);
            raycastHit.material = material;
            return true;
        }

        t = (-b + sqrtDisc) / (2.0f * a);
        if (t > minDistance && t < maxDistance) {
            raycastHit.distance = t;
            raycastHit.point = ray.origin + t * ray.direction;
            raycastHit.normal = glm::normalize(raycastHit.point - transform.position);
            raycastHit.material = material;
            return true;
        }
    }
    return false;

}
