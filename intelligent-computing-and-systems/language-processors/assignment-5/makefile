
all: clean semantico

semantico: semantico.tab.o TS.o lex.yy.o GenCod.o
	gcc -o semantico semantico.tab.o TS.o lex.yy.o GenCod.o

GenCod.o: GenCod.c TS.h
	gcc -c GenCod.c


TS.o: TS.c
	gcc -c TS.c

# ---- Bison ----
semantico.tab.c semantico.tab.h: semantico.y
	bison -t -v -d semantico.y

semantico.tab.o: semantico.tab.c
	gcc -c semantico.tab.c

# ---- Flex ----
lex.yy.c: Lexico.lex semantico.tab.h
	flex -l Lexico.lex


lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

# ---- Limpieza ----
clean:
	rm -f semantico semantico.tab.o lex.yy.o TS.o \
	      semantico.tab.c semantico.tab.h lex.yy.c semantico.output \
		  GenCod.o 

todo:
	make --no-print-directory clean
	make --no-print-directory semantico
