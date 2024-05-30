#pragma once

#include "Common.h"
#include "HittableList.h"

class Camera
{
public:

    Camera(const Vec3& position  = Vec3{ 0., 0., 0. },
           double aspectRatio    = 1.,
           int imageHeight       = 720,
           double viewportHeight = 2.,
           double focalLength    = 1.,
           int numSamples        = 1) :
      position(position),
      aspectRatio(aspectRatio),
      imageHeight(imageHeight),
      imageWidth(static_cast<int>(imageHeight * aspectRatio)),
      viewportHeight(viewportHeight),
      viewportWidth(viewportHeight * (static_cast<double>(imageWidth) / imageHeight)),
      focalLength(focalLength),
      numSamples(numSamples)
    {
        assert(aspectRatio > 0.);
        assert(imageHeight > 0);
        assert(viewportHeight > 0);
        assert(numSamples > 0);
    }

    void render(const HittableList& scene) const;

private:

    Ray get_ray(int i, int j) const;
    Vec3 get_ray_color(const Ray& ray, const HittableList& scene) const;

private:

    Vec3 position;
    double aspectRatio;
    int imageHeight;
    int imageWidth;
    double viewportHeight;
    double viewportWidth;
    double focalLength;
    int numSamples;
};