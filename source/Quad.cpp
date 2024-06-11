#include "Quad.h"

std::optional<Hit> Quad::test_hit(const Ray& ray, const Interval& interval) const
{
    auto ndotd{ dot(m_normal, ray.get_direction()) };
    if (std::fabs(ndotd) < 1e-8) {
        return std::nullopt;
    }

    auto ndoto{ dot(m_normal, ray.get_origin()) };
    auto t{ (D - ndoto) / ndotd };
    if (!interval.contains(t)) {
        return std::nullopt;
    }

    auto P{ ray.at(t) };

    auto QtoP{ P - Q };

    auto alpha{ dot(w, cross(QtoP, v)) };
    auto beta{ dot(w, cross(u, QtoP)) };

    const auto unitInterval{ Interval(0., 1.) };
    if (!(unitInterval.contains(alpha) && unitInterval.contains(beta))) {
        return std::nullopt;
    }

    return Hit(P, m_normal, m_material, t, ray, alpha, beta);
}

AABB Quad::get_bounding_box() const
{
    return m_bbox;
}

AABB Quad::quad_bounding_box(const Vec3& Q, const Vec3& u, const Vec3& v)
{
    AABB diagBox1{ Q, Q + u + v };
    AABB diagBox2{ Q + u, Q + v };
    return AABB(diagBox1, diagBox2);
}
