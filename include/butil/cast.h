#ifndef BUTIL_CAST_H_INCLUDED
#define BUTIL_CAST_H_INCLUDED

#include <tuple>
#include <utility>

namespace butil {

template<typename T, typename ... Args>
struct enable_if_not_self : std::enable_if<true> {};

template<typename T, typename U>
struct enable_if_not_self<T, U> : std::enable_if<
        !std::is_base_of<T, std::decay_t<U>>::value>
{};

template<typename T, typename ... Args>
using enable_if_not_self_t = typename enable_if_not_self<T, Args...>::type;

// -- Unsafe wrapper --

template<typename ... Types>
struct unsafe_forward_wrapper;

namespace detail {

template<typename ... Types>
struct is_unsafe_forward_wrapper : std::false_type {};

template<typename ... Types>
struct is_unsafe_forward_wrapper<unsafe_forward_wrapper<Types...>> : std::true_type {};

}//namespace detail

template<typename ... Types>
using is_unsafe_forward_wrapper = detail::is_unsafe_forward_wrapper<std::decay_t<Types>...>;

static_assert(!is_unsafe_forward_wrapper<int>::value, "");
static_assert(!is_unsafe_forward_wrapper<int, int>::value, "");

template<typename ... Types>
struct unsafe_forward_wrapper : std::tuple<Types...>
{
    using tuple_type = std::tuple<Types...>;

    static constexpr size_t tuple_size = sizeof...(Types);

    template<typename ... Args,
        typename = std::enable_if_t<!is_unsafe_forward_wrapper<Args...>::value>>
    explicit constexpr unsafe_forward_wrapper(Args && ... args)
        : tuple_type(std::forward<Args>(args)...) {}

    unsafe_forward_wrapper(unsafe_forward_wrapper &&) = default;
    unsafe_forward_wrapper(unsafe_forward_wrapper const &) = default;

    template<typename Wrap,
        typename = std::enable_if_t<is_unsafe_forward_wrapper<Wrap>::value>>
    unsafe_forward_wrapper(Wrap && wrap)
        : tuple_type(std::forward<Wrap>(wrap)) {}

    ~unsafe_forward_wrapper() noexcept = default;
};

template<typename ... Args>
constexpr unsafe_forward_wrapper<Args && ...> unsafe_forward(Args && ... args) noexcept
{
    return unsafe_forward_wrapper<Args && ...>(std::forward<Args>(args)...);
}

// -- Assert wrapper --

template<typename ... Types>
struct assert_forward_wrapper;

namespace detail {

template<typename ... Types>
struct is_assert_forward_wrapper : std::false_type {};

template<typename ... Types>
struct is_assert_forward_wrapper<assert_forward_wrapper<Types...>> : std::true_type {};

}//namespace detail

template<typename ... Types>
using is_assert_forward_wrapper = detail::is_assert_forward_wrapper<std::decay_t<Types>...>;

template<typename ... Types>
struct assert_forward_wrapper : std::tuple<Types...>
{
    using tuple_type = std::tuple<Types...>;

    static constexpr size_t tuple_size = sizeof...(Types);

    template<typename ... Args,
        typename = std::enable_if_t<!is_assert_forward_wrapper<Args...>::value>>
    explicit constexpr assert_forward_wrapper(Args && ... args)
        : tuple_type(std::forward<Args>(args)...) {}

    assert_forward_wrapper(assert_forward_wrapper &&) = default;
    assert_forward_wrapper(assert_forward_wrapper const &) = default;

    template<typename Wrap,
        typename = std::enable_if_t<is_assert_forward_wrapper<Wrap>::value>>
    assert_forward_wrapper(Wrap && wrap)
        : tuple_type(std::forward<Wrap>(wrap)) {}

    ~assert_forward_wrapper() noexcept = default;
};

template<typename ... Args>
constexpr assert_forward_wrapper<Args && ...> assert_forward(Args && ... args) noexcept
{
    return assert_forward_wrapper<Args && ...>(std::forward<Args>(args)...);
}

//
// --
//

namespace detail {

template<typename T, typename Tuple, std::size_t ... Index>
constexpr T make_from_tuple_impl(Tuple && t, std::index_sequence<Index...>)
{
    return T(std::get<Index>(std::forward<Tuple>(t))...);
}

}//namespace detail

template<typename T, typename Tuple>
constexpr T make_from_tuple(Tuple && t)
{
    return detail::make_from_tuple_impl<T>(std::forward<Tuple>(t),
        std::make_index_sequence<
            std::tuple_size<std::decay_t<Tuple>>::value
        >());
}

}//namespace butil

#endif//BUTIL_CAST_H_INCLUDED
