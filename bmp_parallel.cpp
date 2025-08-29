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
 
     if (one_dim.size() != static_cast<size_t>(width * height)) {
         throw std::runtime_error("Buffer size mismatch in oneDimToTwoDim");
     }
 
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
 
 std::vector<unsigned char> Image::rotateImpl(const std::vector<unsigned char>& img, bool clockwise) {
     if (img.size() != static_cast<size_t>(width * height)) {
         throw std::runtime_error("Input image size mismatch");
     }
 
     auto img_2d = oneDimToTwoDim(img);
     const int out_width = height;
     const int out_height = width;
     
     std::vector<std::vector<unsigned char>> rotated_img(out_height, std::vector<unsigned char>(out_width));
 
     #pragma omp parallel for collapse(2)
     
     for (int y = 0; y < height; y++) {
         for (int x = 0; x < width; x++) {
             if (clockwise) {
                 rotated_img[x][height - 1 - y] = img_2d[y][x];
             } else {
                 rotated_img[width - 1 - x][y] = img_2d[y][x];
             }
         }
     }
 
     std::vector<unsigned char> result(out_width * out_height);
     for (int y = 0; y < out_height; y++) {
         for (int x = 0; x < out_width; x++) {
             result[y * out_width + x] = rotated_img[y][x]; 
         }
     }
 
     return result;
 }
 
 std::vector<unsigned char> Image::clockwiseRotate(const std::vector<unsigned char>& img) {
     return rotateImpl(img, true);
 }
 
 std::vector<unsigned char> Image::counterClockwiseRotate(const std::vector<unsigned char>& img) {
     return rotateImpl(img, false);
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
 
 
 void Image::saveAsGaussianImage(const std::vector<unsigned char>& img, int sigma) {
     const int original_width = width;
     const int original_height = height;
     
     auto img_2d = oneDimToTwoDim(img);
     
     auto [kernel, kernel_length] = makeKernel(sigma);
     const int kernel_radius = kernel_length / 2;
     
     std::vector<std::vector<unsigned char>> blurred_img(original_height, 
                                                       std::vector<unsigned char>(original_width, 0));
 
     #pragma omp parallel for schedule(static)
                                                 
     for (int y = 0; y < original_height; y++) {
         for (int x = 0; x < original_width; x++) {
             float sum = 0.0f;
             float weight_sum = 0.0f;
 
             for (int ky = -kernel_radius; ky <= kernel_radius; ky++) {
                 for (int kx = -kernel_radius; kx <= kernel_radius; kx++) {
                     int ny = y + ky;
                     int nx = x + kx;
                     
                     if (ny >= 0 && ny < original_height && nx >= 0 && nx < original_width) {
                         float weight = kernel[ky + kernel_radius][kx + kernel_radius];
                         sum += img_2d[ny][nx] * weight;
                         weight_sum += weight;
                     }
                 }
             }
 
             if (weight_sum > 0) sum /= weight_sum;
             blurred_img[y][x] = static_cast<unsigned char>(clamp(std::round(sum), 0.0f, 255.0f));
         }
     }
 
     auto blurred_1d = twoDimToOneDim(blurred_img);
     writeToRaw("gaussian_blurred.raw", blurred_1d);
  
     width = original_width;
     height = original_height;
 
 }

 