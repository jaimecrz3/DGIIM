/* TS.c – Gestión de la Tabla de Símbolos y parte del análisis semántico */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TS.h"

extern int yylineno;

// Variables globales de la TS
unsigned int TOPE = 0;
unsigned int subProg = 0;
unsigned int nParam = 0;
dtipo tipoTmp = no_asignado;
int dimTmp     = 0;
int tam1Tmp    = 0;
int tam2Tmp    = 0;
dtipo tipoSubprog = vacio;
unsigned int actualSubprog = 0;   // Entrada de la TS de un subprograma
int filasInit2D = 0;              // nº de filas leídas en la inicialización 2D
int maxColumnas2DInit = -1;       // límite máximo de columnas (TamDimen2 del array)
int posArray2D = -1;              // posición en TS de la variable a la que estamos asignando
int subprogLlamada = -1;
dtipo aux_id = vacio;

entradaTS TS[MAX_TS]; // Pila de la tabla de símbolos

static void TS_SubirTope(void) {
    if (TOPE >= MAX_TS) {
        fprintf(stderr, "*** Error: desbordamiento de la tabla de símbolos\n");
        exit(1);
    }
    TOPE++;
}

/* Buscar identificador desde arriba (respeta ámbitos) */
int TS_Buscar(char *nombre) {
    int i;
    for (i = (int)TOPE - 1; i >= 0; --i) {
        if (TS[i].entrada != marca && TS[i].nombre && strcmp(TS[i].nombre, nombre) == 0)
            return i;
    }
    return -1;
}

dtipo TS_TipoDe(char *nombre) {
    int pos = TS_Buscar(nombre);
    if (pos < 0) return desconocido;
    return TS[pos].tipoDato;
}

/* --------- Conversión atributo TIPOVAR -> dtipo --------- */

dtipo AtributoATipo(int atrib) {
    switch ((atr_tipovar)atrib) {
        case TV_ENTERO: return entero;
        case TV_REAL:   return real;
        case TV_CAR:    return caracter;
        case TV_BOOL:   return booleano;
        default:        return desconocido;
    }
}

/* --------- Inicialización / depuración --------- */

void TS_Inicializar(void) {
    TOPE = 0;
    subProg = 0;
    nParam = 0;
    tipoTmp = no_asignado;
    tipoSubprog = vacio;
    actualSubprog = 0;
}

void TS_Mostrar(void) {
    unsigned int i;
    printf("\n===== TABLA DE SÍMBOLOS (TOPE = %u) =====\n", TOPE);
    for (i = 0; i < TOPE; ++i) {
        printf("[%3u] ", i);
        switch (TS[i].entrada) {
            case marca:            printf("MARCA         "); break;
            case procedimiento:    printf("PROCEDIMIENTO "); break;
            case variable:         printf("VARIABLE      "); break;
            case parametro_formal: printf("PARAM_FORMAL  "); break;
        }
        printf(" %s  tipo=%d  nPar=%u dim=%u (%d,%d)\n",
            TS[i].nombre ? TS[i].nombre : "-",
            TS[i].tipoDato, TS[i].parametros,
            TS[i].dimensiones, TS[i].TamDimen1, TS[i].TamDimen2);
    }
    printf("=========================================\n");
}

/* --------- Manejo de bloques --------- */

/* Inserta marca de comienzo de bloque */
void TS_InsertaMARCA(void) {
    TS_SubirTope();
    TS[TOPE-1].entrada      = marca;
    TS[TOPE-1].nombre       = NULL;
    TS[TOPE-1].tipoDato     = no_asignado;
    TS[TOPE-1].parametros   = 0;
    TS[TOPE-1].dimensiones  = 0;
    TS[TOPE-1].TamDimen1    = 0;
    TS[TOPE-1].TamDimen2    = 0;
}

/* Vacía entradas hasta la última marca incluida */
void TS_VaciarENTRADAS(void) {
    while (TOPE > 0) {
        TOPE--;
        if (TS[TOPE].entrada == marca) break;
    }
}

/* --------- Inserción de identificadores --------- */

/* Comprueba si ya existe en el bloque actual */
static int ExisteEnBloqueActual(char *nombre) {
    int i;
    for (i = (int)TOPE - 1; i >= 0; --i) {
        if (TS[i].entrada == marca) break;
        if (TS[i].entrada == variable || TS[i].entrada == parametro_formal) {
            if (TS[i].nombre && strcmp(TS[i].nombre, nombre) == 0)
                return 1;
        }
    }
    return 0;
}

/* Comprueba si hay parámetro formal con el mismo nombre
   en el subprograma actual */
static int EsParametroDeActual(char *nombre) {
    if (TOPE == 0) return 0;
    int i = (int)actualSubprog + 1;
    while (i < (int)TOPE && TS[i].entrada == parametro_formal) {
        if (TS[i].nombre && strcmp(TS[i].nombre, nombre) == 0)
            return 1;
        i++;
    }
    return 0;
}

/* Declaración de variables*/
void TS_InsertaIDENT(atributos id) {
    if (ExisteEnBloqueActual(id.lexema)) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: identificador '%s' duplicado en el mismo bloque\n",
                yylineno, id.lexema);
        return;
    }

    if (subProg && EsParametroDeActual(id.lexema)) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: '%s' ya es parámetro formal del subprograma actual\n",
                yylineno, id.lexema);
        return;
    }

    TS_SubirTope();
    TS[TOPE-1].entrada      = variable;
    TS[TOPE-1].nombre       = strdup(id.lexema);

  
    TS[TOPE-1].tipoDato    = tipoTmp;     

    TS[TOPE-1].dimensiones = dimTmp;     
    TS[TOPE-1].TamDimen1   = tam1Tmp;
    TS[TOPE-1].TamDimen2   = tam2Tmp;
}

