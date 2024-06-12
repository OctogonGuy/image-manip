#include <iostream>
#include <sstream>
#include <map>
#include <cstdint>
#include "image_functions.h"
#include "util.h"
using namespace std;

// Menu loop sentinel
const string sentinel = "q";

// Valid file extentions
const string valid_exts[4] = { "png", "bmp", "jpg", "jpeg" };

// Map of image functions
map<string, function<uint8_t* (uint8_t*, int&, int&, int&, string*)>> funcMap;
void create_vararg_functions() {
	funcMap["pixel"] = [](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return pixel_image(image, width, height, bpp, stoi(args[0]));
		};
	funcMap["grayscale"] = [](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return grayscale_image(image, width, height, bpp);
		};
	funcMap["red"] = [](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return red_image(image, width, height, bpp);
		};
	funcMap["green"] = [](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return green_image(image, width, height, bpp);
		};
	funcMap["blue"] = [](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return blue_image(image, width, height, bpp);
		};
}

// Exit function
int exit() {
	cout << "Goodbye." << endl;
	exit(0);
}


int main() {
	string input;

	// Add vararg versions of functions to function map
	create_vararg_functions();

	// Print greeting
	cout << "Welcome to the image manipulator." << endl;

	// Prompt for reference image path
	bool invalid_file = true;
	do {
		cout << "Enter the path of the reference image:" << flush;
		getline(cin, input);
		// Assert valid reference file type
		const string ext = input.substr(input.find_last_of('.') + 1);
		for (const string& valid_ext : valid_exts) {
			if (equals_ignore_case(ext, valid_ext)) {
				invalid_file = false;
				break;
			}
		}
		if (equals_ignore_case(input, sentinel)) {
			exit();
		}
		if (invalid_file) {
			cout << "Invalid file type: " << ext << endl;
		}
	} while (invalid_file);
	const string ref_path = input;

	// Prompt for output directory
	cout << "Enter the output directory:";
	getline(cin, input);
	if (equals_ignore_case(input, sentinel)) {
		exit();
	}
	// Add slash to end of string if there is none already
	if (input.back() != '/' && input.back() != '\\') {
		if (input.find('/') != string::npos)
			input += '/';
		else
			input += '\\';
	}
	const string out_dir = input;

	// Priming read
	cout << ">>";
	getline(cin, input);
	// Menu loop
	while (!equals_ignore_case(input, sentinel)) {
		// Parse arguments
		vector<string> tokens = split(input);
		string func_str = tokens[0];
		string out_path = out_dir + tokens[1];
		vector<string> args(tokens.begin() + 2, tokens.end());

		// Read image
		int width, height, bpp;
		uint8_t* image = read_image(ref_path, width, height, bpp);
		// Print image data
		cout << "Reference path: " << ref_path << endl;
		cout << "Output path: " << out_path << endl;
		cout << "Width: " << width << " px" << endl;
		cout << "Height: " << height << " px" << endl;
		cout << "# of 8-bit components per pixel: " << bpp << endl;
		// Create data for pixel image
		uint8_t* new_image = funcMap[func_str](image, width, height, bpp, &args[0]);
		// Free old image resources
		free_image(image);
		// Write new image
		write_image(out_path, new_image, width, height, bpp);
		// Free new image resources
		free_image(new_image);
		// Print completion confirmation message
		cout << "Finished writing image." << endl;

		// Get next input
		cout << ">>";
		getline(cin, input);
	}

	// Print closing message
	exit();
}