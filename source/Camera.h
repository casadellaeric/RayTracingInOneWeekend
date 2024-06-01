#pragma once

#include "Common.h"
#include "HittableList.h"

struct CameraParams {
    const Vec3& position     = Vec3{ 0., 0., 0. };
    double aspectRatio       = 1.;
    size_t imageHeight       = 720;
    double viewportHeight    = 2.;
    double focalLength       = 1.;
    unsigned int numSamples  = 1;
    unsigned int maxRayDepth = 10;
};

class Camera
{
public:

    Camera(const CameraParams& params) :
      m_position{ params.position },
      m_aspectRatio{ params.aspectRatio },
      m_imageHeight{ params.imageHeight },
      m_imageWidth{ static_cast<size_t>(m_imageHeight * m_aspectRatio) },
      m_viewportHeight{ params.viewportHeight },
      m_viewportWidth{ m_viewportHeight * (static_cast<double>(m_imageWidth) / m_imageHeight) },
      m_focalLength{ params.focalLength },
      m_numSamples{ params.numSamples },
      m_maxRayDepth{ params.maxRayDepth }
    {
        assert(m_aspectRatio > 0.);
        assert(m_imageHeight > 0);
        assert(m_viewportHeight > 0);
        assert(m_numSamples > 0);
        assert(m_maxRayDepth > 0);
        m_frameBuffer.resize(m_imageHeight, std::vector<tVec3<int>>(m_imageWidth, tVec3<int>{}));
    }

    void render(const HittableList& scene);
    void render_region(const HittableList& scene, unsigned int startRow, unsigned int endRow);

private:

    Ray get_ray(int i, int j) const;
    Vec3 get_ray_color(const Ray& ray, const HittableList& scene) const;
    Vec3 get_ray_color_rec(const Ray& ray,
                           const HittableList& scene,
                           unsigned int currentDepth) const;

private:

    Vec3 m_position;
    double m_aspectRatio;
    size_t m_imageHeight;
    size_t m_imageWidth;
    double m_viewportHeight;
    double m_viewportWidth;
    double m_focalLength;
    unsigned int m_numSamples;
    unsigned int m_maxRayDepth;

    std::vector<std::vector<tVec3<int>>> m_frameBuffer{};
};