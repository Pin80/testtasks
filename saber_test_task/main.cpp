#include <iostream>
#include <string>
#include <type_traits>
#include <limits>
#include "custom_list.h"
using namespace std;

//Напишите функцию, которая принимает на вход знаковое целое число
//и печатает его двоичное представление, не используя библиотечных классов
//или функций.
template<typename T1>
std::string toBinaryRepr(T1 _i)
{
    static constexpr auto BITS = sizeof (T1)* 8;
    static_assert(std::is_integral<T1>::value, "Type is not integral");
    static_assert(std::is_signed<T1>::value, "Type is not signed");
    std::size_t idxbit = BITS - 1;
    T1 mask1 = static_cast<T1>(0x01 << (BITS - 1));
    char result[BITS + 1];
    // remove leading zero`s
    while (!(mask1 & _i) && (~mask1))
    {
        mask1 >>= 1; idxbit--;
    }
    const auto c_idxbit = idxbit;
    for(std::size_t idxchar = 0; idxchar <= c_idxbit; idxchar++)
    {
        result[idxchar] = ((1 << idxbit--) & (_i))?'1':'0';
    }
    result[c_idxbit + 1] = 0;
    return result;
}

// Напишите функцию, удаляющую последовательно дублирующиеся символы в строке:
void RemoveDups(char* str)
{
    std::size_t idx_new = 0;
    std::size_t idx_old = 0;
    char old_char = 0;
    while(str[idx_old])
    {
        if (old_char != str[idx_old])
        {
            str[idx_new] = str[idx_old];
            idx_new++;
        }
        old_char = str[idx_old];
        idx_old++;
    }
    str[idx_new] = 0;
}



void test_binary()
{

    auto str = toBinaryRepr<char>(-35);
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr<short>(-35);
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr(-35);
    std::cout << "result:" << str << std::endl;

    // str = toBinaryRepr<double>(-35); // error
    // str = toBinaryRepr<unsigned>(35); // error

    str = toBinaryRepr<char>(35);
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr<short>(35);
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr(0x55AA23);
    std::cout << "result:" << str << std::endl;

    str = toBinaryRepr<char>(0);
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr<short>(0);
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr(0);
    std::cout << "result:" << str << std::endl;

    str = toBinaryRepr<char>(std::numeric_limits<char>::min());
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr<short>(std::numeric_limits<short>::min());
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr(std::numeric_limits<int>::min());
    std::cout << "result:" << str << std::endl;

    str = toBinaryRepr<char>(std::numeric_limits<char>::max());
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr<short>(std::numeric_limits<short>::max());
    std::cout << "result:" << str << std::endl;
    str = toBinaryRepr(std::numeric_limits<int>::max());
    std::cout << "result:" << str << std::endl;
}

void test_remdups()
{
    // пример использования
    char data1[] = "AAA BBB  AAA";
    std::cout << "source:" << data1;
    RemoveDups(data1);
    std::cout << " result:" << data1 << std::endl; // “A B A”
    char data2[] = "";
    std::cout << "source:" << data2;
    RemoveDups(data2);
    std::cout << " result:" << data2 << std::endl; // “”
}

void test_list()
{
    try
    {

        List l = {
            "qwerty", "1234567890","", "Hello World!", "zxcvbnm", "asdfghj"
        };
        FILE * file = fopen("dumplist.bin","wb");
        l.Serialize(file);
        fclose(file);

        FILE * file2 = fopen("dumplist.bin","rb");
        List l2;
        l2.Deserialize(file);
        std::cout << l2.GetText();
        fclose(file2);

        FILE * file3 = fopen("empty.bin","wb");
        fclose(file3);

        FILE * file4 = fopen("wrong.bin","wb");
        char arr[] = "1234";
        fwrite(arr, 4, 1, file4);
        fclose(file4);

        FILE * file5 = fopen("empty.bin","wb");
        List l5;
        l5.Deserialize(file5);
        std::cout << l5.GetText();
        fclose(file5);

        FILE * file6 = fopen("wrong.bin","rb");
        List l6;
        // l6.Deserialize(file6); // raise exception
        std::cout << l6.GetText();
        fclose(file6);
    }
    catch (std::runtime_error& _e)
    {
        std::cerr << "error:" << _e.what();
    }
    catch (...)
    {
        std::cerr << "unknow error!";
    }
}

int main()
{
    cout << "Hello Saber!" << endl;
    //test_binary();
    //test_remdups();
    test_list();
    return 0;
}
