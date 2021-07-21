#pragma once
#include <string>

// returns a string formated in c style printf
// for use in to_string member functions
// this function aims for convenience not speed
std::string fmt_string(const char* fmt, ...);


