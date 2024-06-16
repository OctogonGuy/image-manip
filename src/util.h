#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <cstdint>
#include <functional>
#include <vector>

/**
 * Represents a pixel with RGB data
*/
class Pixel {
public:
 std::uint8_t r;	// Red
	std::uint8_t g;	// Green
	std::uint8_t b;	// Blue

	/**
	 * Instantiates a pixel.
	 * @param r The red value
	 * @param g The green value
	 * @param b The blue value
	*/
	Pixel(std::uint8_t r, std::uint8_t g, std::uint8_t b);
};


/**
 * Represents an image function
*/
class ImageFunction {
public:
 std::function<uint8_t* (uint8_t*, int&, int&, int&, std::string*)> func;	// The image function
 std::string name;	// The name of the function
 std::string desc;	// A description of the function
 std::vector<std::string> params;	// Parameters of the function

 /**
  * Instantiates an image function
  * @param func The image function
  * @param name The name of the function
  * @param desc A description of the function
  * @param params Parameters of the function
 */
 ImageFunction(const std::function<uint8_t* (uint8_t*, int&, int&, int&, std::string*)>& func,
  const std::string& name, const std::string& desc, const std::initializer_list<std::string>& params);

 /**
  * Destructor
  */
 virtual ~ImageFunction();

 /**
  * @return A string with the function's description and parameters.
  */
 std::string helpStr() const;
};


/**
 * Compares two strings, ignoring capitalization
 * @param str1 A string
 * @param str2 A string
 * @return Whether the two strings are the same, ignoring case
*/
bool equals_ignore_case(const std::string& str1, const std::string& str2);


/**
 * Splits a string into an vector of strings wherever whitespace is found except within quotes
 * @param str A string
 * @return The vector of strings
*/
std::vector<std::string> split(const std::string& str);


/**
 * Reads a file and returns its contents as a string
 * @param filename The file name
 * @return The contents of the file as a string
 */
std::string read_file(const std::string& filename);


/**
 * Reads an image
 * @param ref_path The path of the image
 * @param width A reference to be overridden with the image's width
 * @param height A reference to be overridden with the image's height
 * @param bpp A reference to be overridden with the number of bits per pixel
 * @return The image data that was read
*/
std::uint8_t* read_image(const std::string& ref_path, int& width, int& height, int& bpp);


/**
 * Writes an image
 * @param out_path The destination path for the new image
 * @param new_image The new image
 * @param width The image's width
 * @param height The image's height
 * @param bpp # of bits per pixel
*/
void write_image(const std::string& out_path, const std::uint8_t* new_image, const int& width, const int& height, const int& bpp);


/**
 * Removes an image from memory
 * @param image The image data
*/
void free_image(std::uint8_t* image);

#endif