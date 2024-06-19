#include <iostream>
#include <sstream>
#include <map>
#include <boost/program_options.hpp>
#include "image_functions.h"
#include "util.h"
using namespace std;
namespace po = boost::program_options;


int main(const int argc, const char* argv[]) {
	int width, height, bpp;
	string ref_path, out_path;

	// Stop execution if no arguments were provided
	if (argc == 1) {
		cout << "Please execute with options." << endl;
		return 1;
	}

	// --- Options ---
	// General options
	po::options_description general_desc("General options - Only one of these will be executed if given");
	general_desc.add_options()
		("help",
			"Prints a list of options and their parameters")
	;
	// Required options
	po::options_description req_desc("Required options - You must include these if using an image function");
	req_desc.add_options()
		("ref",
			po::value<string>()->value_name("path"),
			"Reference image path")
		("out",
			po::value<string>()->value_name("path"),
			"Output image path")
	;
	// Image functions
	po::options_description func_desc("Image functions - specify multiple to perform functions in given order");
	func_desc.add_options()
		("pixelate",
			po::value<int>()->value_name("divs"),
			"Transforms an image into a pixelated version divided a number of times along the longest side")
		("outline",
			"Highlights large differences in pixel values")
		("sharpen",
			"Emphasizes differences in adjacent pixel values")
		("box-blur",
			po::value<int>()->value_name("radius"),
			"Averages each pixel's value with the value of its neighboring pixels")
		("gaussian-blur",
			po::value<int>()->value_name("radius"),
			"Blurs the image by a Gaussian function")
		("grayscale",
			"Averages the colors of an image to make it grayscale")
		("invert",
			"Inverts the colors of the image")
		("sepia",
			"Adds a warm brown tone to the image")
		("color",
			po::value<string>()->value_name("hex"),
			"Replaces all existing color with the corresponding shades of a new color")
		("enable-channels",
			po::value<string>()->value_name("rgb"),
			"Specify channels (r, g, and/or b) to enable in an image (do not separate with spaces or commas)")
	;
	// Combine option categories
	po::options_description desc;
	desc.add(general_desc).add(req_desc).add(func_desc);
	// Create map of parsed command line arguments
	po::variables_map vm;
	store(parse_command_line(argc, argv, desc), vm);
	notify(vm);

	// Handle help
	if (vm.count("help")) {
		cout << desc << endl;
		exit(1);
	}

	// Get reference image path
	if (vm.count("ref")) {
		ref_path = vm["ref"].as<string>();
		// Remove argument from arguments
		vm.erase("ref");
	}
	// Quit the program if user did not provide reference image
	else {
		cout << "Please include a reference image path." << endl;
		exit(2);
	}
	// Get output image path
	if (vm.count("out")) {
		out_path = vm["out"].as<string>();
		// Remove argument from arguments
		vm.erase("out");
	}
	// Quit the program if user did not provide output image
	else {
		cout << "Please include an output image path." << endl;
		exit(2);
	}

	// Read image
	ImageMatrix* image = read_image(ref_path, width, height, bpp);
	// Print read image confirmation message
	cout << "Finished reading reference image." << endl;

	// Go through parsed image functions
	for (auto it = vm.begin(); it != vm.end(); ++it) {
		ImageMatrix* temp;
		string key = it->first;
		if (key == "pixelate") {
			temp = pixelate(*image, vm["pixelate"].as<int>());
		}
		else if (key == "outline") {
			temp = outline(*image);
		}
		else if (key == "sharpen") {
			temp = sharpen(*image);
		}
		else if (key == "box-blur") {
			temp = box_blur(*image, vm["box-blur"].as<int>());
		}
		else if (key == "gaussian-blur") {
			temp = gaussian_blur(*image, vm["gaussian-blur"].as<int>());
		}
		else if (key == "grayscale") {
			temp = grayscale(*image);
		}
		else if (key == "invert") {
			temp = invert(*image);
		}
		else if (key == "sepia") {
			temp = sepia(*image);
		}
		else if (key == "color") {
			const string hex = vm["color"].as<string>();
			temp = color(*image, hex);
		}
		else if (key == "enable-channels") {
			const string channels = vm["enable-channels"].as<string>();
			const bool r = channels.find('r') != string::npos;
			const bool g = channels.find('g') != string::npos;
			const bool b = channels.find('b') != string::npos;
			temp = enable_channels(*image, r, g, b);
		}
		else {
			cout << "Please include a valid image function" << endl;
			exit(1);
		}
		delete image;
		image = temp;
	}

	// Write the output image
	write_image(out_path, *image);
	// Print completion confirmation message
	cout << "Finished writing output image." << endl;

	return 0;
}