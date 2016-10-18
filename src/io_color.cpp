#include <butil/io/color.h>

#include <butil/underlying_value.h>

#include <iostream>
#include <stdexcept>

#include <windows.h>

namespace butil {
namespace io {

static_assert(FOREGROUND_BLUE      == 0x1, "");
static_assert(FOREGROUND_GREEN     == 0x2, "");
static_assert(FOREGROUND_RED       == 0x4, "");
static_assert(FOREGROUND_INTENSITY == 0x8, "");
static_assert(BACKGROUND_BLUE      == 0x10, "");
static_assert(BACKGROUND_GREEN     == 0x20, "");
static_assert(BACKGROUND_RED       == 0x40, "");
static_assert(BACKGROUND_INTENSITY == 0x80, "");

namespace {

constexpr WORD FOREGROUND_WHITE = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED;
constexpr WORD FOREGROUND_MASK = FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED | FOREGROUND_INTENSITY;

// pword is winapi HANDLE
// iword is CONSOLE_SCREEN_BUFFER_INFO::wAttributes
int g_xalloc_color_index = std::ios_base::xalloc();

HANDLE get_output_handle(std::ostream & stream)
{
    HANDLE handle = stream.pword(g_xalloc_color_index);
    if(!handle)
    {
        if(&stream == &std::cout)
            handle = GetStdHandle(STD_OUTPUT_HANDLE);
        else if((&stream == &std::clog) || (&stream == &std::cerr))
            handle = GetStdHandle(STD_ERROR_HANDLE);
        //if(handle == INVALID_HANDLE_VALUE)
        //    throw std::runtime_error("GetStdHandle failed");
        if(handle && (handle != INVALID_HANDLE_VALUE))
        {
            CONSOLE_SCREEN_BUFFER_INFO info;
            if(GetConsoleScreenBufferInfo(handle, &info))
                stream.iword(g_xalloc_color_index) = info.wAttributes;
            else
                stream.iword(FOREGROUND_WHITE);
        }
        stream.pword(g_xalloc_color_index) = handle;
    }
    return handle == INVALID_HANDLE_VALUE ? NULL : handle;
}

WORD get_output_attributes(std::ostream & stream)
{
    return get_output_handle(stream) ? stream.iword(g_xalloc_color_index) : FOREGROUND_WHITE;
}

}//local stuff

output_color get_output_foreground(std::ostream & stream)
{
    static_cast<output_color>(get_output_attributes(stream) & FOREGROUND_MASK); 
}

void set_output_foreground(std::ostream & stream, output_color color)
{
    HANDLE handle = get_output_handle(stream);
    if(handle)
    {
        stream.flush();
        SetConsoleTextAttribute(handle, (stream.iword(g_xalloc_color_index) & ~FOREGROUND_MASK) | underlying_value(color));
    }
//    stream << "\x1b[" << (30 + (color & white)) << (color & bright ? ";1" : "") << 'm';
}

basic_ostream_color_saver<char, std::char_traits<char>>::basic_ostream_color_saver(std::ostream & stream)
    : m_stream(stream)
    , m_handle(get_output_handle(stream))
{
    if(m_handle)
    {
        m_old = stream.iword(g_xalloc_color_index);
    }
}

basic_ostream_color_saver<char, std::char_traits<char>>::~basic_ostream_color_saver() noexcept
{
    if(m_handle)
    {
        SetConsoleTextAttribute(m_handle, m_old);
    }
}

void basic_ostream_color_saver<char, std::char_traits<char>>::set(output_color color)
{
    if(m_handle)
    {
        m_stream.flush();
        SetConsoleTextAttribute(m_handle, (m_old & ~FOREGROUND_MASK) | underlying_value(color));
    }
}

}//namespace io
}//namespace butil
