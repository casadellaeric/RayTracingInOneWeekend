#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:

    Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material) :
      m_center{ center },
      m_radius{ std::fmax(0., radius) },
      m_material{ material }
    {
    }

    std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override;

private:

    Vec3 m_center;
    double m_radius;
    std::shared_ptr<Material> m_material;
};