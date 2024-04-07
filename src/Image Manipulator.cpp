//============================================================================
// Name        : Image.cpp
// Author      : Alex Gill
// Version     :
// Description : Create a new edited image from an old one
//============================================================================

#include <iostream>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
using namespace std;

const int COMP = 3;		// RGB
const int QUALITY = 50;	// 0 - 100

int main() {

	// Get user input
	string input, reference_path, new_path;
	cout << "Path of reference image: ";
	getline(cin, reference_path);
	const char* orig_image_path = reference_path.c_str();
	cout << "Desired path of new image: ";
	getline(cin, new_path);
	const char* new_image_path = new_path.c_str();
	cout << "Length (longest side) in pixels: ";
	cin >> input;
	int length_pixels = stoi(input);

	// Read image
	int width, height, bpp;
	uint8_t* image = stbi_load(orig_image_path, &width, &height, &bpp, 3);
	// ... process data if not NULL ...
	// ... x = width, y = height, n = # 8-bit components per pixel ...
	// ... replace '0' with '1'..'4' to force that many components per pixel
	// ... but 'n' will always be the number that it would have been if you said 0

	// Determine new image info
	int longest_side = width > height ? width : height;
	int chunk_length = longest_side / length_pixels;
	int chunk_size = chunk_length * chunk_length;
	int chunks = width * height / chunk_size;

	// Create bit data for new image
	int rgb_totals[chunks * COMP] = { 0 };
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int index = COMP * (j + i * width);
			int chunk_index = COMP * ((j / chunk_length) + (i / chunk_length) * (width / chunk_length));
			rgb_totals[chunk_index] += image[index];
			rgb_totals[chunk_index + 1] += image[index + 1];
			rgb_totals[chunk_index + 2] += image[index + 2];
		}
	}
	uint8_t new_image[width * height * COMP];
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			int index = COMP * (j + i * width);
			int chunk_index = COMP * ((j / chunk_length) + (i / chunk_length) * (width / chunk_length));
			new_image[index] = round(1.0 * rgb_totals[chunk_index] / chunk_size);
			new_image[index + 1] = round(1.0 * rgb_totals[chunk_index + 1] / chunk_size);
			new_image[index + 2] = round(1.0 * rgb_totals[chunk_index + 2] / chunk_size);
		}
	}

	// Free old image resources
	stbi_image_free(image);

	// Write new image
	stbi_write_jpg(new_image_path, width, height, COMP, new_image, QUALITY);

	return 0;
}
