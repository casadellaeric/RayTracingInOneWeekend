#include "Sphere.h"

std::optional<Hit> Sphere::test_hit(const Ray& ray, const Interval& interval) const
{
    // 2nd degree eq.
    const auto& d{ ray.get_direction() };
    const auto oc{ m_center - ray.get_origin() };

    const double a{ d.length2() };
    const double h{ dot(d, oc) };
    const double c{ oc.length2() - (m_radius * m_radius) };

    const double discriminant{ h * h - a * c };
    if (discriminant < 0.) {
        return std::nullopt;
    }

    const double sqrtDiscriminant{ std::sqrt(discriminant) };
    const double t1{ (h + sqrtDiscriminant) / a };
    const double t2{ (h - sqrtDiscriminant) / a };
    const double t{ std::min(t1, t2) };
    if (!interval.contains(t)) {
        return std::nullopt;
    }

    const Vec3 point = ray.at(t);
    return Hit(point, (point - m_center) / m_radius, t, ray);
}