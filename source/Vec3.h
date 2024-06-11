#pragma once

#include "Common.h"

template<typename T>
class tVec3
{
public:

    constexpr tVec3() :
      x{},
      y{},
      z{} {};

    constexpr tVec3(T x, T y, T z) :
      x{ x },
      y{ y },
      z{ z } {};

    constexpr explicit tVec3(T v) :
      x{ v },
      y{ v },
      z{ v } {};

    constexpr ~tVec3() = default;

    // Public data to make it more usable

    union {
        struct {
            T x, y, z;
        };
        T vec[3];
    };

    // Operators

    T operator[](int i) const { return vec[i]; }
    T& operator[](int i) { return vec[i]; }
    tVec3 operator-() const { return tVec3(-x, -y, -z); }
    tVec3& operator+=(const tVec3& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
        return *this;
    }
    tVec3& operator*=(T t)
    {
        x *= t;
        y *= t;
        z *= t;
        return *this;
    }
    tVec3& operator/=(T t)
    {
        x /= t;
        y /= t;
        z /= t;
        return *this;
    }
    tVec3 operator/(const tVec3& other) { return { x / other.x, y / other.y, z / other.z }; }

    double length() const { return std::sqrt(x * x + y * y + z * z); }
    double length2() const { return x * x + y * y + z * z; }
    bool near_zero() const
    {
        auto smallValue{ 1e-8 };
        return std::fabs(x) < smallValue && std::fabs(y) < smallValue && std::fabs(z) < smallValue;
    }
    tVec3 normalized() const { return *this / length(); }
};

using Vec3 = tVec3<double>;

template<typename T>
std::ostream& operator<<(std::ostream& out, const tVec3<T>& vec)
{
    return out << vec.x << ' ' << vec.y << ' ' << vec.z;
}

template<typename T>
tVec3<T> operator+(const tVec3<T>& u, const tVec3<T>& v)
{
    return { u.x + v.x, u.y + v.y, u.z + v.z };
}

template<typename T>
tVec3<T> operator-(const tVec3<T>& u, const tVec3<T>& v)
{
    return { u.x - v.x, u.y - v.y, u.z - v.z };
}

template<typename T>
tVec3<T> operator*(const tVec3<T>& u, const tVec3<T>& v)
{
    return { u.x * v.x, u.y * v.y, u.z * v.z };
}

template<typename T>
tVec3<T> operator*(const tVec3<T>& u, double t)
{
    return { u.x * t, u.y * t, u.z * t };
}

template<typename T>
tVec3<T> operator*(double t, const tVec3<T>& u)
{
    return { u.x * t, u.y * t, u.z * t };
}

template<typename T>
tVec3<T> operator/(const tVec3<T>& u, const tVec3<T>& v)
{
    return { u.x / v.x, u.y / v.y, u.z / v.z };
}

template<typename T>
tVec3<T> operator/(const tVec3<T>& u, double t)
{
    return { u.x / t, u.y / t, u.z / t };
}

template<typename T>
double dot(const tVec3<T>& u, const tVec3<T>& v)
{
    return u.x * v.x + u.y * v.y + u.z * v.z;
}

template<typename T>
tVec3<T> cross(const tVec3<T>& u, const tVec3<T>& v)
{
    return { (u.y * v.z) - (u.z * v.y), (u.z * v.x) - (u.x * v.z), (u.x * v.y) - (u.y * v.x) };
}

inline Vec3 reflect(const Vec3& v, const Vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline Vec3 refract(const Vec3& v,
                    const Vec3& n,
                    double ratio)  // Ratio of refractive indices n1/n2
{
    auto rayOutPerp{ ratio * (v + std::fmin(1., dot(-v, n)) * n) };
    auto rayOutPar{ -std::sqrt(std::fabs(1. - rayOutPerp.length2())) * n };
    return rayOutPerp + rayOutPar;
}
