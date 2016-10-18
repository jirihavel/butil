#ifndef BUTIL_TAGGED_TYPE_H_INCLUDED
#define BUTIL_TAGGED_TYPE_H_INCLUDED

#include <utility>

namespace butil {

/** \brief Adds tag to create a new different type.
 */
template<typename T, typename Tag>
struct tagged_type
{
public :
    template<typename ... Args>
    tagged_type(Args && ... args)
        : m_value(std::forward<Args>(args)...) {}

    tagged_type(tagged_type &&) = default;
    tagged_type(tagged_type const &) = default;
    tagged_type & operator=(tagged_type &&) = default;
    tagged_type & operator=(tagged_type const &) = default;

    T const & get() const noexcept
    {
        return m_value;
    }

    explicit operator T const &() const noexcept
    {
        return m_value;
    }
private :
    T m_value;
};

}//namespace butil

#endif//BUTIL_TAGGED_TYPE_H_INCLUDED
