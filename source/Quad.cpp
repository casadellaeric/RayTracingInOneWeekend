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

std::shared_ptr<HittableList> box(const Vec3& a, const Vec3& b, std::shared_ptr<Material> material)
{
    auto min{ Vec3(std::fmin(a.x, b.x), std::fmin(a.y, b.y), std::fmin(a.z, b.z)) };
    auto max{ Vec3(std::fmax(a.x, b.x), std::fmax(a.y, b.y), std::fmax(a.z, b.z)) };

    Vec3 dx{ max.x - min.x, 0., 0. };
    Vec3 dy{ 0., max.y - min.y, 0. };
    Vec3 dz{ 0., 0., max.z - min.z };

    auto box{ std::make_shared<HittableList>() };
    box->add(std::make_shared<Quad>(Vec3(min.x, min.y, max.z), dx, dy, material));
    box->add(std::make_shared<Quad>(Vec3(max.x, min.y, max.z), -dz, dy, material));
    box->add(std::make_shared<Quad>(Vec3(max.x, min.y, min.z), -dx, dy, material));
    box->add(std::make_shared<Quad>(Vec3(min.x, min.y, min.z), dz, dy, material));
    box->add(std::make_shared<Quad>(Vec3(min.x, max.y, max.z), dx, -dz, material));
    box->add(std::make_shared<Quad>(Vec3(min.x, min.y, min.z), dx, dz, material));

    return box;
}
