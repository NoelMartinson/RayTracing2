# Computer-Graphics-Software-Renderer
Introduction

This repository contains two custom0built grpahics projects. A software rasterizer and a CPU-based ray tracer. Both systems are written from scratch using SDL, OpenGL, and supporting libraries such as glm for math.
The goal of this repository is to demonstrate a complete rendering pipeline from rasterization to full ray tracing while showcasing lighting, models, material handling, shadow techniques, and real time scene interaction.

---
Features
Software Renderer

Custom rasterization pipeline (vertex -> triangle -> fragment).

Shader-like system supporting:

Vertex shading

Gouraud shading

Phong shading

Perspective-correct texture mapping.

Material system with diffuse/specular properties.

Post-processing effects such as:

Grayscale

Inversion

Sharpen/Blur (kernel convolution)

Support for importing OBJ models.

Interactive 3D scene navigation.

Ray Tracer

Recursive ray tracing pipeline.

Multiple material types:

Diffuse (Lambertian)

Metallic (with fuzziness)

Dielectric/Glass (refraction)

Anti-aliasing through stochastic sampling.

Support for shadows, reflections, and global illumination basics.

Configurable per-frame sample count.

CPU accelerated but optimized through multithreading (if applicable).
---
Installation
Requirements

Visual Studio 2022 (or newer)

C++20 (or C++17 if your project targets it)

SDL3 or SDL2 (depending on your build setup)

glm

CMake (optional if not using Visual Studio project files)

Steps

Clone the repository

git clone https://github.com/NoelMartinson/Computer-Graphics-Software-Renderer.git


Install dependencies

Download and install SDL development libraries.

Ensure glm is available in your include path.

Open the solution

Open the .sln file in Visual Studio.

Set the Startup Project

Choose either the SoftwareRenderer or RayTracer project.

Build & Run

Press F5 to compile and launch.
---
Technical Details
Shader Support in the Software Renderer

Vertex shader performs model-view-projection transformations.

Fragment shader computes final color per pixel.

Shading Pipeline:

Gouraud shading: lighting computed per vertex then interpolated.

Phong shading: normals interpolated, lighting computed per pixel.

Lighting Techniques

Ambient + Diffuse + Specular components.

Blinn–Phong and Phong specular models supported.

Multiple light sources possible depending on scene configuration.

Ray Tracer Material System

Lambertian materials scatter rays with cosine-weighted randomness.

Metal materials reflect incoming rays with adjustable roughness.

Dielectric materials support refraction using Snell's Law + Schlick’s approximation.

Supports recursive depth limiting to prevent infinite bounces.

Anti-Aliasing

Pixel color is computed from multiple rays jittered within the pixel.

Final color is averaged, reducing jagged edges and noise.

Optional progressive sampling for real-time accumulation.
---
Visual Showcase: 

<img width="277" height="278" alt="image" src="https://github.com/user-attachments/assets/ac008add-6779-46d0-b4b0-36122415312f" />
Fully rendered cartoon cow with shadow and lighting

<img width="505" height="440" alt="image" src="https://github.com/user-attachments/assets/e6255d36-410d-44dc-8dad-195eb731eb19" />
Shadow casting, emmissive and normal mapping.

---
Attribution

This project uses:

SDL for window creation and framebuffer display

glm for vector/matrix math

---
Conclusion

This repository serves as a deep dive into how 3D rendering works under the hood, from triangle rasterization to full ray tracing.
Feel free to explore the codebase, submit issues, and open pull requests for improvements or new features.
