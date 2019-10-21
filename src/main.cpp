#include "ip_filter.h"

int main(int argc, char const *argv[])
{
    std::istream* input = &std::cin;
    if (argc > 1)
    {
        auto inputDataPath = argv[1];
        input = new std::ifstream(inputDataPath);
    }

    try
    {
        auto ipPool = fill(input);
        std::sort(ipPool.begin(), ipPool.end(), std::greater<Ip>());

        std::cout << toString(ipPool);
        std::cout << toString(filter(ipPool, 1));
        std::cout << toString(filter(ipPool, 46, 70));
        std::cout << toString(filter_any(ipPool, 46));
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }

    if (argc > 1)
    {
        delete input;
    }
    return 0;
}