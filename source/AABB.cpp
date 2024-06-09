#include "AABB.h"

bool AABB::hit(const Ray& ray, Interval interval) const
{
    const auto& o{ ray.get_origin() };
    const auto& dir{ ray.get_direction() };

    for (int axis = 0; axis < 3; ++axis) {
        auto& axisInterval{ v[axis] };

        auto t0 = (axisInterval.min - o[axis]) / dir[axis];
        auto t1 = (axisInterval.max - o[axis]) / dir[axis];

        if (t0 > t1) {
            std::swap(t0, t1);
        }
        if (t0 > interval.min) {
            interval.min = t0;
        }
        if (t1 < interval.max) {
            interval.max = t1;
        }
        if (interval.max <= interval.min) {
            return false;
        }
    }
    return true;
}

int AABB::longest_axis() const
{
    if (x.size() > y.size()) {
        return x.size() > z.size() ? 0 : 2;
    } else {
        return y.size() > z.size() ? 1 : 2;
    }
}
