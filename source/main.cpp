#include <chrono>

#define STB_IMAGE_IMPLEMENTATION

#include "Material.h"
#include "Common.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Node.h"
#include "Quad.h"
#include "ConstantMedium.h"

std::pair<HittableList, CameraParams> sceneDielectrics()
{
    HittableList scene{};

    auto materialGround{ std::make_shared<Lambertian>(Vec3(0.8, 0.8, 0.0)) };
    auto materialCenter{ std::make_shared<Lambertian>(Vec3(0.1, 0.2, 0.5)) };
    auto materialLeft{ std::make_shared<Dielectric>(1.50) };
    auto materialBubble{ std::make_shared<Dielectric>(1.00 / 1.50) };
    auto materialRight{ std::make_shared<Metal>(Vec3(0.8, 0.6, 0.2), 1.) };

    scene.add(std::make_shared<Sphere>(Sphere({ 0.0, -100.5, -1.0 }, 100.0, materialGround)));
    scene.add(std::make_shared<Sphere>(Sphere({ 0.0, 0., -1.2 }, 0.5, materialCenter)));
    scene.add(std::make_shared<Sphere>(Sphere({ -1.0, 0., -1.0 }, 0.5, materialLeft)));
    scene.add(std::make_shared<Sphere>(Sphere({ -1.0, 0., -1.0 }, 0.4, materialBubble)));
    scene.add(std::make_shared<Sphere>(Sphere({ 1.0, 0., -1.0 }, 0.5, materialRight)));

    CameraParams params{
        .position        = Vec3(0., 0., 0.),
        .lookAt          = Vec3(0., 0., -1.),
        .up              = Vec3(0., 1., 0.),
        .aspectRatio     = 16. / 9.,
        .imageHeight     = 480,
        .defocusAngle    = 0.,
        .focusDist       = 1.,
        .vFov            = 90,
        .numSamples      = 50,
        .maxRayDepth     = 20,
        .backgroundColor = Vec3{0.70, 0.80, 1.00}
    };

    return std::make_pair(scene, params);
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
                auto albedo{ random_vec() * random_vec() };
                sphereMat = std::make_shared<Lambertian>(albedo);
                Vec3 spherePos2{
                    spherePos + Vec3{0., random_double(0., 0.5), 0.}
                };
                scene.add(std::make_shared<Sphere>(Sphere(spherePos, spherePos2, 0.2, sphereMat)));
            } else if (sphereMatRand < 0.95) {
                auto Vec3{ random_vec(0.5, 1.) };
                auto fuzz{ random_double(0., 0.5) };
                sphereMat = std::make_shared<Metal>(Vec3, fuzz);
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
        .position        = Vec3(13., 2., 3.),
        .lookAt          = Vec3(0., 0., 0.),
        .up              = Vec3(0., 1., 0.),
        .aspectRatio     = 16. / 9.,
        .imageHeight     = 720,
        .defocusAngle    = 0.6,
        .focusDist       = 10.,
        .vFov            = 20,
        .numSamples      = 100,
        .maxRayDepth     = 30,
        .backgroundColor = Vec3{0.70, 0.80, 1.00}
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
        .position        = Vec3(13., 2., 3.),
        .lookAt          = Vec3(0., 0., 0.),
        .up              = Vec3(0., 1., 0.),
        .aspectRatio     = 16. / 9.,
        .imageHeight     = 720,
        .defocusAngle    = 0.,
        .focusDist       = 10.,
        .vFov            = 20,
        .numSamples      = 100,
        .maxRayDepth     = 30,
        .backgroundColor = Vec3{0.70, 0.80, 1.00}
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
        .position        = Vec3(0., 0., 12.),
        .lookAt          = Vec3(0., 0., 0.),
        .up              = Vec3(0., 1., 0.),
        .aspectRatio     = 16. / 9.,
        .imageHeight     = 720,
        .defocusAngle    = 0.,
        .focusDist       = 10.,
        .vFov            = 20,
        .numSamples      = 100,
        .maxRayDepth     = 30,
        .backgroundColor = Vec3{0.70, 0.80, 1.00}
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
        .position        = Vec3(13., 2., 3.),
        .lookAt          = Vec3(0., 0., 0.),
        .up              = Vec3(0., 1., 0.),
        .aspectRatio     = 16. / 9.,
        .imageHeight     = 720,
        .defocusAngle    = 0.,
        .focusDist       = 10.,
        .vFov            = 20,
        .numSamples      = 100,
        .maxRayDepth     = 30,
        .backgroundColor = Vec3{0.70, 0.80, 1.00}
    };

    return std::make_pair(scene, params);
}

