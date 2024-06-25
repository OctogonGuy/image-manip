#ifndef IMAGE_FUNCTIONS_H
#define IMAGE_FUNCTIONS_H

#include "util.h"

/**
 * Transforms an image into a pixelated version of itself.
 * @param image The image
 * @param divs The number of times the image will be divided on the longest side
 * @return The output image
*/
ImageMatrix* pixelate(const ImageMatrix& image, const int& divs);


/**
 * Highlights large differences in pixel values
 * @param image The image
 * @return The output image
 */
ImageMatrix* outline(const ImageMatrix& image);


/**
 * Emphasizes differences in adjacent pixel values
 * @param image The image
 * @return The output image
 */
ImageMatrix* sharpen(const ImageMatrix& image);


/**
 * Averages each pixel's value with the value of its neighboring pixels
 * @param image The image
 * @param radius 2 * radius + 1 = Width and height of the kernel
 * @return The output image
 */
ImageMatrix* box_blur(const ImageMatrix& image, const int& radius);


/**
 * Blurs the image by a Gaussian function
 * @param image The image
 * @param radius 2 * radius + 1 = Width and height of the kernel
 * @param sigma The standard deviation of the Gaussian distribution
 * @return The output image
 */
ImageMatrix* gaussian_blur(const ImageMatrix& image, const int& radius, const double& sigma);


/**
 * Averages the colors of an image to make it grayscale.
 * @param image The image
 * @return The output image
*/
ImageMatrix* grayscale(const ImageMatrix& image);


/**
 * Inverts the colors of the image
 * @param image The image
 * @return The output image
*/
ImageMatrix* invert(const ImageMatrix& image);


/**
 * Adds a warm brown tone to the image
 * @param image The image
 * @return The output image
*/
ImageMatrix* sepia(const ImageMatrix& image);


/**
 * Enables and disables particular channels in an image
 * @param image The image
 * @param r_on Whether the red channel is enabled
 * @param g_on Whether the green channel is enabled
 * @param b_on Whether the blue channel is enabled
 * @return The output image
 */
ImageMatrix* enable_channels(const ImageMatrix& image, const bool& r_on, const bool& g_on, const bool& b_on);


/**
 * Replaces all existing color with the corresponding shade of a new color
 * @param image The image
 * @param hex The desired color as a hexidecimal value
 * @return The output image
 */
ImageMatrix* color(const ImageMatrix& image, const std::string& hex);


/**
 * Shifts the colors to mix of blue and orange tones
 * @param image The image
 * @return The output image
 */
ImageMatrix* octopus_dragon(const ImageMatrix& image);


#endif