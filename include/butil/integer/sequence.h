#ifndef BUTIL_INTEGER_SEQUENCE_H_INCLUDED
#define BUTIL_INTEGER_SEQUENCE_H_INCLUDED

#include <butil/integer/intset.h>

#include <algorithm>
#include <utility>

namespace butil {
namespace integer {

template<typename T, T ... Ints>
using sequence = std::integer_sequence<T, Ints...>;

template<typename S>
using sequence_empty = std::integral_constant<bool, S::size == 0>;

// -- all - logical and --

template<typename S>
struct sequence_all;

template<>
struct sequence_all<sequence<bool>>
    : std::true_type {};

template<bool X, bool ... Bools>
struct sequence_all<sequence<bool, X, Bools...>>
    : std::integral_constant<bool,
        X && sequence_all<sequence<bool, Bools...>>::value>
{};

// -- any - logical or --

template<typename S>
struct sequence_any;

template<>
struct sequence_any<sequence<bool>>
    : std::false_type {};

template<bool X, bool ... Bools>
struct sequence_any<sequence<bool, X, Bools...>>
    : std::integral_constant<bool,
        X || sequence_any<sequence<bool, Bools...>>::value>
{};

// -- none - nor --

template<typename S>
using sequence_none = std::integral_constant<bool, !sequence_any<S>::value>;

// -- does sequence contain a value --

template<typename Seq, typename Seq::value_type X>
struct static_sequence_contains;

template<typename T, T X, T ... Ints>
struct static_sequence_contains<sequence<T, Ints...>, X>
    : sequence_any<sequence<bool, (X == Ints)...>> {};

template<typename A, typename B>
struct sequence_includes;

template<typename Seq, typename T, T ... Ints>
struct sequence_includes<Seq, sequence<T, Ints...>>
    : sequence_all<sequence<bool, (static_sequence_contains<Seq, Ints>::value)...>> {};

template<typename A, typename B>
struct sequence_meet_empty;

template<typename Seq, typename T, T ... Ints>
struct sequence_meet_empty<Seq, sequence<T, Ints...>>
    : sequence_none<sequence<bool, (static_sequence_contains<Seq, Ints>::value)...>> {};

template<typename T, T ... Ints, typename X>
constexpr bool sequence_contains(sequence<T, Ints...>, X x)
{
    using std::max;
    return max({ (intset_traits<X>::value(x) == Ints)... });
}

namespace check {

using A = sequence<int, 1, 2, 3>;
using B = sequence<int, 1, 2, 4>;
using C = sequence<int, 1, 2>;
using D = sequence<int, 0, 5>;

static_assert(!sequence_includes<A, B>::value, "");
static_assert( sequence_includes<A, C>::value, "");
static_assert(!sequence_includes<A, D>::value, "");

static_assert(!sequence_meet_empty<A, B>::value, "");
static_assert(!sequence_meet_empty<A, C>::value, "");
static_assert( sequence_meet_empty<A, D>::value, "");

}//namespace check

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_SEQUENCE_H_INCLUDED
