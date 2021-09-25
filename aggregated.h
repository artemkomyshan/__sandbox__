#pragma once
#include <string>

template <typename T>
struct aggregated
{
    T val;
    std::string tag;
};

aggregated(const char*, const char *) -> aggregated<std::string>;