#include "image_functions.h"
#include <iostream>
#include <functional>
#include <cstdint>
#include <cmath>
#include <sstream>

#include "util.h"
using namespace std;

// --- Helper functions ---

uint8_t* pixel_transform(const uint8_t* image, const int& width, const int& height, const int& bpp, const function<void(Pixel&)>& func) {
	auto* new_image = new uint8_t[width * height * bpp];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			// Index of pixel in original image
			int index = bpp * (j + i * width);
			// RGB values of pixel
			const uint8_t r = image[index];
			const uint8_t g = image[index + 1];
			const uint8_t b = image[index + 2];
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

uint8_t* pixel_image(const uint8_t* image, const int& width, const int& height, const int& bpp, const int& divs) {
	string input, new_path;

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
				rgb_totals[chunk_index + k] += image[index + k];
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

	return new_image;
}


uint8_t* grayscale_image(const uint8_t* image, const int& width, const int& height, const int& bpp) {
	const function<void(Pixel&)> lambda = [](Pixel& pixel) -> void {
		pixel.g = pixel.r = pixel.b = (pixel.r + pixel.g + pixel.b) / 3;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}


uint8_t* channel_image(const uint8_t* image, const int& width, const int& height, const int& bpp,
	const bool& r_enabled, const bool& g_enabled, const bool& b_enabled) {
	const function<void(Pixel&)> lambda = [r_enabled, g_enabled, b_enabled](Pixel& pixel) -> void {
		if (!r_enabled) pixel.r = 0;
		if (!g_enabled) pixel.g = 0;
		if (!b_enabled) pixel.b = 0;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}


uint8_t* color_image(const uint8_t* image, const int& width, const int& height, const int& bpp,
	const string& hex_str) {
	const function<void(Pixel&)> lambda = [hex_str](Pixel& pixel) -> void {
		const uint8_t r = static_cast<uint8_t>(stoul(hex_str.substr(0, 2), nullptr, 16));
		const uint8_t g = static_cast<uint8_t>(stoul(hex_str.substr(2, 2), nullptr, 16));
		const uint8_t b = static_cast<uint8_t>(stoul(hex_str.substr(4, 2), nullptr, 16));
		const int avg = (pixel.r + pixel.g + pixel.b) / 3;
		pixel.r = static_cast<uint8_t>(round(avg * (r / 255.0)));
		pixel.g = static_cast<uint8_t>(round(avg * (g / 255.0)));
		pixel.b = static_cast<uint8_t>(round(avg * (b / 255.0)));
	};
	return pixel_transform(image, width, height, bpp, lambda);
}


uint8_t* red_image(const uint8_t* image, const int& width, const int& height, const int& bpp) {
	const function<void(Pixel&)> lambda = [](Pixel& pixel) -> void {
		pixel.r = (pixel.r + pixel.g + pixel.b) / 3;
		pixel.g = pixel.b = 0;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}


uint8_t* green_image(const uint8_t* image, const int& width, const int& height, const int& bpp) {
	const function<void(Pixel&)> lambda = [](Pixel& pixel) -> void {
		pixel.g = (pixel.r + pixel.g + pixel.b) / 3;
		pixel.r = pixel.b = 0;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}


uint8_t* blue_image(const uint8_t* image, const int& width, const int& height, const int& bpp) {
	const function<void(Pixel&)> lambda = [](Pixel& pixel) -> void {
		pixel.b = (pixel.r + pixel.g + pixel.b) / 3;
		pixel.r = pixel.g = 0;
	};
	return pixel_transform(image, width, height, bpp, lambda);
}