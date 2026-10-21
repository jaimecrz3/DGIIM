# Archivo: makefile
# Descripción: Script de construcción automatizada.
#              Contiene las directivas para compilar el proyecto: genera el código C
#              a partir de Flex y Bison, compila los objetos y los enlaza para
#              crear el ejecutable final 'sintactico'. Incluye reglas de limpieza.
all: clean sintactico

sintactico: sintactico.tab.o lex.yy.o
	gcc -o sintactico sintactico.tab.o lex.yy.o

# ---- Bison ----
sintactico.tab.c sintactico.tab.h: sintactico.y
	bison -t -v -d sintactico.y

sintactico.tab.o: sintactico.tab.c
	gcc -c sintactico.tab.c

# ---- Flex ----
lex.yy.c: Lexico.lex sintactico.tab.h
	flex -l Lexico.lex


lex.yy.o: lex.yy.c
	gcc -c lex.yy.c

# ---- Limpieza ----
clean:
	rm -f sintactico sintactico.tab.o lex.yy.o \
	      sintactico.tab.c sintactico.tab.h lex.yy.c sintactico.output

todo:
	make --no-print-directory clean
	make --no-print-directory sintactico
