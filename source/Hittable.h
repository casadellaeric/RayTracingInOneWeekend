#pragma once

#include "Common.h"

enum class HitType { back = false, front = true };

struct Hit {
    Hit(const Vec3& point, const Vec3& normal, double t, const Ray& ray) :
      point(point),
      normal(type == HitType::front ? normal : -normal),
      t(t),
      type(static_cast<HitType>(dot(normal, ray.get_direction()) <= 0.))
    {
    }

    Hit(const Vec3& point, const Vec3& normal, double t, HitType type) :
      point(point),
      normal(normal),
      t(t),
      type(type)
    {
    }

    Hit(const Hit& other) = default;

    // Do not move this below normal
    const HitType type;

    const Vec3 point;
    const Vec3 normal;
    const double t;
};

class Hittable
{
public:

    virtual ~Hittable() = default;

    virtual std::optional<Hit> test_hit(const Ray& ray, double tMin, double tMax) const = 0;
};