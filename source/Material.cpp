#include "Material.h"

bool Lambertian::scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const
{
    auto outDirection{ hit.normal + random_vec_on_unit_sphere() };
    if (outDirection.near_zero()) {
        outDirection = hit.normal;
    }
    outRay      = Ray(hit.point, outDirection, ray.get_time());
    attenuation = m_albedo;
    return true;
}

bool Metal::scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const
{
    auto reflectedRay{ reflect(ray.get_direction(), hit.normal) };
    outRay      = Ray(hit.point,
                 (reflectedRay.normalized() + random_vec_on_unit_sphere() * m_fuzz),
                 ray.get_time());
    attenuation = m_albedo;
    return dot(hit.normal, reflectedRay) > 0.;
}

bool Dielectric::scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const
{
    auto refractionIndex{ hit.type == HitType::front ? 1. / m_refractionRatio : m_refractionRatio };
    auto rayDir{ ray.get_direction().normalized() };
    auto cosTheta{ std::fmin(dot(-rayDir, hit.normal), 1.) };
    auto sinTheta{ std::sqrt(1. - cosTheta * cosTheta) };

    // If index * sinTheta > 1, Snell's law cannot hold, and the ray reflects instead
    bool shouldReflect{ refractionIndex * sinTheta > 1.
                        || reflectance(cosTheta, refractionIndex) > random_double() };

    Vec3 outDir{ shouldReflect ? reflect(rayDir, hit.normal)
                               : refract(rayDir, hit.normal, refractionIndex) };

    outRay      = Ray(hit.point, outDir, ray.get_time());
    attenuation = Vec3{ 1. };
    return true;
}

// Schlick approximation
double Dielectric::reflectance(double cosine, double refractionIndex) const
{
    auto r0{ std::pow((1. - refractionIndex) / (1. + refractionIndex), 2) };
    return r0 + (1. - r0) * std::pow(1. - cosine, 5);
}
