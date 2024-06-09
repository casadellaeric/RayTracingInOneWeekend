#pragma once

#include "Common.h"

class AABB
{
public:

    AABB() :
      x{},
      y{},
      z{}
    {
    }

    AABB(const Interval& x, const Interval& y, const Interval& z) :
      x(x),
      y(y),
      z(z)
    {
    }

    AABB(const Vec3& a, const Vec3& b) :
      x(a.x < b.x ? Interval(a.x, b.x) : Interval(b.x, a.x)),
      y(a.y < b.y ? Interval(a.y, b.y) : Interval(b.y, a.y)),
      z(a.z < b.z ? Interval(a.z, b.z) : Interval(b.z, a.z))
    {
    }

    AABB(const AABB& a, const AABB& b) :
      x{ Interval(a.x, b.x) },
      y{ Interval(a.y, b.y) },
      z{ Interval(a.z, b.z) }
    {
    }

    const Interval& axis_interval(int n) const { return v[n]; }

    bool hit(const Ray& ray, Interval interval) const;

    int longest_axis() const;

    static const AABB empty, universe;

private:

    union {
        struct {
            Interval x, y, z;
        };
        Interval v[3];
    };
};

inline const AABB AABB::empty{ Interval::empty, Interval::empty, Interval::empty };
inline const AABB AABB::universe{ Interval::universe, Interval::universe, Interval::universe };