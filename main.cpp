/* Timur Nikitenko st128375@student.spbu.ru
 *first labwork
 */


#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include "bmp.hpp"


 
int main()
{
    std::string path = "test.raw";
    Image image = Image(512, 512, path);
    std::cout<<512*512<<" byte will be ocupied by img";
    std::vector<unsigned char> vector = image.readToVector(path, 512*512);
    image.writeToRaw("cwrotated.raw",image.clockwiseRotate(vector));
    image.writeToRaw("ccwrotated.raw", image.counterClockwiseRotate(vector));
    image.saveAsGaussianImage(image.counterClockwiseRotate(vector),5);

    // commands below convert raw files to png format
    system("convert -size 512x512 -depth 8 gray:cwrotated.raw cwrotated.png");
    system("convert -size 512x512 -depth 8 gray:ccwrotated.raw ccwrotated.png");
    system("convert -size 512x512 -depth 8 gray:gaussian_blurred.raw gaussian_blurred.png");

    return 0;
}
