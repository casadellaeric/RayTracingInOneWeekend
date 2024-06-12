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

    constexpr Interval(const Interval& a, const Interval& b) :
      min{ a.min <= b.min ? a.min : b.min },
      max{ a.max >= b.max ? a.max : b.max }
    {
    }

    constexpr double size() const { return max - min; }
    constexpr bool contains(double t) const { return t >= min && t <= max; }
    constexpr bool surrounds(double t) const { return t > min && t < max; }
    constexpr void expand(double d)
    {
        double pad{ d / 2. };
        min = min - pad;
        max = max + pad;
    }

    double min, max;
    static const Interval empty, universe;
};

inline const Interval Interval::empty{ Interval(rt::INF, -rt::INF) };
inline const Interval Interval::universe{ Interval(-rt::INF, rt::INF) };

inline Interval operator+(const Interval& interval, double offset)
{
    return Interval{ interval.min + offset, interval.max + offset };
}

inline Interval operator+(double offset, const Interval& interval)
{
    return Interval{ interval.min + offset, interval.max + offset };
}