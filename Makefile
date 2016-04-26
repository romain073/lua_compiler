compiler: lex.yy.c compiler.tab.o main.cc
	g++ -Wall -g -std=c++11 -DYYDEBUG=1 -o compiler compiler.tab.o lex.yy.c main.cc
compiler.tab.o: compiler.tab.cc headers/*.cpp
	g++ -Wall -g -std=c++11 -c compiler.tab.cc headers/*.cpp
compiler.tab.cc: compiler.yy
	bison -v compiler.yy
lex.yy.c: compiler.ll compiler.tab.cc
	flex compiler.ll
clean:
	rm -f compiler.tab.* lex.yy.c* compiler