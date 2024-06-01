#include "HittableList.h"

HittableList::HittableList(const std::vector<std::shared_ptr<Hittable>>& list)
{
    for (const auto& elem : list) {
        add(elem);
    }
}

void HittableList::add(std::shared_ptr<Hittable> object)
{
    if (object.get() != this) {  // Check for cyclic references
        m_list.push_back(object);
    } else {
        throw std::runtime_error("Trying to add HittableList to itself!");
    }
}

void HittableList::clear()
{
    m_list.clear();
}

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