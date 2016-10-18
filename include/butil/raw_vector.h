#ifndef BUTIL_RAW_VECTOR_H_INCLUDED
#define BUTIL_RAW_VECTOR_H_INCLUDED

#include <cassert>
#include <cstring>

#include <memory>

namespace butil {

/** \brief Unformatted vector-like container for raw data.
 */
class raw_vector
{
public :
    explicit raw_vector(size_t elem, size_t size = 0)
        : m_data(size > 0 ? new char[size*elem] : nullptr)
        , m_size(size), m_capacity(size), m_elem(elem)
    {
        if(size > max_size())
        {
            //if new didn't fail already, the size is wrong anyway
            throw std::bad_array_new_length();
        }
        if(m_data)
        {
            memset(m_data.get(), 0, size*elem);
        }
    }

    raw_vector(raw_vector const &) = delete;

    raw_vector & operator=(raw_vector const &) = delete;

    char * data() noexcept
    {
        return m_data.get();
    }

    char const * data() const noexcept
    {
        return m_data.get();
    }

    size_t size() const noexcept
    {
        return m_size;
    }

    size_t capacity() const noexcept
    {
        return m_capacity;
    }

    size_t elem() const noexcept
    {
        return m_elem;
    }

    size_t max_size() const noexcept
    {
        return SIZE_MAX/elem();
    }

    char * at(size_t i)
    {
        assert(i < size());
        return data() + i*elem();
    }

    char const * at(size_t i) const
    {
        assert(i < size());
        return data() + i*elem();
    }

    bool equal(size_t i, void const * ptr) const noexcept
    {
        return std::memcmp(at(i), ptr, elem()) == 0;
    }

    size_t find(void const * ptr) const noexcept
    {
        size_t i;
        for(i = 0; i < size(); ++i)
        {
            if(equal(i, ptr))
            {
                return i;
            }
        }
        return i;
    }

    void reserve(size_t cap)
    {
        if(capacity() < cap)
        {
            if(cap > max_size())
            {
                throw std::bad_array_new_length();
            }
            std::unique_ptr<char[]> new_data(new char[cap*elem()]);
            std::memcpy(new_data.get(), m_data.get(), elem()*size());
            m_data = std::move(new_data);
            m_capacity = cap;
        }
    }

    void resize(size_t s)
    {
        if(s > capacity())
        {
            reserve(std::max(s, 2*capacity()));
        }
        size_t const old = std::exchange(m_size, s);
        if(old < s)
        {
            std::memset(at(old), 0, (s-old)*elem());
        }
    }

    char * add_back()
    {
        size_t const s = size();
        resize(s+1);
        return at(s);
    }

    void push_back(void const * ptr)
    {
        std::memcpy(add_back(), ptr, elem());
    }
private :
    std::unique_ptr<char[]> m_data;
    size_t m_size;
    size_t m_capacity;
    size_t const m_elem;
};

}//namespace butil

#endif//BUTIL_RAW_VECTOR_H_INCLUDED
