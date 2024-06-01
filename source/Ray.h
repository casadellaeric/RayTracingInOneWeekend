#pragma once

class Ray
{
public:

    Ray() :
      m_origin{},
      m_direction{}
    {
    }
    Ray(const Vec3& origin, const Vec3& direction) :
      m_origin{ origin },
      m_direction{ direction } {};
    ~Ray() = default;

    const Vec3& get_origin() const { return m_origin; }
    const Vec3& get_direction() const { return m_direction; }

    Vec3 at(double t) const { return m_origin + t * m_direction; }

private:

    Vec3 m_origin;
    Vec3 m_direction;
};