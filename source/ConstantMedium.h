#pragma once

#include "Common.h"
#include "Material.h"
#include "Texture.h"
#include "Hittable.h"

class ConstantMedium : public Hittable
{
public:

    ConstantMedium(std::shared_ptr<Hittable> boundary, double density, const Vec3& color) :
      m_boundary{ boundary },
      m_density{ density },
      m_phaseFunction{ std::make_shared<Isotropic>(color) }
    {
    }

    ConstantMedium(std::shared_ptr<Hittable> boundary,
                   double density,
                   std::shared_ptr<Texture> texture) :
      m_boundary{ boundary },
      m_density{ density },
      m_phaseFunction{ std::make_shared<Isotropic>(texture) }
    {
    }

    virtual std::optional<Hit> test_hit(const Ray& ray, const Interval& interval) const override
    {
        // Test the infinite line for boundary distance (enter to exit)
        // Compute delta t using the density
        // Return the hit at the next distance (if still inside)

        auto firstHit{ m_boundary->test_hit(ray, Interval::universe) };
        if (!firstHit) {
            return std::nullopt;
        }
        double firstT{ firstHit->t };

        auto secondHit{ m_boundary->test_hit(ray, Interval{ firstT + 1e-4, rt::INF }) };
        if (!secondHit) {
            return std::nullopt;
        }
        double secondT{ secondHit->t };

        // Clamp to interval
        if (firstT < interval.min) {
            firstT = interval.min;
        }
        if (secondT > interval.max) {
            secondT = interval.max;
        }

        if (firstT >= secondT) {
            return std::nullopt;
        }

        if (firstT < 0) {
            firstT = 0;
        }

        double rayLength{ ray.get_direction().length() };
        double maxLengthInsideBoundary{ rayLength * (secondT - firstT) };
        double deltaDist{ (-1. / m_density) * std::log(random_double()) };

        if (deltaDist > maxLengthInsideBoundary) {
            return std::nullopt;
        }

        double nextT{ firstT + (deltaDist / rayLength) };
        return Hit{
            ray.at(nextT),
            Vec3{1., 0., 0.},
            m_phaseFunction,
            nextT,
            ray
        };
    };

    virtual AABB get_bounding_box() const override { return m_boundary->get_bounding_box(); }

private:

    std::shared_ptr<Hittable> m_boundary{};
    double m_density{};
    std::shared_ptr<Material> m_phaseFunction{};  // Material inside the volume
};
