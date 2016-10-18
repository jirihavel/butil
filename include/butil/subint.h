#ifndef BUTIL_SUBINT_H_INCLUDED
#define BUTIL_SUBINT_H_INCLUDED

#include <butil/subtype.h>
#include <butil/integer/add.h>
#include <butil/integer/interval.h>
#include <butil/io/debug.h>

#include <ostream>

namespace butil {

template<typename Interval>
struct interval_predicate
{
    constexpr interval_predicate() {}

    template<typename I2, typename = std::enable_if_t<
        integer::interval_includes<Interval, I2>::value>>
    constexpr interval_predicate(interval_predicate<I2> const &) {}

    template<typename Integer>
    constexpr bool operator()(Integer x) const
    {
        static_assert(
            !integer::interval_meet_empty<Interval, Integer>::value,
            "Ranges do not overlap, test will always fail.");
        return integer::interval_contains<Interval>(x);
    }
};

template<typename T, typename Interval>
struct subtype_traits<T, interval_predicate<Interval>> : subtype_traits_base
{
    /* Make a subtype with interval_predicate an intset.
     */
    template<typename Derived>
    struct base : subtype_base
    {
        static constexpr T max = integer::intset_traits<Interval>::max;
        static constexpr T min = integer::intset_traits<Interval>::min;
    };
};

template<typename Tag, typename Interval>
struct tagged_interval_predicate
    : private interval_predicate<Interval>
{
    constexpr tagged_interval_predicate() {}

    template<typename I2, typename = std::enable_if_t<
        integer::interval_includes<Interval, I2>::value>>
    constexpr tagged_interval_predicate(tagged_interval_predicate<Tag, I2> const &) {}

    using interval_predicate<Interval>::operator();
};

template<typename T, typename Tag, typename Interval>
struct subtype_traits<T, tagged_interval_predicate<Tag, Interval>> : subtype_traits<T, interval_predicate<Interval>>
{
};

template<typename Interval,
    typename Integer = typename integer::intset_type<Interval>::least>
using subint = subtype<Integer, interval_predicate<Interval>>;

template<typename Interval,
    typename Integer = typename integer::intset_type<Interval>::least>
using explicit_subint = explicit_subtype<Integer, interval_predicate<Interval>>;

template<typename Interval,
    typename Integer = typename integer::intset_type<Interval>::least>
using implicit_subint = implicit_subtype<Integer, interval_predicate<Interval>>;

namespace detail {

template<typename T>
struct is_subint : std::false_type {};

template<typename Interval, typename Integer, typename Traits>
struct is_subint<subtype<Integer, interval_predicate<Interval>, Traits>>
    : std::true_type {};

}//namespace detail

// -- Subint comparison

template<typename A, typename B,
    typename = std::enable_if_t<detail::is_subint<A>::value || detail::is_subint<B>::value>>
bool operator==(A const & a, B const & b)
{
    return integer::cmp::eq(a, b);
}

template<typename A, typename B,
    typename = std::enable_if_t<detail::is_subint<A>::value || detail::is_subint<B>::value>>
bool operator!=(A const & a, B const & b)
{
    return integer::cmp::ne(a, b);
}

template<typename A, typename B,
    typename = std::enable_if_t<detail::is_subint<A>::value || detail::is_subint<B>::value>>
bool operator<(A const & a, B const & b)
{
    return integer::cmp::lt(a, b);
}

template<typename A, typename B,
    typename = std::enable_if_t<detail::is_subint<A>::value || detail::is_subint<B>::value>>
bool operator<=(A const & a, B const & b)
{
    return integer::cmp::le(a, b);
}

template<typename A, typename B,
    typename = std::enable_if_t<detail::is_subint<A>::value || detail::is_subint<B>::value>>
bool operator>(A const & a, B const & b)
{
    return integer::cmp::gt(a, b);
}

template<typename A, typename B,
    typename = std::enable_if_t<detail::is_subint<A>::value || detail::is_subint<B>::value>>
bool operator>=(A const & a, B const & b)
{
    return integer::cmp::ge(a, b);
}

// -- Subint arithmetics


// -- Subint IO

template<typename Char, typename CharTraits, typename Interval, typename Integer, typename Traits>
std::basic_ostream<Char, CharTraits> & operator<<(
        std::basic_ostream<Char, CharTraits> & stream,
        subtype<Integer, interval_predicate<Interval>, Traits> const & value)
{
    return stream << +value.value();// "+" converts char to int
}

}//namespace butil

#endif//BUTIL_SUBINT_H_INCLUDED
