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
        pad_to_minimums();
    }

    AABB(const Vec3& a, const Vec3& b) :
      x(a.x < b.x ? Interval(a.x, b.x) : Interval(b.x, a.x)),
      y(a.y < b.y ? Interval(a.y, b.y) : Interval(b.y, a.y)),
      z(a.z < b.z ? Interval(a.z, b.z) : Interval(b.z, a.z))
    {
        pad_to_minimums();
    }

    AABB(const AABB& a, const AABB& b) :
      x{ Interval(a.x, b.x) },
      y{ Interval(a.y, b.y) },
      z{ Interval(a.z, b.z) }
    {
        pad_to_minimums();
    }

    const Interval& axis_interval(int n) const { return v[n]; }

    bool hit(const Ray& ray, Interval interval) const;

    int longest_axis() const;

    static const AABB empty, universe;

private:

    void pad_to_minimums();

public:

    union {
        struct {
            Interval x, y, z;
        };
        Interval v[3];
    };
};

inline const AABB AABB::empty{ Interval::empty, Interval::empty, Interval::empty };
inline const AABB AABB::universe{ Interval::universe, Interval::universe, Interval::universe };

inline AABB operator+(const AABB& box, const Vec3& offsets)
{
    return AABB(box.axis_interval(0) + offsets.x,
                box.axis_interval(1) + offsets.y,
                box.axis_interval(2) + offsets.z);
}

inline AABB operator+(const Vec3& offsets, const AABB& box)
{
    return AABB(box.axis_interval(0) + offsets.x,
                box.axis_interval(1) + offsets.y,
                box.axis_interval(2) + offsets.z);
}
