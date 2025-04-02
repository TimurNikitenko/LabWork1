/* Timur Nikitenko st128375@student.spbu.ru
 * functions definitions
 */

#include "bmp.hpp"

Image::Image(int width, int height, std::string path)
    : width(width), height(height), path(path)
{
}

std::vector<unsigned char> Image::readToVector(const std::string& path, int size)
{
    std::ifstream file(path, std::ios::binary);
    std::vector<unsigned char> output(size);

    if (!file.read(reinterpret_cast<char*>(output.data()), size))
    {
        throw std::runtime_error("Error reading file: " + path);
    }

    return output;
}

void Image::writeToRaw(const std::string& filename, const std::vector<unsigned char>& data)
{
    std::ofstream file(filename, std::ios::binary);
    file.write(reinterpret_cast<const char*>(data.data()), data.size());
}

std::vector<std::vector<unsigned char>> Image::oneDimToTwoDim(const std::vector<unsigned char>& one_dim)
{
    std::vector<std::vector<unsigned char>> two_dim(height, std::vector<unsigned char>(width));
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            two_dim[y][x] = one_dim[y * width + x];
        }
    }
    return two_dim;
}

std::vector<unsigned char> Image::twoDimToOneDim(const std::vector<std::vector<unsigned char>>& two_dim)
{
    std::vector<unsigned char> one_dim(width * height);
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            one_dim[x + y * width] = two_dim[y][x];
        }
    }
    return one_dim;
}

std::vector<unsigned char> Image::clockwiseRotate(const std::vector<unsigned char>& img)
{
    auto img_2d = oneDimToTwoDim(img);
    std::vector<std::vector<unsigned char>> rotated_img(width, std::vector<unsigned char>(height));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            rotated_img[x][height - y - 1] = img_2d[y][x];
        }
    }

    return twoDimToOneDim(rotated_img);
}

std::vector<unsigned char> Image::counterClockwiseRotate(const std::vector<unsigned char>& img)
{
    return clockwiseRotate(clockwiseRotate(clockwiseRotate(img)));
}

std::pair<std::vector<std::vector<float>>, int> Image::makeKernel(int sigma)
{
    int kernel_size = (sigma * 6) + 1;
    int kernel_length = kernel_size;
    std::vector<std::vector<float>> kernel(kernel_length, std::vector<float>(kernel_length));

    float sum = 0.0f;
    int radius = kernel_length / 2;
    for (int y = -radius; y <= radius; y++)
    {
        for (int x = -radius; x <= radius; x++)
        {
            kernel[y + radius][x + radius] = (1.0f / (2.0f * 3.14f * sigma * sigma)) *
                                              std::exp(-((x * x + y * y) / (2.0f * sigma * sigma)));
            sum += kernel[y + radius][x + radius];
        }
    }

    for (int y = 0; y < kernel_length; y++)
    {
        for (int x = 0; x < kernel_length; x++)
        {
            kernel[y][x] /= sum;
        }
    }

    return {kernel, kernel_length};
}

template <typename T>
T Image::clamp(T value, T min_value, T max_value)
{
    return (value < min_value) ? min_value : (value > max_value ? max_value : value);
}

void Image::saveAsGaussianImage(const std::vector<unsigned char>& img, int sigma)
{
    auto img_2d = oneDimToTwoDim(img);
    auto kernel_pair = makeKernel(sigma);
    auto kernel = kernel_pair.first;
    int kernel_length = kernel_pair.second;
    int kernel_radius = kernel_length / 2;

    std::vector<std::vector<unsigned char>> blurred_img(height, std::vector<unsigned char>(width, 0));

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            float sum = 0.0f;

            for (int ky = -kernel_radius; ky <= kernel_radius; ky++)
            {
                for (int kx = -kernel_radius; kx <= kernel_radius; kx++)
                {
                    int ny = y + ky;
                    int nx = x + kx;

                    if (ny < 0) ny = 0;
                    if (ny >= height) ny = height - 1;
                    if (nx < 0) nx = 0;
                    if (nx >= width) nx = width - 1;

		    sum += img_2d[ny][nx] * kernel[ky + kernel_radius][kx + kernel_radius];
                }
            }

            blurred_img[y][x] = static_cast<unsigned char>(clamp(std::round(sum), 0.0f, 255.0f));
        }
    }

    auto blurred_img_1d = twoDimToOneDim(blurred_img);
    writeToRaw("gaussian_blurred.raw", blurred_img_1d);
}
