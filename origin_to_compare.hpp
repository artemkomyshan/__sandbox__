#pragma once

template <typename T>
class origin_to_compare
{
public:
    template <typename... Args>
    constexpr bool any_of(Args const &...args)
    {
        return any_of_impl(args...);
    }

    template <typename... Args>
    constexpr bool none_of(Args const &...args)
    {

        return none_of_impl(args...);
    }

    origin_to_compare(origin_to_compare const &other) = delete;
    origin_to_compare &operator=(origin_to_compare const &other) = delete;

private:
    constexpr origin_to_compare(T const &v) : m_value(v)
    {
    }

    template <typename U>
    friend constexpr origin_to_compare<U> is(U const &v);

    template <typename U, typename... Args>
    constexpr bool any_of_impl(U const &first, Args const &...others)
    {
        return first == m_value ? true : any_of_impl(others...);
    }

    template <typename U>
    constexpr bool any_of_impl(U const &other)
    {
        return m_value == other;
    }

    template <typename U, typename... Args>
    constexpr bool none_of_impl(U const &first, Args const &...others)
    {
        return first == m_value ? false : none_of_impl(others...);
    }

    template <typename U>
    constexpr bool none_of_impl(U const &other)
    {
        return m_value != other;
    }

private:
    T const &m_value;
};

template <typename T>
constexpr origin_to_compare<T> is(T const &v)
{
    return origin_to_compare<T>(v);
}

// int main(int argc, const char *argv[])
// {
//     static_assert(is(5).any_of(1, 2, 3, 4, 5));
//     static_assert(is(5).none_of(1, 2, 3, 4, 6));
//     static_assert(!is(5).none_of(1, 2, 3, 4, 5));
//     return 0;
// }