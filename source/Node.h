#pragma once

#include <algorithm>

#include "Common.h"
#include "Hittable.h"
#include "HittableList.h"

// BVH Node
class Node : public Hittable
{
public:

    Node(HittableList list) :
      Node(list.m_list, 0, list.m_list.size())
    {
    }

    Node(std::vector<std::shared_ptr<Hittable>>& list, size_t start, size_t end)
    {
        m_bbox = AABB::empty;
        for (size_t i = start; i < end; ++i) {
            m_bbox = AABB(m_bbox, list[i]->get_bounding_box());
        }

        int longestAxis{ m_bbox.longest_axis() };
        auto comparator{ (longestAxis == 0)   ? bbox_compare_x
                         : (longestAxis == 1) ? bbox_compare_y
                                              : bbox_compare_z };

        size_t spanSize{ end - start };
        if (spanSize == 1) {
            m_left = m_right = list[start];
        } else if (spanSize == 2) {
            m_left  = list[start];
            m_right = list[start + 1];
        } else {
            std::sort(list.begin() + start, list.begin() + end, comparator);
            m_left  = std::make_shared<Node>(list, start, start + spanSize / 2);
            m_right = std::make_shared<Node>(list, start + spanSize / 2, end);
        }
    }

    std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override
    {
        if (!m_bbox.hit(ray, interval)) {
            return std::nullopt;
        }

        auto hitLeft{ m_left->test_hit(ray, interval) };
        auto hitRight{ m_right->test_hit(
            ray,
            Interval(interval.min, hitLeft.has_value() ? hitLeft.value().t : interval.max)) };

        if (hitRight.has_value()) {
            return hitRight;
        }
        return hitLeft;
    };

    AABB get_bounding_box() const override { return m_bbox; };

private:

    static bool bbox_compare_axis(const std::shared_ptr<Hittable> a,
                                  const std::shared_ptr<Hittable> b,
                                  int axis)
    {
        return a->get_bounding_box().axis_interval(axis).min
               < b->get_bounding_box().axis_interval(axis).min;
    }

    static bool bbox_compare_x(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return bbox_compare_axis(a, b, 0);
    }

    static bool bbox_compare_y(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return bbox_compare_axis(a, b, 1);
    }

    static bool bbox_compare_z(const std::shared_ptr<Hittable> a, const std::shared_ptr<Hittable> b)
    {
        return bbox_compare_axis(a, b, 2);
    }

    std::shared_ptr<Hittable> m_left{};
    std::shared_ptr<Hittable> m_right{};
    AABB m_bbox{};
};
