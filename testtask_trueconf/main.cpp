/*
Задание именно на знание С++, качество важно, лучше позже, чем хуже.
Приветствуется использование современных стандартов С++
Результат должен быть в виде проекта, компилироваться и работать.
Необходимо:
1.заполнить случайными числами от 1 до 9 значения контейнеров vector[i] и map[i];
2.удалить случайное число элементов (не более 15) в каждом контейнере;
3.после этого провести синхронизацию,
чтобы в vector и map остались только имеющиеся в обоих контейнерах элементы
(дубликаты не удалять).
*/
#include <iostream>
#include "container.h"

typedef Icontainer_t::idx_t idx_t;
typedef std::pair<idx_t, idx_t> pairsize_t;

pairsize_t parse_inputstr(const std::string& _str)
{
    std::string tmpstr;
    std::istringstream istm(_str);
    pairsize_t result;
    if (std::getline(istm, tmpstr, ' '))
    {
        auto item = std::stoi(tmpstr);
        result.first = item;
        if (std::getline(istm, tmpstr, ' '))
        {
            item = std::stoi(tmpstr);
            result.second = item;
        }
    }
    return result;
}


void show_cont(Icontainer_t& _cont)
{
    using std::cout;
    using std::cerr;
    using std::cin;
    using std::endl;
    cout << "name:" << _cont.getName() << endl;
    cout << "max index:" << int(_cont.max()) << endl;
    cout << "histogram:" << _cont.getHistReport() << endl;
    if (_cont.max() == Icontainer_t::m_emtyidx) return;
    for(idx_t i = 0; i <= _cont.max(); i++)
    {
        if ((i % 10) == 0) { cout <<  endl;}
        if (_cont.isIndexFound(i))
        {
            int val = _cont[i];
            cout << "[" << i << "]:" << val << " ";
        }
    }
    cout <<  endl;
}

idx_t getRandom(idx_t _to)
{
    typedef std::mt19937 rnggen_t;
    std::random_device rd;
    rnggen_t rnggen(rd());
    std::uniform_int_distribution<idx_t> dist(3, _to);
    return dist(rnggen);
}

bool compare_histcont(Icontainer_t& _cont1, Icontainer_t& _cont2)
{
    Icontainer_t::histogram_t hist1, hist2;
    _cont1.fillHistogram(hist1);
    _cont2.fillHistogram(hist2);
    for (auto i = 1; i <= Icontainer_t::m_numbers; i++)
    {
        bool result = ((hist1[i - 1] == 0) ^ (hist2[i - 1] == 0));
        if (result) return false;
    }
    return  true;
}

bool compare_cont(Icontainer_t& _cont1, Icontainer_t& _cont2)
{
    if (_cont1.max() != _cont2.max()) return false;
    for(idx_t i = 0; i <= _cont1.max(); i++)
    {
        if (!(_cont1.isIndexFound(i)) && !(_cont2.isIndexFound(i))) continue;
        if ((_cont1.isIndexFound(i)) && (_cont2.isIndexFound(i)))
        {
            if (_cont1[i] != _cont2[i]) { return false; }
        }
        else
        {
            return false;
        }
    }
    return true;
}


void manual_input()
{
    using std::cout;
    using std::cerr;
    using std::cin;
    using std::endl;
    std::string tmpstr;
    cout << "Enter size1 and size2 of each container:";
    std::getline(cin, tmpstr);
    auto sizes = parse_inputstr(tmpstr);
    cout << "size1:" << sizes.first << " ";
    cout << "size2:" << sizes.second << endl;
    vcontainer_t vcont(sizes.first);
    show_cont(vcont);
    mcontainer_t mcont(sizes.second);
    show_cont(mcont);
    idx_t rem = getRandom(vcont.max());
    rem = (rem % 16)?(rem % 16):1;
    cout << "vcont removed:" << int(rem) << endl;
    for (idx_t i = 0; i < rem; i++)
    { vcont.removeRandom(); }
    rem = getRandom(mcont.max());
    rem = (rem % 16)?(rem % 16):1;
    cout << "mcont removed:" << int(rem) << endl;
    for (idx_t i = 0; i < rem; i++)
    { mcont.removeRandom(); }
    show_cont(vcont);
    show_cont(mcont);

    cout << "removeMismatched(vcont) call" << endl;
    vcont.removeMismatched(mcont);
    show_cont(vcont);
    bool cmpres = compare_histcont(vcont, mcont);
    cout << "matched:" << cmpres << endl;
    cout << "removeMismatched(mcont) call" << endl;
    mcont.removeMismatched(vcont);
    show_cont(mcont);
    cmpres = compare_histcont(vcont, mcont);
    cout << "matched:" << cmpres << endl;
}

