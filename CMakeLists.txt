﻿# CMakeList.txt : CMake project for RayTracing, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("RayTracing")

# Add source to this project's executable.
add_executable (RayTracing  "source/main.cpp" "source/Vec3.h"  "source/Ray.h" "source/Sphere.h" "source/Sphere.cpp" "source/Hittable.h"  "source/HittableList.h" "source/HittableList.cpp"  "source/Common.h"    "source/Camera.h" "source/Camera.cpp" "source/Material.h" "source/Interval.h" "source/Material.cpp"  "source/AABB.h" "source/AABB.cpp" "source/Node.h" "source/Texture.h" "source/rtw_stb_image.h" "external/stb_image.h" "source/Perlin.h" "source/Quad.h" "source/Quad.cpp"  "source/Hittable.cpp"  "source/ConstantMedium.h")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET RayTracing PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
