/*Nikitenko Timur st128375@student.spbu.ru
  BMP
*/
#ifndef BMP_HPP
#define BMP_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include <utility>

struct Image
{
public:
    int width;
    int height;
    std::string path;

    Image(int width, int height, std::string path);

    std::vector<unsigned char> readToVector(const std::string& path, int size);
    void writeToRaw(const std::string& filename, const std::vector<unsigned char>& data);
    std::vector<std::vector<unsigned char>> oneDimToTwoDim(const std::vector<unsigned char>& one_dim);
    std::vector<unsigned char> twoDimToOneDim(const std::vector<std::vector<unsigned char>>& two_dim);
    
    std::vector<unsigned char> clockwiseRotate(const std::vector<unsigned char>& img);
    std::vector<unsigned char> counterClockwiseRotate(const std::vector<unsigned char>& img);

    void saveAsGaussianImage(const std::vector<unsigned char>& img, int sigma);

private:
    std::pair<std::vector<std::vector<float>>, int> makeKernel(int sigma);
    template<typename T>
    T clamp(T value, T min_value, T max_value);
};

#endif // BMP_HPP             
