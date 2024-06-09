#include "Sphere.h"

std::optional<Hit> Sphere::test_hit(const Ray& ray, const Interval& interval) const
{
    // 2nd degree eq.
    const auto& d{ ray.get_direction() };
    Vec3 center{ m_moving ? center_at_time(ray.get_time()) : m_center };
    const auto oc{ center - ray.get_origin() };

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

    const Vec3 point{ ray.at(t) };
    const auto normal{ (point - center) / m_radius };
    auto [u, v]{ get_uv_coords(normal) };
    return Hit(point, normal, m_material, t, ray, u, v);
}

AABB Sphere::get_bounding_box() const
{
    return m_bbox;
}

Vec3 Sphere::center_at_time(double time) const
{
    return m_center + m_targetVec * time;
}

AABB Sphere::sphere_bounding_box(const Vec3& center, double radius) const
{
    return AABB(center - Vec3{ radius }, center + Vec3{ radius });
}

AABB Sphere::moving_sphere_bounding_box(const Vec3& center, const Vec3& target, double radius) const
{
    AABB box1{ AABB(center - Vec3{ radius }, center + Vec3{ radius }) };
    AABB box2{ AABB(target - Vec3{ radius }, target + Vec3{ radius }) };
    return AABB(box1, box2);
}

constexpr auto pi = std::numbers::pi;

std::pair<double, double> Sphere::get_uv_coords(const Vec3& point)
{
    auto theta{ std::acos(-point.y) };
    auto phi{ std::atan2(-point.z, point.x) + pi };

    return std::make_pair(phi / (2 * pi), theta / pi);
}
