#include <chrono>

#include "Common.h"
#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"

int main()
{
    // Scene

    HittableList scene{};
    scene.add(std::make_shared<Sphere>(Sphere({ 0., 0., -1. }, 0.5)));
    scene.add(std::make_shared<Sphere>(Sphere({ 0, -100.5, -1 }, 100.)));

    // Render

    Camera camera(Vec3(0., 0., 0.), 16. / 9., 720, 2., 1., 10);

    auto t0{ std::chrono::steady_clock::now() };
    camera.render(scene);
    auto t1{ std::chrono::steady_clock::now() };

    std::clog << "Render time: " << std::chrono::duration<double, std::milli>(t1 - t0) << " "
              << std::endl;

    std::ignore = std::getchar();
}