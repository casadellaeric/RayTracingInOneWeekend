#pragma once

#include "Common.h"
#include "Hittable.h"

class Quad : public Hittable
{
public:

    Quad(const Vec3& Q, const Vec3& u, const Vec3& v, std::shared_ptr<Material> material) :
      Q{ Q },
      u{ u },
      v{ v },
      m_normal{ cross(u, v).normalized() },
      D{ dot(m_normal, Q) },
      w{ cross(u, v) / dot(cross(u, v), cross(u, v)) },
      m_material{ material },
      m_bbox{ quad_bounding_box(Q, u, v) }
    {
    }

    std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override;
    AABB get_bounding_box() const override;

private:

    static AABB quad_bounding_box(const Vec3& Q, const Vec3& u, const Vec3& v);

private:

    const Vec3 Q, u, v;

    const Vec3 m_normal;
    const double D;  // From eq. Ax + By + Cz = D

    const Vec3 w;  // To compute point inside quad test

    std::shared_ptr<Material> m_material;
    const AABB m_bbox;
};
