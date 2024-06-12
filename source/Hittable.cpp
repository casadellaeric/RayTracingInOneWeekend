#include "Hittable.h"

std::optional<Hit> Translation::test_hit(const Ray& ray, const Interval& interval) const
{
    Ray translatedRay{ ray.get_origin() - m_offset, ray.get_direction(), ray.get_time() };

    auto hit{ m_object->test_hit(translatedRay, interval) };
    if (!hit) {
        return std::nullopt;
    }

    hit->point += m_offset;
    return hit;
}

AABB Translation::get_bounding_box() const
{
    return m_bbox;
}

RotationY::RotationY(std::shared_ptr<Hittable> object, double angle) :
  m_object{ object },
  m_cosTheta{ std::cos(degrees_to_radians(angle)) },
  m_sinTheta{ std::sin(degrees_to_radians(angle)) }
{
    auto bbox{ m_object->get_bounding_box() };

    Vec3 currentMin{ rt::INF };
    Vec3 currentMax{ -rt::INF };

    // Rotate every point and compute new min and max
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                Vec3 point{ { i * m_bbox.x.min + (1 - i) * m_bbox.x.max },
                            { j * m_bbox.y.min + (1 - j) * m_bbox.y.max },
                            { k * m_bbox.z.min + (1 - k) * m_bbox.z.max } };

                Vec3 rotated{ object_to_world(point) };

                for (int c = 0; c < 3; ++c) {
                    currentMin[c] = std::fmin(currentMin[c], rotated[c]);
                    currentMax[c] = std::fmax(currentMax[c], rotated[c]);
                }
            }
        }
    }

    m_bbox = AABB(currentMin, currentMax);
}

std::optional<Hit> RotationY::test_hit(const Ray& ray, const Interval& interval) const
{
    auto oRot{ world_to_object(ray.get_origin()) };
    auto dirRot{ world_to_object(ray.get_direction()) };

    Ray rayRot{ oRot, dirRot, ray.get_time() };

    auto hit{ m_object->test_hit(rayRot, interval) };
    if (!hit) {
        return std::nullopt;
    }

    hit->point  = object_to_world(hit->point);
    hit->normal = object_to_world(hit->normal);

    return hit;
}

AABB RotationY::get_bounding_box() const
{
    return m_bbox;
}

Vec3 RotationY::world_to_object(const Vec3& vec) const
{
    return Vec3{ m_cosTheta * vec.x - m_sinTheta * vec.z,
                 vec.y,
                 m_sinTheta * vec.x + m_cosTheta * vec.z };
}

Vec3 RotationY::object_to_world(const Vec3& vec) const
{
    return Vec3{ m_cosTheta * vec.x + m_sinTheta * vec.z,
                 vec.y,
                 -m_sinTheta * vec.x + m_cosTheta * vec.z };
}