std::pair<HittableList, CameraParams> sceneQuads()
{
    HittableList scene{};

    auto backGreen{ std::make_shared<Lambertian>(Vec3{ 0.2, 1.0, 0.2 }) };
    auto leftRed{ std::make_shared<Lambertian>(Vec3{ 1.0, 0.2, 0.2 }) };
    auto rightBlue{ std::make_shared<Lambertian>(Vec3{ 0.2, 0.2, 1.0 }) };
    auto upperOrange{ std::make_shared<Lambertian>(Vec3{ 1.0, 0.5, 0.0 }) };
    auto lowerTeal{ std::make_shared<Lambertian>(Vec3{ 0.2, 0.8, 0.8 }) };

    scene.add(std::make_shared<Quad>(Vec3{ -2., -2., 0. },
                                     Vec3{ 4., 0., 0. },
                                     Vec3{ 0., 4., 0. },
                                     backGreen));
    scene.add(std::make_shared<Quad>(Vec3{ -3., -2., 5. },
                                     Vec3{ 0., 0., -4. },
                                     Vec3{ 0., 4., 0. },
                                     leftRed));
    scene.add(std::make_shared<Quad>(Vec3{ 3., -2., 1. },
                                     Vec3{ 0., 0., 4. },
                                     Vec3{ 0., 4., 0. },
                                     rightBlue));
    scene.add(std::make_shared<Quad>(Vec3{ -2., 3., 1. },
                                     Vec3{ 4., 0., 0. },
                                     Vec3{ 0., 0., 4. },
                                     upperOrange));
    scene.add(std::make_shared<Quad>(Vec3{ -2., -3., 5. },
                                     Vec3{ 4., 0., 0. },
                                     Vec3{ 0., 0., -4. },
                                     lowerTeal));

    CameraParams params{
        .position        = Vec3(0., 0., 9.),
        .lookAt          = Vec3(0., 0., 0.),
        .up              = Vec3(0., 1., 0.),
        .aspectRatio     = 1.,
        .imageHeight     = 720,
        .defocusAngle    = 0.,
        .vFov            = 80,
        .numSamples      = 100,
        .maxRayDepth     = 30,
        .backgroundColor = Vec3{0.70, 0.80, 1.00}
    };

    return std::make_pair(scene, params);
}

std::pair<HittableList, CameraParams> sceneSimpleLight()
{
    HittableList scene{};

    auto noiseTexture{ std::make_shared<NoiseTexture>(4.) };
    scene.add(std::make_shared<Sphere>(
        Sphere({ 0., -1000., 0. }, 1000., std::make_shared<Lambertian>(noiseTexture))));
    scene.add(std::make_shared<Sphere>(
        Sphere({ 0., 2., 0. }, 2., std::make_shared<Lambertian>(noiseTexture))));

    auto lightTexture{ std::make_shared<DiffuseLight>(Vec3{ 4. }) };
    scene.add(std::make_shared<Sphere>(Sphere({ 0., 7., 0. }, 2., lightTexture)));
    scene.add(std::make_shared<Quad>(Quad({ 3., 1., -2. },  // Q
                                          { 2., 0., 0. },   // u
                                          { 0., 2., 0. },   // v
                                          lightTexture)));

    CameraParams params{ .position        = Vec3(26., 3., 6.),
                         .lookAt          = Vec3(0., 2., 0.),
                         .up              = Vec3(0., 1., 0.),
                         .aspectRatio     = 16. / 9.,
                         .imageHeight     = 720,
                         .defocusAngle    = 0.,
                         .focusDist       = 10.,
                         .vFov            = 20,
                         .numSamples      = 100,
                         .maxRayDepth     = 30,
                         .backgroundColor = Vec3{ 0. } };

    return std::make_pair(scene, params);
}

