#include <iostream>
#include <stdint.h>
#include "image_functions.h"
#include "util.h"
using namespace std;



string ask_ref_path();
string ask_dest_path();



// Valid file extentions
const string valid_exts[4] = {"png", "bmp", "jpg", "jpeg"};



int main()
{
	int width, height, bpp;

	// Prompt user for original image path
	string ref_path = ask_ref_path();

	// Read image
	uint8_t *image = read_image(ref_path, width, height, bpp);

	// Create data for pixel image
	uint8_t *new_image = pixel_image(image, width, height, bpp);

	// Free old image resources
	free_image(image);

	// Prompt user for new image path
	string dest_path = ask_dest_path();

	// Write new image
	write_image(ref_path, dest_path, new_image, width, height, bpp);

	// Free new image resources
	free_image(new_image);

	// Print done message
	cout << "Finished writing image." << endl;
	return 0;
}



string ask_ref_path()
{
	string ref_path;

	// Get user input for original image path
	bool invalid_file;
	do
	{
		// Get user input
		cout << "Path of reference image: ";
		getline(cin, ref_path);
		trim(ref_path);

		// Check if invalid file type
		string ext = ref_path.substr(ref_path.find_last_of(".") + 1);
		invalid_file = true;
		for (string valid_ext : valid_exts) {
			if (equals_ignore_case(ext, valid_ext)) {
				invalid_file = false;
				break;
			}
		}
		if (invalid_file)
			cout << "Invalid file type." << endl;
	} while (invalid_file);

	return ref_path;
}



string ask_dest_path()
{
	// Get user input for new image path
	string dest_path;
	cout << "Desired path of new image: ";
	getline(cin, dest_path);
	trim(dest_path);
	return dest_path;
}