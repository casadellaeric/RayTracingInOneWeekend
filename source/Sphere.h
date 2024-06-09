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
      m_material{ material },
      m_bbox{ sphere_bounding_box(m_center, m_radius) }
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
      m_material{ material },
      m_bbox{ moving_sphere_bounding_box(m_center, target, m_radius) }
    {
    }

    std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override;
    AABB get_bounding_box() const override;

private:

    Vec3 center_at_time(double time) const;

    AABB sphere_bounding_box(const Vec3& center, double radius) const;
    AABB moving_sphere_bounding_box(const Vec3& center, const Vec3& target, double radius) const;

    static std::pair<double, double> get_uv_coords(const Vec3& point);

private:

    Vec3 m_center;
    Vec3 m_targetVec;
    double m_radius;
    bool m_moving;
    std::shared_ptr<Material> m_material;

    AABB m_bbox;
};