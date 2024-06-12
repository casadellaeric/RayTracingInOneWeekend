#pragma once

#include "Common.h"
#include "HittableList.h"

struct CameraParams {
    const Vec3& position        = Vec3{ 0., 0., 0. };
    const Vec3& lookAt          = Vec3{ 0., 0., -1. };
    const Vec3& up              = Vec3{ 0., 1., 0. };
    double aspectRatio          = 1.;
    size_t imageHeight          = 720;
    double defocusAngle         = 10.;
    double focusDist            = 1.;
    double vFov                 = 90.;  // Degrees
    unsigned int numSamples     = 1;
    unsigned int maxRayDepth    = 10;
    const Vec3& backgroundColor = Vec3{ 0. };
};

class Camera
{
public:

    Camera(const CameraParams& params) :
      m_position{ params.position },
      m_lookAt{ params.lookAt },
      m_up{ params.up },
      m_aspectRatio{ params.aspectRatio },
      m_imageHeight{ params.imageHeight },
      m_imageWidth{ static_cast<size_t>(m_imageHeight * m_aspectRatio) },
      m_defocusAngle{ params.defocusAngle },
      m_focusDist{ params.focusDist },
      m_vFov{ params.vFov },
      m_numSamples{ params.numSamples },
      m_maxRayDepth{ params.maxRayDepth },
      m_backgroundColor{ params.backgroundColor }
    {
        assert(m_aspectRatio > 0.);
        assert(m_imageHeight > 0);
        assert(m_numSamples > 0);
        assert(m_maxRayDepth > 0);
        m_frameBuffer.resize(m_imageHeight, std::vector<tVec3<int>>(m_imageWidth, tVec3<int>{}));

        init_viewport();
    }

    void render(const HittableList& scene);
    void render_region(const HittableList& scene,
                       unsigned int startRow,
                       unsigned int endRow,
                       int threadIdx);

private:

    void init_viewport();

    Vec3 random_point_defocus_disk() const;

    Ray get_ray(int i, int j) const;
    Vec3 get_ray_color(const Ray& ray, const HittableList& scene) const;
    Vec3 get_ray_color_rec(const Ray& ray,
                           const HittableList& scene,
                           unsigned int currentDepth) const;

private:

    Vec3 m_position;
    Vec3 m_lookAt;
    Vec3 m_up;

    double m_aspectRatio;
    size_t m_imageHeight;
    size_t m_imageWidth;
    double m_viewportHeight;
    double m_viewportWidth;
    double m_vFov;

    double m_defocusAngle;
    double m_focusDist;

    Vec3 m_pixelDeltaU;
    Vec3 m_pixelDeltaV;
    Vec3 m_viewportUpperLeft;
    Vec3 u, v, w;  // Camera frame basis

    Vec3 m_defocusDiskU;
    Vec3 m_defocusDiskV;

    unsigned int m_numSamples;
    unsigned int m_maxRayDepth;

    Vec3 m_backgroundColor;

    std::vector<std::vector<tVec3<int>>> m_frameBuffer{};
};