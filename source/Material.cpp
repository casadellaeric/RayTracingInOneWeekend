#include "Material.h"

bool Lambertian::scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& out_ray) const
{
    auto outDirection{ hit.normal + random_vec_unit_sphere() };
    if (outDirection.near_zero()) {
        outDirection = hit.normal;
    }
    out_ray     = Ray(hit.point, outDirection);
    attenuation = m_albedo;
    return true;
}

bool Metal::scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& out_ray) const
{
    out_ray     = Ray(hit.point, reflect(ray.get_direction(), hit.normal));
    attenuation = m_albedo;
    return true;
}
