#ifndef BASE_TYPES_H
#define BASE_TYPES_H
#include <array>
#include <memory>
#include <vector>

typedef unsigned char byte_t;
typedef int input_t;
typedef std::uint32_t bound_t;
typedef std::vector<bound_t> numberlist_t;
typedef void terminator_t;

class econv_error_t
{ };

/// @class base_dict_t
/// @brief Интерфейс для словаря элементов множества.
template <byte_t lng, class T, byte_t aux = 1>
class base_dict_t
{
    base_dict_t() = delete;
    base_dict_t(const base_dict_t&) = delete;
public:
    static constexpr byte_t  m_length = lng;
    static constexpr byte_t  m_auxlength = aux;
    // Двумерный массив для очень слоных случаев
    using dict_t = typename std::array<const wchar_t*, m_length> ;
    using dictaux_t = typename std::array<dict_t, m_auxlength> ;
    static std::wstring get_element(byte_t _idx, byte_t _aux)
    {
        if ((_idx > lng) || (_aux > aux)) throw econv_error_t();
        return T::m_dict[_aux][_idx];
    }

};

/// @class base_dictdef_t
/// @brief Словарь по умолчанию для элементов множества.
class base_dictdef_t: public base_dict_t<0, base_dictdef_t>
{
public:
    static constexpr dictaux_t m_dict =
    {{     }};
};

/// @class base_dict_t
/// @brief Интерфейс для списка префиксов/постфиксов.
template <byte_t lng, class T>
class base_dictpp_t
{
    base_dictpp_t() = delete;
    base_dictpp_t(const base_dictpp_t&) = delete;
public:
    static constexpr byte_t  m_pplength = lng;
    using dict_t = typename std::array<const wchar_t*, m_pplength> ;
    static std::wstring get_ppelement(byte_t _idx)
    {
        if (_idx > lng) throw econv_error_t();
        return T::m_dictpostfix[_idx];
    }
};

/// @class base_dictpostfixe_t
/// @brief Список по умолчанию для префиксов/постфиксов.
class base_dictpostfixe_t: public base_dictpp_t<0, base_dictpostfixe_t>
{
public:
    static constexpr dict_t m_dictpostfix =
    {     };
};

struct localgre_t
{
    static byte_t _get_auxdictindex(byte_t _idx, numberlist_t& _nlist)
    { return 0; }
};


struct rangedef_t;

/// @class range_t
/// @brief Класс (статического) интерфеса диапазона.
template <bound_t _min,
          bound_t _max,
          bound_t _denom,
          class SUBSET,
          class SUBRANGE,
          class LOCGRAMMAR = localgre_t>
class range_t
{
    range_t() = delete;
    range_t(const range_t&) = delete;
public:
    /// Класс подмножества, т.е. той части числа,
    /// имеющих отношение к текущему диапазону,
    /// но не к его поддиапазону
    typedef SUBSET subset_t;
    /// Класс поддиапазона
    typedef SUBRANGE subrange_t;
    /// Минимальное значение деноминатора
    static constexpr bound_t m_mindenom = 1;
    /// Минимальное значение диапазона
    static constexpr bound_t m_min = _min;
    /// Максимальное значение диапазона
    static constexpr bound_t m_max = _max;
    /// Делитель для диапазона
    static constexpr bound_t m_denominator =
            std::is_void<SUBRANGE>::value?m_mindenom:_denom;
    /// Делитель для диапазона
    static constexpr bound_t m_setdenominator =
            std::is_void<SUBSET>::value?m_mindenom:_denom;
    static byte_t get_auxdictindex(byte_t _idx, numberlist_t& _nlist)
    { return LOCGRAMMAR::_get_auxdictindex(_idx, _nlist); }
};

class globgrammar_def;

/// @class global_grammarrules
/// @brief Интерфейс для глобальных правил.
template<class T, class NEXT>
class global_grammarrules
{
    global_grammarrules() = delete;
    global_grammarrules(const global_grammarrules&) = delete;
public:
    /// Максимальное число правил граматики
    static constexpr byte_t maxrulecnt = T::_maxrulecnt;
    typedef NEXT nextrule_t;
    static constexpr bool check_rule(input_t _val)
    {
        return T::_check_rule(_val);
    }
    static void correct_text(std::wstring& _resstr)
    {
        T::_correct_text(_resstr);
    }
};

/// @class basetype
/// @brief Интерфейс таблицы соответствия типов.
template<class _globgrammar_t = globgrammar_def,
         class _range_uint32_t = rangedef_t,
         class _range_uint16_t = rangedef_t,
         class _range_uint8_t = rangedef_t>
class basetype
{
public:
    basetype() = delete;
    basetype(const basetype&) = delete;
    using globgrammar_t = _globgrammar_t;
    typedef  _range_uint32_t range_uint32_t;
    typedef  _range_uint16_t range_uint16_t;
    typedef  _range_uint8_t range_uint8_t;
};

#endif // BASE_TYPES_H