/* Declaración de subprograma (cabecera) */
void TS_InsertaSUBPROG(atributos id) {
    /* Comprueba que no existe ya un subprog con el mismo nombre
       en el ámbito actual */
    if (ExisteEnBloqueActual(id.lexema)) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: subprograma '%s' duplicado\n",
                yylineno, id.lexema);
        return;
    }

    TS_SubirTope();

    TS[TOPE-1].entrada      = procedimiento;
    TS[TOPE-1].nombre       = strdup(id.lexema);
    TS[TOPE-1].tipoDato     = vacio;  /* procedimiento siempre void */
    TS[TOPE-1].parametros   = 0;
    TS[TOPE-1].dimensiones  = 0;
    TS[TOPE-1].TamDimen1    = 0;
    TS[TOPE-1].TamDimen2    = 0;

    actualSubprog = TOPE-1;
    nParam = 0;
}

/* Parámetros formales de subprograma */
void TS_InsertaPARAMF(atributos id) {
    /* Evitar nombres de parámetro repetidos */
    int i;
    for (i = (int)TOPE - 1; i > (int)actualSubprog; --i) {
        if (TS[i].entrada == parametro_formal &&
            TS[i].nombre && strcmp(TS[i].nombre, id.lexema) == 0) {
            fprintf(stderr,
                    "*** [Linea %d] Error semántico: parámetro formal '%s' duplicado\n",
                    yylineno, id.lexema);
            return;
        }
    }

    TS_SubirTope();
    TS[TOPE-1].entrada      = parametro_formal;
    TS[TOPE-1].nombre       = strdup(id.lexema);
    TS[TOPE-1].tipoDato     = tipoTmp;    /* tipoTmp debe valer tipo del parámetro */
    TS[TOPE-1].parametros   = 0;
    TS[TOPE-1].dimensiones  = dimTmp;
    TS[TOPE-1].TamDimen1    = 0;
    TS[TOPE-1].TamDimen2    = 0;

    nParam++;
    TS[actualSubprog].parametros = nParam;
}

/* --------- Uso de identificadores en expresiones --------- */

void TS_UsaIdent(atributos *dest, atributos id) {
    int pos = TS_Buscar(id.lexema);
    if (pos < 0) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: identificador '%s' no declarado\n",
                yylineno, id.lexema);
        dest->tipo = desconocido;
    } else {
        dest->tipo = TS[pos].tipoDato;
    }
}

char* tipoAString(dtipo tipo_dato) {
  switch (tipo_dato) {
    case real:
      return "real";
    case entero:
      return "entero";
    case booleano:
      return "booleano";
    case caracter:
      return "caracter";
    case array:
      return "array";
    case desconocido:
      return "desconocido";
    case no_asignado:
      return "no_asignado";
    case vacio:
      return "vacio";
    default:
      fprintf(stderr, "Error en tipoAString(), no se conoce el tipo dato\n");
      exit(EXIT_FAILURE);
  }
}

void comprobarAsignacion(atributos id, dtipo td) { /*Comprueba que los tipos coincidan, pues sólo se pueden asignar cosas del mismo tipo*/
  int i = TS_Buscar(id.lexema);
  if (i >= 0) {
    if (TS[i].entrada != variable && TS[i].entrada != parametro_formal) {
      fprintf(stderr, "*** [Linea %d] ERROR SEMÁNTICO: se intenta asignar a %s, y no es una variable\n", yylineno, id.lexema);
    } else {
      if (td == desconocido || td == no_asignado || td == vacio || td != TS[i].tipoDato) {
        fprintf(stderr, "*** [Linea %d] ERROR SEMÁNTICO: asignación incorrecta, %s es tipo %s y se obtuvo %s\n",
            yylineno, id.lexema, tipoAString(TS[i].tipoDato), tipoAString(td));
      }
    }
  } else{
    fprintf(stderr, "*** [Linea %d] ERROR SEMÁNTICO:  Id (%s) inexistente", yylineno, id.lexema);
  }
}

void expresionBooleana(dtipo td) {
  if (td == desconocido || td == no_asignado || td == vacio || td != booleano) {
    fprintf(stderr, "*** [Linea %d] ERROR SEMÁNTICO: condición no es de tipo booleano, se tiene tipo %s",
        yylineno, tipoAString(td));
  }
}

void expresionEntera(dtipo td) {
  if (td == desconocido || td == no_asignado || td == vacio || td != entero) {
    fprintf(stderr, "*** [Linea %d] ERROR SEMÁNTICO: indice no es de tipo entero, se tiene tipo %s",
        yylineno, tipoAString(td));
  }
}

int existeVariable(atributos id) {
    int pos = TS_Buscar(id.lexema);
      if (pos < 0) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: variable '%s' no declarada.\n",
                yylineno, id.lexema);
      } else if (TS[pos].entrada != variable && TS[pos].entrada != parametro_formal) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: '%s' no es una variable (lectura)\n",
                yylineno, id.lexema);
      } 
    return pos;
}
