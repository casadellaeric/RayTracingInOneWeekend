#include <chrono>

#define STB_IMAGE_IMPLEMENTATION

#include "Material.h"
#include "Common.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Node.h"

HittableList sceneDielectrics()
{
    HittableList scene{};

    auto materialGround{ std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0)) };
    auto materialCenter{ std::make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5)) };
    auto materialLeft{ std::make_shared<Dielectric>(1.50) };
    auto materialBubble{ std::make_shared<Dielectric>(1.00 / 1.50) };
    auto materialRight{ std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 1.) };

    scene.add(std::make_shared<Sphere>(Sphere({ 0.0, -100.5, -1.0 }, 100.0, materialGround)));
    scene.add(std::make_shared<Sphere>(Sphere({ 0.0, 0.0, -1.2 }, 0.5, materialCenter)));
    scene.add(std::make_shared<Sphere>(Sphere({ -1.0, 0., -1.0 }, 0.5, materialLeft)));
    scene.add(std::make_shared<Sphere>(Sphere({ -1.0, 0.0, -1.0 }, 0.4, materialBubble)));
    scene.add(std::make_shared<Sphere>(Sphere({ 1.0, 0., -1.0 }, 0.5, materialRight)));

    return scene;
}

HittableList sceneFov()
{
    HittableList scene{};

    auto materialLeft{ std::make_shared<Lambertian>(Vec3(0, 0, 1)) };
    auto materialRight{ std::make_shared<Lambertian>(Vec3(1, 0, 0)) };

    auto R{ std::cos(std::numbers::pi / 4) };
    scene.add(std::make_shared<Sphere>(Sphere(Vec3(-R, 0, -1), R, materialLeft)));
    scene.add(std::make_shared<Sphere>(Sphere(Vec3(R, 0, -1), R, materialRight)));

    return scene;
}

std::pair<HittableList, CameraParams> sceneFinalBook1()
{
    HittableList scene{};

    auto checkerTexture{
        std::make_shared<CheckerTexture>(0.32, Vec3{ .2, .3, .1 }, Vec3{ .9, .9, .9 })
    };
    auto materialGround{ std::make_shared<Lambertian>(checkerTexture) };
    scene.add(std::make_shared<Sphere>(Sphere({ 0., -1000., 0. }, 1000., materialGround)));

    auto sphereMatDielectric{ std::make_shared<Dielectric>(1.5) };
    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            Vec3 spherePos{ a + 0.9 * random_double(), 0.2, b + 0.9 * random_double() };

            if ((spherePos - Vec3(4, 0.2, 0)).length() < 0.9) {
                continue;
            }

            std::shared_ptr<Material> sphereMat;
            auto sphereMatRand{ random_double() };
            if (sphereMatRand < 0.8) {
                auto color{ random_vec() * random_vec() };
                sphereMat = std::make_shared<Lambertian>(color);

                Vec3 spherePos2{
                    spherePos + Vec3{0., random_double(0., 0.5), 0.}
                };
                scene.add(std::make_shared<Sphere>(Sphere(spherePos, spherePos2, 0.2, sphereMat)));
            } else if (sphereMatRand < 0.95) {
                auto color{ random_vec(0.5, 1.) };
                auto fuzz{ random_double(0., 0.5) };
                sphereMat = std::make_shared<Metal>(color, fuzz);
                scene.add(std::make_shared<Sphere>(Sphere(spherePos, 0.2, sphereMat)));
            } else {
                sphereMat = sphereMatDielectric;
                scene.add(std::make_shared<Sphere>(Sphere(spherePos, 0.2, sphereMat)));
            }
        }
    }

    auto material1{ std::make_shared<Dielectric>(1.5) };
    scene.add(std::make_shared<Sphere>(Vec3(0., 1., 0.), 1.0, material1));

    auto material2{ std::make_shared<Lambertian>(Vec3(0.4, 0.2, 0.1)) };
    scene.add(std::make_shared<Sphere>(Vec3(-4., 1., 0.), 1.0, material2));

    auto material3{ std::make_shared<Metal>(Vec3(0.7, 0.6, 0.5), 0.0) };
    scene.add(std::make_shared<Sphere>(Vec3(4., 1., 0.), 1.0, material3));

    CameraParams params{
        .position     = Vec3(13., 2., 3.),
        .lookAt       = Vec3(0., 0., 0.),
        .up           = Vec3(0., 1., 0.),
        .aspectRatio  = 16. / 9.,
        .imageHeight  = 720,
        .defocusAngle = 0.6,
        .focusDist    = 10.,
        .vFov         = 20,
        .numSamples   = 100,
        .maxRayDepth  = 30,
    };

    return std::make_pair(scene, params);
}

