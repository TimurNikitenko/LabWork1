/* Timur Nikitenko st128375@student.spbu.ru
 *first labwork main function
 */


#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include <cstdlib>
#include "bmp.hpp"

int main() {
    try {
        Image image(400, 300, "test.raw");
        auto original = image.readToVector("test.raw", 400*300);
        std::cout << "loaded image (" << original.size() << " bytes)\n";

        auto cw_rotated = image.clockwiseRotate(original);
        Image(300, 400, "").writeToRaw("clockwise.raw", cw_rotated);
        system("convert -size 300x400 -depth 8 GRAY:clockwise.raw clockwise.png");
        std::cout << "saved clockwise rotated image\n";

        auto ccw_rotated = image.counterClockwiseRotate(original);
        Image(300, 400, "").writeToRaw("counter_clockwise.raw", ccw_rotated);
        system("convert -size 300x400 -depth 8 GRAY:counter_clockwise.raw counter_clockwise.png");
        std::cout << "saved counter-clockwise rotated image\n";

        image.saveAsGaussianImage(original, 5); 
        system("convert -size 400x300 -depth 8 GRAY:gaussian_blurred.raw gaussian_blurred.png");
        std::cout << "saved Gaussian blurred image\n";       

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "program completed successfully\n";
    return 0;

}
