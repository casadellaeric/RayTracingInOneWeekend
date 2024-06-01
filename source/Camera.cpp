#include "Camera.h"

#include <thread>

#include "Material.h"

void Camera::render(const HittableList& scene)
{
    {
        const auto numThreads{ std::thread::hardware_concurrency() };
        std::vector<std::jthread> threads;
        const auto blockSize{ m_imageHeight / numThreads };
        for (size_t i = 0; i < numThreads; ++i) {
            threads.emplace_back(std::jthread(&Camera::render_region,
                                              this,
                                              scene,
                                              unsigned int(i * blockSize),
                                              unsigned int((i + 1) * blockSize)));
        }
        std::this_thread::yield();
    }
    // render_region(scene, 0, m_imageHeight);
    if (WRITE_TO_IMAGE) {
        std::cout << "P3\n" << m_imageWidth << ' ' << m_imageHeight << "\n255\n";
        for (size_t i = 0; i < m_frameBuffer.size(); ++i) {
            for (size_t j = 0; j < m_frameBuffer[i].size(); ++j) {
                std::cout << m_frameBuffer[i][j] << '\n';
            }
        }
    }
}

void Camera::render_region(const HittableList& scene, unsigned int startRow, unsigned int endRow)
{
    assert(startRow < endRow);

    for (size_t i = startRow; i < endRow; ++i) {
        std::clog << "\rScanlines remaining: " << (m_imageHeight - i) << " " << std::flush;
        for (size_t j = 0; j < m_imageWidth; ++j) {
            Vec3 sumColor{};
            for (size_t k = 0; k < m_numSamples; ++k) {
                sumColor += get_ray_color(get_ray(int(i), int(j)), scene);
            }
            m_frameBuffer[i][j] = to_rgb(sumColor / m_numSamples);
        }
    }
    std::clog << "\rDone.                   \n" << std::flush;
}

Ray Camera::get_ray(int i, int j) const
{
    static const double pixelDeltaU{ m_viewportWidth / m_imageWidth };
    static const double pixelDeltaV{ -(m_viewportHeight / m_imageHeight) };

    // Camera config
    static const Vec3 viewportUpperLeft{ m_position
                                         - Vec3(0., 0., m_focalLength)  // Center of the viewport
                                         - Vec3(m_viewportWidth / 2., 0., 0.)  // Left side
                                         + Vec3(0., m_viewportHeight / 2., 0.) };

    Vec3 rayTarget{
        viewportUpperLeft
        + Vec3{pixelDeltaU * (j + random_double()), pixelDeltaV * (i + random_double()), 0.}
    };
    return Ray(m_position, rayTarget - m_position);
};

Vec3 Camera::get_ray_color(const Ray& ray, const HittableList& scene) const
{
    return get_ray_color_rec(ray, scene, m_maxRayDepth);
}

Vec3 Camera::get_ray_color_rec(const Ray& ray,
                               const HittableList& scene,
                               unsigned int currentDepth) const
{
    if (currentDepth <= 0) {
        return Vec3{ 0. };
    }

    std::optional<Hit> hit{ scene.test_hit(
        ray,
        Interval(
            0. + std::numeric_limits<double>::epsilon(),  // Start from epsilon to avoid shadow acne
            std::numeric_limits<double>::infinity())) };

    if (hit) {
        Vec3 attenuation;
        Ray scatteredRay;
        if (hit->material->scatter(ray, *hit, attenuation, scatteredRay)) {
            return attenuation * get_ray_color_rec(scatteredRay, scene, --currentDepth);
        }
        return Vec3{ 0. };
    }

    Vec3 rayNorm{ ray.get_direction().normalized() };
    auto yPercent{ (rayNorm.y + 1.) / 2. };  // [-1, 1] -> [0, 1]
    return (1. - yPercent) * Vec3{ 1., 1., 1. } + yPercent * Vec3{ 0.5, 0.7, 1. };
}