std::pair<HittableList, CameraParams> sceneCheckeredSpheres()
{
    HittableList scene{};

    auto checkerTexture{
        std::make_shared<CheckerTexture>(0.32, Vec3{ .2, .3, .1 }, Vec3{ .9, .9, .9 })
    };
    scene.add(std::make_shared<Sphere>(
        Sphere({ 0., -10., 0. }, 10., std::make_shared<Lambertian>(checkerTexture))));
    scene.add(std::make_shared<Sphere>(
        Sphere({ 0., 10., 0. }, 10., std::make_shared<Lambertian>(checkerTexture))));

    CameraParams params{
        .position     = Vec3(13., 2., 3.),
        .lookAt       = Vec3(0., 0., 0.),
        .up           = Vec3(0., 1., 0.),
        .aspectRatio  = 16. / 9.,
        .imageHeight  = 720,
        .defocusAngle = 0.,
        .focusDist    = 10.,
        .vFov         = 20,
        .numSamples   = 100,
        .maxRayDepth  = 30,
    };

    return std::make_pair(scene, params);
}

std::pair<HittableList, CameraParams> sceneEarth()
{
    HittableList scene{};

    auto earthTexture{ std::make_shared<ImageTexture>("../textures/earthmap.jpg") };
    scene.add(std::make_shared<Sphere>(
        Sphere({ 0., 0., 0. }, 2., std::make_shared<Lambertian>(earthTexture))));

    CameraParams params{
        .position     = Vec3(0., 0., 12.),
        .lookAt       = Vec3(0., 0., 0.),
        .up           = Vec3(0., 1., 0.),
        .aspectRatio  = 16. / 9.,
        .imageHeight  = 720,
        .defocusAngle = 0.,
        .focusDist    = 10.,
        .vFov         = 20,
        .numSamples   = 100,
        .maxRayDepth  = 30,
    };

    return std::make_pair(scene, params);
}

std::pair<HittableList, CameraParams> scenePerlinSpheres()
{
    HittableList scene{};

    auto noiseTexture{ std::make_shared<NoiseTexture>(4.) };
    scene.add(std::make_shared<Sphere>(
        Sphere({ 0., -1000., 0. }, 1000., std::make_shared<Lambertian>(noiseTexture))));
    scene.add(std::make_shared<Sphere>(
        Sphere({ 0., 2., 0. }, 2., std::make_shared<Lambertian>(noiseTexture))));

    CameraParams params{
        .position     = Vec3(13., 2., 3.),
        .lookAt       = Vec3(0., 0., 0.),
        .up           = Vec3(0., 1., 0.),
        .aspectRatio  = 16. / 9.,
        .imageHeight  = 720,
        .defocusAngle = 0.,
        .focusDist    = 10.,
        .vFov         = 20,
        .numSamples   = 100,
        .maxRayDepth  = 30,
    };

    return std::make_pair(scene, params);
}

int main()
{
    auto [scene, camParams]{ scenePerlinSpheres() };

    camParams.numSamples = 20;

    Camera camera{ camParams };
    auto t0{ std::chrono::steady_clock::now() };
    camera.render(scene);
    auto t1{ std::chrono::steady_clock::now() };

    std::clog << "Render time: " << std::chrono::duration<double, std::milli>(t1 - t0) << " "
              << std::endl;

    std::ignore = std::getchar();
}