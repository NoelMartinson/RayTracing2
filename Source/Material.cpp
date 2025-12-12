#include "Material.h"
#include "Random.h"
#include <iostream>

bool Lambertian::Scatter(const Ray& incident, const raycastHit_t& raycastHit, color3_t& attenuation, Ray& scattered) const {
    // set scattered ray using random direction from normal, diffuse the outgoing ray
    scattered.origin = raycastHit.point; // set raycast hit point
    scattered.direction = glm::normalize(raycastHit.normal + random::getRandomPointOnUnitSphere()); // using random point on unit sphere

    attenuation = albedo;

    return true;
}

bool Metal::Scatter(const Ray& incident, const raycastHit_t& raycastHit, color3_t& attenuation, Ray& scattered) const {
    glm::vec3 reflected = glm::reflect(glm::normalize(incident.direction), raycastHit.normal); // use glm reflect function

    // set scattered ray from reflected ray + random point in sphere (fuzz = 0 no randomness, fuzz = 1 random reflected)
    // a mirror has a fuzz value of 0 and a diffused metal surface a higher value
    scattered.origin = raycastHit.point; // set raycast hit point
    scattered.direction = glm::normalize(reflected + random::getRandomPointOnUnitSphere() * fuzz); // using random point on unit sphere

    attenuation = albedo;

    // check that reflected ray is going away from surface normal (dot product (scattered direction, normal) > 0)
    return glm::dot(scattered.direction, raycastHit.normal) > 0; // updated to correct the return condition
}

static float Schlick(float cosine, float refIdx) {
    float r0 = (1.0f - refIdx) / (1.0f + refIdx);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow(1.0f - cosine, 5.0f);
}

bool Dielectric::Scatter(const Ray& incident,
    const raycastHit_t& hit,
    color3_t& attenuation,
    Ray& scattered) const
{
    attenuation = albedo; 

    glm::vec3 unitDirection = glm::normalize(incident.direction);
    glm::vec3 normal = hit.normal;

    float refRatio = glm::dot(unitDirection, normal) < 0 ?
        1.0f / refractiveIndex :
        refractiveIndex;

    bool frontFace = glm::dot(unitDirection, normal) < 0;
    if (!frontFace) normal = -normal;

    float cosTheta = glm::min(-glm::dot(unitDirection, normal), 1.0f);
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);

    bool cannotRefract = refRatio * sinTheta > 1.0f;

    glm::vec3 direction;
    if (cannotRefract || random::getReal(0.0f, 1.0f) < Schlick(cosTheta, refractiveIndex)) {
        direction = glm::reflect(unitDirection, normal);
    }
    else {
        direction = glm::refract(unitDirection, normal, refRatio);
    }

    scattered = Ray(hit.point, direction);
    return true;
}
