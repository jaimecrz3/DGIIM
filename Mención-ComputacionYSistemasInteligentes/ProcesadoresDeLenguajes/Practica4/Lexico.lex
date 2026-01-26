%option yylineno
%option noyywrap
%{
    #include "TS.h"
    #include "semantico.tab.h"
    #include <string.h>
    extern YYSTYPE yylval;
%}

%%

[0-9]+[a-zA-Z_][a-zA-Z0-9_]* {
                                fprintf(stderr,"Error Léxico en la linea %d: Identificador inválido '%s' no puede empezar con un número.\n", yylineno, yytext);
                            }
                                
"("                         { return PIZQ; }
")"                         { return PDCHA; }
"entero"                    { yylval.tipo = entero; yylval.atrib = TV_ENTERO; return TIPO; }
"real"                      { yylval.tipo = real; yylval.atrib = TV_REAL; return TIPO; }
"caracter"                  { yylval.tipo = caracter; yylval.atrib = TV_CAR; return TIPO; }
"booleano"                  { yylval.tipo = booleano; yylval.atrib = TV_BOOL; return TIPO; }
"true"                      { yylval.tipo = booleano; return TRUE; }
"false"                     { yylval.tipo = booleano; return FALSE; }
"."                         { return PUNTO; }
"}"                         { return LLAVED; }
"{"                         { return LLAVEI; }
"++"                        { yylval.atrib = OU_INC; return OPERUNI; }
"--"                        { yylval.atrib = OU_DEC; return OPERUNI; }
"**"                        { yylval.atrib= OB_POT; return OPERBIN; }
"%"                         { yylval.atrib= OB_MOD; return OPERBIN; }
">="                        { yylval.atrib= OB_MAYORIG; return OPERBIN; }
"<="                        { yylval.atrib= OB_MENORIG; return OPERBIN; }
"<"                         { yylval.atrib= OB_MENOR; return OPERBIN; }
">"                         { yylval.atrib= OB_MAYOR; return OPERBIN; }
"+="                        { yylval.atrib= OB_MASIGUAL; return OPERBIN; }
"-="                        { yylval.atrib= OB_MENOSIGUAL; return OPERBIN; }
"*="                        { yylval.atrib= OB_PORIGUAL; return OPERBIN; }
"/="                        { yylval.atrib= OB_DIVIGUAL; return OPERBIN; }
"+"                         { yylval.atrib= OB_SUMA; return OPERBIN; }
"-"                         { yylval.atrib= OB_RESTA; return OPERBIN; }
"*"                         { yylval.atrib= OB_MULT; return OPERBIN; }
"/"                         { yylval.atrib= OB_DIV; return OPERBIN; }
"&&"                        { yylval.atrib= OL_Y; return OPERLOG; }
"&"                         { yylval.atrib = OU_REF; return OPERUNI; }  
"||"                        { yylval.atrib= OL_O; return OPERLOG; }
"=="                        { yylval.atrib= OL_IGUAL; return OPERLOG; }
"!="                        { yylval.atrib= OL_DISTINTO; return OPERLOG; } 
"="                         { return ASIG; }
"!"                         { yylval.atrib= OU_NEG; return  OPERUNI; }
":"                         { return DOSPUNTOS; }
"]"                         { return CORD; }
"["                         { return CORI; }
";"                         { return PUNCOMA; }
"~"                         { yylval.atrib= OB_COMPL1; return OPERBIT; }
"|"                         { yylval.atrib= OB_OR; return OPERBIT; }
"^"                         { yylval.atrib= OB_XOR; return OPERBIT; }
"<<"                        { yylval.atrib= OB_DESPLIZQ; return OPERBIT; }
">>"                        { yylval.atrib= OB_DESPLDRCHA; return OPERBIT; }
"locales"                   { return LOCALES; }
"lectura"                   { return LECTURA; }
"escritura"                 { return ESCRITURA; }
"si"                        { return SI; } 
"si"[ \t]*"no"              { return SINO; }
"mientras"                  { return MIENTRAS; }
"hacer"                     { return HACER; }
"parar"                     { return PARAR; }
"segun"                     { return SEGUN; }
"caso"                      { return CASO; }
"otro"                      { return OTRO; }
"programa"                  { return PROG; }
"procedimiento"             { return PROCD; }
","                         { return COMA; }
(\"([^\\\"\n]|\\.)*\")|('([^\\'\n]|\\.)')    { yylval.tipo = caracter; yylval.lexema = strdup(yytext); return CADENA; }
[a-zA-Z_][a-zA-Z0-9_]*      { yylval.lexema = strdup(yytext); return ID; }
[0-9]+\.[0-9]*|\.[0-9]+     { yylval.tipo = real; yylval.lexema = strdup(yytext); return CONSTANTE_REAL; }
[0-9]+                      { yylval.tipo = entero; yylval.lexema = strdup(yytext); return CONSTANTE_ENTERA; }

"//"[^\n]*                  { }
"/*"([^*]|\*+[^/])*"\*/"    { }

[ \t\n]                     { ; }                
.                           { printf("Error en la linea %d. Lexema %s no reconocido.\n", yylineno, yytext); }

                             
%%


