#ifndef BUTIL_IO_COLOR_H_INCLUDED
#define BUTIL_IO_COLOR_H_INCLUDED

#include <ostream>

namespace butil {
namespace io {

/** \brief Terminal colors that should be supported on all reasonable platforms.
 *
 * Works also as stream manipulator.
 */
enum /*class*/ output_color : uint8_t
{
    // basic flags
    black  = 0x0,
    blue   = 0x1,
    green  = 0x2,
    red    = 0x4,
    bright = 0x8,
    // combined colors
    aqua   = blue  | green,
    purple = blue  | red,
    yellow = green | red,
    white  = blue  | green | red,
    grey   = bright | black, 
    // bright colors
    bright_blue   = bright | blue,
    bright_green  = bright | green,
    bright_red    = bright | red,
    bright_aqua   = bright | aqua,
    bright_purple = bright | purple,
    bright_yellow = bright | yellow,
    bright_white  = bright | white,
};

template<typename Char, typename Traits>
output_color get_output_foreground(std::basic_ostream<Char, Traits> &) noexcept
{
    return output_color::white;
}

output_color get_output_foreground(std::ostream & stream);

template<typename Char, typename Traits>
void set_output_foreground(std::basic_ostream<Char, Traits> &, output_color) noexcept
{
}

void set_output_foreground(std::ostream & stream, output_color color);

template<typename Char, typename Traits>
std::basic_ostream<Char, Traits> & operator<<(std::basic_ostream<Char, Traits> & stream, output_color color)
{
    set_output_foreground(stream, color);
    return stream;
}

template<typename Char, typename Traits>
struct basic_ostream_color_saver
{
    basic_ostream_color_saver(std::basic_ostream<Char, Traits> &) {}
    void set(output_color) {}
};

template<>
class basic_ostream_color_saver<char, std::char_traits<char>>
{
public :
    basic_ostream_color_saver(std::ostream & stream);
    ~basic_ostream_color_saver() noexcept;
    void set(output_color color);
private :
    std::ostream & m_stream;
    void * m_handle;
    unsigned m_old;
};

using ostream_color_saver = basic_ostream_color_saver<char, std::char_traits<char>>;

}//namespace io
}//namespace butil

#endif//BUTIL_IO_COLOR_H_INCLUDED
