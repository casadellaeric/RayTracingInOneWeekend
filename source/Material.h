#pragma once

#include "Common.h"
#include "Hittable.h"

class Material
{
public:

    virtual ~Material() = default;

    virtual bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& out_ray) const = 0;
};

class Lambertian : public Material
{
public:

    Lambertian(const Vec3& albedo) :
      m_albedo(albedo)
    {
    }

    bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& out_ray) const override;

private:

    Vec3 m_albedo;
};

class Metal : public Material
{
public:

    Metal(const Vec3& albedo) :
      m_albedo(albedo)
    {
    }

    bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& out_ray) const override;

private:

    Vec3 m_albedo;
};
