#ifndef BUTIL_IO_INDENTATION_H_INCLUDED
#define BUTIL_IO_INDENTATION_H_INCLUDED

#include <butil/integer/add.h>
#include <butil/io/iteration.h>
#include <butil/string.h>

#include <ostream>
#include <string>
#include <string_ref.h>

/*
 * - jen vložit odsazení ( << indent << )
 * - vložit odsazení se zanořením ( << indent(x) << )
 * - vložit odsazení, odrážku, nastavit další zanoření podle délky odrážky ( << indent.item("-  ") << )
 *
 * {
 *   indenter indent(stream);
 *   - faster, only prints indentation when put into stream
 *   return stream << "aaa" << '\n'
 *       << indent << "bbb" << '\n'
 *          << indent(1) << "ccc" << '\n'
 *              << indent(2) << "ddd";
 *
 * }
 */

namespace butil {
namespace io {

namespace detail {

class indentation_base
{
protected :
    static int const s_xalloc_index;
};

// -- Simple indentation state --

class indentation_state : protected indentation_base
{
    friend std::ostream & operator<<(std::ostream & stream, indentation_state const & self)
    {
        assert(stream.iword(s_xalloc_index) >= 0);
        assert(stream.pword(s_xalloc_index) == &self);
        return stream << self.m_prefix << iterate_ref(self.m_step, stream.iword(s_xalloc_index));
    }
public :
    explicit indentation_state(std::ostream & stream)
    {
        void * const ptr = stream.pword(s_xalloc_index);
        if(ptr)
        {
            *this = *reinterpret_cast<indentation_state*>(ptr);
        }
        else
        {
            m_prefix = "";
            m_step = "  ";
        }
    }

    std::string const & prefix() const noexcept { return m_prefix; }

    std::string const & step() const noexcept { return m_step; }
protected :
    indentation_state(std::string prefix, std::string step)
        : m_prefix(std::move(prefix)), m_step(std::move(step))
    {}

    std::string m_prefix;
    std::string m_step;
};

// -- Indentation token for single use --

class indentation_token : protected indentation_base
{
    friend std::ostream & operator<<(std::ostream & stream, indentation_token const &)
    {
        void * const ptr = stream.pword(s_xalloc_index);
        if(ptr)
        {
            return stream << *reinterpret_cast<indentation_state*>(ptr);
        }
        else
        {
            assert(stream.iword(s_xalloc_index) >= 0);
            return stream << iterate("  ", stream.iword(s_xalloc_index));
        }
    }
};

class indentation_token_endl
{
    friend std::ostream & operator<<(std::ostream & stream, indentation_token_endl const &)
    {
        return stream << '\n' << indentation_token();
    }
};

// -- Indentation state that allows nesting --

class nest_indentation : private indentation_base
{
    friend std::ostream & operator<<(std::ostream & stream, nest_indentation const & self)
    {
        stream.iword(s_xalloc_index) = self.m_depth;
        stream.pword(s_xalloc_index) = const_cast<indentation_state*>(&self.m_state);
        return stream << self.m_state;
    }
public :
    nest_indentation(indentation_state const & state, long depth)
        : m_state(state), m_depth(depth) {}
private :
    indentation_state const & m_state;
    long const m_depth;
};

// -- '\n' followed by indentation --

class endl_indentation : private nest_indentation
{
    friend std::ostream & operator<<(std::ostream & stream, endl_indentation const & self)
    {
        return stream << '\n' << static_cast<nest_indentation const&>(self);
    }
public :
    endl_indentation(indentation_state const & state, long depth)
        : nest_indentation(state, depth) {}
};

// -- Itemize --

class item_indentation : protected indentation_state
{
    friend std::ostream & operator<<(std::ostream & stream, item_indentation const & self)
    {
        indentation_state const & state = self;
        stream.iword(s_xalloc_index) = 0;
        stream.pword(s_xalloc_index) = const_cast<indentation_state*>(&state);
        return stream << self.m_bullet;
    }
public :
    item_indentation(std::string bullet, indentation_state const & state, long depth)
        : indentation_state(
             str_cat(state.prefix(), iterate_ref(state.step(), depth), iterate(' ', bullet.size())),
             state.step())
        , m_bullet(std::move(bullet)) {}
private :
    std::string m_bullet;
};

}//namespace detail

constexpr detail::indentation_token indent;

constexpr detail::indentation_token_endl endl;

class indenter : protected detail::indentation_state
{
    friend std::ostream & operator<<(std::ostream & stream, indenter const & self)
    {
        detail::indentation_state const & state = self;
        stream.iword(s_xalloc_index) = self.m_depth;
        stream.pword(s_xalloc_index) = const_cast<detail::indentation_state*>(&state);
        return stream << state;
    }
public :
    indenter(std::ostream & stream)
        : detail::indentation_state(stream)
        , m_stream(stream)
        , m_state(reinterpret_cast<detail::indentation_state*>(stream.pword(s_xalloc_index)))
        , m_depth(stream.iword(s_xalloc_index))
    {
        assert(m_depth >= 0);
        stream.pword(s_xalloc_index) = this;
    }

    ~indenter() noexcept
    {
        m_stream.iword(s_xalloc_index) = m_depth;
        m_stream.pword(s_xalloc_index) = const_cast<detail::indentation_state*>(m_state);
    }

    long depth() const noexcept { return m_depth; }

    using detail::indentation_state::prefix;
    using detail::indentation_state::step;

    detail::endl_indentation endl(long depth = 0) const
    {
        assert(depth >= 0);
        assert(!integer::add_overflow(m_depth, depth));
        return { *this, m_depth + depth };
    }

    detail::item_indentation item(std::string bullet, long depth = 0) const
    {
        assert(depth >= 0);
        assert(!integer::add_overflow(m_depth, depth));
        return { std::move(bullet), *this, m_depth + depth };
    }

    detail::nest_indentation operator()(long depth) const
    {
        assert(depth >= 0);
        assert(!integer::add_overflow(m_depth, depth));
        return { *this, m_depth + depth };
    }
private :
    std::ostream & m_stream;
    detail::indentation_state const * const m_state;
    long const m_depth;
};

}//namespace io
}//namespace butil

#endif//BUTIL_IO_INDENTATION_H_INCLUDED