std::pair<HittableList, CameraParams> sceneCornellBox()
{
    HittableList scene{};

    auto red{ std::make_shared<Lambertian>(Vec3{ .65, .05, .05 }) };
    auto white{ std::make_shared<Lambertian>(Vec3{ .73, .73, .73 }) };
    auto green{ std::make_shared<Lambertian>(Vec3{ .12, .45, .15 }) };
    auto light{ std::make_shared<DiffuseLight>(Vec3{ 15, 15, 15 }) };

    scene.add(
        std::make_shared<Quad>(Vec3{ 555, 0, 0 }, Vec3{ 0, 555, 0 }, Vec3{ 0, 0, 555 }, green));
    scene.add(std::make_shared<Quad>(Vec3{ 0, 0, 0 }, Vec3{ 0, 555, 0 }, Vec3{ 0, 0, 555 }, red));
    scene.add(std::make_shared<Quad>(Vec3{ 0, 0, 0 }, Vec3{ 555, 0, 0 }, Vec3{ 0, 0, 555 }, white));
    scene.add(std::make_shared<Quad>(Vec3{ 555, 555, 555 },
                                     Vec3{ -555, 0, 0 },
                                     Vec3{ 0, 0, -555 },
                                     white));

    scene.add(std::make_shared<Quad>(Vec3{ 343, 554, 332 },
                                     Vec3{ -130, 0, 0 },
                                     Vec3{ 0, 0, -105 },
                                     light));

    scene.add(
        std::make_shared<Quad>(Vec3{ 0, 0, 555 }, Vec3{ 555, 0, 0 }, Vec3{ 0, 555, 0 }, white));

    std::shared_ptr<Hittable> box1{ box(Vec3{ 0, 0, 0 }, Vec3{ 165, 330, 165 }, white) };
    box1 = std::make_shared<RotationY>(box1, 15.);
    box1 = std::make_shared<Translation>(box1, Vec3{ 265, 0, 295 });
    scene.add(box1);

    std::shared_ptr<Hittable> box2{ box(Vec3{ 0, 0, 0 }, Vec3{ 165, 165, 165 }, white) };
    box2 = std::make_shared<RotationY>(box2, -18.);
    box2 = std::make_shared<Translation>(box2, Vec3{ 130, 0, 65 });
    scene.add(box2);

    CameraParams params{ .position        = Vec3(278., 278., -800.),
                         .lookAt          = Vec3(278., 278., 0.),
                         .up              = Vec3(0., 1., 0.),
                         .aspectRatio     = 1.,
                         .imageHeight     = 720,
                         .defocusAngle    = 0.,
                         .focusDist       = 10.,
                         .vFov            = 40,
                         .numSamples      = 200,
                         .maxRayDepth     = 50,
                         .backgroundColor = Vec3{ 0. } };

    return std::make_pair(scene, params);
}

std::pair<HittableList, CameraParams> sceneCornellBoxSmoke()
{
    HittableList scene{};

    auto red{ std::make_shared<Lambertian>(Vec3{ .65, .05, .05 }) };
    auto white{ std::make_shared<Lambertian>(Vec3{ .73, .73, .73 }) };
    auto green{ std::make_shared<Lambertian>(Vec3{ .12, .45, .15 }) };
    auto light{ std::make_shared<DiffuseLight>(Vec3{ 15, 15, 15 }) };

    scene.add(
        std::make_shared<Quad>(Vec3{ 555, 0, 0 }, Vec3{ 0, 555, 0 }, Vec3{ 0, 0, 555 }, green));
    scene.add(std::make_shared<Quad>(Vec3{ 0, 0, 0 }, Vec3{ 0, 555, 0 }, Vec3{ 0, 0, 555 }, red));
    scene.add(std::make_shared<Quad>(Vec3{ 0, 0, 0 }, Vec3{ 555, 0, 0 }, Vec3{ 0, 0, 555 }, white));
    scene.add(std::make_shared<Quad>(Vec3{ 555, 555, 555 },
                                     Vec3{ -555, 0, 0 },
                                     Vec3{ 0, 0, -555 },
                                     white));

    scene.add(std::make_shared<Quad>(Vec3{ 343, 554, 332 },
                                     Vec3{ -130, 0, 0 },
                                     Vec3{ 0, 0, -105 },
                                     light));

    scene.add(
        std::make_shared<Quad>(Vec3{ 0, 0, 555 }, Vec3{ 555, 0, 0 }, Vec3{ 0, 555, 0 }, white));

    std::shared_ptr<Hittable> box1{ box(Vec3{ 0, 0, 0 }, Vec3{ 165, 330, 165 }, white) };
    box1 = std::make_shared<RotationY>(box1, 15.);
    box1 = std::make_shared<Translation>(box1, Vec3{ 265, 0, 295 });
    box1 = std::make_shared<ConstantMedium>(box1, 0.01, Vec3{ 0. });
    scene.add(box1);

    std::shared_ptr<Hittable> box2{ box(Vec3{ 0, 0, 0 }, Vec3{ 165, 165, 165 }, white) };
    box2 = std::make_shared<RotationY>(box2, -18.);
    box2 = std::make_shared<Translation>(box2, Vec3{ 130, 0, 65 });
    box2 = std::make_shared<ConstantMedium>(box2, 0.01, Vec3{ 1. });
    scene.add(box2);

    CameraParams params{ .position        = Vec3(278., 278., -800.),
                         .lookAt          = Vec3(278., 278., 0.),
                         .up              = Vec3(0., 1., 0.),
                         .aspectRatio     = 1.,
                         .imageHeight     = 720,
                         .defocusAngle    = 0.,
                         .focusDist       = 10.,
                         .vFov            = 40,
                         .numSamples      = 100,
                         .maxRayDepth     = 50,
                         .backgroundColor = Vec3{ 0. } };

    return std::make_pair(scene, params);
}

