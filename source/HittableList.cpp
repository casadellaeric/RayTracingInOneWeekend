#include "HittableList.h"

std::optional<Hit> HittableList::test_hit(const Ray& ray, const Interval& interval) const
{
    std::optional<Hit> closestHit{ std::nullopt };
    double closestT{ std::numeric_limits<double>::max() };

    for (const auto& hittable : m_list) {
        auto hit{ hittable->test_hit(ray, interval) };
        if (hit && hit->t < closestT) {
            closestHit.emplace(*hit);
            closestT = hit->t;
        }
    }
    return closestHit;
}