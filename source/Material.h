#pragma once

#include "Texture.h"
#include "Common.h"
#include "Hittable.h"

class Material
{
public:

    virtual ~Material() = default;

    virtual bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const = 0;
    virtual Vec3 emitted(const Vec3& point, double u, double v) const;
};

class Lambertian : public Material
{
public:

    Lambertian(const Vec3& albedo) :
      m_tex(std::make_shared<SolidColor>(albedo))
    {
    }

    Lambertian(std::shared_ptr<Texture> tex) :
      m_tex(tex)
    {
    }

    bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const override;

private:

    std::shared_ptr<Texture> m_tex;
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

class DiffuseLight : public Material
{
public:

    DiffuseLight(std::shared_ptr<Texture> texture) :
      m_texture{ texture }
    {
    }

    DiffuseLight(const Vec3& color) :
      m_texture{ std::make_shared<SolidColor>(color) }
    {
    }

    bool scatter(const Ray& ray, const Hit& hit, Vec3& attenuation, Ray& outRay) const override;
    Vec3 emitted(const Vec3& point, double u, double v) const override;

private:

    std::shared_ptr<Texture> m_texture{};
};