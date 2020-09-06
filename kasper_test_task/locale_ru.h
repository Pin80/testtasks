#ifndef LOCALE_RU_H
#define LOCALE_RU_H
#include "base_types.h"

namespace nslacale_ru
{

// ========== Словари элементов множества ==========
class base_dict0_t: public base_dict_t<1, base_dict0_t>
{
public:
    static constexpr dictaux_t m_dict =
    {{
        L" ноль "
    }};
};

class base_dict19m_t: public base_dict_t<19, base_dict19m_t>
{
public:
    static constexpr dictaux_t m_dict =
    {{
        L" один ", L" два ", L" три ", L" четыре ", L" пять ", L" шесть ",
        L" семь ", L" восемь ", L" девять ", L" десять ", L" одиннадцать ",
        L" двенадцать ", L" тринадцать ", L" четырнадцать ", L" пятнадцать ",
        L" шестнадцать ", L" семнадцать ", L" восемнадцать ", L" девятнадцать "
    }};
};

class base_dict19f_t: public base_dict_t<19, base_dict19f_t>
{
public:
    static constexpr dictaux_t m_dict =
    {{
        L" одна ", L" две ", L" три ", L" четыре ", L" пять ", L" шесть ",
        L" семь ", L" восемь ", L" девять ", L" десять ", L" одиннадцать ",
        L" двенадцать ", L" тринадцать ", L" четырнадцать ", L" пятнадцать ",
        L" шестнадцать ", L" семнадцать ", L" восемнадцать ", L" девятнадцать "
    }};
};

class base_dict90_t: public base_dict_t<8, base_dict90_t>
{
public:
    static constexpr dictaux_t m_dict =
    {{
        L" двадцать ", L" тридцать ", L" сорок ", L" пятьдесят ",
        L" шестьдесят ", L" семьдесят ", L" восемьдесят ", L" девяносто "
    }};
};

class base_dict900_t: public base_dict_t<10, base_dict900_t>
{
public:
    static constexpr dictaux_t m_dict =
    {{
        L" сто ", L" двести ", L" триста ", L" четыреста ", L" пятьсот ",
        L" шестьсот ", L" семьсот ", L" восемьсот ", L" девятьсот "
     }};
};

// ========== Списки вариантов префиксов/постфиксов ==========

struct base_dictpostfix3_t: public base_dictpp_t<3, base_dictpostfix3_t>
{
    static constexpr dict_t m_dictpostfix =
    {
        L" тысяча ", L" тысячи ", L" тысяч "
    };
};

struct base_dictpostfix6_t: public base_dictpp_t<3, base_dictpostfix6_t>
{
    static constexpr dict_t m_dictpostfix =
    {
        L" миллион ", L" миллиона ", L" миллионов "
    };
};

struct base_dictpostfix9_t: public base_dictpp_t<3, base_dictpostfix9_t>
{
    static constexpr dict_t m_dictpostfix =
    {
        L" миллиард ", L" миллиарда ", L" миллиардов "
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

struct range999_t: public range_t<100, 999, 100, range99_t, terminator_t>,
                   public base_dict900_t,
                   public base_dictpostfixe_t
{ };

struct range19f_t: public range_t<1, 19, 1, terminator_t, terminator_t>,
                   public base_dict19f_t,
                   public base_dictpostfixe_t
{ };

struct range99f_t: public range_t<20, 99, 10, range19f_t, terminator_t>,
                   public base_dict90_t,
                   public base_dictpostfixe_t
{ };

struct range999f_t: public range_t<100, 999, 100, range99f_t, terminator_t>,
                    public base_dict900_t,
                    public base_dictpostfixe_t
{ };

struct rangepow3_t: public range_t<1'000,
                                   999'999,
                                   1'000,
                                   range999f_t,
                                   range999_t,
                                   rangepow3_t>,
                    public base_dict0_t,
                    public base_dictpostfix3_t
{
    static byte_t _get_auxdictindex(byte_t _idx, numberlist_t& _nlist)
    {
        if ((_nlist[_idx] <= 1) ) return 0;
        if (_nlist[_idx + 3] < 5) return 1;
        return 2;
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
        if ((_nlist[_idx] <= 1) ) return 0;
        if ((_nlist[_idx + 3] < 5)) return 1;
        return 2;
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
        if ((_nlist[_idx] <= 1) ) return 0;
        if ((_nlist[_idx + 3] < 5)) return 1;
        return 2;
    }
};


struct rule0 : public global_grammarrules<rule0, terminator_t>
{
    static constexpr bool _check_rule(input_t _val)
    {
        return (_val == 0);
    }
    static void _correct_text(std::wstring& _resstr)
    {
        const wchar_t* dst_str = L" ноль ";
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
        const wchar_t* dst_str = L" минус ";
        _resstr = dst_str + _resstr;
    }
};

// Таблица соответсвия типов
struct basetype_ru : public basetype<nslacale_ru::rule1, // globgrammar_t
                                     nslacale_ru::rangepow9_t, // range_uint32_t
                                     nslacale_ru::rangepow3_t, // range_uint16_t
                                     nslacale_ru::range999_t> // range_uint8_t
{ };

}
#endif // LOCALE_RU_H
