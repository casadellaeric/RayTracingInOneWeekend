#pragma once

class Ray
{
public:

    Ray() :
      m_origin{},
      m_direction{},
      m_time{} {};

    Ray(const Vec3& origin, const Vec3& direction) :
      m_origin{ origin },
      m_direction{ direction },
      m_time{} {};

    Ray(const Vec3& origin, const Vec3& direction, double time) :
      m_origin{ origin },
      m_direction{ direction },
      m_time{ time } {};

    ~Ray() = default;

    const Vec3& get_origin() const { return m_origin; }
    const Vec3& get_direction() const { return m_direction; }
    double get_time() const { return m_time; }

    Vec3 at(double t) const { return m_origin + t * m_direction; }

private:

    Vec3 m_origin;
    Vec3 m_direction;
    double m_time;
};