void automatic_test1()
{
    using std::cout;
    using std::cerr;
    using std::cin;
    using std::endl;
    std::string tmpstr;
    cout << "vcont1" << endl;
    vcontainer_t vcont1 = {3, 3, 4, 6, 1, 8, 7, 2, 3, 1, 9};
    show_cont(vcont1);
    cout << "vcont2" << endl;
    vcontainer_t vcont2 = {1, 3, 4, 5, 1, 8, 3, 2, 3, 1, 6};
    show_cont(vcont2);
    cout << "removeMismatched(vcont1) call" << endl;
    vcont1.removeMismatched(vcont2);
    show_cont(vcont1);
    bool cmpres = compare_histcont(vcont1, vcont2);
    cout << "matched:" << cmpres << endl;
    cout << "removeMismatched(vcont2) call" << endl;
    vcont2.removeMismatched(vcont1);
    show_cont(vcont2);
    cmpres = compare_histcont(vcont1, vcont2);
    cout << "matched:" << cmpres << endl;
}

void automatic_test2()
{
    using std::cout;
    using std::cerr;
    using std::cin;
    using std::endl;
    std::string tmpstr;
    cout << "mcont1" << endl;
    mcontainer_t mcont1 = {3, 3, 4, 6, 1, 8, 7, 2, 3, 1, 9};
    show_cont(mcont1);
    cout << "mcont2" << endl;
    mcontainer_t mcont2 = {1, 3, 4, 5, 1, 8, 3, 2, 3, 1, 6};
    show_cont(mcont2);
    cout << "removeMismatched(mcont1) call" << endl;
    mcont1.removeMismatched(mcont2);
    show_cont(mcont1);
    bool cmpres = compare_histcont(mcont1, mcont2);
    cout << "matched:" << cmpres << endl;
    cout << "removeMismatched(mcont2) call" << endl;
    mcont2.removeMismatched(mcont1);
    show_cont(mcont2);
    cmpres = compare_histcont(mcont1, mcont2);
    cout << "matched:" << cmpres << endl;
}


void automatic_test3()
{
    using std::cout;
    using std::cerr;
    using std::cin;
    using std::endl;
    std::string tmpstr;

    vcontainer_t vcont = {3, 3, 4, 6, 1, 8, 3, 2, 3, 1, 9};
    show_cont(vcont);
    mcontainer_t mcont = {1, 3, 4, 5, 1, 8, 3, 2, 3, 1, 6};
    show_cont(mcont);
    idx_t rem = 5;
    cout << "vcont removed:" << int(rem) << endl;
    for (idx_t i = 0; i < rem; i++)
    { vcont.removeRandom(); }
    cout << "mcont removed:" << int(rem) << endl;
    for (idx_t i = 0; i < rem; i++)
    { mcont.removeRandom(); }
    show_cont(vcont);
    show_cont(mcont);
}


int main(int argc, char *argv[])
{
    const char * FUNCTION = __FUNCTION__;
    using std::cout;
    using std::cerr;
    using std::cin;
    using std::endl;
    try
    {
        if ((argc <= 1) || (argv[argc - 1] == nullptr))
        {
          cout << "no arguments found:" << endl;
          return EXIT_SUCCESS;
        }
        std::string arg1 = argv[argc - 1];
        switch (arg1[0])
        {
            case '0':
            {
                cout << "choosen:0" << endl;
                manual_input();
                break;
            }
            case '1':
            {
                cout << "choosen:1" << endl;
                automatic_test1();
                break;
            }
            case '2':
            {
                cout << "choosen:2" << endl;
                automatic_test2();
                break;
            }
            case '3':
            {
                cout << "choosen:3" << endl;
                automatic_test3();
                break;
            }
            default:
            {
                cout << "choosen:unknown" << endl;
                break;
            }
        }
    }
    catch (const std::invalid_argument&)
    {
        cerr << "Conversion error, maybe input data is not correct." << endl;
        return EXIT_FAILURE;
    }
    catch (...)
    {
        cerr << "Error in:" << FUNCTION << endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
