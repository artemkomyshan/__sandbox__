#pragma once

#include <boost/range/algorithm_ext/insert.hpp>
#include <boost/range/value_type.hpp>

namespace range_ext {

template<typename Container, typename Range>
Container make_container(const Range& range)
{
    Container result;
    boost::insert(result, result.end(), range);
    return result;
}

template<typename T, typename Range>
std::vector<T> make_vector(const Range& range)
{
    return make_container<std::vector<T>>(range);
}

template<typename Range, typename T = typename boost::range_value<Range>::type>
std::vector<T> vector(const Range& range)
{
    return make_container<std::vector<T>>(range);
}

}