/*
 * Archivo: tabla.h
 * Descripción: Definición de estructuras y constantes compartidas.
 * Contiene los tipos enumerados (enums) para los tipos de variables,
 * códigos de operadores (unarios, binarios, lógicos) y atributos semánticos
 * utilizados tanto por el analizador léxico como por el sintáctico.
 */
#ifndef TABLA_H
#define TABLA_H

typedef enum { TV_ENTERO=0, TV_REAL=1, TV_CAR=2, TV_BOOL=3 } atr_tipovar; 
typedef enum { OU_NEG=0, OU_INC=1, OU_DEC=2, OU_REF=3, OU_PUNTERO=4  } atr_operuni; /*"-", "++", "--", "&", "*"*/
typedef enum {
  OB_SUMA=0, OB_RESTA=1, OB_MULT=2, OB_POT=3, OB_DIV=4, OB_MOD=5,
  OB_MAYOR=6, OB_MAYORIG=7, OB_MENOR=8, OB_MENORIG=9, OB_MASIGUAL=10, OB_MENOSIGUAL=11,
  OB_PORIGUAL=12, OB_DIVIGUAL=13
} atr_operbin;
typedef enum { OL_Y=0, OL_O=1, OL_NEG=2, OL_IGUAL=3, OL_DISTINTO=4 } atr_operlog; 
typedef enum { OB_COMPL1=0, OB_AND=1, OB_OR=2, OB_XOR=3, OB_DESPLIZQ=4, OB_DESPLDRCHA=5 } atr_operbit;




#endif




