all: spawn

spawn: lex.l yac.y
	bison -d yac.y
	flex lex.l
	g++ yac.tab.c framework/NUBasic.cpp lex.yy.c -lfl -o NUBasic

clean:
	rm yac.tab.c lex.yy.c yac.tab.h nub.s simulator.config result.o
