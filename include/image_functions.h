#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include <string>
#include <stdint.h>
#include <functional>
#include "util.h"

std::uint8_t *read_image(std::string old_image_path, int &width, int &height, int &bpp);
void write_image(std::string ref_path, std::string dest_path, std::uint8_t *new_image, int &width, int &height, int &bpp);
void free_image(std::uint8_t *image);
std::uint8_t *pixel_transform(std::uint8_t *image, int &width, int &height, int &bpp, std::function<void(Pixel&)> func);
std::uint8_t *pixel_image(std::uint8_t *image, int &width, int &height, int &bpp);
std::uint8_t *grayscale_image(std::uint8_t *image, int &width, int &height, int &bpp);
std::uint8_t *red_image(std::uint8_t *image, int &width, int &height, int &bpp);
std::uint8_t *green_image(std::uint8_t *image, int &width, int &height, int &bpp);
std::uint8_t *blue_image(std::uint8_t *image, int &width, int &height, int &bpp);

#endif