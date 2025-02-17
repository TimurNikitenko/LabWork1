/* Timur Nikitenko st128375@student.spbu.ru
 *first labwork
 */


#include<iostream>
#include<fstream>
#include<vector>
#include<string>
#include "bmp.hpp"


void test()
{

    std::string path = "test.raw";
    Image image = Image(512, 512, path);
    std::cout<<512*512<<" byte will be ocupied by img";
    std::vector<unsigned char> vector = image.readToVector(path, 512*512);
    /*
    for(int i = 0; i<= 10; i++)
    {
        std::cout<<vector[i]<<std::endl;
    }
    */
    //image.writeToFile("new.bmp",vector);
    //image.writeToRaw("new.raw", vector);
    image.writeToRaw("cwrotated.raw",image.clockwiseRotate(vector));
    image.writeToRaw("ccwrotated.raw", image.counterClockwiseRotate(vector));
    image.saveAsGaussianImage(image.counterClockwiseRotate(vector),5);
}

int main()
{
    test();
}
