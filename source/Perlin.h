#pragma once
#include <array>
#include "Common.h"

class Perlin
{
public:

    Perlin()
    {
        for (size_t i = 0; i < pointCount; ++i) {
            m_randVec[i] = random_vec(-1., 1.).normalized();
        }
        m_xPerm = generatePerm();
        m_yPerm = generatePerm();
        m_zPerm = generatePerm();
    }

    double get_noise(const Vec3& point) const
    {
        auto u{ point.x - std::floor(point.x) };
        auto v{ point.y - std::floor(point.y) };
        auto w{ point.z - std::floor(point.z) };

        auto i{ static_cast<int>(std::floor(point.x)) & 255 };
        auto j{ static_cast<int>(std::floor(point.y)) & 255 };
        auto k{ static_cast<int>(std::floor(point.z)) & 255 };

        Vec3 c[2][2][2];
        for (int di = 0; di < 2; ++di) {
            for (int dj = 0; dj < 2; ++dj) {
                for (int dk = 0; dk < 2; ++dk) {
                    c[di][dj][dk] = m_randVec[m_xPerm[(i + di) & 255] ^ m_yPerm[(j + dj) & 255]
                                              ^ m_zPerm[(k + dk) & 255]];
                }
            }
        }

        return perlin_interpolation(c, u, v, w);
    }

    double get_turbulence(Vec3 point, int depth) const
    {
        double accum{ 0. };
        double ithWeight{ 1. };
        for (int i = 0; i < depth; i++) {
            accum     += ithWeight * get_noise(point);
            ithWeight /= 2.;
            point     *= 2.;
        }

        return std::fabs(accum);
    }

private:

    static const int pointCount{ 256 };

    std::array<Vec3, pointCount> m_randVec{};
    std::array<int, pointCount> m_xPerm{};
    std::array<int, pointCount> m_yPerm{};
    std::array<int, pointCount> m_zPerm{};

private:

    std::array<int, pointCount> generatePerm()
    {
        std::array<int, pointCount> ret{};
        for (size_t i = 0; i < pointCount; ++i) {
            ret[i] = i;
        }
        permute(ret, pointCount);
        return ret;
    }

    static void permute(std::array<int, pointCount>& p, int n)
    {
        for (size_t i = n - 1; i > 0; i--) {
            int target{ random_int(0, i) };
            std::swap(p[i], p[target]);
        }
    }

    static double trilinear_interpolation(double c[2][2][2], double u, double v, double w)
    {
        double accum{ 0. };
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                for (size_t k = 0; k < 2; ++k) {
                    accum += (i * u + (1 - i) * (1 - u)) * (j * v + (1 - j) * (1 - v))
                             * (k * w + (1 - k) * (1 - w)) * c[i][j][k];
                }
            }
        }
        return accum;
    }

    static double perlin_interpolation(const Vec3 c[2][2][2], double u, double v, double w)
    {
        auto uu{ u * u * (3 - 2 * u) };
        auto vv{ v * v * (3 - 2 * v) };
        auto ww{ w * w * (3 - 2 * w) };

        double accum{ 0. };
        for (size_t i = 0; i < 2; ++i) {
            for (size_t j = 0; j < 2; ++j) {
                for (size_t k = 0; k < 2; ++k) {
                    Vec3 weight{ u - i, v - j, w - k };
                    accum += (i * uu + (1 - i) * (1 - uu)) * (j * vv + (1 - j) * (1 - vv))
                             * (k * ww + (1 - k) * (1 - ww)) * dot(c[i][j][k], weight);
                }
            }
        }
        return accum;
    }
};
