flags = -Wall -std=c++11

compiler: lex.yy.c compiler.tab.o main.cc
	g++ $(flags) -o $@ $^
compiler.tab.o: compiler.tab.cc headers/*.cpp
	g++ $(flags) -c $^
compiler.tab.cc: compiler.yy
	bison -v $?
lex.yy.c: compiler.ll compiler.tab.cc
	flex compiler.ll
clean:
	rm -f compiler.tab.* lex.yy.c* *.o stack.hh tree.dot tree.pdf compiler.output