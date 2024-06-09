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
#include "Interval.h"

#ifndef NDEBUG
constexpr auto WRITE_TO_IMAGE = false;
#else
constexpr auto WRITE_TO_IMAGE = true;
#endif

namespace rt
{
inline constexpr double INF{ std::numeric_limits<double>::infinity() };
}

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

inline double random_double_normal()
{
    static std::normal_distribution<double> dist(0., 1.);
    static std::mt19937_64 rng;
    return dist(rng);
}

inline double random_double(double min, double max)
{
    return min + random_double() * (max - min);
}

inline int random_int(int min, int max)
{
    return static_cast<int>(random_double(min, max + 1));
}

inline Vec3 random_vec()
{
    return Vec3(random_double(), random_double(), random_double());
}

inline Vec3 random_vec(double min, double max)
{
    return Vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

inline Vec3 random_vec_on_unit_sphere()
{
    return Vec3(random_double_normal(), random_double_normal(), random_double_normal())
        .normalized();
}

// Disk in the XY plane
inline Vec3 random_vec_in_unit_disk()
{
    while (true) {
        Vec3 randomVec{ random_double(-1., 1.), random_double(-1., 1.), 0. };
        if (randomVec.length2() < 1.) {
            return randomVec;
        }
    }
}

inline Vec3 random_vec_hempsphere(const Vec3& normal)
{
    Vec3 rVec{ random_vec_on_unit_sphere() };
    return dot(rVec, normal) >= 0. ? rVec : -rVec;
}

inline double degrees_to_radians(double degrees)
{
    return degrees * std::numbers::pi / 180.0;
}

inline double linear_to_gamma(double linearVal)
{
    return linearVal > 0. ? std::sqrt(linearVal) : 0.;
}

inline tVec3<int> to_rgb(const tVec3<double>& vec)
{
    static const Interval colorRange(0., 1. - std::numeric_limits<double>::epsilon());
    tVec3<int> rgb{
        static_cast<int>(rt::clamp(linear_to_gamma(vec.x), colorRange) * 256.),
        static_cast<int>(rt::clamp(linear_to_gamma(vec.y), colorRange) * 256.),
        static_cast<int>(rt::clamp(linear_to_gamma(vec.z), colorRange) * 256.),
    };
    return rgb;
}