# ====== PRACTICA: (Flex) ======

LEX=flex
CC=gcc
CFLAGS=-Wall -Wextra -O2
LDFLAGS=-lfl

.PHONY: all clean todo

all: lexico

# Generar el C a partir del .lex
lex.yy.c: Lexico.lex
	$(LEX) -l $<

# Compilar el C generado
lex.yy.o: lex.yy.c
	$(CC) $(CFLAGS) -c $< -o $@

# Enlazar el ejecutable del analizador léxico
lexico: lex.yy.o
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

clean:
	rm -f lexico lex.yy.c lex.yy.o

todo: clean lexico

