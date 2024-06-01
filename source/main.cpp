#include <chrono>

#include "Common.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Material.h"

int main()
{
    // Scene

    HittableList scene{};

    auto materialGround{ std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0)) };
    auto materialCenter{ std::make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5)) };
    auto materialLeft{ std::make_shared<Metal>(Vec3(0.8, 0.8, 0.8)) };
    auto materialRight{ std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2)) };

    scene.add(std::make_shared<Sphere>(Sphere({ 0.0, -100.5, -1.0 }, 100.0, materialGround)));
    scene.add(std::make_shared<Sphere>(Sphere({ 0.0, 0.0, -1.2 }, 0.5, materialCenter)));
    scene.add(std::make_shared<Sphere>(Sphere({ -1.0, 0., -1.0 }, 0.5, materialLeft)));
    scene.add(std::make_shared<Sphere>(Sphere({ 1.0, 0., -1.0 }, 0.5, materialRight)));

    // Render

    CameraParams params{
        .position       = Vec3(0., 0., 0.),
        .aspectRatio    = 16. / 9.,
        .imageHeight    = 720,
        .viewportHeight = 2.,
        .focalLength    = 1.,
        .numSamples     = 50,
        .maxRayDepth    = 10,
    };
    Camera camera{ params };

    auto t0{ std::chrono::steady_clock::now() };
    camera.render(scene);
    auto t1{ std::chrono::steady_clock::now() };

    std::clog << "Render time: " << std::chrono::duration<double, std::milli>(t1 - t0) << " "
              << std::endl;

    std::ignore = std::getchar();
}
