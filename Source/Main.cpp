#include "Renderer.h"
#include "Framebuffer.h"
#include "Camera.h"
#include "Scene.h"
#include "Sphere.h"
#include "Plane.h"
#include "Random.h"
#include "Material.h"
#include "Object.h"
#include "Transform.h"

#include <SDL3/SDL.h>
#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include <iostream>
#include <chrono>

int main()
{
    constexpr int SCREEN_WIDTH = 800;
    constexpr int SCREEN_HEIGHT = 600;

    Renderer renderer;
    renderer.Initialize();
    renderer.CreateWindow("Ray Tracer Showcase", SCREEN_WIDTH, SCREEN_HEIGHT);

    Framebuffer framebuffer(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    float aspectRatio = framebuffer.width / float(framebuffer.height);
    Camera camera(50.0f, aspectRatio);

    camera.SetView(
        { 13.0f, 3.0f, 6.0f },   
        { 0.0f, 1.0f, 0.0f }  
    );

    Scene scene;

    auto redLambert = std::make_shared<Lambertian>(color3_t{ 0.9f, 0.2f, 0.2f });
    auto glassMat = std::make_shared<Dielectric>(1.5f);
    auto metalMat = std::make_shared<Metal>(color3_t{ 0.9f, 0.9f, 0.9f }, 0.05f);
    auto lightMat = std::make_shared<Emissive>(color3_t{ 1.0f, 1.0f, 1.0f }, 3.0f);

    scene.AddObject(std::make_unique<Sphere>(
        Transform{ glm::vec3{-3.0f, 1.0f,  0.0f} },
        1.0f,
        redLambert
    ));

    scene.AddObject(std::make_unique<Sphere>(
        Transform{ glm::vec3{ 0.0f, 1.0f,  0.0f} },
        1.0f,
        glassMat
    ));

    scene.AddObject(std::make_unique<Sphere>(
        Transform{ glm::vec3{ 3.0f, 1.0f,  0.0f} },
        1.0f,
        metalMat
    ));

    scene.AddObject(std::make_unique<Sphere>(
        Transform{ glm::vec3{ 6.0f, 1.0f,  0.0f} },
        1.0f,
        lightMat
    ));

    for (int a = -15; a < 15; a++)
    {
        for (int b = -15; b < 15; b++)
        {
            if (random::getReal(0, 1) < 0.8f) continue;

            glm::vec3 center(
                a + random::getReal(0.0f, 0.9f),
                0.2f,
                b + random::getReal(0.0f, 0.9f)
            );

            if (glm::length(center - glm::vec3(0, 1, 0)) < 2.5f) continue;
            if (glm::length(center - glm::vec3(-3, 1, 0)) < 2.5f) continue;
            if (glm::length(center - glm::vec3(3, 1, 0)) < 2.5f) continue;

            float choose = random::getReal(0.0f, 1.0f);
            std::shared_ptr<Material> mat;

            if (choose < 0.7f)
            {
                mat = std::make_shared<Lambertian>(color3_t{
                    random::getReal(0.2f, 1.0f),
                    random::getReal(0.2f, 1.0f),
                    random::getReal(0.2f, 1.0f)
                    });
            }
            else if (choose < 0.9f)
            {
                mat = std::make_shared<Metal>(color3_t{
                    random::getReal(0.5f,1.0f),
                    random::getReal(0.5f,1.0f),
                    random::getReal(0.5f,1.0f)
                    }, random::getReal(0.0f, 0.4f));
            }
            else
            {
                mat = std::make_shared<Dielectric>(1.5f);
            }

            scene.AddObject(std::make_unique<Sphere>(
                Transform{ center },
                0.2f,
                mat
            ));
        }
    }

    auto groundMat = std::make_shared<Lambertian>(color3_t{ 0.5f,0.5f,0.5f });
    scene.AddObject(std::make_unique<Plane>(
        Transform{ glm::vec3{0.0f, 0.0f, 0.0f} },
        groundMat
    ));

    SDL_Event event;
    bool quit = false;

    framebuffer.ResetAccumulation();

    int sample = 0;
    const int MaxSamples = 200;  

    while (!quit && sample < MaxSamples)
    {
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT) quit = true;
            if (event.type == SDL_EVENT_KEY_DOWN &&
                event.key.scancode == SDL_SCANCODE_ESCAPE)
            {
                quit = true;
            }
        }

        auto start = std::chrono::high_resolution_clock::now();

        framebuffer.sampleCount++;
        scene.Render(framebuffer, camera, 400);

        auto end = std::chrono::high_resolution_clock::now();

        sample++;

        std::cout << "Sample " << sample << " / " << MaxSamples
            << " | Frame time: "
            << std::chrono::duration<double>(end - start).count()
            << " sec\n";

        framebuffer.Update();
        renderer.CopyFramebuffer(framebuffer);
        renderer.Show();
    }

    return 0;
}
