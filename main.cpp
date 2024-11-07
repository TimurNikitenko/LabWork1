#include "bmp.hpp"
#include <iostream>

int main() {
    BMPImage image;

    try {
        image.load("input.bmp");

        image.rotateClockwise();
        image.save("output_clockwise.bmp");

        image.rotateCounterclockwise();
        image.save("output_counterclockwise.bmp");

        image.applyGaussianFilter();
        image.save("output_gaussian.bmp");

    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
} 

