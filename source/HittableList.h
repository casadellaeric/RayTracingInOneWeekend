#pragma once

#include "Hittable.h"

class HittableList : public Hittable
{
public:

    HittableList() = default;
    HittableList(const std::vector<std::shared_ptr<Hittable>>& list) :
      m_list(list)
    {
    }

    void add(std::shared_ptr<Hittable> object) { m_list.push_back(object); }
    void clear() { m_list.clear(); }

    std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override;

private:

    std::vector<std::shared_ptr<Hittable>> m_list{};
};
