#ifndef BUTIL_TAGGED_POINTER_H_INCLUDED
#define BUTIL_TAGGED_POINTER_H_INCLUDED

#include <butil/fail_fast.h>
#include <butil/pun.h>

#include <cstdint>

#include <boost/integer.hpp>
#include <boost/integer/static_log2.hpp>

namespace butil {

/** \brief Raw pointer that uses its alignment to store additional data.
 * On some architectures, pointers are aligned, so lower bits are always zero.
 *
 * The exact alignment depends on architecture, compiler & settings.
 *
 * The interface tries to match smart pointer interface with additional methods for payload manipulation.
 *
 * \param T Pointed type.
 * \param TagBits Number of zero bits. Default value is calculated from alignment of T.
 */
template<
    typename T,
    std::size_t TagBits = boost::static_log2<alignof(T)>::value
>
class tagged_ptr;

namespace internal {

template<typename T, std::size_t TagBits>
class tagged_ptr_base
{
public :
    using element_type = T;
    using pointer_type = T*;
    using tag_type = typename boost::uint_t<TagBits>::fast;

    static constexpr std::size_t tag_bits = TagBits;
    static constexpr tag_type tag_max = (1<<tag_bits)-1;

    void set(pointer_type p, tag_type t = 0)
    {
        Expects(pointer_aligned(p));
        Expects(t <= tag_max);
        data = pun::p2u(p) | t;
    }

    pointer_type get() const noexcept
    {
        return pun::u2p<element_type>(data & ptr_mask);
    }

    void set_tag(tag_type t) 
    {
        Expects(t <= tag_mask);
        data = (data & ptr_mask) | t;
    }

    tag_type get_tag() const noexcept
    {
        return data & tag_mask;
    }

    /** \brief Check pointer
     * pointer != nullptr
     */
    explicit operator bool() const noexcept
    {
        return get();
    }

    /** \brief Check pointer
     * pointer != nullptr
     */
    bool operator!() const noexcept
    {
        return !get();
    }
protected :
    static constexpr std::uintptr_t tag_mask = (1<<tag_bits)-1;
    static constexpr std::uintptr_t ptr_mask = ~tag_mask;

    static bool pointer_aligned(pointer_type p) noexcept
    {
        return (pun::p2u(p) & tag_mask) == 0;
    }

    constexpr tagged_ptr_base() = default;

    tagged_ptr_base(pointer_type p, tag_type t)
        : data(pun::p2u(p) | t)
    {
        Expects(pointer_aligned(p));
        Expects(t <= tag_max);
    }
private :
    std::uintptr_t data;
};

}//namespace internal

template<typename T, std::size_t TagBits>
class tagged_ptr
    : public internal::tagged_ptr_base<T, TagBits>
{
    using base = internal::tagged_ptr_base<T, TagBits>;
public :
    using element_type = typename base::element_type;
    using pointer_type = typename base::pointer_type;
    using tag_type     = typename base::tag_type;

    /** \brief Default constructor
     * pointer=?, tag=?
     */
    constexpr tagged_ptr() = default;

    /** \brief Construct from aligned pointer + tag
     * pointer=p, tag=t
     */
    explicit tagged_ptr(pointer_type p, tag_type t = 0)
        : base(p, t) {}

    /** \brief Construct from tag
     * pointer=nullptr, tag=t
     */
    explicit tagged_ptr(tag_type t)
        : base(nullptr, t) {}

    /** \brief Dereference object
     * Exclusive for single objects
     */
    element_type & operator*() const noexcept
    {
        return *base::get();
    }

    /** \brief Dereference object member
     * Exclusive for single objects
     */
    element_type * operator->() const noexcept
    {
        return base::get();
    }
};

template<typename T, std::size_t TagBits>
class tagged_ptr<T[], TagBits>
    : private internal::tagged_ptr_base<T, TagBits>
{
    using base = internal::tagged_ptr_base<T, TagBits>;
public :
    using element_type = typename base::element_type;
    using pointer_type = typename base::pointer_type;
    using tag_type     = typename base::tag_type;

    /** \brief Default constructor
     * pointer=?, tag=?
     */
    constexpr tagged_ptr() = default;

    /** \brief Construct from pointer + tag
     * pointer=p, tag=t
     */
    explicit tagged_ptr(pointer_type p, tag_type t = 0)
        : base(p, t) {}

    /** \brief Construct from tag
     * pointer=nullptr, tag=t
     */
    explicit tagged_ptr(tag_type t)
        : base(nullptr, t) {}

    /** Offset access
     * Exclusive for arrays
     */
    element_type & operator[](size_t i) const noexcept
    {
        return base::get()[i];
    }
};

}//namespace butil

#endif//BUTIL_TAGGED_POINTER_H_INCLUDED
