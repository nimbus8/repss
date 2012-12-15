#include "ScanWordTransitionMap.hpp"
#include <iostream>

void ScanWordTransitionMap::testMap()
{
    ScanWordTransitionMap swtm1;

    std::cout << swtm1.incr() << std::endl;
    std::cout << swtm1.incr() << std::endl;   //these two dont
    return 0;
}
