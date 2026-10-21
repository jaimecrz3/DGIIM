/* TablaTS.h */
#ifndef TS_H
#define TS_H

typedef enum { TV_ENTERO=0, TV_REAL=1, TV_CAR=2, TV_BOOL=3 } atr_tipovar; 
typedef enum { OU_NEG=0, OU_INC=1, OU_DEC=2, OU_REF=3, OU_PUNTERO=4  } atr_operuni; /*"!", "++", "--", "&", "*"*/
typedef enum {
  OB_SUMA=0, OB_RESTA=1, OB_MULT=2, OB_POT=3, OB_DIV=4, OB_MOD=5,
  OB_MAYOR=6, OB_MAYORIG=7, OB_MENOR=8, OB_MENORIG=9, OB_MASIGUAL=10, OB_MENOSIGUAL=11,
  OB_PORIGUAL=12, OB_DIVIGUAL=13
} atr_operbin;
typedef enum { OL_Y=0, OL_O=1, OL_IGUAL=3, OL_DISTINTO=4 } atr_operlog; /* '!' lo llevamos en OPERUNI */
typedef enum { OB_COMPL1=0, OB_AND=1, OB_OR=2, OB_XOR=3, OB_DESPLIZQ=4, OB_DESPLDRCHA=5 } atr_operbit;


/* Tipo de entrada en la TS */
typedef enum {
    marca,            /* marca comienzo de bloque */
    procedimiento,    /* subprograma procedimiento */
    variable,         /* variable normal */
    parametro_formal  /* parámetro formal */
} tipoEntrada;

/* Tipo de dato semántico */
typedef enum {
    entero,
    real,
    caracter,
    booleano,
    array,
    desconocido,
    no_asignado,
    vacio
} dtipo;

/* Entrada de la tabla de símbolos */
typedef struct {
    tipoEntrada entrada;
    char *nombre; // Contiene los caracteres que forman el identificador. En el caso de que TipoEntrada sea marca, no se usará 
    dtipo tipoDato;
    unsigned int parametros;   /* nº parámetros si es proc/func */
    unsigned int dimensiones;  /* para array */
    int TamDimen1;
    int TamDimen2;
} entradaTS;


#define MAX_TS 500

/* Declaración de la TS como pila */
extern unsigned int TOPE;        /* número de entradas usadas */
extern unsigned int subProg;     /* estamos en cuerpo de subprog o no */
extern unsigned int nParam;      /* nº de parámetros del subprog actual */
extern dtipo tipoTmp;            /* tipo de la lista de variables actual */
extern int dimTmp;
extern int tam1Tmp;
extern int tam2Tmp;
extern dtipo tipoSubprog;        /* tipo que devuelve la función actual */
extern unsigned int actualSubprog; /* índice del subprog actual en TS */
extern int filasInit2D;             // nº de filas leídas en la inicialización 2D
extern int maxColumnas2DInit;      // límite máximo de columnas (TamDimen2 del array)
extern int posArray2D;         // posición en TS de la variable a la que estamos asignando
extern int subprogLlamada;
extern dtipo aux_id;
extern entradaTS TS[MAX_TS];


typedef struct {
    int   atrib;   
    char *lexema;  
    dtipo tipo;    /* tipo que vamos propagando por las expresiones */
    int dimensiones; /* 0=escalar, 1=vector, 2=matriz */
    int tam1;        /* Tamaño dimensión 1 */
    int tam2;        /* Tamaño dimensión 2 */
} atributos;

#define YYSTYPE atributos

/* Inicialización / depuración */
void TS_Inicializar(void);
void TS_Mostrar(void);         

/* Manejo de bloques */
void TS_InsertaMARCA(void);
void TS_VaciarENTRADAS(void);

/* Inserciones */
void TS_InsertaIDENT(atributos id);     
void TS_InsertaSUBPROG(atributos id);   
void TS_InsertaPARAMF(atributos id);    

/* Resto */
int   TS_Buscar(char *nombre);      /* devuelve índice o -1 */
dtipo TS_TipoDe(char *nombre);      /* tipo del identificador (o desconocido) */
dtipo AtributoATipo(int atrib);     

void TS_UsaIdent(atributos *dest, atributos id); 
void comprobarAsignacion(atributos id, dtipo td);
void expresionBooleana(dtipo td);
void expresionEntera(dtipo td);

int existeVariable(atributos id);

char* tipoAString(dtipo tipo_dato);


#endif




