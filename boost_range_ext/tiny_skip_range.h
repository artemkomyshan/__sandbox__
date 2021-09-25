#pragma once

#include <cstddef>

template <typename Range>
struct tiny_skip_range
{
  using iterator = std::conditional<std::is_const<Range>::value, typename Range::const_iterator, typename Range::iterator>::type;
  using const_iterator = iterator;

  Range &m_range;
  std::ptrdiff_t m_number;

  tiny_skip_range(Range &range, std::ptrdiff_t n) : m_range(range), m_number(n) {}
  iterator begin()
  {
    return std::next(std::begin(m_range),
                     std::min(
                         std::distance(
                             std::begin(m_range),
                             std::end(m_range)),
                         m_number));
  }

  iterator begin() const
  {
    return std::next(std::begin(m_range),
                     std::min(
                         std::distance(
                             std::begin(m_range),
                             std::end(m_range)),
                         m_number));
  }

  iterator end()
  {
    return std::end(m_range);
  }

  iterator end() const
  {
    return std::end(m_range);
  }
};

struct hold_count
{
  hold_count() = default;
  hold_count(int n) : count(n) {}
  int count;
  hold_count operator()(int n) const
  {
    return hold_count(n);
  }
};

template <typename R>
tiny_skip_range<R> operator|(R &r, hold_count hc)
{
  return tiny_skip_range<R>(r, hc.count);
}

template <typename R>
tiny_skip_range<const R> operator|(R const &r, hold_count hc)
{
  return tiny_skip_range<const R>(r, hc.count);
}

const auto skiped = hold_count{};

template <typename Range>
tiny_skip_range<Range> skip(Range &range, std::ptrdiff_t n)
{
  return tiny_skip_range<Range>(range, n);
}

template <typename Range>
tiny_skip_range<Range> skip(Range const&range, std::ptrdiff_t n)
{
  return tiny_skip_range<const Range>(range, n);
}
