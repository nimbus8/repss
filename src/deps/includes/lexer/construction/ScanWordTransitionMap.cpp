#include "ScanWordTransitionMap.hpp"
#include <iostream>

ScanWordTransitionMap* ScanWordTransitionMap::_self;

ScanWordTransitionMap* ScanWordTransitionMap::getInstance()
{
    if (_self == nullptr)
    {
        std::cout << "Self == null" << std::endl;
        _self = new ScanWordTransitionMap();
    }
    else
    {
        std::cout << "Self != null" << std::endl;
        return _self;
    }
}

int main()
{
    const auto swtm1 = ScanWordTransitionMap::getInstance();
    //const auto swtm2 = ScanWordTransitionMap::getInstance();

    std::cout << swtm1->incr() << std::endl;
    //std::cout << swtm2->incr() << std::endl; //cause exception...
    std::cout << swtm1->incr() << std::endl;   //these two dont
    //std::cout << swtm1->incr() << std::endl; //this does
    //std::cout << swtm1->incr() << std::endl; //so does this
    //all have bad output...look at this...
    return 0;
}
