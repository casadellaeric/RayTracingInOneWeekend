#include "Vec3.h"
#include "Ray.h"

#include <iostream>
#include <cassert>

// IMAGE AND VIEWPORT

constexpr double ASPECT_RATIO{ 16. / 9. };
constexpr int IMAGE_HEIGHT{ 720 };
constexpr int IMAGE_WIDTH{ static_cast<int>(IMAGE_HEIGHT * ASPECT_RATIO) };

constexpr double VIEWPORT_HEIGHT{ 2. };
constexpr double VIEWPORT_WIDTH{ VIEWPORT_HEIGHT
                                 * (static_cast<double>(IMAGE_WIDTH) / IMAGE_HEIGHT) };

static_assert(IMAGE_HEIGHT > 0 && IMAGE_WIDTH > 0 && VIEWPORT_HEIGHT > 0. && VIEWPORT_WIDTH > 0.,
              "Invalid image parameters!");

constexpr double FOCAL_LENGTH{ 1. };
constexpr Vec3 PIXEL_DELTA_U{ VIEWPORT_WIDTH / IMAGE_WIDTH, 0., 0. };
constexpr Vec3 PIXEL_DELTA_V{ 0., -(VIEWPORT_HEIGHT / IMAGE_HEIGHT), 0. };

Vec3 get_ray_color(const Ray& ray)
{
    Vec3 rayNorm{ ray.get_direction().normalized() };
    auto yPercent{ (rayNorm.y + 1.) / 2. };  // [-1, 1] -> [0, 1]
    return (1. - yPercent) * Vec3{ 1., 1., 1. } + yPercent * Vec3{ 0.5, 0.7, 1. };
}

int main()
{
    // Camera config

    const Vec3 cameraPosition{ 0., 0., 0. };
    const Vec3 viewportUpperLeft{ cameraPosition
                                  - Vec3(0., 0., -FOCAL_LENGTH)            // Center of the viewport
                                  - Vec3(VIEWPORT_WIDTH / 2., 0., 0.)      // Left side
                                  + Vec3(0., VIEWPORT_HEIGHT / 2., 0.) };  // Upper-left corner

    // We want to shoot rays through the center of the pixel

    const Vec3 firstPixelCenter{ viewportUpperLeft + (PIXEL_DELTA_U + PIXEL_DELTA_V) * 0.5 };

    // Render

    std::cout << "P3\n" << IMAGE_WIDTH << ' ' << IMAGE_HEIGHT << "\n255\n";

    for (int j = 0; j < IMAGE_HEIGHT; j++) {
        std::clog << "\rScanlines remaining: " << (IMAGE_HEIGHT - j) << " " << std::flush;

        for (int i = 0; i < IMAGE_WIDTH; i++) {
            const Vec3 pixelCenter{ firstPixelCenter + PIXEL_DELTA_U * i + PIXEL_DELTA_V * j };
            Ray ray(cameraPosition, pixelCenter - cameraPosition);

            std::cout << to_rgb(get_ray_color(ray)) << '\n';
        }
    }
    std::clog << "\rDone.                   \n";
}