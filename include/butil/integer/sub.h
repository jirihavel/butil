#ifndef BUTIL_INTEGER_SUBTRACTION_H_INCLUDED
#define BUTIL_INTEGER_SUBTRACTION_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/intset.h>
#include <butil/integer/value.h>

#include <butil/integer/detail/default_type.h>
#include <butil/integer/detail/sub.h>

namespace butil {
namespace integer {

namespace detail {

template<bool OVF_SAFE, typename Set, value_flags FlgA, value_flags FlgB, typename Int>
constexpr auto sub_impl(intset_value<Set, FlgA, Int> a, intset_value<Set, FlgB, Int> b) noexcept
{
    // Set should be able to hold all calculations
    return intset_value<Set, FlgA | FlgB | (OVF_SAFE ? value_flags::none : value_flags::overflow), Int>(
            a.unsafe_value()-b.unsafe_value(),
            OVF_SAFE ? value_flags::none :
                (sub_negative_overflow(a.unsafe_value(), b.unsafe_value()) ? value_flags::negative_overflow : value_flags::none) |
                (sub_positive_overflow(a.unsafe_value(), b.unsafe_value()) ? value_flags::positive_overflow : value_flags::none)
            );
}

}//namespace detail

/** \brief Subtract two intset values.
 *
 * Sets :
 * - A, B
 * - Out (intset)  [ = interval_sub<A, B> ]
 * - Tmp (integer) [ = interval_type<Out, A, B>::fast ]
 *
 *
 */

template<typename Out = default_type, typename Tmp = default_type, typename A, typename B>
constexpr auto sub(A a, B b) noexcept
{
    using result = detail::type_or_default_t<Out, interval_sub<A, B>>;
    using common = detail::type_or_default_t<Tmp, typename interval_type<result, A, B>::fast>;

    return value_cast<result>(detail::sub_impl<interval_includes<common, result>::value>(value_cast<common>(a), value_cast<common>(b)));
/*
    // if A or B don't fit into Tmp, range_error is returned
    // if Tmp(A)-Tmp(B) overflow, overflow is returned
    // if Tmp(A)-Tmp(B) don't fit into Out, overflow is returned

    constexpr value_flags Flags = interval_includes<common, A>::value && interval_includes<common, B>::value
        ? value_flags::none : value_flags::range_error;
    value_flags flags = interval_contains<common>(a) && interval_contains<common>(b)
        ? value_flags::none : value_flags::range_error;

    constexpr value_flags Flags = interval_includes<common, result>::value
        ? value_flags::none : value_flags::overflow;
    constexpr value_flags Flags = interval_includes<common, result>::value
        ? value_flags::none : value_flags::overflow;

    common ca = intset_traits<A>::value(a);
    common cb = intset_traits<A>::value(b);

    constexpr value_flags Flags = interval_includes<common, A>::value && interval_includes<common, B>::value
        ? value_flags::none : value_flags::range_error;
    value_flags flags = interval_contains<common>(a) && interval_contains<common>(b)
        ? value_flags::none : value_flags::range_error;


    return intset_value<result>(
          static_cast<common>(intset_traits<A>::value(a))
        - static_cast<common>(intset_traits<B>::value(b)));*/
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_SUBTRACTION_H_INCLUDED
