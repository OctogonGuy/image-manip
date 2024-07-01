#include <iostream>
#include <sstream>
#include "image_functions.h"
#include "util.h"
#include "CLI11.hpp"
using namespace std;



/**
 * Adds commands and their respective options to the command line application Then,
 * parses commands given by the user, reads the reference image, executes the
 * corresponding function(s), and writes the output image.
 * @param argc The number of command line arguments
 * @param argv The command line arguments
 * @return Exit code
 */
int process_commands(int argc, char** argv);



int main(int argc, char** argv) {
	return process_commands(argc, argv);
}



int process_commands(int argc, char** argv) {
	int width, height, bpp;
	string ref_path, out_path;


	// Initialize CLI App
	CLI::App app{"App description"};
	argv = app.ensure_utf8(argv);
	app.require_subcommand();


	// Add app options that are applicable to all functions
	app.add_option("--ref", ref_path,
		"Reference image path")->required();
	app.add_option("--out", out_path,
	"Output image path")->required();


	// --- Commands and their respective options ---
    app.add_subcommand("pixelate",
    	"Transforms an image into a pixelated version");
	int pixelate_divs{100};
    app.get_subcommand("pixelate")->add_option("--divs", pixelate_divs,
    "The number of times the image will be divided on the longest side");

	app.add_subcommand("ascii",
		"Transforms an image into ASCII art");
	int ascii_cols{80};
	app.get_subcommand("ascii")->add_option("--cols", ascii_cols,
	"The number of characters along the width");
	double ascii_ratio{2.0};
	app.get_subcommand("ascii")->add_option("--ratio", ascii_ratio,
		"The width/height ratio to stretch the image by");

    app.add_subcommand("outline",
    	"Highlights large differences in pixel values");

	app.add_subcommand("sharpen",
	"Emphasizes differences in adjacent pixel values");

	app.add_subcommand("contrast",
		"Controls the amount of color differentiation");
	int contrast_value{0};
	app.get_subcommand("contrast")->add_option("--value", contrast_value,
		"the contrast value (-255 - 255)")
	->check(CLI::Range(-255, 255));

    app.add_subcommand("box-blur",
		"Averages each pixel's value with the value of its neighboring pixels");
	int box_blur_radius{1};
	app.get_subcommand("box-blur")->add_option("--radius", box_blur_radius,
	"Radius of the kernel")
	->check(CLI::Range(1, 5));

	app.add_subcommand("gaussian-blur",
		"Blurs the image by a Gaussian function");
	int gaussian_blur_radius{1};
	app.get_subcommand("gaussian-blur")->add_option("--radius", gaussian_blur_radius,
	"Radius of the kernel")
	->check(CLI::Range(1, 5));
	double gaussian_blur_sigma{0.3};
	app.get_subcommand("gaussian-blur")->add_option("--sigma", gaussian_blur_sigma,
		"Standard deviation of the Gaussian distribution")
	->check(CLI::Range(0.0, 25.0));

    app.add_subcommand("grayscale",
    	"Averages the colors of an image to make it grayscale");

    app.add_subcommand("invert",
    	"Inverts the colors of the image");

    app.add_subcommand("sepia",
    	"Adds a warm brown tone to the image");

    app.add_subcommand("color",
    "Replaces all existing color with the corresponding shades of a new color");
	string color_hex{"000000"};
	app.get_subcommand("color")->add_option("--hex", color_hex,
		"The desired color as a hexidecimal value");

    app.add_subcommand("enable-channels",
    "Specify channels to enable in an image");
	int red_channel_enabled{0};
	int green_channel_enabled{0};
	int blue_channel_enabled{0};
	app.get_subcommand("enable-channels")->add_flag("-r,--red", red_channel_enabled,
	"Whether to enable the red channel");
	app.get_subcommand("enable-channels")->add_flag("-g,--green", green_channel_enabled,
	"Whether to enable the green channel");
	app.get_subcommand("enable-channels")->add_flag("-b,--blue", blue_channel_enabled,
	"Whether to enable the blue channel");

	app.add_subcommand("octopus-dragon",
	"Shifts the colors to mix of blue and orange tones");


	// --- Parse commands ---
	CLI11_PARSE(app, argc, argv);


	// Read image
	const ImageMatrix* image = read_image(ref_path, width, height, bpp);
	// Print read image confirmation message
	cout << "Finished reading reference image." << endl;


	// --- Perform functions ---
	for (const auto *subcom : app.get_subcommands()) {
		ImageMatrix* temp;
		string key = subcom->get_name();

		if (key == "pixelate") {
			temp = pixelate(*image,
				pixelate_divs);
		}

		else if (key == "ascii") {
			const string ascii_str = ascii(*image, ascii_cols, ascii_ratio);
			write_textfile(out_path, ascii_str);
			// Delete image
			delete image;
			// Print the ASCII art
			cout << ascii_str << endl;
			// Print completion confirmation message
			cout << "Finished writing output text file. Cannot perform further operations" << endl;
			// Exit program
			return 0;
		}

		else if (key == "outline") {
			temp = outline(*image);
		}

		else if (key == "sharpen") {
			temp = sharpen(*image);
		}

		else if (key == "contrast") {
			cout << contrast_value;
			temp = contrast(*image,
				contrast_value);
		}

		else if (key == "box-blur") {
			temp = box_blur(*image,
			box_blur_radius);
		}

		else if (key == "gaussian-blur") {
			temp = gaussian_blur(*image,
			gaussian_blur_radius,
			gaussian_blur_sigma);
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
			temp = color(*image,
				color_hex);
		}

		else if (key == "enable-channels") {
			temp = enable_channels(*image,
				red_channel_enabled > 0,
				green_channel_enabled > 0,
				blue_channel_enabled > 0);
		}

		else if (key == "octopus-dragon") {
			temp = octopus_dragon(*image);
		}

		else {
			cout << "Could not find valid image processing function command." << endl;
			exit(1);
		}

		delete image;
		image = temp;
	}


	// Write the output image
	write_image(out_path, *image);

	// Delete image
	delete image;

	// Print completion confirmation message
	cout << "Finished writing output image." << endl;

	return 0;
}