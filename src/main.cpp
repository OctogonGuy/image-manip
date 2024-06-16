#include <iostream>
#include <sstream>
#include <map>
#include <cstdint>
#include "image_functions.h"
#include "util.h"
using namespace std;

// Menu options
const string sentinel = "q";
const string help_cmd = "help";
const string all_cmd = "all";

// Valid file extentions
const string valid_exts[4] = { "png", "bmp", "jpg", "jpeg" };

// Map of image functions
map<string, ImageFunction*> funcMap;
void add_to_funcMap(const function<uint8_t* (uint8_t*, int&, int&, int&, string*)>& func,
	const string& name, const string& desc, const initializer_list<string>& params) {
	auto* image_function = new ImageFunction(func, name, desc, params);
	funcMap[name] = image_function;
}
void create_vararg_functions() {
	add_to_funcMap([](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return pixel_image(image, width, height, bpp, stoi(args[0]));
		},
		"pixel",
		"Transforms an image into a pixelated version of itself",
		{"The number of times the image will be divided on the longest side"});
	add_to_funcMap([](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return grayscale_image(image, width, height, bpp);
		},
		"grayscale",
		"Averages the colors of an image to make it grayscale",
		{});
	add_to_funcMap([](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return red_image(image, width, height, bpp);
		},
		"red",
		"Removes all green and blue from an image",
		{});
	add_to_funcMap([](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return green_image(image, width, height, bpp);
		},
		"green",
		"Removes all red and blue from an image",
		{});
	add_to_funcMap([](const uint8_t* image, const int& width, const int& height, const int& bpp, const string* args) {
			return blue_image(image, width, height, bpp);
		},
		"blue",
		"Removes all red and green from an image",
		{});
}
// Performs all functions
void do_all() {

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
	cout << "Type '" << sentinel << "' at any time to quit." << endl;

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

	// Prompt for function input
	cout << "Enter the name of a function followed by the desired filename of the image and any parameters." << endl;
	cout << "Type '" << help_cmd << "' for a list of all functions with a description and parameters." << endl;
	cout << "Type '" << all_cmd << "' to perform all functions with default parameters." << endl;

	// Priming read
	cout << ">>";
	getline(cin, input);
	// Menu loop
	while (!equals_ignore_case(input, sentinel)) {
		// Show help
		if (equals_ignore_case(input, help_cmd)) {
			for (const auto & it : funcMap) {
				cout << it.second->helpStr() << endl;
			}
		}
		// Perform all functions
		if (equals_ignore_case(input, all_cmd)) {
			for (const auto & it : funcMap) {
				// Read image
				int width, height, bpp;
				uint8_t* image = read_image(ref_path, width, height, bpp);
				// Create data for pixel image
				string out_path = out_dir + it.second->name + ".jpg";
				string args[] = {"50"};
				uint8_t* new_image = it.second->func(image, width, height, bpp, args);
				// Free old image resources
				free_image(image);
				// Write new image
				write_image(out_path, new_image, width, height, bpp);
				// Free new image resources
				free_image(new_image);
				// Print completion confirmation message
				cout << "Completed " + it.second->name + ". Output to: " << out_path << endl;
			}
			cout << "Finished writing all images." << endl;
		}
		// Perform image function
		else {
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
			uint8_t* new_image = funcMap[func_str]->func(image, width, height, bpp, &args[0]);
			// Free old image resources
			free_image(image);
			// Write new image
			write_image(out_path, new_image, width, height, bpp);
			// Free new image resources
			free_image(new_image);
			// Print completion confirmation message
			cout << "Finished writing image." << endl;
		}

		// Get next input
		cout << ">>";
		getline(cin, input);
	}

	// Print closing message
	exit();
}