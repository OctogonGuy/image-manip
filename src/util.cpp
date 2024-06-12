#include "util.h"
#include <iostream>
#include <cstdint>
#include <vector>
#include <regex>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "stb_image_write.h"
using namespace std;


constexpr int QUALITY = 50; // Image quality | 0 - 100


Pixel::Pixel(const uint8_t r, const uint8_t g, const uint8_t b) {
    this->r = r;
    this->g = g;
    this->b = b;
}


bool equals_ignore_case(const string& str1, const string& str2) {
    if (str1.length() != str2.length())
        return false;
    for (int i = 0; i < str1.length(); ++i) {
        if (toupper(str1[i]) != toupper(str2[i]))
            return false;
    }
    return true;
}


vector<string> split(const string& str) {
    vector<string> tokens;
    string s = str;
    unsigned long long pos;
    const string delim = " ";
    while ((pos = s.find(delim)) != string::npos) {
        tokens.push_back(s.substr(0, pos));
        s.erase(0, pos + delim.length());
    }
    tokens.push_back(s.substr(0, pos));
    return tokens;
}


uint8_t* read_image(const string& ref_path, int& width, int& height, int& bpp) {
    uint8_t* image = stbi_load(ref_path.c_str(), &width, &height, &bpp, 0);
    // ... process data if not NULL ...
    // ... x = width, y = height, n = # 8-bit components per pixel ...
    // ... replace '0' with '1'..'4' to force that many components per pixel
    // ... but 'n' will always be the number that it would have been if you said 0
    return image;
}


void write_image(const string& out_path, const uint8_t* new_image, const int& width, const int& height, const int& bpp) {
    const string ext = string(out_path).substr(string(out_path).find_last_of('.') + 1);
    if (equals_ignore_case(ext, "png"))
        stbi_write_png(out_path.c_str(), width, height, bpp, new_image, width * bpp);
    else if (equals_ignore_case(ext, "bmp"))
        stbi_write_bmp(out_path.c_str(), width, height, bpp, new_image);
    else if (equals_ignore_case(ext, "jpg") || equals_ignore_case(ext, "jpeg"))
        stbi_write_jpg(out_path.c_str(), width, height, bpp, new_image, QUALITY);
}


void free_image(uint8_t* image) {
    stbi_image_free(image);
}