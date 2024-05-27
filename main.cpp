#include "Vec3.h"

#include <iostream>

int main()
{
    // Image

    int imageWidth  = 1024;
    int imageHeight = 1024;

    // Render

    std::cout << "P3\n" << imageWidth << ' ' << imageHeight << "\n255\n";

    for (int j = 0; j < imageHeight; j++) {
        std::clog << "\rScanlines remaining: " << (imageHeight - j) << " " << std::flush;
        for (int i = 0; i < imageWidth; i++) {
            std::cout << to_rgb({ double(i) / (imageWidth - 1), double(j) / (imageHeight - 1), 0. })
                      << '\n';
        }
    }
    std::clog << "\rDone.\n";
}