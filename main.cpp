#include <iostream>
#include "origin_to_compare.hpp"

int main(int argc, const char *argv[])
{
    static_assert(is(5).any_of(1, 2, 3, 4, 5));
    static_assert(is(5).none_of(1, 2, 3, 4, 6));
    static_assert(!is(5).none_of(1, 2, 3, 4, 5));
    static_assert(!is(5).none_of(5));
    static_assert(is(5).any_of(5));
    return 0;
}
