/*
 REPSS
 Copyright (C) 2012  Khalique Williams

 This file is part of REPSS.

 REPSS is free software: you can redistribute it and/or modify
 it under the terms of the   GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 REPSS is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with REPSS.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _STOPWATCH_
#define _STOPWATCH_

#include <time.h>

class StopWatch
{
private:
    clock_t startTime;
public:
    StopWatch() {}
    ~StopWatch() {}

    void start() //also acts as reset and start
    {
        startTime = clock();
        std::cout << startTime << std::endl;

    }

    float getElapsed() //can be called multiple of times
    {
        auto currentTime = clock();
        auto diff = currentTime - startTime;

        auto millisecs = (float)diff * (1000.0f/CLOCKS_PER_SEC);
        return millisecs;
    }

    void getElapsedAndPrintfd(const char* const message)
    {
       auto elapsed = getElapsed();
       printf(message, elapsed);
    }
};

#endif
