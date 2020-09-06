#include <iostream>
#include <gtest/gtest.h>
#include "../converter.h"

using namespace std;


///@class TTestBench
///@brief Class for the test of algorithm.
class TTestBench_other : public ::testing::Test
{
public:
    /// Destructor of TTestBench class
    virtual ~TTestBench_other() = default;
protected:

    void SetUp()
    {
    }
    void TearDown()  {     }
};

TEST_F(TTestBench_other, test_en)
{
    bool result = true;
    const wchar_t * str1 = L" one  billion  seven hundred and  two  million  five  thousand  one hundred and  twenty ";
    const wchar_t * str2 = L" one  million  one  thousand  nine hundred and  two ";
    const wchar_t * str3 = L" seven hundred and  two  million  three  thousand  thirteen ";
    const wchar_t * str4 = L" zero ";
    const wchar_t * str5 = L" one ";
    const wchar_t * str6 = L" minus  two hundred and  ninty  three  million  one  thousand  two hundred and  twelve ";
    const wchar_t * str7 = L" twelve  million  twelve  thousand  two hundred and  twelve ";
    const wchar_t * str8 = L" one  billion  seven hundred and  two  million  fifteen  thousand  one hundred and  twenty ";
    const wchar_t * str9 = L" two  billion  two  million  five  thousand  twenty  one ";
    const wchar_t * str10 = L" three  thousand  twenty  one ";
    const wchar_t * str11 = L" ninty  one ";
    const wchar_t * str12 = L" two  million  one  thousand  one hundred and  twenty ";
    converter_t<nslacale_en::basetype_en> cnv;
    wstring resstr;

    resstr = (cnv << 1'702'005'120);
    result &= (resstr == std::wstring(str1));

    resstr = (cnv << 1'001'902);
    result &= (resstr == std::wstring(str2));
    resstr = (cnv << 702'003'013);
    result &= (resstr == std::wstring(str3));
    resstr = (cnv << 0);
    result &= (resstr == std::wstring(str4));
    resstr = (cnv << 1);

    result &= (resstr == std::wstring(str5));
    resstr = (cnv << -293'001'212);
    result &= (resstr == std::wstring(str6));
    resstr = (cnv << 12'012'212);
    result &= (resstr == std::wstring(str7));
    resstr = (cnv << 1'702'015'120);
    result &= (resstr == std::wstring(str8));
    resstr = (cnv << 2'002'005'021);
    result &= (resstr == std::wstring(str9));
    resstr = (cnv << 3'021);
    result &= (resstr == std::wstring(str10));
    resstr = (cnv << 91);
    result &= (resstr == std::wstring(str11));
    resstr = (cnv << 2'001'120);
    result &= (resstr == std::wstring(str12));

    ASSERT_EQ(result, true);
}

TEST_F(TTestBench_other, test_ru)
{
    bool result = true;
    const wchar_t * str1 =
            L" один  миллиард  семьсот  два  миллиона  пять "
            L" тысяч  сто  двадцать ";
    const wchar_t * str2 =
            L" один  миллион  одна  тысяча  девятьсот  два ";
    const wchar_t * str3 =
            L" семьсот  два  миллиона  три  тысячи  тринадцать ";
    const wchar_t * str4 =
            L" ноль ";
    const wchar_t * str5 =
            L" один ";
    const wchar_t * str6 =
            L" минус  двести  девяносто  три  миллиона  одна  тысяча  двести "
            L" двенадцать ";
    const wchar_t * str7 =
            L" двенадцать  миллионов  двенадцать  тысяч  двести  двенадцать ";
    const wchar_t * str8 =
            L" один  миллиард  семьсот  два  миллиона  пятнадцать  тысяч  сто"
            L"  двадцать ";
    const wchar_t * str9 =
            L" два  миллиарда  два  миллиона  пять  тысяч  двадцать  один ";
    const wchar_t * str10 =
            L" три  тысячи  двадцать  один ";
    const wchar_t * str11 =
            L" девяносто  один ";
    const wchar_t * str12 =
            L" два  миллиона  одна  тысяча  сто  двадцать ";
    converter_t<nslacale_ru::basetype_ru> cnv;
    wstring resstr;

    resstr = (cnv << 1'702'005'120);
    result &= (resstr == std::wstring(str1));

    resstr = (cnv << 1'001'902);
    result &= (resstr == std::wstring(str2));

    resstr = (cnv << 702'003'013);
    result &= (resstr == std::wstring(str3));

    resstr = (cnv << 0);
    result &= (resstr == std::wstring(str4));
    resstr = (cnv << 1);
    result &= (resstr == std::wstring(str5));

    resstr = (cnv << -293'001'212);
    result &= (resstr == std::wstring(str6));

    resstr = cnv << 12'012'212;
    result &= (resstr == std::wstring(str7));
    resstr = (cnv << 1'702'015'120);
    result &= (resstr == std::wstring(str8));
    resstr = (cnv << 2'002'005'021);
    result &= (resstr == std::wstring(str9));
    resstr = (cnv << 3'021);
    result &= (resstr == std::wstring(str10));
    resstr = (cnv << 91);
    result &= (resstr == std::wstring(str11));
    resstr = (cnv << 2'001'120);
    result &= (resstr == std::wstring(str12));

    ASSERT_EQ(result, true);
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    try
    {
        ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
    }
    catch (std::out_of_range& _e)
    {
        wcerr << L"Очень большое число" << endl;
    }
    catch (std::invalid_argument& _e)
    {
        wcerr << L"Неизвестное число" << endl;
    }
    catch (econv_error_t& _e)
    {
        wcerr << L"Ошибка при локализации с диапазонами" << endl;
    }
    catch (std::logic_error& _e)
    {
        wcerr << L"Неизвестная ошибка при локализации" << endl;
    }
    catch (...)
    {
        wcerr << L"Неизвестная ошибка" << endl;
    }
    return 0;
}
