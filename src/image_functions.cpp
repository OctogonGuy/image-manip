#include <iostream>
#include <stdint.h>
#include <functional>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "image_functions.h"
#include "util.h"
using namespace std;



const int QUALITY = 50; // Image quality | 0 - 100



// --- Helper functions ---

uint8_t *read_image(string old_image_path, int &width, int &height, int &bpp)
{
	uint8_t *image = stbi_load(old_image_path.c_str(), &width, &height, &bpp, 0);
	// ... process data if not NULL ...
	// ... x = width, y = height, n = # 8-bit components per pixel ...
	// ... replace '0' with '1'..'4' to force that many components per pixel
	// ... but 'n' will always be the number that it would have been if you said 0
	cout << "Width: " << width << " px" << endl;
	cout << "Height: " << height << " px" << endl;
	cout << "# of 8-bit components per pixel: " << bpp << endl;

	return image;
}



void write_image(string ref_path, string dest_path, uint8_t *new_image, int &width, int &height, int &bpp)
{
	string ext = string(dest_path).substr(string(dest_path).find_last_of(".") + 1);
	if (equals_ignore_case(ext, "png"))
		stbi_write_png(dest_path.c_str(), width, height, bpp, new_image, width * bpp);
	else if (equals_ignore_case(ext, "bmp"))
		stbi_write_bmp(dest_path.c_str(), width, height, bpp, new_image);
	else if (equals_ignore_case(ext, "jpg") || equals_ignore_case(ext, "jpeg"))
		stbi_write_jpg(dest_path.c_str(), width, height, bpp, new_image, QUALITY);
}



void free_image(uint8_t *image) {
	stbi_image_free(image);
}



uint8_t *pixel_transform(uint8_t *image, int &width, int &height, int &bpp, function<void(Pixel&)> func) {
	uint8_t *new_image = new uint8_t[width * height * bpp];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Index of pixel in original image
			int index = bpp * (j + i * width);
			// RGB values of pixel
			uint8_t r = image[index];
			uint8_t g = image[index + 1];
			uint8_t b = image[index + 2];
			// Perform operation on RGB
			Pixel pixel = Pixel(r, g, b);
			func(pixel);
			new_image[index] = pixel.r;
			new_image[index + 1] = pixel.g;
			new_image[index + 2] = pixel.b;
		}
	}
	return new_image;
}



// --- Image manipulation functions ---

uint8_t *pixel_image(uint8_t *image, int &width, int &height, int &bpp)
{
	string input, new_path;

	// Prompt user for pixel density
	cout << "Length (longest side) in pixels: ";
	getline(cin, input);
	int length_pixels = stoi(input);

	// Determine new image info
	int longest_side = width > height ? width : height;
	double chunk_length = double(longest_side) / length_pixels;
	int chunks = max(1, (int)(round(width / chunk_length) * round(height / chunk_length)));
	int width_pixels = width > height ? length_pixels : max(1, (int)round(width / chunk_length));
	int height_pixels = height >= width ? length_pixels : max(1, (int)round(height / chunk_length));
	double chunk_width = width > height ? chunk_length : double(width) / width_pixels;
	double chunk_height = height >= width ? chunk_length : double(height) / height_pixels;

	// Create bit data for new image
	int *rgb_totals = new int[chunks * bpp]();	// Parentheses initialize all elements with 0
	int *num_ref_px = new int[chunks]();
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// Index of pixel in original image
			int index = bpp * (j + i * width);
			// Index of pixel in pixelated image
			int chunk_col = floor((j + j + 1) / 2.0 / chunk_width);
			int chunk_row = floor((i + i + 1) / 2.0 / chunk_height);
			int chunk_index = bpp * (chunk_col + chunk_row * width_pixels);
			// Add RGB values
			for (int k = 0; k < bpp; k++)
			{
				rgb_totals[chunk_index + k] += image[index + k];
			}
			num_ref_px[chunk_index / bpp]++;
		}
	}
	width = width_pixels * round(chunk_length);	  // Width of new image
	height = height_pixels * round(chunk_length); // Height of new image
	uint8_t *new_image = new uint8_t[width * height * bpp];
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			// Index of pixel in new image
			int index = bpp * (j + i * width);
			// Index of pixel in pixelated image
			int chunk_col = j * width_pixels / width;
			int chunk_row = i * height_pixels / height;
			int chunk_index = bpp * (chunk_col + chunk_row * width_pixels);
			// Find average RGB value in chunk
			for (int k = 0; k < bpp; k++)
			{
				new_image[index + k] = round(1.0 * rgb_totals[chunk_index + k] / num_ref_px[chunk_index / bpp]);
			}
		}
	}

	// Delete arrays from dynamic memory
	delete[] rgb_totals;
	delete[] num_ref_px;

	return new_image;
}



uint8_t *grayscale_image(uint8_t *image, int &width, int &height, int &bpp) {
	function<void(Pixel&)> lambda = [](Pixel &pixel) -> void {
		int avg = (pixel.r + pixel.g + pixel.b) / 3;
		pixel.g = pixel.r = pixel.b = avg;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}



uint8_t *red_image(uint8_t *image, int &width, int &height, int &bpp) {
	function<void(Pixel&)> lambda = [](Pixel &pixel) -> void {
		int avg = (pixel.r + pixel.g + pixel.b) / 3;
		pixel.r = avg;
		pixel.g = pixel.b = 0;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}



uint8_t *green_image(uint8_t *image, int &width, int &height, int &bpp) {
	function<void(Pixel&)> lambda = [](Pixel &pixel) -> void {
		int avg = (pixel.r + pixel.g + pixel.b) / 3;
		pixel.g = avg;
		pixel.r = pixel.b = 0;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}



uint8_t *blue_image(uint8_t *image, int &width, int &height, int &bpp) {
	function<void(Pixel&)> lambda = [](Pixel &pixel) -> void {
		int avg = (pixel.r + pixel.g + pixel.b) / 3;
		pixel.b = avg;
		pixel.r = pixel.g = 0;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}