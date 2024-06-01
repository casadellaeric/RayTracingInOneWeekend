#pragma once

#include "Common.h"
#include "Hittable.h"

class Material
{
public:

    virtual ~Material() = default;

    virtual bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const = 0;
};

class Lambertian : public Material
{
public:

    Lambertian(const Vec3& albedo) :
      m_albedo(albedo)
    {
    }

    bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const override;

private:

    Vec3 m_albedo;
};

class Metal : public Material
{
public:

    Metal(const Vec3& albedo, double fuzz) :
      m_albedo{ albedo },
      m_fuzz{ fuzz < 1. ? fuzz : 1. }
    {
    }

    bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const override;

private:

    Vec3 m_albedo;
    double m_fuzz;
};

class Dielectric : public Material
{
public:

    Dielectric(double refractionRatio) :
      m_refractionRatio{ refractionRatio }
    {
    }

    bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const override;

private:

    double reflectance(double cosine, double refractionIndex) const;

private:

    double m_refractionRatio;
};
