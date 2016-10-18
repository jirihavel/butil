#ifndef BUTIL_FLOAT_COMPARE_H_INCLUDED
#define BUTIL_FLOAT_COMPARE_H_INCLUDED

#include <cassert>
#include <cfloat>
#include <cmath>

namespace butil {
namespace floating {

template<typename Float>
class CmpAbs
{
public :
    using float_type = Float;

    constexpr explicit CmpAbs(float_type epsilon) noexcept
        : m_epsilon((assert(epsilon >= 0), epsilon)) {}

    // |a - b| <= epsilon
    // - infinity != anything finite (except when epsilon == infinity)
    // - nan != anything
    bool eq(float_type a, float_type b) const noexcept
    {
        // (a == b) handles infinities
        return (a == b) || (std::abs(a - b) <= m_epsilon);
    }

    bool ne(float_type a, float_type b) const noexcept
    {
        return !eq(a, b);
    }
private :
    float_type m_epsilon;
};

template<typename Float>
class CmpRel
{
public :
    using float_type = Float;

    constexpr explicit CmpRel(float_type epsilon) noexcept
        : m_epsilon((assert(epsilon >= 0), epsilon)) {}

    float_type rel_to_abs_eps(float_type a, float_type b) const noexcept
    {
        auto const val = std::max(std::abs(a), std::abs(b));
        return std::isinf(val) ? m_epsilon : val*m_epsilon;
    }

    // |a - b| <= max(|a|, |b|)*epsilon
    // - infinity != anything finite
    // - nan != anything
    bool eq(float_type a, float_type b) const noexcept
    {
        return (a == b) || (std::abs(a - b) <= rel_to_abs_eps(a, b));
    }

    bool ne(float_type a, float_type b) const noexcept
    {
        return !eq(a, b);
    }
private :
    float_type m_epsilon;
};

template<typename Float>
class CmpAbsRel
{
public :
    using float_type = Float;

    constexpr explicit CmpAbsRel(float_type abs_eps, float_type rel_eps) noexcept
        : m_abs_eps((assert(abs_eps >= 0), abs_eps))
        , m_rel_eps((assert(rel_eps >= 0), rel_eps)) {}

    float_type rel_to_abs_eps(float_type a, float_type b) const noexcept
    {
        auto const val = std::max(std::abs(a), std::abs(b));
        return std::isinf(val) ? m_rel_eps : val*m_rel_eps;
    }

    // |a - b| <= max(|a|, |b|)*epsilon
    // - infinity != anything finite
    // - nan != anything
    bool eq(float_type a, float_type b) const noexcept
    {
        auto const diff = std::abs(a - b);
        return (a == b) || (diff <= m_abs_eps) || (diff <= rel_to_abs_eps(a, b));
    }

    bool ne(float_type a, float_type b) const noexcept
    {
        return !eq(a, b);
    }
private :
    float_type m_abs_eps;
    float_type m_rel_eps;
};

#if 0
template<typename Float>
class CmpUlp
{
public :
    using float_type = Float;

    constexpr explicit CmpUlp(unsigned epsilon) noexcept
        : m_epsilon(epsilon) {}
/*
    bool eq(float_type a, float_type b) const noexcept
    {
        return (a == b) || (std::abs(a - b) <= abs_eps(a, b));
    }

    bool ne(float_type a, float_type b) const noexcept
    {
        return !eq(a, b);
    }
    */
private :
    unsigned m_epsilon;
};
#endif
}//namespace floating
}//namespace butil

#endif//BUTIL_FLOAT_COMPARE_H_INCLUDED
