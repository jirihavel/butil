#ifndef BUTIL_IO_YAML_H_INCLUDED
#define BUTIL_IO_YAML_H_INCLUDED

#include <butil/io/indentation.h>
#include <butil/io/itemize.h>

#include <utility>

namespace butil {
namespace io {
namespace yaml {

template<typename Type>
class sequence_wrapper
{
    template<typename Char, typename Traits>
    friend std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, sequence_wrapper const & self)
    {
        indenter indent(stream);
        return stream << itemize(std::cref(self.m_value), indent.item("- "), indent.endl());
    }
public :
    template<typename T,
        typename = std::enable_if_t<std::is_constructible<Type, T&&>::value>>
    explicit sequence_wrapper(T && x)
        : m_value(std::forward<T>(x))
    {}
private :
    Type m_value;
};

template<typename T>
auto seq(T && x)
{
    return sequence_wrapper<std::decay_t<T>>(std::forward<T>(x));
}

template<typename T>
auto seq_ref(T const & x)
{
    return sequence_wrapper<T const &>(x);
}

/** \brief Print any sequence as yaml seq
 *
 * Sequence is detected by begin function,
 */
template<typename Char, typename Traits, typename Seq>
auto operator<<(std::basic_ostream<Char, Traits> & stream, Seq const & seq)
    -> decltype(begin(seq),stream)
{
    return stream << seq_ref(seq);
};

}//namespace yaml
}//namespace io
}//namespace butil

#endif//BUTIL_IO_YAML_H_INCLUDED
