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
        // Load original image
        Image image(400, 300, "test.raw");
        auto original = image.readToVector("test.raw", 400*300);
        std::cout << "Loaded 400x300 image (" << original.size() << " bytes)\n";

        // Clockwise rotation (400x300 → 300x400)
        auto cw_rotated = image.clockwiseRotate(original);
        Image(300, 400, "").writeToRaw("clockwise.raw", cw_rotated);
        system("convert -size 300x400 -depth 8 GRAY:clockwise.raw clockwise.png");
        std::cout << "Saved clockwise rotated image (300x400)\n";

        // Counter-clockwise rotation (400x300 → 300x400)
        auto ccw_rotated = image.counterClockwiseRotate(original);
        Image(300, 400, "").writeToRaw("counter_clockwise.raw", ccw_rotated);
        system("convert -size 300x400 -depth 8 GRAY:counter_clockwise.raw counter_clockwise.png");
        std::cout << "Saved counter-clockwise rotated image (300x400)\n";

        // Gaussian blur (original dimensions)
        image.saveAsGaussianImage(original, 5); // sigma=5
        system("convert -size 400x300 -depth 8 GRAY:gaussian_blurred.raw gaussian_blurred.png");
        std::cout << "Saved Gaussian blurred image (400x300)\n";

        // Bonus: Blur then rotate
        Image(400, 300, "").saveAsGaussianImage(original, 3); // lighter blur
        auto blurred = image.readToVector("gaussian_blurred.raw", 400*300);
        auto rotated_blurred = image.counterClockwiseRotate(blurred);
        Image(300, 400, "").writeToRaw("blurred_then_rotated.raw", rotated_blurred);
        system("convert -size 300x400 -depth 8 GRAY:blurred_then_rotated.raw blurred_then_rotated.png");
        std::cout << "Saved blurred then rotated image (300x400)\n";

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "All operations completed successfully!\n";
    return 0;
}