std::pair<HittableList, CameraParams> sceneFinalBook2()
{
    HittableList scene{};

    HittableList groundBoxes{};
    auto groundMat{ std::make_shared<Lambertian>(Vec3(0.48, 0.83, 0.53)) };
    int boxesPerSide{ 20 };
    for (int i = 0; i < boxesPerSide; ++i) {
        for (int j = 0; j < boxesPerSide; ++j) {
            auto w{ 100.0 };
            auto x0{ -1000.0 + i * w };
            auto z0{ -1000.0 + j * w };
            auto y0{ 0.0 };
            auto x1{ x0 + w };
            auto y1{ random_double(1., 101.) };
            auto z1{ z0 + w };

            groundBoxes.add(box(Vec3(x0, y0, z0), Vec3(x1, y1, z1), groundMat));
        }
    }
    scene.add(std::make_shared<Node>(groundBoxes));

    // Ceiling light
    auto light{ std::make_shared<DiffuseLight>(Vec3(7, 7, 7)) };
    scene.add(std::make_shared<Quad>(Vec3(123, 554, 147), Vec3(300, 0, 0), Vec3(0, 0, 265), light));

    // Moving sphere
    auto center1{ Vec3(400, 400, 200) };
    auto center2{ center1 + Vec3(30, 0, 0) };
    auto sphere_material{ std::make_shared<Lambertian>(Vec3(0.7, 0.3, 0.1)) };
    scene.add(std::make_shared<Sphere>(center1, center2, 50, sphere_material));

    //// Glass ball
    scene.add(std::make_shared<Sphere>(Vec3(260, 150, 45), 50, std::make_shared<Dielectric>(1.5)));

    // Metallic ball
    scene.add(std::make_shared<Sphere>(Vec3(0, 150, 145),
                                       50,
                                       std::make_shared<Metal>(Vec3(0.8, 0.8, 0.9), 1.0)));

    //// Blue ball
    auto boundary{
        std::make_shared<Sphere>(Vec3(360, 150, 145), 70, std::make_shared<Dielectric>(1.5))
    };
    scene.add(boundary);
    scene.add(std::make_shared<ConstantMedium>(boundary, 0.2, Vec3(0.2, 0.4, 0.9)));

    // White fog
    boundary = std::make_shared<Sphere>(Vec3(0, 0, 0), 5000, std::make_shared<Dielectric>(1.5));
    scene.add(std::make_shared<ConstantMedium>(boundary, .0001, Vec3(1, 1, 1)));

    // Earth ball
    auto earthMat{ std::make_shared<Lambertian>(
        std::make_shared<ImageTexture>("../textures/earthmap.jpg")) };
    scene.add(std::make_shared<Sphere>(Vec3(400, 200, 400), 100, earthMat));
    auto pertext{ std::make_shared<NoiseTexture>(0.2) };
    scene.add(
        std::make_shared<Sphere>(Vec3(220, 280, 300), 80, std::make_shared<Lambertian>(pertext)));

    // Foam box
    HittableList foamBox{};
    auto white{ std::make_shared<Lambertian>(Vec3(.73, .73, .73)) };
    int ns{ 1000 };
    for (int j = 0; j < ns; j++) {
        foamBox.add(std::make_shared<Sphere>(random_vec(0, 165), 10, white));
    }
    scene.add(std::make_shared<Translation>(
        std::make_shared<RotationY>(std::make_shared<Node>(foamBox), 15),
        Vec3(-100, 270, 395)));

    CameraParams params{ .position        = Vec3(478., 278., -600.),
                         .lookAt          = Vec3(278., 278., 0.),
                         .up              = Vec3(0., 1., 0.),
                         .aspectRatio     = 1.,
                         .imageHeight     = 1080,
                         .defocusAngle    = 0.,
                         .focusDist       = 10.,
                         .vFov            = 40,
                         .numSamples      = 5000,
                         .maxRayDepth     = 30,
                         .backgroundColor = Vec3{ 0. } };

    return std::make_pair(scene, params);
}

int main()
{
    auto [scene, camParams]{ sceneFinalBook2() };

    Camera camera{ camParams };
    auto t0{ std::chrono::steady_clock::now() };
    camera.render(scene);
    auto t1{ std::chrono::steady_clock::now() };

    std::clog << "Render time: " << std::chrono::duration<double, std::milli>(t1 - t0) << " "
              << std::endl;

    std::ignore = std::getchar();
}