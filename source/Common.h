#pragma once

#include <optional>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <limits>
#include <numbers>
#include <random>
#include <chrono>

#include "Vec3.h"
#include "Ray.h"

#ifndef NDEBUG
constexpr auto WRITE_TO_IMAGE = false;
#else
constexpr auto WRITE_TO_IMAGE = true;
#endif

namespace rt
{
constexpr double INF{ std::numeric_limits<double>::infinity() };
}

struct Interval {
    Interval() :
      min(rt::INF),
      max(-rt::INF)
    {
    }

    constexpr Interval(double min, double max) :
      min(min),
      max(max)
    {
    }

    constexpr double size() const { return max - min; }
    constexpr bool contains(double t) const { return t >= min && t <= max; }
    constexpr bool surrounds(double t) const { return t > min && t < max; }

    const double min, max;
    static const Interval empty, universe;
};

inline constexpr Interval Interval::empty{ Interval(rt::INF, -rt::INF) };
inline constexpr Interval Interval::universe{ Interval(-rt::INF, rt::INF) };

namespace rt
{
inline double clamp(double t, const Interval& interval)
{
    if (t < interval.min) {
        return interval.min;
    }
    if (t > interval.max) {
        return interval.max;
    }
    return t;
}
}  // namespace rt

inline double random_double()
{
    static std::uniform_real_distribution<double> dist(0.0, 1.0);
    static std::mt19937_64 rng;
    return dist(rng);
}

inline double random_double(double min, double max)
{
    return min + random_double() * (max - min);
}

inline double degrees_to_radians(double degrees)
{
    return degrees * std::numbers::pi / 180.0;
}

inline tVec3<int> to_rgb(const tVec3<double>& vec)
{
    static const Interval colorRange(0., 1. - std::numeric_limits<double>::epsilon());
    tVec3<int> rgb{
        static_cast<int>(rt::clamp(vec.x, colorRange) * 256.),
        static_cast<int>(rt::clamp(vec.y, colorRange) * 256.),
        static_cast<int>(rt::clamp(vec.z, colorRange) * 256.),
    };
    return rgb;
}