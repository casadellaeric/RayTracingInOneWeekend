#pragma once

#include "Vec3.h"

class Ray
{
public:

    Ray(const Vec3& origin, const Vec3& direction) :
      origin(origin),
      direction(direction){};
    ~Ray() = default;

    const Vec3& get_origin() const { return origin; }
    const Vec3& get_direction() const { return direction; }

    Vec3 at(double t) const { return origin + t * direction; }

private:

    Vec3 origin;
    Vec3 direction;
};