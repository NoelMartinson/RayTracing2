#pragma once
#include <cstdlib>
#include <random>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace random {
    inline std::mt19937& generator() {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        return gen;
    }

    inline void seed(unsigned int value) {
        generator().seed(value);
    }
    
    inline int getInt(int min, int max) {
        std::uniform_int_distribution<> dist(min, max);
        return dist(generator());
    }

    inline int getInt(int max) {
        return getInt(0, max - 1);
    }

    inline int getInt() {
        static std::uniform_int_distribution<> dist;
        return dist(generator());
    }

    template <typename T = float>
    inline T getReal(T min, T max) {
        std::uniform_real_distribution<T> dist(min, max);
        return dist(generator());
    }

    template <typename T = float>
    inline T getReal(T max) {
        return getReal(static_cast<T>(0), static_cast<T>(max));
    }

    template <typename T = float>
    inline T getReal() {
        static std::uniform_real_distribution<T> dist(static_cast<T>(0), static_cast<T>(1));
        return dist(generator());
    }

    inline glm::vec3 getReal(const glm::vec3 min, const glm::vec3 max) {
        return glm::vec3{
            getReal(min.x, max.x),
            getReal(min.y, max.y),
            getReal(min.z, max.z)
        };
    }

    inline bool getBool() {
        static std::bernoulli_distribution dist(0.5);
        return dist(generator());
    }

    inline glm::vec2 onUnitCircle() {
        float radians = getReal(glm::two_pi<float>());

        return glm::vec2{ std::cos(radians), std::sin(radians) };
    }

    inline glm::vec3 getRandomPointInUnitSphere() {
        while (true) {
            glm::vec3 p{
                getReal(-1.0f, 1.0f),
                getReal(-1.0f, 1.0f),
                getReal(-1.0f, 1.0f)
            };
            if (glm::dot(p, p) < 1.0f) return p;
        }
    }

    inline glm::vec3 getRandomPointOnUnitSphere() {
        glm::vec3 p = getRandomPointInUnitSphere();
        return glm::normalize(p);
    }

}