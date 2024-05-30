#include "Camera.h"

void Camera::render(const HittableList& scene) const
{
    // We want to shoot rays through the center of the pixel
    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int i = 0; i < imageHeight; ++i) {
        std::clog << "\rScanlines remaining: " << (imageHeight - i) << " " << std::flush;
        for (int j = 0; j < imageWidth; ++j) {
            Vec3 sumColor{};
            for (int k = 0; k < numSamples; ++k) {
                sumColor += get_ray_color(get_ray(i, j), scene);
            }
            if (WRITE_TO_IMAGE) {
                std::cout << to_rgb(sumColor / numSamples) << '\n';
            }
        }
    }

    std::clog << "\rDone.                   \n" << std::flush;
}

Ray Camera::get_ray(int i, int j) const
{
    static const double pixelDeltaU{ viewportWidth / imageWidth };
    static const double pixelDeltaV{ -(viewportHeight / imageHeight) };

    // Camera config
    static const Vec3 viewportUpperLeft{ position
                                         - Vec3(0., 0., focalLength)  // Center of the viewport
                                         - Vec3(viewportWidth / 2., 0., 0.)  // Left side
                                         + Vec3(0., viewportHeight / 2., 0.) };

    Vec3 rayTarget{
        viewportUpperLeft
        + Vec3{pixelDeltaU * (j + random_double()), pixelDeltaV * (i + random_double()), 0.}
    };
    return Ray(position, rayTarget - position);
};

Vec3 Camera::get_ray_color(const Ray& ray, const HittableList& scene) const
{
    std::optional<Hit> hit{ scene.test_hit(ray,
                                           Interval(0., std::numeric_limits<double>::infinity())) };
    if (hit) {
        return (hit->normal + Vec3(1.)) / 2.;
    }
    Vec3 rayNorm{ ray.get_direction().normalized() };
    auto yPercent{ (rayNorm.y + 1.) / 2. };  // [-1, 1] -> [0, 1]
    return (1. - yPercent) * Vec3{ 1., 1., 1. } + yPercent * Vec3{ 0.5, 0.7, 1. };
}
