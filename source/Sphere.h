#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:

    Sphere(const Vec3& center, double radius) :
      m_center(center),
      m_radius(radius)
    {
    }

    std::optional<Hit> test_hit(const Ray& ray, double tMin, double tMax) const override;

private:

    Vec3 m_center;
    double m_radius;
};