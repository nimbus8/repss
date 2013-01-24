/*
 REPSS
 Copyright (C) 2013  Khalique Williams

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

 If you wish to contact the author of REPSS, you may do so at
 kwillia.pub@gmail.com
 */  

#include <stdio.h>
#include <algorithm>
#include <iostream>

//Some very useful concepts we have to integrate into the project at some point

#define positive_integer unsigned int

void stuffToDoAtExit(void)
{
   puts("We've exited successfully (without exception)!");
}

void moreStuffToDoAtExit(void)
{
    puts("We've done even more, before we've exited (hopefully, without exception)!");
}

std::function<int(positive_integer)> makeFun(const positive_integer numberGreaterThanZero)
{
    auto leapingFun = [numberGreaterThanZero](int someOtherNumber)->int
      {
          return numberGreaterThanZero + someOtherNumber;
      };

    return leapingFun;
}

auto theLengthOf(const std::string something)->positive_integer
{
    return something.size();
}

int main(int argc, char* yourArgument[])
{
    atexit(moreStuffToDoAtExit);
    atexit(stuffToDoAtExit);

    if (argc < 2)
    {
        perror("You must have at least one argument, if not we'll raise exception");
        throw std::exception();
    }

    const auto magically = makeFun(theLengthOf(yourArgument[1]));
    const auto thisNumberShouldBeNegative = -10;

    if (magically(thisNumberShouldBeNegative) == true)
    {
        //This would be, of course, a positive
        printf("Fun is always greater that the absolute of it!\n");
    }
    else if (!magically(thisNumberShouldBeNegative))
    {
        printf("Fun's boundaries are set, be we still don't know (just) where were are yet.\n");
    }

    return 0;
}
