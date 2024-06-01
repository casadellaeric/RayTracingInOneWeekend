#pragma once

namespace rt
{
extern const double INF;
}

class Interval
{
public:

    Interval() :
      min{ rt::INF },
      max{ -rt::INF }
    {
    }

    constexpr Interval(double min, double max) :
      min{ min },
      max{ max }
    {
    }

    constexpr double size() const { return max - min; }
    constexpr bool contains(double t) const { return t >= min && t <= max; }
    constexpr bool surrounds(double t) const { return t > min && t < max; }

    const double min, max;
    static const Interval empty, universe;
};

inline const Interval Interval::empty{ Interval(rt::INF, -rt::INF) };
inline const Interval Interval::universe{ Interval(-rt::INF, rt::INF) };