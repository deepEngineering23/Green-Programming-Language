#include <iostream>
#include <string>
#include <vector>

#include "lexertk.hpp"


int main()
{
   std::string expression = "(cos(x/pi)cos(2y) + 1) == (sin(x / pi) * cos(2 * y) + 1)";

   lexertk::generator generator;

   if (!generator.process(expression))
   {
      return 0;
   }

   lexertk::helper::dump(generator);

   return 0;
}

