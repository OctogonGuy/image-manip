#include <iostream>
#include <stdint.h>
#include <algorithm>
#include "util.h"
using namespace std;



Pixel::Pixel(uint8_t r, uint8_t g, uint8_t b) {
    this->r = r;
    this->g = g;
    this->b = b;
}



void trim(string &str) {
    // Left trim
    str.erase(str.begin(), find_if(str.begin(), str.end(), [](unsigned char ch) {
        return !isspace(ch) && ch != '"';
    }));
    // Right trim
    str.erase(find_if(str.rbegin(), str.rend(), [](unsigned char ch) {
        return !isspace(ch) && ch != '"';
    }).base(), str.end());
}



bool equals_ignore_case(string str1, string str2)
{
    if (str1.length() != str2.length())
        return false;
    for (int i = 0; i < str1.length(); ++i) {
        if (toupper(str1[i]) != toupper(str2[i]))
            return false;
    }
    return true;
}