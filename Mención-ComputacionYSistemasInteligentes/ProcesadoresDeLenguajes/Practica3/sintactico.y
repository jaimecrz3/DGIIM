/*
 * Archivo: sintactico.y
 * Descripción: Especificación gramatical para el generador sintáctico Bison.
 * Define la Gramática Libre de Contexto (GLC) del lenguaje, estableciendo
 * las reglas de producción (programa, sentencias, expresiones) y la precedencia
 * de operadores para validar la estructura sintáctica del código fuente.
 */

%{
  #include <stdio.h>
  #include <stdlib.h>
  #include "tabla.h"     
  int yylex(void);
  extern int yylineno;
  /*extern char *yytext;*/
  void yyerror(const char *s){ 
    /*fprintf(stderr,"[Sintaxis] Error en la linea: %d, por el lexema: %s. %s\n", yylineno, yytext, s); */
    fprintf(stderr,"[Sintaxis] Error en la linea: %d, Error: %s\n", yylineno, s);
  }
%}

/* Errores verbosos */
%define parse.error verbose

%code requires { typedef struct { int atrib; char *cadena; } yystype_t; }
%define api.value.type {yystype_t}

/* ---- Tokens ---- */
%token ID TIPO ASIG TRUE FALSE PUNTO LLAVED LLAVEI
%token OPERUNI OPERBIN OPERLOG OPERBIT DOSPUNTOS CORD CORI PUNCOMA
%token LOCALES LECTURA ESCRITURA SI SINO MIENTRAS HACER PARAR
%token SEGUN CASO OTRO PROG PROCD CADENA CONSTANTE
%token PIZQ PDCHA COMA

/* ---- Precedencias y asociatividades ---- */
%left  OPERLOG                 
%left  OPERBIN                 
%left  '*' '/' '%'             
%right OPERUNI     
%nonassoc MENOR_QUE_SINO       
%nonassoc SINO

/* símbolo inicial */
%start programa

%%

/* ---------------- PROGRAMA ---------------- */
programa
  : PROG bloque                         
  | PROG error                          { yyerror("cabecera de programa"); yyerrok; }
  ;

/* ---------------- BLOQUES ---------------- */
bloque
  : LLAVEI cabecera_bloque lista_sentencias LLAVED  
  ;



cabecera_bloque
  : /* vacío */
  | LOCALES LLAVEI lista_decls LLAVED
  | LOCALES LLAVEI error LLAVED        { yyerror("declaraciones locales"); yyerrok; }
  | seccion_procs
  | LOCALES LLAVEI lista_decls LLAVED seccion_procs
  | LOCALES LLAVEI error LLAVED seccion_procs { yyerror("declaraciones locales"); yyerrok; }
  ;


lista_decls
  : decl
  | lista_decls decl
  ;

lista_id
  : ID
  | ID COMA lista_id
  | ID CORI tam CORD
  | ID CORI tam CORD COMA lista_id
  | ID CORI tam CORD CORI tam CORD
  | ID CORI tam COMA tam CORD
  | ID CORI tam CORD CORI tam CORD COMA lista_id
  | ID CORI tam COMA tam CORD COMA lista_id
  ;

decl
  : TIPO lista_id PUNCOMA
  | error PUNCOMA                    { yyerror("declaración"); yyerrok; }
  ;

tam
  : CONSTANTE        
  | ID                
  ;

seccion_procs
  : declar_proc
  | seccion_procs declar_proc
  ;


declar_proc
  : cabecera_proc bloque
  | cabecera_proc error    { yyerror("bloque de procedimiento"); yyerrok; }
  ;

cabecera_proc
  : PROCD ID PIZQ lista_param PDCHA
  ;

lista_param
  : /* vacío */
  | parametros
  | parametros COMA error  { yyerror("parámetro"); yyerrok; } 
  ;

parametros
  : parametro
  | parametros COMA parametro
  ;


parametro
  : TIPO ID
  | TIPO OPERUNI ID         
  | TIPO ID CORI CORD
  | TIPO ID CORI CORD CORI CORD
  ;

/* ---------------- SENTENCIAS ---------------- */
lista_sentencias
  : sentencia 
  | lista_sentencias sentencia
  ;



sentencia
  : bloque
  | asignacion PUNCOMA
  | sentencia_if
  | sentencia_while
  | sentencia_case
  | sentencia_entrada PUNCOMA
  | sentencia_salida PUNCOMA
  | llamada_proc PUNCOMA
  | expresion PUNCOMA                  
  | error PUNCOMA      { yyerror("sentencia"); yyerrok; }
  ;

/* asignación */
asignacion
  : ID ASIG exp_o_cad
  | ID ASIG LLAVEI lista_exp_o_cad LLAVED
  ;

/* lectura/escritura */
sentencia_entrada
  : LECTURA lista_vars_opt
  ;

lista_vars_opt
  : lista_vars
  ;

lista_vars
  : /* vacío */
  | ID
  | lista_vars COMA ID
  ;

/* escritura admite expresiones y cadenas */
sentencia_salida
  : ESCRITURA lista_exp_o_cad
  ;

lista_exp_o_cad
  : exp_o_cad
  | lista_exp_o_cad COMA exp_o_cad
  ;

exp_o_cad
  : expresion
  | CADENA
  ;

sentencia_if
  : SI PIZQ expresion PDCHA sentencia %prec MENOR_QUE_SINO                 
  | SI PIZQ expresion PDCHA sentencia SINO sentencia
  ;

sentencia_while
  : MIENTRAS PIZQ expresion PDCHA HACER sentencia
  ;

/* switch*/
sentencia_case
  : SEGUN PIZQ expresion PDCHA LLAVEI lista_casos LLAVED
  ;

lista_casos
  : caso
  | lista_casos caso
  ;

caso
  : CASO constante DOSPUNTOS sentencia PARAR PUNCOMA
  | CASO constante DOSPUNTOS sentencia
  | OTRO DOSPUNTOS sentencia
  | OTRO DOSPUNTOS sentencia PARAR PUNCOMA
  ;

constante
  : CONSTANTE
  | TRUE
  | FALSE
  ;

/* llamada a procedimiento */
llamada_proc
  : ID PIZQ lista_arg_call_opt PDCHA
  ;

lista_arg_call_opt
  : /* vacío */
  | lista_arg_call
  ;

lista_arg_call
  : arg_call
  | lista_arg_call COMA arg_call
  ;

arg_call
  : exp_o_cad
  ;

/* ---------------- EXPRESIONES ---------------- */
primario
  : PIZQ expresion PDCHA
  | constante
  | ID
  ;

/* acceso indexado a vectores/matrices*/
postfijo
  : primario
  | ID indices
  ;

indices
  : indice
  | indice indice
  ;

indice
  : CORI expresion CORD
  ;

unario
  : postfijo
  | OPERUNI unario
  | unario OPERUNI
  ;

expresion
  : unario
  | exp_o_cad OPERBIN exp_o_cad
  | exp_o_cad OPERLOG exp_o_cad
  ;

%%

/* main para enlazar con FLEX */
int main(void){
  return yyparse();
}
