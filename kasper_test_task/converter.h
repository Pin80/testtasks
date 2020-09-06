#ifndef CONVERTER_H
#define CONVERTER_H
#include <vector>
#include "base_types.h"
#include "locale_ru.h"
#include "locale_en.h"

template <class SET>
static constexpr byte_t get_depthset(byte_t _idx)
{
    return get_depthset<typename SET::subset_t>(_idx + 1);
}

template <class RANGE>
static constexpr byte_t get_depthrange(byte_t _idx)
{
    if (RANGE::m_denominator != RANGE::m_mindenom)
    {
        typedef typename RANGE::subset_t subset_t;
        _idx = get_depthset<subset_t>(_idx + 1);
    }
    else
    {
        _idx = get_depthset<RANGE>(_idx + 1);
    }
    using subrange_t = typename RANGE::subrange_t;
    return get_depthrange<subrange_t>(_idx);
}

template <>
byte_t constexpr get_depthrange<void>(byte_t _idx)
{
    return _idx;
}

template <>
byte_t constexpr get_depthset<void>(byte_t _idx)
{
    return _idx;
}

/// @class converteriml_t
/// @brief Внутренняя реализация конвертера числа в текстовое представление.
class converteriml_t
{
    converteriml_t(byte_t _size)
        : m_split_number(_size, 0), m_context(_size, 0)
    {}
    converteriml_t(const converteriml_t& _obj)
    {
        m_split_number = _obj.m_split_number;
        m_context = _obj.m_context;
    }
    converteriml_t(converteriml_t&& _obj)
    {
        m_split_number = std::move(_obj.m_split_number);
        m_context = std::move(_obj.m_context);
    }
    template<class T>
    bool inRange(bound_t _val) const
    {
        return (_val <= T::m_max) && (_val >= T::m_min);
    }
    template<class T>
    bool belowUpBound(bound_t _val) const
    {
        return (_val <= T::m_max);
    }

    template <class RANGE>
    void fill_range(bound_t _val, byte_t _idx)
    {
        if (!belowUpBound<RANGE>(_val)) return;
        auto currden = RANGE::m_denominator;
        bound_t left_part = _val / RANGE::m_denominator;
        bound_t right_part = 0;
        if (RANGE::m_denominator != RANGE::m_mindenom)
        {
            // Для случаев когда RANGE::m_min больше чем
            // _val / RANGE::m_denominator left_part = 0
            auto test_left = left_part * RANGE::m_denominator >= RANGE::m_min;
            left_part = (test_left)?left_part:0;
            right_part = _val - left_part * RANGE::m_denominator;
            m_split_number[_idx] = left_part;
            typedef typename RANGE::subset_t subset_t;
            if (inRange<RANGE>(_val))
            {
                _idx = fill_set<subset_t>(left_part, ++_idx);
            }
            else
            {
                _idx = fill_set<subset_t>(0, ++_idx);
            }
        }
        else
        {
            m_split_number[_idx] = left_part;
            _idx = fill_set<RANGE>(left_part, ++_idx);
        }
        using subrange_t = typename RANGE::subrange_t;
        fill_range<subrange_t>(right_part, _idx);
    }
    template <class SET>
    byte_t fill_set(bound_t _val, byte_t _idx)
    {
        if (!belowUpBound<SET>(_val)) return _idx;
        bound_t left_part = _val / SET::m_setdenominator;
        auto test_left = left_part * SET::m_setdenominator >= SET::m_min;
        left_part = (test_left)?left_part:0;
        bound_t right_part = _val - left_part * SET::m_setdenominator;
        if (inRange<SET>(_val))
        {
            m_split_number[_idx] = left_part;
        }
        else
        {
            m_split_number[_idx] = 0;
        }
        return fill_set<typename SET::subset_t>(right_part, ++_idx);
    }
    template <class RANGE>
    void fill_contextrange(byte_t _idx)
    {
        typedef typename RANGE::subset_t subset_t;
        if (RANGE::m_denominator != RANGE::m_mindenom)
        {
            auto auxidx = RANGE::get_auxdictindex(_idx, m_split_number);
            m_context[_idx] = auxidx;
            _idx = fill_contextset<subset_t>(++_idx);
        }
        else
        {
            auto auxidx = RANGE::get_auxdictindex(_idx, m_split_number);
            m_context[_idx] = auxidx;
            _idx = fill_contextset<RANGE>(++_idx);
        }
        using subrange_t = typename RANGE::subrange_t;
        fill_contextrange<subrange_t>(_idx);
    }
    template <class SET>
    byte_t fill_contextset(byte_t _idx)
    {
        auto auxidx = SET::get_auxdictindex(_idx, m_split_number);
        m_context[_idx] = auxidx;
        return fill_contextset<typename SET::subset_t>(++_idx);
    }
    template <class RANGE>
    void decode_range(byte_t _idx, std::wstring& _resstr) const
    {
        auto val = m_split_number[_idx];
        typedef typename RANGE::subset_t subset_t;
        if (RANGE::m_denominator != RANGE::m_mindenom)
        {
            if (val)
            {
                auto idx2 = _idx;
                _idx = decode_set<subset_t>(++_idx, _resstr);
                if (idx2 < m_context.size())
                {
                    auto ppidx = m_context[idx2];
                    if (RANGE::m_pplength)
                    { _resstr += RANGE::get_ppelement(ppidx); }
                }
            }
            else
            {
                _idx = decode_set<subset_t>(++_idx, _resstr);
            }
        }
        else if ((RANGE::m_denominator == 1))
        {
            if (val)
            {
                _idx = decode_set<RANGE>(++_idx, _resstr);
            }
            else
            {
                _idx = decode_set<RANGE>(++_idx, _resstr);
            }
        }
        using subrange_t = typename RANGE::subrange_t;
        decode_range<subrange_t>(_idx, _resstr);
    }
    template <class SET>
    byte_t decode_set(byte_t _idx, std::wstring& _resstr) const
    {
        if ((_idx < m_context.size()))
        {
            auto val = m_split_number[_idx];
            if (val)
            {
                auto idx = val - (SET::m_min / SET::m_setdenominator);
                auto auxidx = m_context[_idx];
                if (SET::m_length)
                { _resstr += SET::get_element(idx, auxidx); }
            }
        }
        return decode_set<typename SET::subset_t>(++_idx, _resstr);
    }
    void clear() { m_split_number.clear(); m_context.clear(); }

