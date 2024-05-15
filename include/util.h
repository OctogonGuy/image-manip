#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <stdint.h>

class Pixel {
	public:
		std::uint8_t r;
		std::uint8_t g;
		std::uint8_t b;
		Pixel(std::uint8_t r, std::uint8_t g, std::uint8_t b);
};
void trim(std::string &);
bool equals_ignore_case(std::string, std::string);

#endif