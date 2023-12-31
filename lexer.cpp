#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "lexertk.hpp"

using namespace std;

struct function_definition
{
   std::string name;
   std::string body;
   std::vector<std::string> var_list;

   void clear()
   {
      name    .clear();
      body    .clear();
      var_list.clear();
   }
};

struct parse_function_definition_impl : public lexertk::parser_helper
{
   /*
      Structure: function <name> (v0,v1,...,vn) { expression }
   */
   bool process(std::string& func_def, function_definition& fd)
   {
      if (!init(func_def))
         return false;

      if (!token_is(token_t::e_symbol,"function"))
         return false;

      if (!token_is_then_assign(token_t::e_symbol,fd.name))
         return false;

      if (!token_is(token_t::e_lbracket))
         return false;

      if (!token_is(token_t::e_rbracket))
      {
         std::vector<std::string> var_list;

         for ( ; ; )
         {
            // (x,y,z,....w)
            if (!token_is_then_assign(token_t::e_symbol,var_list))
               return false;

            if (token_is(token_t::e_rbracket))
               break;

            if (!token_is(token_t::e_comma))
               return false;
         }

         var_list.swap(fd.var_list);
      }

      std::size_t body_begin = current_token().position;
      std::size_t body_end   = current_token().position;

      int bracket_stack = 0;

      if (!token_is(token_t::e_lcrlbracket,e_hold))
         return false;

      for ( ; ; )
      {
         body_end = current_token().position;

         if (token_is(token_t::e_lcrlbracket))
            bracket_stack++;
         else if (token_is(token_t::e_rcrlbracket))
         {
            if (0 == --bracket_stack)
               break;
         }
         else
         {
            if (lexer().finished())
               return false;

            next_token();
         }
      }

      std::size_t size = body_end - body_begin + 1;

      fd.body = func_def.substr(body_begin,size);

      const std::size_t index = body_begin + size;

      if (index < func_def.size())
         func_def = func_def.substr(index,func_def.size() - index);
      else
         func_def = "";

      return true;
   }
};

bool parse_function_definition(std::string& func_def, function_definition& fd)
{
   parse_function_definition_impl parser;
   return parser.process(func_def,fd);
}
void parsingBody(string expression)
{
   lexertk::generator generator;

   if (!generator.process(expression))
   {
      std::cout << " Failed to lex: " << expression << std::endl;
      return;
   }

   lexertk::helper::bracket_checker bc;
   bc.process(generator);

   if (!bc.result())
   {
      std::cout << "Failed Bracket Checker!" << std::endl;
      return;
   }

   std::cout << "***** actual expression parsing*****" << std::endl;
   lexertk::helper::dump(generator);
   std::cout << "*********************" << std::endl;
}

char readCharIgnoringEscapeSequences(ifstream& file) {
  char c = file.get();

  if (c == '\\') {
  
    char nextChar = file.peek();
    if (nextChar == '\\' || nextChar == '"' || nextChar == '\'' || nextChar == 'n' || nextChar == 'r' || nextChar == 't') {
     
      c = file.get();
    }
  }

  return c;
}

void fileString(char * filename)
{  

   std::ifstream file(filename);
   string f;

   if (file.is_open()) {

      char c;
      while ((c = readCharIgnoringEscapeSequences(file)) != EOF) {
         f += c;
      }

      file.close();
   } else {
      std::cerr << "Error: Could not open file" << std::endl;
      return ;
   }

   /*const std::string f1 =
      "function foo0( ) { if (x < '}}}') { x+y; x+=1;} else {x;} }          "
      "function foo1(x) { if (x < '}}}') { x+y; x+=1;} else {x;} }          "
      "function foo2(x,y) { if (x < '}}}') { x+y; x+=1;} else {x;} }        "
      "function foo3(x,y,z) { if (x < '}}}') { x+y; x+=1;} else {x;} }      "
      "function foo4(x,y,z,w) { if (x < '}}}') { x+y; w+=1;} else {x; w;} } "
      "function foo5( ) { if (x < '}}}') { x+y; x+=1;} else {x;} }          "
      "function foo6(x) { if (x < '}}}') { x+y; x+=1;} else {x;} }          "
      "function foo7(x,y) { if (x < '}}}') { x+y; x+=1;} else {x;} }        "
      "function foo8(x,y,z) { if (x < '}}}') { x+y; x+=1;} else {x;} }      "
      "function foo9(x,y,z,w) { if (x < '}}}') { x+y; w+=1;} else {x; w;} } "
      "function foo10(x,y,z,w) { if (x < '\\'}\\'}\\'}\\'') { x+y; w+=1;} else {x; w;} } "
      "function foox( )       {  } "
      "function fooy(x)       {  } "
      "function fooz(x,y)     {  } "
      "function foow(x,y,z)   {  } "
      "function foou(x,y,z,w) {  } "
      "function main() { int c=0;c=c+1;print(c);}"
      "{xxx + yyy + zzz {k / l} }  ";*/

   function_definition fd;

   std::string residual = f;

   std::cout << "***** actual function parsing *****" << std::endl;

   int function_count = 0;

   do
   {
      if (parse_function_definition(residual,fd))
      {
         std::string vars;

         for (std::size_t i = 0; i < fd.var_list.size(); ++i)
         {
            vars += fd.var_list[i] + ((i < fd.var_list.size() - 1) ? "," : "");
         }

         printf("Function[%02d]\n",function_count++);
         printf("Name: %s\n"      ,fd.name.c_str() );
         printf("Vars: (%s)\n"    ,vars.c_str()    );
         printf("Body: \n%s\n"    ,fd.body.c_str() );
         parsingBody(fd.body.c_str());
         printf("-----------------------------\n\n");

         fd.clear();
      }
      else
         break;
   }
   while (!residual.empty());

   if (!residual.empty())
   {
      printf("Residual: %s\n",residual.c_str());
   }

   std::cout << "*********************" << std::endl;
}


int main(int argc, char *argv[])
{
   int n = argc;
   char *filename = argv[1];

   cout<<n<<"\n";

   fileString(filename);

   return 0;
}

