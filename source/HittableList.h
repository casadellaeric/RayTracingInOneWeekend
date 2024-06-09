#pragma once

#include "Hittable.h"

class HittableList : public Hittable
{
public:

    HittableList() = default;
    HittableList(std::shared_ptr<Hittable> object) { add(object); }
    HittableList(const std::vector<std::shared_ptr<Hittable>>& list);

    void add(std::shared_ptr<Hittable> object);
    void clear();

    std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override;

    AABB get_bounding_box() const override;

private:

    friend class Node;

    std::vector<std::shared_ptr<Hittable>> m_list{};
    AABB m_bbox{};
};
