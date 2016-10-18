#ifndef BUTIL_INTEGER_ADDITION_H_INCLUDED
#define BUTIL_INTEGER_ADDITION_H_INCLUDED

#include <butil/integer/interval.h>
#include <butil/integer/intset.h>
#include <butil/integer/value.h>

#include <butil/integer/detail/default_type.h>
#include <butil/integer/detail/add.h>

namespace butil {
namespace integer {

namespace detail {

template<bool OVF_SAFE, typename S, typename I, value_flags F>
constexpr auto add_impl(intset_value<S, F, I> x) noexcept
{
    return x;
}

template<bool OVF_SAFE, typename S, typename I, value_flags Fa, value_flags Fb>
constexpr auto add_impl(intset_value<S, Fa, I> a, intset_value<S, Fb, I> b) noexcept
{
    return intset_value<S, OVF_SAFE ? Fa | Fb : Fa | Fb | value_flags::overflow, I>(
            a.unsafe_value()+b.unsafe_value(),
            a.flags() | b.flags() | (
                OVF_SAFE ? value_flags::none :
                    (add_negative_overflow(a.unsafe_value(), b.unsafe_value()) ? value_flags::negative_overflow : value_flags::none) |
                    (add_positive_overflow(a.unsafe_value(), b.unsafe_value()) ? value_flags::positive_overflow : value_flags::none)
                )
            );
}

template<bool OVF_SAFE, typename S, typename I, value_flags Fa, value_flags Fb, typename ... Args>
constexpr auto add_impl(intset_value<S, Fa, I> a, intset_value<S, Fb, I> b, Args ... args) noexcept
{
    return add_impl<OVF_SAFE>(add_impl<OVF_SAFE>(a, b), args...);
}

}//namespace detail

template<typename Out = default_type, typename Tmp = default_type, typename ... Args>
constexpr auto add(Args ... args) noexcept
{
    using result = detail::type_or_default_t<Out, interval_add<Args...>>;
    using common = detail::type_or_default_t<Tmp, typename interval_type<result, Args...>::fast>;

    return value_cast<result>(detail::add_impl<interval_includes<common, result>::value>(value_cast<common>(args)...));
}

}//namespace integer
}//namespace butil

#endif//BUTIL_INTEGER_ADDITION_H_INCLUDED
