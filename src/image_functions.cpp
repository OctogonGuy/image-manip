#include "image_functions.h"
#include <iostream>
#include <cstdint>
#include <cmath>
#include <sstream>

#include "util.h"
using namespace std;

ImageMatrix* pixelate(const ImageMatrix& image, const int& divs) {
	string input, new_path;
	const int width = image.getWidth();
	const int height = image.getHeight();
	const int bpp = image.getBpp();

	// Determine new image info
	const int longest_side = width > height ? width : height;
	const double chunk_length = static_cast<double>(longest_side) / divs;	// Divisions on longest side
	const int chunks = max(1, static_cast<int>(round(width / chunk_length) * round(height / chunk_length)));
	const int width_pixels = width > height ? divs : max(1, static_cast<int>(round(width / chunk_length)));
	const int height_pixels = height >= width ? divs : max(1, static_cast<int>(round(height / chunk_length)));
	const double chunk_width = width > height ? chunk_length : static_cast<double>(width) / width_pixels;
	const double chunk_height = height >= width ? chunk_length : static_cast<double>(height) / height_pixels;

	// Create bit data for new image
	const auto rgb_totals = new int[chunks * bpp]();	// Parentheses initialize all elements with 0
	const auto num_ref_px = new int[chunks]();	// Number of reference pixels in each destination chunk
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Index of pixel in original image
			const int index = bpp * (j + i * width);
			// Index of pixel in pixelated image
			const int chunk_col = floor((j + j + 1) / 2.0 / chunk_width);
			const int chunk_row = floor((i + i + 1) / 2.0 / chunk_height);
			const int chunk_index = bpp * (chunk_col + chunk_row * width_pixels);
			// Add RGB values
			for (int k = 0; k < bpp; k++) {
				rgb_totals[chunk_index + k] += image.getImageData()[index + k];
			}
			num_ref_px[chunk_index / bpp]++;
		}
	}
	const int new_width = width_pixels * static_cast<int>(round(chunk_length));	  // Width of new image
	const int new_height = height_pixels * static_cast<int>(round(chunk_length)); // Height of new image
	auto* new_image = new uint8_t[new_width * new_height * bpp];
	for (int i = 0; i < new_height; i++) {
		for (int j = 0; j < new_width; j++) {
			// Index of pixel in new image
			const int index = bpp * (j + i * new_width);
			// Index of pixel in pixelated image
			const int chunk_col = j * width_pixels / new_width;
			const int chunk_row = i * height_pixels / new_height;
			const int chunk_index = bpp * (chunk_col + chunk_row * width_pixels);
			// Find average RGB value in chunk
			for (int k = 0; k < bpp; k++) {
				const int rgb_total = rgb_totals[chunk_index + k];
				const int num_px = num_ref_px[chunk_index / bpp];
				new_image[index + k] = static_cast<int>(round(static_cast<double>(rgb_total) / num_px));
			}
		}
	}

	// Delete arrays from dynamic memory
	delete[] rgb_totals;
	delete[] num_ref_px;

	return new ImageMatrix(new_image, new_width, new_height, bpp);
}


ImageMatrix* outline(const ImageMatrix& image) {
	constexpr double kernel[] = {
		-1,	-1,	-1,
		-1,	8,	-1,
		-1,	-1,	-1
	};
	return image.convolve(kernel, sizeof(kernel)/sizeof(kernel[0]));
}


ImageMatrix* sharpen(const ImageMatrix& image) {
	constexpr double kernel[] = {
		0,	-1,	0,
		-1,	5,	-1,
		0,	-1,	0
	};
	return image.convolve(kernel, sizeof(kernel)/sizeof(kernel[0]));
}


ImageMatrix* box_blur(const ImageMatrix& image, const int& radius) {
	const int kernel_size = static_cast<int>(pow(2 * radius + 1, 2));
	double kernel[kernel_size];
	fill_n(kernel, kernel_size, 1);
	return image.convolve(kernel, kernel_size, 1.0/kernel_size);
}


ImageMatrix* gaussian_blur(const ImageMatrix& image, const int& radius) {
	const int kernel_size = static_cast<int>(pow(2 * radius + 1, 2));
	const int kernel_rows = 2*radius + 1;
	constexpr double sigma = 0.3;
	double kernel[kernel_size];
	for (int i = -radius; i <= radius; i++) {
		for (int j = -radius; j <= radius; j++) {
			const int index = (i + radius) * kernel_rows + (j + radius);
			kernel[index] = 1 / (2 * M_PI * pow(sigma, 2)) * pow(M_E, -((pow(j, 2) + pow(i, 2)) / (2 * pow(sigma, 2))));
		}
	}
	return image.convolve(kernel, kernel_size, 1.0/pow(16, radius));
}


ImageMatrix* grayscale(const ImageMatrix& image) {
	constexpr double matrix[] = {
		1.0/3.0,	1.0/3.0,	1.0/3.0,	0,
		1.0/3.0,	1.0/3.0,	1.0/3.0,	0,
		1.0/3.0,	1.0/3.0,	1.0/3.0,	0
	};
	return image.filter(matrix);
}


ImageMatrix* invert(const ImageMatrix& image) {
	constexpr double matrix[] = {
		-1,		0,		0,	255,
		0,		-1,		0,	255,
		0,		0,		-1,	255
	};
	return image.filter(matrix);
}


ImageMatrix* sepia(const ImageMatrix& image) {
	constexpr double matrix[] = {
		0.393,	0.769,	0.189,	0,
		0.349,	0.686,	0.168,	0,
		0.272,	0.534,	0.131,	0
	};
	return image.filter(matrix);
}


ImageMatrix* enable_channels(const ImageMatrix& image, const bool& r_on, const bool& g_on, const bool& b_on) {
	const double r_bit = r_on ? 1 : 0;
	const double g_bit = g_on ? 1 : 0;
	const double b_bit = b_on ? 1 : 0;
	const double matrix[] = {
		r_bit,	0,		0,		0,
		0,		g_bit,	0,		0,
		0,		0,		b_bit,	0
	};
	return image.filter(matrix);
}


ImageMatrix* color(const ImageMatrix& image, const string& hex) {
	const double r_frac = static_cast<float>(static_cast<uint8_t>(
		stoul(hex.substr(0, 2), nullptr, 16)) / 255.0);
	const double g_frac = static_cast<float>(static_cast<uint8_t>(
		stoul(hex.substr(2, 2), nullptr, 16)) / 255.0);
	const double b_frac = static_cast<float>(static_cast<uint8_t>(
		stoul(hex.substr(4, 2), nullptr, 16)) / 255.0);
	const double matrix[] = {
		r_frac/3.0,	r_frac/3.0,	r_frac/3.0,	0,
		g_frac/3.0,	g_frac/3.0,	g_frac/3.0,	0,
		b_frac/3.0,	b_frac/3.0,	b_frac/3.0,	0
	};
	return image.filter(matrix);
}