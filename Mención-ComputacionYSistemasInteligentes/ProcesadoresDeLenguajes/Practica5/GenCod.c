#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "TS.h"

static FILE *fout = NULL;
static int numTemp = 0;
static int numEtiqueta = 0;


// funciones auxiliares

const char *tipoC(dtipo t) {
    switch (t) {
        case entero:    return "int";
        case real:      return "float";
        case caracter:  return "char";
        case booleano:  return "int";
        default:        return "int";
    }
}

char *concatena(const char *s1, const char *s2) {
    size_t l1 = s1 ? strlen(s1) : 0;
    size_t l2 = s2 ? strlen(s2) : 0;
    char *r = (char *)malloc(l1 + l2 + 1);
    if (!r) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    if (s1) memcpy(r, s1, l1);
    if (s2) memcpy(r + l1, s2, l2);
    r[l1 + l2] = '\0';
    return r;
}

char *catf(const char *fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    int needed = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    char *buf = (char *)malloc(needed + 1);
    if (!buf) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    va_start(ap, fmt);
    vsnprintf(buf, needed + 1, fmt, ap);
    va_end(ap);

    return buf;
}

char* tipoImprimir(dtipo tipo) {
  if (tipo == booleano || tipo == entero)
    return "%d";
  else if (tipo == real)
    return "%f";
  else if (tipo == caracter)
    return "%s";
  else {
    fprintf(stderr, "Error en tipoImprimir(), no se conoce el tipo dato: %s\n", tipoAString(tipo));
    exit(EXIT_FAILURE);
  }
}


// funciones para la generacion del codigo

void iniciarCodigo(const char *nombreFich) {
    fout = fopen(nombreFich, "w");
    if (!fout) {
        perror("fopen salida código intermedio");
        exit(EXIT_FAILURE);
    }

    fprintf(fout, "#include <stdio.h>\n\n");
    fprintf(fout, "int main(void)\n{\n");
}

void cerrarCodigo(void) {
    if (!fout) return;

    fprintf(fout, "\treturn 0;\n");
    fprintf(fout, "}\n");

    fclose(fout);
    fout = NULL;
}

void volcarCodigo(const char *codigo) {
    if (!fout || !codigo) return;
    fprintf(fout, "%s", codigo);
}

// para generar variables temporales
char *nuevaTemporal(void) {
    char nombre[32];
    snprintf(nombre, sizeof(nombre), "temp%d", numTemp++);
    return strdup(nombre);
}

// para generar etiquetas
char *nuevaEtiqueta(void) {
    char nombre[32];
    snprintf(nombre, sizeof(nombre), "etiqueta%d", numEtiqueta++);
    return strdup(nombre);
}
