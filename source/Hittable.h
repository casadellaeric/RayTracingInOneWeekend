#pragma once

#include "Common.h"
#include "AABB.h"

class Material;

enum class HitType { back = false, front = true };

struct Hit {
    Hit(const Vec3& point,
        const Vec3& normal,
        std::shared_ptr<Material> material,
        double t,
        const Ray& ray,
        double u = 0.,
        double v = 0.) :
      type{ static_cast<HitType>(dot(normal, ray.get_direction()) <= 0.) },
      point{ point },
      normal{ type == HitType::front ? normal : -normal },
      material{ material },
      t{ t },
      u{ u },
      v{ v }
    {
    }

    Hit(const Hit& other) = default;

    // Do not move this below normal
    const HitType type;

    Vec3 point;
    Vec3 normal;
    std::shared_ptr<Material> material;
    double t;

    double u;
    double v;
};

class Hittable
{
public:

    virtual ~Hittable() = default;

    virtual std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const = 0;
    virtual AABB get_bounding_box() const                                               = 0;
};

class Translation : public Hittable
{
public:

    Translation(std::shared_ptr<Hittable> object, const Vec3& offset) :
      m_object{ object },
      m_offset{ offset },
      m_bbox{ object->get_bounding_box() + offset }
    {
    }

    virtual std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override;
    virtual AABB get_bounding_box() const override;

private:

    std::shared_ptr<Hittable> m_object{};

    Vec3 m_offset{};

    AABB m_bbox{};
};

class RotationY : public Hittable
{
public:

    RotationY(std::shared_ptr<Hittable> object, double angle);

    virtual std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override;
    virtual AABB get_bounding_box() const override;

    Vec3 world_to_object(const Vec3& vec) const;
    Vec3 object_to_world(const Vec3& vec) const;

private:

    std::shared_ptr<Hittable> m_object{};

    double m_sinTheta{};
    double m_cosTheta{};

    AABB m_bbox;
};
