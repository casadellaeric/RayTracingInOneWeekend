#pragma once

#include <cmath>
#include <iostream>
#include <limits>

template<typename T>
class tVec3
{
public:

    tVec3() :
      x{},
      y{},
      z{} {};

    tVec3(T x, T y, T z) :
      x{ x },
      y{ y },
      z{ z } {};

    explicit tVec3(T v) :
      x{ v },
      y{ v },
      z{ v } {};

    ~tVec3() = default;

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
    tVec3 operator-() { return tVec3(-x, -y, -z); }
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

    double length() { return std::sqrt(x * x + y * y + z * z); }
    double length2() { return x * x + y * y + z * z; }
    tVec3 normalized() { return *this /= length(); }
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
tVec3<T> operator/(const tVec3<T>& u, const tVec3<T>& v)
{
    return { u.x / v.x, u.y / v.y, u.z / v.z };
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

inline tVec3<int> to_rgb(const tVec3<double>& vec)
{
    return {
        static_cast<int>(vec.x * (256. - std::numeric_limits<double>::epsilon())),
        static_cast<int>(vec.y * (256. - std::numeric_limits<double>::epsilon())),
        static_cast<int>(vec.z * (256. - std::numeric_limits<double>::epsilon())),
    };
}