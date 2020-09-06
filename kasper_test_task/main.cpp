#include <iostream>
#include "converter.h"

using namespace std;

int main(int argc, char *argv[])
{
    setlocale(LC_ALL,"");
    try
    {
        if (argc > 1)
        {
           wcout << L"Hello user!" << endl;
            std::string strnum;
            int num = 0;
            if (argv[1])
            {
                strnum = argv[1];
                num = std::stoi(strnum);
                converter_t<nslacale_en::basetype_en> cnv;
                auto strtxtnum = cnv << num;
                wcout << strtxtnum << endl;
            }
        }
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
