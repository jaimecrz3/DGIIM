/* Tabla.h PRÁTICA 2 PL GRUPO A3 EQUIPO 3*/

// Descripcion : Definicion de los nombres de los tokens 

#ifndef TABLA_H
#define TABLA_H

#define ID          257
#define TIPO        258
#define ASIG        259
#define TRUE        260
#define FALSE       261
#define PUNTO       262
#define LLAVED      263

#define LLAVEI      264   
#define OPERUNI     265 
#define OPERBIN     266 
#define OPERLOG     267 
#define DOSPUNTOS   268
#define CORD        269
#define CORI        270
#define PUNCOMA     271
#define LOCALES     272
#define LECTURA     273
#define ESCRITURA   274
#define SI          275

#define SINO        276
#define MIENTRAS    277
#define HACER       278
#define PARAR       279
#define SEGUN       280
#define CASO        281
#define OTRO        282
#define PROG        283
#define PROCD       284
#define CADENA      285
#define CONSTANTE   286
#define PIZQ        287
#define PDCHA       288
#define COMA        289

#define OPERBIT     290


// Definición de los atributos de cada token


typedef enum { TV_ENTERO=0, TV_REAL=1, TV_CAR=2, TV_BOOL=3 } atr_tipovar; 
typedef enum { OU_NEG=0, OU_INC=1, OU_DEC=2, OU_REF=3, OU_PUNTERO=4  } atr_operuni; /*"-", "++", "--", "&", "*"*/
typedef enum {
  OB_SUMA=0, OB_RESTA=1, OB_MULT=2, OB_POT=3, OB_DIV=4, OB_MOD=5,
  OB_MAYOR=6, OB_MAYORIG=7, OB_MENOR=8, OB_MENORIG=9, OB_MASIGUAL=10, OB_MENOSIGUAL=11,
  OB_PORIGUAL=12, OB_DIVIGUAL=13
} atr_operbin;
typedef enum { OL_Y=0, OL_O=1, OL_NEG=2, OL_IGUAL=3, OL_DISTINTO=4 } atr_operlog; /* '!' lo llevamos en OPERUNI */
typedef enum { OB_COMPL1=0, OB_AND=1, OB_OR=2, OB_XOR=3, OB_DESPLIZQ=4, OB_DESPLDRCHA=5 } atr_operbit;




#endif




