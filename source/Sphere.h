#pragma once

#include "Hittable.h"

class Sphere : public Hittable
{
public:

    Sphere(const Vec3& center, double radius, std::shared_ptr<Material> material) :
      m_center{ center },
      m_targetVec{ Vec3{ 0. } },
      m_radius{ std::fmax(0., radius) },
      m_moving{ false },
      m_material{ material }
    {
    }

    Sphere(const Vec3& center,
           const Vec3& target,
           double radius,
           std::shared_ptr<Material> material) :
      m_center{ center },
      m_targetVec{ target - center },
      m_radius{ std::fmax(0., radius) },
      m_moving{ true },
      m_material{ material }
    {
    }

    std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override;

private:

    Vec3 center_at_time(double time) const;

private:

    Vec3 m_center;
    Vec3 m_targetVec;
    double m_radius;
    bool m_moving;
    std::shared_ptr<Material> m_material;
};