    numberlist_t m_split_number;
    std::vector<bound_t> m_context;
    template <class LOC>
    friend class converter_t;
    template <class RULE>
    void process_glrule(bound_t _val, std::wstring& _resstr)
    {
        if (RULE::check_rule(_val))
        {
            RULE::correct_text(_resstr);
        }
        using nextrule_t = typename RULE::nextrule_t;
        process_glrule<nextrule_t>(_val, _resstr);
    }
};

template <>
void converteriml_t::fill_range<terminator_t>(bound_t _val, byte_t _idx)
{ }

template <>
byte_t converteriml_t::fill_set<terminator_t>(bound_t _val, byte_t _idx)
{ return _idx; }

template <>
void converteriml_t::fill_contextrange<terminator_t>(byte_t _idx)
{ }

template <>
byte_t converteriml_t::fill_contextset<terminator_t>(byte_t _idx)
{ return _idx; }

template <>
void converteriml_t::decode_range<terminator_t>(byte_t _idx,
                                                std::wstring& _str) const
{ }

template <>
byte_t converteriml_t::decode_set<terminator_t>(byte_t _idx,
                                                std::wstring& _str) const
{ return _idx; }

template <>
void converteriml_t::process_glrule<terminator_t>(bound_t _val,
                                                  std::wstring& _resstr)
{ }

/// @class converter_t
/// @brief Преобразователь числа в текстовое предсавление
template <class LOC>
class converter_t
{
public:
    typedef typename LOC::range_uint8_t range_uint8_t;
    typedef typename LOC::range_uint16_t range_uint16_t;
    typedef typename LOC::range_uint32_t range_uint32_t;
    typedef typename LOC::globgrammar_t globgrammar_t;
    explicit converter_t() : m_impl(m_depth32)
    {     }
    converter_t(const converter_t& _obj) : m_impl(_obj.m_impl)
    {     }
    converter_t(converter_t&& _obj) : m_impl(std::move(_obj.m_impl))
    {     }
    std::wstring operator <<(input_t _val)
    {
        convert(_val);
        return m_result;
    }
private:
    void convert(input_t _val)
    {
        byte_t idx = 0;
        m_result.clear();
        if (_val)
        {
            auto val = abs(_val);
            if (val <= std::numeric_limits<unsigned char>::max())
            {
                m_impl.fill_range<range_uint8_t>(val, 0);
                m_impl.fill_contextrange<range_uint8_t>(0);
                m_impl.decode_range<range_uint8_t>(idx, m_result);
            }
            else if (val <= std::numeric_limits<unsigned short>::max())
            {
                m_impl.fill_range<range_uint16_t>(val, 0);
                m_impl.fill_contextrange<range_uint16_t>(0);
                m_impl.decode_range<range_uint16_t>(idx, m_result);
            }
            else
            {
                m_impl.fill_range<range_uint32_t>(val, 0);
                m_impl.fill_contextrange<range_uint32_t>(0);
                m_impl.decode_range<range_uint32_t>(idx, m_result);
            }
        }
        m_impl.process_glrule<globgrammar_t>(_val, m_result);
    }

    static constexpr byte_t m_depth32 = get_depthrange<range_uint32_t>(0);
    static constexpr byte_t m_depth16 = get_depthrange<range_uint16_t>(0);
    static constexpr byte_t m_depth8 = get_depthrange<range_uint8_t>(0);
    converteriml_t m_impl;
    std::wstring m_result;
};

#endif // CONVERTER_H
