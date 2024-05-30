#pragma once

#include <optional>
#include <vector>
#include <iostream>
#include <cassert>
#include <cmath>
#include <limits>
#include <numbers>

#include "Vec3.h"
#include "Ray.h"

inline double degrees_to_radians(double degrees)
{
    return degrees * std::numbers::pi / 180.0;
}
