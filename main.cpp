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
     const int H = 400, W = 300;    
         Image image(H, W, "test_400x300.raw");
         auto original = image.readToVector("test_400x300.raw", H*W);
         std::cout << "loaded image (" << original.size() << " bytes)\n";
         auto cw_rotated = image.clockwiseRotate(original);
         Image(W, H, "").writeToRaw("clockwise.raw", cw_rotated);
         system("convert -size 300x400 -depth 8 GRAY:clockwise.raw clockwise.png");
         std::cout << "saved clockwise rotated image\n";
         auto ccw_rotated = image.counterClockwiseRotate(original);
         Image(W, H, "").writeToRaw("counter_clockwise.raw", ccw_rotated);
         system("convert -size 300x400 -depth 8 GRAY:counter_clockwise.raw counter_clockwise.png");
         std::cout << "saved counter-clockwise rotated image\n";
         Image(H, W, "").saveAsGaussianImage(original, 10);
         auto blurred = image.readToVector("gaussian_blurred.raw", H*W);
         auto rotated_blurred = image.counterClockwiseRotate(blurred);
         Image(W, H, "").writeToRaw("rotated_and_blurred.raw", rotated_blurred);
         system("convert -size 300x400 -depth 8 GRAY:rotated_and_blurred.raw rotated_and_blurred.png");
         std::cout << "saved blurred image\n";
 
     } catch (const std::exception& e) {
         std::cerr << "Error: " << e.what() << std::endl;
         return 1;
     }
 
     std::cout << "program completed successfully\n";
     return 0;
 
 }