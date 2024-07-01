#include "util.h"
#include <iostream>
#include <cstdint>
#include <regex>
#include <fstream>
#include <boost/algorithm/string.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
using namespace std;


constexpr int QUALITY = 50; // JPG Image quality | 0 - 100

const string valid_exts[4] = { "png", "bmp", "jpg", "jpeg" };   // Valid file extentions


PixelVector::PixelVector(const uint8_t r, const uint8_t g, const uint8_t b) {
    this->r = r;
    this->g = g;
    this->b = b;
}


ImageMatrix::ImageMatrix(const int& width, const int& height, const int& bpp) {
    this->image_data = new uint8_t[width * height * bpp]{};
    this->width = width;
    this->height = height;
    this->bpp = bpp;
}


ImageMatrix::ImageMatrix(std::uint8_t* image_data, const int& width, const int& height, const int& bpp) {
    this->image_data = image_data;
    this->width = width;
    this->height = height;
    this->bpp = bpp;
}


ImageMatrix::ImageMatrix(const ImageMatrix& image) {
    ImageMatrix(width, height, bpp);
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            PixelVector pixel_data = image.get(i, j);
            set(i, j, pixel_data);
        }
    }
}


ImageMatrix::~ImageMatrix() {
    delete[] image_data;
    image_data = nullptr;
}


PixelVector ImageMatrix::get(const int& row, const int& column) const {
    // Index of pixel in original image
    const int index = bpp * (column + row * width);
    // RGB values of pixel
    const uint8_t r = image_data[index];
    const uint8_t g = image_data[index + 1];
    const uint8_t b = image_data[index + 2];
    // Create and return Pixel object
    return {r, g, b};
}


void ImageMatrix::set(const int& row, const int& column, const PixelVector& pixel_data) const {
    // Index of pixel in original image
    const int index = bpp * (column + row * width);
    // Set the byte values
    image_data[index] = pixel_data.r;
    image_data[index + 1] = pixel_data.g;
    image_data[index + 2] = pixel_data.b;
}


ImageMatrix* ImageMatrix::filter(const double* matrix) const {
    auto* new_image = new ImageMatrix(width, height, bpp);
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            PixelVector pixel_data = get(i, j);
            const double r = pixel_data.r;
            const double g = pixel_data.g;
            const double b = pixel_data.b;
            const double new_r = matrix[0]*r + matrix[1]*g + matrix[2]*b + matrix[3];
            const double new_g = matrix[4]*r + matrix[5]*g + matrix[6]*b + matrix[7];
            const double new_b = matrix[8]*r + matrix[9]*g + matrix[10]*b + matrix[11];
            pixel_data.r = static_cast<uint8_t>(round(min(255.0, max(0.0, new_r))));
            pixel_data.g = static_cast<uint8_t>(round(min(255.0, max(0.0, new_g))));
            pixel_data.b = static_cast<uint8_t>(round(min(255.0, max(0.0, new_b))));
            new_image->set(i, j, pixel_data);
        }
    }
    return new_image;
}


ImageMatrix* ImageMatrix::convolve(const double* kernel, const size_t& kernel_size) const {
    return convolve(kernel, kernel_size, 1.0);
}


ImageMatrix* ImageMatrix::convolve(const double* kernel, const size_t& kernel_size, const double& scalar) const {
    auto* new_image = new ImageMatrix(width, height, bpp);
    const int kernel_rows = static_cast<int>(sqrt(kernel_size));
    const int kernel_radius = kernel_rows / 2;
    // Iterate through image matrix
    for (int i = 0; i < getHeight(); i++) {
        for (int j = 0; j < getWidth(); j++) {
            // Iterate through the kernel
            double r_total = 0.0;
            double g_total = 0.0;
            double b_total = 0.0;
            for (int k = -kernel_radius; k <= kernel_radius; k++) {
                for (int l = -kernel_radius; l <= kernel_radius; l++) {
                    // Skip pixel if out of range
                    if (i - k < 0 || i - k >= height || j - l < 0 || j - l >= width) {
                        continue;
                    }
                    const double kernel_entry = kernel[(k + kernel_radius) * kernel_rows + (l + kernel_radius)];
                    const PixelVector pixel_data = get(i - k, j - l);
                    r_total += pixel_data.r * kernel_entry * scalar;
                    g_total += pixel_data.g * kernel_entry * scalar;
                    b_total += pixel_data.b * kernel_entry * scalar;
                }
            }
            const uint8_t r = static_cast<uint8_t>(round(min(255.0, max(0.0, r_total))));
            const uint8_t g = static_cast<uint8_t>(round(min(255.0, max(0.0, g_total))));
            const uint8_t b = static_cast<uint8_t>(round(min(255.0, max(0.0, b_total))));
            new_image->set(i, j, PixelVector(r, g, b));
        }
    }
    return new_image;
}


ImageMatrix* read_image(const string& ref_path, int& width, int& height, int& bpp) {
    // Assert valid reference file type
    const string ext = ref_path.substr(ref_path.find_last_of('.') + 1);
    bool invalid_file = true;
    for (const string& valid_ext : valid_exts) {
        if (boost::iequals(ext, valid_ext)) {
            invalid_file = false;
            break;
        }
    }
    if (invalid_file) {
        cout << "Invalid file type: " << ext << endl;
        exit(2);
    }
    // Read image
    uint8_t* image = stbi_load(ref_path.c_str(), &width, &height, &bpp, 0);
    // Move to dynamically loaded array so it can be deleted with the delete[] keyword later
    auto* new_image = new uint8_t[width * height * bpp];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            // Index of pixel in original image
            const int index = bpp * (j + i * width);
            // RGB values of pixel
            const uint8_t r = image[index];
            const uint8_t g = image[index + 1];
            const uint8_t b = image[index + 2];
            const auto pixel = PixelVector(r, g, b);
            new_image[index] = pixel.r;
            new_image[index + 1] = pixel.g;
            new_image[index + 2] = pixel.b;
        }
    }
    stbi_image_free(image);
    // ... process data if not NULL ...
    // ... x = width, y = height, n = # 8-bit components per pixel ...
    // ... replace '0' with '1'..'4' to force that many components per pixel
    // ... but 'n' will always be the number that it would have been if you said 0
    return new ImageMatrix(new_image, width, height, bpp);
}


void write_image(const string& out_path, const ImageMatrix& new_image) {
    const string ext = string(out_path).substr(string(out_path).find_last_of('.') + 1);
    const uint8_t* image_data = new_image.getImageData();
    const int width = new_image.getWidth();
    const int height = new_image.getHeight();
    const int bpp = new_image.getBpp();
    if (boost::iequals(ext, "png"))
        stbi_write_png(out_path.c_str(), width, height, bpp, image_data, width * bpp);
    else if (boost::iequals(ext, "bmp"))
        stbi_write_bmp(out_path.c_str(), width, height, bpp, image_data);
    else if (boost::iequals(ext, "jpg") || boost::iequals(ext, "jpeg"))
        stbi_write_jpg(out_path.c_str(), width, height, bpp, image_data, QUALITY);
}


void write_textfile(const std::string& out_path, const std::string& text) {
    ofstream file;
    file.open(out_path);
    file << text;
    file.close();
}