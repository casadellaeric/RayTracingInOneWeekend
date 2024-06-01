#pragma once

#include "Common.h"

class Material;

enum class HitType { back = false, front = true };

struct Hit {
    Hit(const Vec3& point,
        const Vec3& normal,
        std::shared_ptr<Material> material,
        double t,
        const Ray& ray) :
      type{ static_cast<HitType>(dot(normal, ray.get_direction()) <= 0.) },
      point{ point },
      normal{ type == HitType::front ? normal : -normal },
      material{ material },
      t{ t }
    {
    }

    Hit(const Vec3& point,
        const Vec3& normal,
        std::shared_ptr<Material> material,
        double t,
        HitType type) :
      type{ type },
      point{ point },
      normal{ normal },
      material{ material },
      t{ t }
    {
    }

    Hit(const Hit& other) = default;

    // Do not move this below normal
    const HitType type;

    const Vec3 point;
    const Vec3 normal;
    std::shared_ptr<Material> material;
    const double t;
};

class Hittable
{
public:

    virtual ~Hittable() = default;

    virtual std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const = 0;
};