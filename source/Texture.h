#pragma once

#include "rtw_stb_image.h"
#include "Common.h"

class Texture
{
public:

    virtual ~Texture() = default;

    virtual Vec3 value(double u, double v, const Vec3& point) const = 0;
};

class SolidColor : public Texture
{
public:

    SolidColor(const Vec3& albedo) :
      m_albedo(albedo)
    {
    }

    Vec3 value(double u, double v, const Vec3& point) const override { return m_albedo; };

private:

    Vec3 m_albedo{};
};

class CheckerTexture : public Texture
{
public:

    CheckerTexture(double scale, std::shared_ptr<Texture> even, std::shared_ptr<Texture> odd) :
      m_invScale(1. / scale),
      m_even(even),
      m_odd(odd)
    {
    }

    CheckerTexture(double scale, const Vec3& evenColor, const Vec3& oddColor) :
      m_invScale(1. / scale),
      m_even(std::make_shared<SolidColor>(evenColor)),
      m_odd(std::make_shared<SolidColor>(oddColor))
    {
    }

    Vec3 value(double u, double v, const Vec3& point) const override
    {
        auto xInt{ static_cast<int>(std::floor(point.x * m_invScale)) };
        auto yInt{ static_cast<int>(std::floor(point.y * m_invScale)) };
        auto zInt{ static_cast<int>(std::floor(point.z * m_invScale)) };
        bool isEven{ (xInt + yInt + zInt) % 2 == 0 };

        return isEven ? m_even->value(u, v, point) : m_odd->value(u, v, point);
    };

private:

    double m_invScale;
    std::shared_ptr<Texture> m_even;
    std::shared_ptr<Texture> m_odd;
};

class ImageTexture : public Texture
{
public:

    ImageTexture(const char* fileName) :
      m_image(fileName)
    {
    }

    Vec3 value(double u, double v, const Vec3& point) const override
    {
        if (m_image.height() <= 0) {
            return Vec3{ 0., 1., 1. };  // Debug color
        }

        u = rt::clamp(u, Interval{ 0., 1. });
        v = 1. - rt::clamp(v, Interval{ 0., 1. });

        auto color8{ m_image.pixel_data(int(u * m_image.width()), int(v * m_image.height())) };
        return Vec3{ color8[0] / 255., color8[1] / 255., color8[2] / 255. };
    }

private:

    rtw_image m_image{};
};
