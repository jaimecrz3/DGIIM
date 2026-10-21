/*
 * Archivo: Lexico.lex
 * Descripción: Especificación para el generador léxico Flex.
 * Este archivo define las expresiones regulares para identificar y categorizar
 * los tokens del lenguaje (palabras clave, identificadores, operadores, literales),
 * retornando los tokens necesarios para el análisis sintáctico.
 */
%option yylineno
%option noyywrap
%{
    #include "tabla.h"
    #include "sintactico.tab.h"
    #include <string.h>
    extern YYSTYPE yylval;
%}

%%

[0-9]+[a-zA-Z_][a-zA-Z0-9_]* {
                                fprintf(stderr,"Error Léxico en la linea %d: Identificador inválido '%s' no puede empezar con un número.\n", yylineno, yytext);
                            }
                                
"("                         { yylval.cadena = "PIZQ"; return PIZQ; }
")"                         { yylval.cadena = "PDCHA"; return PDCHA; }
"entero"                    { yylval.cadena = "TIPO"; yylval.atrib = TV_ENTERO; return TIPO; }
"real"                      { yylval.cadena = "TIPO"; yylval.atrib = TV_REAL; return TIPO; }
"caracter"                  { yylval.cadena = "TIPO"; yylval.atrib = TV_CAR; return TIPO; }
"booleano"                  { yylval.cadena = "TIPO"; yylval.atrib = TV_BOOL; return TIPO; }
"true"                      { yylval.cadena = "TRUE"; return TRUE; }
"false"                     { yylval.cadena = "FALSE"; return FALSE; }
"."                         { yylval.cadena = "PUNTO"; return PUNTO; }
"}"                         { yylval.cadena = "LLAVED"; return LLAVED; }
"{"                         { yylval.cadena = "LLAVEI"; return LLAVEI; }
"++"                        { yylval.cadena = "OPERUNI"; yylval.atrib = OU_INC; return OPERUNI; }
"--"                        { yylval.cadena = "OPERUNI"; yylval.atrib = OU_DEC; return OPERUNI; }
"**"                        { yylval.cadena = "OPERBIN"; yylval.atrib= OB_POT; return OPERBIN; }
"%"                         { yylval.cadena = "OPERBIN"; yylval.atrib= OB_MOD; return OPERBIN; }
">="                        { yylval.cadena = "OPERBIN"; yylval.atrib= OB_MAYORIG; return OPERBIN; }
"<="                        { yylval.cadena = "OPERBIN"; yylval.atrib= OB_MENORIG; return OPERBIN; }
"<"                         { yylval.cadena = "OPERBIN"; yylval.atrib= OB_MENOR; return OPERBIN; }
">"                         { yylval.cadena = "OPERBIN"; yylval.atrib= OB_MAYOR; return OPERBIN; }
"+="                        { yylval.cadena = "OPERBIN"; yylval.atrib= OB_MASIGUAL; return OPERBIN; }
"-="                        { yylval.cadena = "OPERBIN"; yylval.atrib= OB_MENOSIGUAL; return OPERBIN; }
"*="                        { yylval.cadena = "OPERBIN"; yylval.atrib= OB_PORIGUAL; return OPERBIN; }
"/="                        { yylval.cadena = "OPERBIN"; yylval.atrib= OB_DIVIGUAL; return OPERBIN; }
"+"                         { yylval.cadena = "OPERBIN"; yylval.atrib= OB_SUMA; return OPERBIN; }
"-"                         { yylval.cadena = "OPERBIN"; yylval.atrib= OB_RESTA; return OPERBIN; }
"*"                         { yylval.cadena = "OPERBIN"; yylval.atrib= OB_MULT; return OPERBIN; }
"/"                         { yylval.cadena = "OPERBIN"; yylval.atrib= OB_DIV; return OPERBIN; }
"&&"                        { yylval.cadena = "OPERLOG"; yylval.atrib= OL_Y; return OPERLOG; }
"&"                         { yylval.cadena = "OPERUNI"; yylval.atrib = OU_REF; return OPERUNI; }  
"||"                        { yylval.cadena = "OPERLOG"; yylval.atrib= OL_O; return OPERLOG; }
"=="                        { yylval.cadena = "OPERLOG"; yylval.atrib= OL_IGUAL; return OPERLOG; }
"!="                        { yylval.cadena = "OPERLOG"; yylval.atrib= OL_DISTINTO; return OPERLOG; } 
"="                         { yylval.cadena = "ASIG"; return ASIG; }
"!"                         { yylval.cadena = "OPERLOG"; yylval.atrib= OL_NEG; return  OPERLOG; }
":"                         { yylval.cadena = "DOSPUNTOS"; return DOSPUNTOS; }
"]"                         { yylval.cadena = "CORD"; return CORD; }
"["                         { yylval.cadena = "CORI"; return CORI; }
";"                         { yylval.cadena = "PUNCOMA"; return PUNCOMA; }
"~"                         { yylval.cadena = "OPERBIT"; yylval.atrib= OB_COMPL1; return OPERBIT; }
"|"                         { yylval.cadena = "OPERBIT"; yylval.atrib= OB_OR; return OPERBIT; }
"^"                         { yylval.cadena = "OPERBIT"; yylval.atrib= OB_XOR; return OPERBIT; }
"<<"                        { yylval.cadena = "OPERBIT"; yylval.atrib= OB_DESPLIZQ; return OPERBIT; }
">>"                        { yylval.cadena = "OPERBIT"; yylval.atrib= OB_DESPLDRCHA; return OPERBIT; }
"locales"                   { yylval.cadena = "LOCALES"; return LOCALES; }
"lectura"                   { yylval.cadena = "LECTURA"; return LECTURA; }
"escritura"                 { yylval.cadena = "ESCRITURA"; return ESCRITURA; }
"si"                        { yylval.cadena = "SI"; return SI; } 
"si"[ \t]*"no"               { yylval.cadena = "SINO"; return SINO; }
"mientras"                  { yylval.cadena = "MIENTRAS"; return MIENTRAS; }
"hacer"                     { yylval.cadena = "HACER"; return HACER; }
"parar"                     { yylval.cadena = "PARAR"; return PARAR; }
"segun"                     { yylval.cadena = "SEGUN"; return SEGUN; }
"caso"                      { yylval.cadena = "CASO"; return CASO; }
"otro"                      { yylval.cadena = "OTRO"; return OTRO; }
"programa"                  { yylval.cadena = "PROG"; return PROG; }
"procedimiento"             { yylval.cadena = "PROCD"; return PROCD; }
","                         { yylval.cadena = "COMA"; return COMA; }
(\"([^\\\"\n]|\\.)*\")|('([^\\'\n]|\\.)')    { yylval.cadena = "CADENA"; return CADENA; }
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.cadena = "ID"; return ID; }
[0-9]+\.[0-9]*|\.[0-9]+|[0-9]+      { yylval.cadena = "CONSTANTE"; return CONSTANTE; }

"//"[^\n]*        { }
"/*"([^*]|\*+[^/])*"\*/"   { }

[ \t\n]                     { ; }                
.                           { printf("Error en la linea %d. Lexema %s no reconocido.\n", yylineno, yytext); }

                             
%%


