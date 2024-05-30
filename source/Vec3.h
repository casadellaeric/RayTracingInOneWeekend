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
    return { u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x };
}
