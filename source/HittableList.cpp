#include "HittableList.h"

std::optional<Hit> HittableList::test_hit(const Ray& ray, double tMin, double tMax) const
{
    std::optional<Hit> closestHit{ std::nullopt };
    double closestT{ std::numeric_limits<double>::max() };

    for (const auto& hittable : m_list) {
        auto hit{ hittable->test_hit(ray, tMin, tMax) };
        if (hit && hit->t < closestT) {
            closestHit.emplace(*hit);
            closestT = hit->t;
        }
    }
    return closestHit;
}