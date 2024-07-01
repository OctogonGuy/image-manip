#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <cstdint>

/**
 * Represents a pixel with RGB data
*/
class PixelVector {
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
	PixelVector(std::uint8_t r, std::uint8_t g, std::uint8_t b);
};


/**
 * Represents a matrix which represents an image
*/
class ImageMatrix {
 std::uint8_t* image_data; // Image data
 int width; // The width of the image
 int height; // The height of the image
 int bpp; // bytes per pixel

public:
 /**
  * Creates an empty image
  * @param width The width of the image
  * @param height The height of the image
  * @param bpp Bytes per pixel
  */
 ImageMatrix(const int& width, const int& height, const int& bpp);

 /**
  * Creates a defined image
  * @param image_data Bit data of the image
  * @param width The width of the image
  * @param height The height of the image
  * @param bpp Bytes per pixel
  */
 ImageMatrix(std::uint8_t* image_data, const int& width, const int& height, const int& bpp);

 /**
  * Makes a copy from another image matrix
  * @param image The object to copy
  */
 ImageMatrix(const ImageMatrix& image);

 /**
  * Deletes the image
  */
 virtual ~ImageMatrix();

 std::uint8_t* getImageData() const { return image_data; }
 int getWidth() const { return width; }
 int getHeight() const { return height; }
 int getBpp() const { return bpp; }

 /**
  * Returns the pixel data of the given entry in the image matrix
  * @param row The row of the entry
  * @param column The column of the entry
  * @return The pixel data
  */
 PixelVector get(const int& row, const int& column) const;

 /**
 * Sets the pixel data of the specified entry in the image matrix
  * @param row The row of the entry
  * @param column The column of the entry
  * @param pixel_data The pixel data (byte data)
  */
 void set(const int& row, const int& column, const PixelVector& pixel_data) const;

 /**
 * Performs a single operation on every pixel in an image
 * @param matrix Multiplies the rgb components by the first three rows and adds the last row
 * @return The output image
*/
 ImageMatrix* filter(const double* matrix) const;

 /**
 * Adds each element of the image to its local neighbors, weighted by the kernel
 * @param kernel The kernel matrix
 * @param kernel_size The length of the kernel array
 * @return The output image
*/
 ImageMatrix* convolve(const double* kernel, const size_t& kernel_size) const;

 /**
 * Adds each element of the image to its local neighbors, weighted by the kernel
 * @param kernel The kernel matrix
 * @param kernel_size The length of the kernel array
 * @param scalar A scalar by which to multiply the kernel
 * @return The output image
*/
 ImageMatrix* convolve(const double* kernel, const size_t& kernel_size, const double& scalar) const;
};


/**
 * Reads an image
 * @param ref_path The path of the image
 * @param width A reference to be overridden with the image's width
 * @param height A reference to be overridden with the image's height
 * @param bpp A reference to be overridden with the number of bits per pixel
 * @return The image matrix that was read
*/
ImageMatrix* read_image(const std::string& ref_path, int& width, int& height, int& bpp);


/**
 * Writes an image
 * @param out_path The destination path for the new image
 * @param new_image The new image
*/
void write_image(const std::string& out_path, const ImageMatrix& new_image);


/**
 * Writes a text file
 * @param out_path The destination path for the text file
 * @param text The text to write to the file
*/
void write_textfile(const std::string& out_path, const std::string& text);


/**
 * Compares two characters, ignoring case
 * @param a The first character
 * @param b The second character
 * @return Whether the characters are equal
 */
bool ichar_equals(char a, char b);


/**
 * Compares two strings, ignoring case
 * @param a The first string
 * @param b The second string
 * @return Whether the strings are equal
 */
bool iequals(const std::string& a, const std::string& b);

#endif