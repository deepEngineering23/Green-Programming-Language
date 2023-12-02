
COMPILER         = -c++
OPTIMIZATION_OPT = -O2
BASE_OPTIONS     = -ansi -pedantic-errors -Wall -Wextra -Werror -Wno-long-long
OPTIONS          = $(BASE_OPTIONS) $(OPTIMIZATION_OPT) -o
LINKER_OPT       = -L/usr/lib -lstdc++

BUILD_LIST+=lexer

#all: $(BUILD_LIST)

lexer: lexer.cpp lexertk.hpp
	$(COMPILER) $(OPTIONS) lexer lexer.cpp $(LINKER_OPT)

#strip_bin:
#	strip -s lexer

#valgrind_check:
#	valgrind --leak-check=full --show-reachable=yes --track-origins=yes --log-file=lexer.log -v ./lexer

#clean:
#	rm -f core.* *~ *.o *.bak *stackdump gmon.out *.gcda *.gcno *.gcnor *.gch