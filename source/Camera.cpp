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
        std::clog << "\rScanlines remaining: " << endRow - i << " " << std::flush;
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

void Camera::init_viewport()
{
    auto theta{ degrees_to_radians(m_vFov) };
    auto h{ std::tan(theta / 2.) };
    m_viewportHeight = 2 * h * m_focusDist;
    m_viewportWidth  = m_viewportHeight * (static_cast<double>(m_imageWidth) / m_imageHeight);

    w = (m_position - m_lookAt).normalized();
    u = cross(m_up, w).normalized();
    v = cross(w, u);

    auto viewportU{ m_viewportWidth * u };
    auto viewportV{ m_viewportHeight * -v };

    m_pixelDeltaU = viewportU / static_cast<double>(m_imageWidth);
    m_pixelDeltaV = viewportV / static_cast<double>(m_imageHeight);

    m_viewportUpperLeft = m_position - (m_focusDist * w) - viewportU / 2. - viewportV / 2.;

    auto defocusRadius{ m_focusDist * std::tan(degrees_to_radians(m_defocusAngle / 2.)) };
    m_defocusDiskU = defocusRadius * u;
    m_defocusDiskV = defocusRadius * v;
}

Vec3 Camera::random_point_defocus_disk() const
{
    auto randomDiskPoint{ random_vec_in_unit_disk() };
    return m_position + randomDiskPoint.x * m_defocusDiskU + randomDiskPoint.y * m_defocusDiskV;
}

Ray Camera::get_ray(int i, int j) const
{
    Vec3 rayTarget{ m_viewportUpperLeft + (j + (1 * random_double())) * m_pixelDeltaU
                    + (i + (1 * random_double())) * m_pixelDeltaV };

    Vec3 rayOrigin{ m_defocusAngle <= 0. ? m_position : random_point_defocus_disk() };
    return Ray(rayOrigin, rayTarget - rayOrigin);
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
        Interval(1e-8,  // Start from small value to avoid shadow acne
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
