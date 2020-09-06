#ifndef LOCALE_EN_H
#define LOCALE_EN_H
#include "base_types.h"

namespace nslacale_en
{

// ========== Словари элементов множества ==========
class base_dict0_t: public base_dict_t<1, base_dict0_t>
{
public:
    static constexpr dictaux_t m_dict =
    {{
        L" zero "
    }};
};

class base_dict19m_t: public base_dict_t<19, base_dict19m_t>
{
public:
    static constexpr dictaux_t m_dict =
    {
        L" one ", L" two ", L" three ", L" four ", L" five ", L" six ",
        L" seven ", L" eight ", L" nine ", L" ten ", L" eleven ",
        L" twelve ", L" thirteen ", L" fourteen ", L" fifteen ",
        L" sixteen ", L" seventeen ", L" eighteen ", L" nineteen "
     };
};


class base_dict90_t: public base_dict_t<8, base_dict90_t>
{
public:
    static constexpr dictaux_t m_dict =
    {
        L" twenty ", L" thirty ", L" fourty ", L" fifty ",
        L" sixty ", L" seventy ", L" eighty ", L" ninty "
    };
};

class base_dict900_t: public base_dict_t<10, base_dict900_t, 2>
{
public:
    static constexpr dictaux_t m_dict =
    {{
        {
        L" one hundred ", L" two hundred ", L" three hundred ", L" four hundred ",
        L" five hundred", L" six hundred ", L" seven hundred ",
        L" eight hundred ", L" nine hundred"
        },
        {
         L" one hundred and ", L" two hundred and ", L" three hundred and ",
         L" four hundred and ", L" five hundred and ", L" six hundred and ",
         L" seven hundred and ", L" eight hundred and ", L" nine hundred and "
        }
     }};
};

// ========== Списки вариантов префиксов/постфиксов ==========

struct base_dictpostfix3_t: public base_dictpp_t<3, base_dictpostfix3_t>
{
    static constexpr dict_t m_dictpostfix =
    {
        L" thousand "
    };
};

struct base_dictpostfix6_t: public base_dictpp_t<3, base_dictpostfix6_t>
{
    static constexpr dict_t m_dictpostfix =
    {
        L" million "
    };
};

struct base_dictpostfix9_t: public base_dictpp_t<3, base_dictpostfix9_t>
{
    static constexpr dict_t m_dictpostfix =
    {
        L" billion "
    };
};

// ========== Диапазоны ==========
/// Класс нулевого диапазона
struct range0_t: public range_t<0, 0, 1, terminator_t, terminator_t>,
                 public base_dict0_t
{ };

struct range19_t: public range_t<1, 19, 1, terminator_t, terminator_t>,
                  public base_dict19m_t,
                  public base_dictpostfixe_t
{ };

struct range99_t: public range_t<20, 99, 10, range19_t, terminator_t>,
                  public base_dict90_t,
                  public base_dictpostfixe_t
{ };

struct range999_t: public range_t<100,
                                  999,
                                  100,
                                  range99_t,
                                  terminator_t,
                                  range999_t>,
                   public base_dict900_t,
                   public base_dictpostfixe_t
{
    static byte_t _get_auxdictindex(byte_t _idx, numberlist_t& _nlist)
    {
        if ((_nlist[_idx + 1] != 0) | (_nlist[_idx + 2] != 0)) return 1;
        return 0;
    }
};


struct rangepow3_t: public range_t<1'000,
                                   999'999,
                                   1'000,
                                   range999_t,
                                   range999_t,
                                   rangepow3_t>,
                    public base_dict0_t,
                    public base_dictpostfix3_t
{
    static byte_t _get_auxdictindex(byte_t _idx, numberlist_t& _nlist)
    {
        return 0;
    }
};

struct rangepow6_t: public range_t<1'000'000,
                                   999'999'999,
                                   1'000'000,
                                   range999_t,
                                   rangepow3_t,
                                   rangepow6_t>,
                    public base_dict0_t,
                    public base_dictpostfix6_t
{
    static byte_t _get_auxdictindex(byte_t _idx, numberlist_t& _nlist)
    {
        return 0;
    }
};

struct rangepow9_t: public range_t<1'000'000'000,
                                   std::numeric_limits<bound_t>::max(),
                                   1'000'000'000,
                                   range999_t,
                                   rangepow6_t,
                                   rangepow9_t>,
                    public base_dict0_t,
                    public base_dictpostfix9_t
{
    static byte_t _get_auxdictindex(byte_t _idx, numberlist_t& _nlist)
    {
        return 0;
    }
};

// Глобальные правила или/и глабальная корректировка

struct rule0 : public global_grammarrules<rule0, terminator_t>
{
    static constexpr bool _check_rule(input_t _val)
    {
        return (_val == 0);
    }
    static void _correct_text(std::wstring& _resstr)
    {
        const wchar_t* dst_str = L" zero ";
        _resstr = dst_str;
    }
};

struct rule1 : public global_grammarrules<rule1, rule0>
{
    static constexpr bool _check_rule(input_t _val)
    {
        return (_val < 0);
    }
    static void _correct_text(std::wstring& _resstr)
    {
        const wchar_t* dst_str = L" minus ";
        _resstr = dst_str + _resstr;
    }
};

// Таблица соответсвия типов
struct basetype_en : public basetype<nslacale_en::rule1, // globgrammar_t
                                     nslacale_en::rangepow9_t, // range_uint32_t
                                     nslacale_en::rangepow3_t, // range_uint16_t
                                     nslacale_en::range999_t> // range_uint8_t
{ };

}
#endif // LOCALE_EN_H
