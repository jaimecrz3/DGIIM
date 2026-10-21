%{
  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>
  #include "TS.h"     

  int yylex(void);
  extern int yylineno;
  void yyerror(const char *s){ 
    fprintf(stderr,"[Sintaxis] Error en la linea: %d, Error: %s\n", yylineno, s);
  }
%}

%define parse.error verbose

/* tokens */
%token ID TIPO ASIG TRUE FALSE PUNTO LLAVED LLAVEI
%token OPERUNI OPERBIN OPERLOG OPERBIT DOSPUNTOS CORD CORI PUNCOMA
%token LOCALES LECTURA ESCRITURA SI SINO MIENTRAS HACER PARAR
%token SEGUN CASO OTRO PROG PROCD CADENA CONSTANTE_ENTERA CONSTANTE_REAL
%token PIZQ PDCHA COMA

/* Precedencia y asociatividad */
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
    {
      volcarCodigo($2.codigo);
    }
  | PROG error
    { 
      yyerror("cabecera de programa"); yyerrok; 
    }
  ;

/* ---------------- BLOQUES ---------------- */
bloque
  : LLAVEI
      { TS_InsertaMARCA(); } 
    cabecera_bloque 
    lista_sentencias 
    LLAVED
      {
        TS_VaciarENTRADAS();
        $$.codigo = catf("{\n%s%s}\n", $3.codigo, $4.codigo);
      }
  ;

cabecera_bloque
  : /* vacío */ { $$.codigo = strdup(""); }
  | LOCALES LLAVEI lista_decls LLAVED { $$.codigo=$3.codigo; }
  | LOCALES LLAVEI error LLAVED
      { yyerror("declaraciones locales"); yyerrok; }
  | seccion_procs
  | LOCALES LLAVEI lista_decls LLAVED seccion_procs { $$.codigo=$3.codigo; }
  | LOCALES LLAVEI error LLAVED seccion_procs 
      { yyerror("declaraciones locales"); yyerrok; }
  ;

lista_decls
  : decl
      { $$.codigo = $1.codigo; }
  | lista_decls decl
      { $$.codigo = concatena($1.codigo, $2.codigo); }
  ;

decl
  : TIPO { tipoTmp = $1.tipo; }
    lista_id PUNCOMA
    { 
        $$.codigo = catf("\t%s %s;\n", tipoC($1.tipo), $3.codigo);
    }
  | error PUNCOMA
      { yyerror("declaración"); yyerrok; $$.codigo = strdup(""); }
  ;

lista_id
  /* escalar solo */
  : ID
      {
        dimTmp = 0;
        tam1Tmp = tam2Tmp = 0;
        TS_InsertaIDENT($1);
        $$.codigo = strdup($1.lexema);
      }

  /* escalar + más cosas detrás */
  | ID COMA lista_id
      {
        dimTmp = 0;
        tam1Tmp = tam2Tmp = 0;
        TS_InsertaIDENT($1);
        $$.codigo = catf("%s, %s", $1.lexema, $3.codigo);
      }

  /* array 1D: ID [ tam ] */
  | ID CORI tam CORD
      {
        dimTmp = 1;
        tam1Tmp = atoi($3.lexema);   
        tam2Tmp = 0;
        TS_InsertaIDENT($1);
      }

  | ID CORI tam CORD COMA lista_id
      {
        dimTmp = 1;
        tam1Tmp = atoi($3.lexema);
        tam2Tmp = 0;
        TS_InsertaIDENT($1);
      }

  /* array 2D: ID [tam] [tam] */
  | ID CORI tam CORD CORI tam CORD
      {
        dimTmp = 2;
        tam1Tmp = atoi($3.lexema);
        tam2Tmp = atoi($6.lexema);
        TS_InsertaIDENT($1);
      }

  | ID CORI tam CORD CORI tam CORD COMA lista_id
      {
        dimTmp = 2;
        tam1Tmp = atoi($3.lexema);
        tam2Tmp = atoi($6.lexema);
        TS_InsertaIDENT($1);
      }
  ;

tam
  : CONSTANTE_ENTERA  
  ;

seccion_procs
  : declar_proc
  | seccion_procs declar_proc
  ;

declar_proc
  : cabecera_proc
      { subProg=1; } 
    bloque
      { subProg=0; }
  | cabecera_proc error
      { yyerror("bloque de procedimiento"); yyerrok; }
  ;

cabecera_proc
  : PROCD ID
      { TS_InsertaSUBPROG($2); }
    PIZQ lista_param PDCHA  
  ;

lista_param
  : /* vacío */
  | parametros
  | parametros COMA error
      { yyerror("parámetro"); yyerrok; } 
  ;

parametros
  : parametro
  | parametros COMA parametro
  ;

parametro
  /* parámetro escalar por valor */
  : TIPO ID
    {
      tipoTmp = $1.tipo;  
      dimTmp  = 0;
      TS_InsertaPARAMF($2);
    }

  /* parámetro escalar por referencia */
  | TIPO OPERUNI ID
    {
      tipoTmp = $1.tipo;
      dimTmp  = 0;
      TS_InsertaPARAMF($3);
    }

  /* parámetro vector (ID[]) */
  | TIPO ID CORI CORD
    {
      tipoTmp = $1.tipo;
      dimTmp  = 1;
      TS_InsertaPARAMF($2);
    }

  /* parámetro matriz (ID[][]) */
  | TIPO ID CORI CORD CORI CORD
    {
      tipoTmp = $1.tipo;
      dimTmp  = 2;
      TS_InsertaPARAMF($2);
    }
  ;


/* ---------------- SENTENCIAS ---------------- */
lista_sentencias
  : /* vacío */
      { $$.codigo = strdup(""); }
  | sentencia lista_sentencias
      { $$.codigo = concatena($1.codigo, $2.codigo); }
  ;


sentencia
  : bloque
      { $$.codigo = $1.codigo; }
  | asignacion PUNCOMA
      { $$.codigo = $1.codigo; }
  | sentencia_if
      { $$.codigo = $1.codigo; }
  | sentencia_while
      { $$.codigo = $1.codigo; }
  | sentencia_case
      { $$.codigo = $1.codigo; }
  | sentencia_entrada PUNCOMA
      { $$.codigo = $1.codigo; }
  | sentencia_salida PUNCOMA
      { $$.codigo = $1.codigo; }
  | llamada_proc PUNCOMA
      { $$.codigo = $1.codigo; }
  | expresion PUNCOMA
      { $$.codigo = $1.codigo; }
  | error PUNCOMA
      { yyerror("sentencia"); yyerrok; $$.codigo = strdup(""); }
  ;

/* asignación */
asignacion
  : ID ASIG exp_o_cad         
  { 
    comprobarAsignacion($1, $3.tipo);
    int pos = existeVariable($1); 
    if(TS[pos].dimensiones != 0){
      if((TS[pos].dimensiones != $3.dimensiones) ||
         (TS[pos].TamDimen1 < $3.tam1) ||
         (TS[pos].TamDimen2 < $3.tam2)){
        fprintf(stderr,
                  "*** [Linea %d] Error semántico: asignación de vectores de distinto tamaño\n",
                  yylineno);
      }
    }

    $$.codigo = concatena($3.codigo, catf("\t%s = %s;\n", $1.lexema, $3.nombre));
  } 
  | ID ASIG LLAVEI lista_exp_o_cad LLAVED
  {
    comprobarAsignacion($1, $4.tipo); 
    int pos = existeVariable($1);
    if (TS[pos].dimensiones != 1) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: inicialización con '{...}' solo permitida para vectores 1D\n",
                yylineno);
    } else {
      if (TS[pos].TamDimen1 > 0 && $4.atrib > TS[pos].TamDimen1) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: demasiados elementos en inicialización de '%s' (%d > %d)\n",
                yylineno, $1.lexema, $4.atrib, TS[pos].TamDimen1);
      }
    }

    /* no guardamos esta asignacion, no lo pide la practica */
    $$.codigo = strdup("  /* asignación vector 1D no traducida a codigo intermedio */\n");
  }
  | ID ASIG LLAVEI 
  {
    posArray2D = existeVariable($1);
    if (TS[posArray2D].dimensiones != 2) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: inicialización con '{...}' solo permitida para vectores 2D\n",
                yylineno);
    } 

    filasInit2D = 0;
    maxColumnas2DInit = (posArray2D >= 0) ? TS[posArray2D].TamDimen2 : -1;
    aux_id = TS[posArray2D].tipoDato;
  }
  asignacion2D LLAVED
  {
    if($5.tipo != aux_id){
      fprintf(stderr,
              "*** [Linea %d] Error semántico: Asignación de vectores de tipos distintos \n", yylineno);
    }
    if (posArray2D >= 0) {
      if (TS[posArray2D].TamDimen1 > 0 && filasInit2D > TS[posArray2D].TamDimen1) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: demasiadas filas en inicialización de '%s' (%d > %d)\n",
                yylineno, $1.lexema, filasInit2D, TS[posArray2D].TamDimen1);
      }
    }

    /* no guardamos esta asignacion, no lo pide la practica */
    $$.codigo = strdup("  /* asignación vector 2D no traducida a C intermedio */\n");
  }
  ;

asignacion2D
  : LLAVEI lista_exp_o_cad LLAVED
  {
    int numCols = $2.atrib;
    filasInit2D++;

    /* comprobar que no se pasa del máximo de columnas */
    if (maxColumnas2DInit > 0 && numCols > maxColumnas2DInit) {
      fprintf(stderr,
              "*** [Linea %d] Error semántico: demasiadas columnas en fila de inicialización 2D (%d > %d)\n",
              yylineno, numCols, maxColumnas2DInit);
    }

    $$.tipo = $2.tipo;
  }
  | LLAVEI lista_exp_o_cad LLAVED COMA asignacion2D
  {
    int numCols = $2.atrib;
    filasInit2D++;

    if (maxColumnas2DInit > 0 && numCols > maxColumnas2DInit) {
      fprintf(stderr,
              "*** [Linea %d] Error semántico: demasiadas columnas en fila de inicialización 2D (%d > %d)\n",
              yylineno, numCols, maxColumnas2DInit);
    }

    if($2.tipo != $5.tipo) {
      fprintf(stderr,
                "*** [Linea %d] Error semántico: tipos de datos incompatibles en la declaración del vector\n", yylineno);
    }
    else {
      $$.tipo = $2.tipo;
    }
  }
  ;

/* lectura/escritura */
sentencia_entrada
  : LECTURA lista_vars_opt
    {
      $$.codigo = $2.codigo;
    }
  ;

lista_vars_opt
  : lista_vars
    {
      $$.codigo = $1.codigo;
    }
  ;

lista_vars
  : /* vacío */
    {
      $$.codigo = strdup("");
    }
  | ID
    { 
      int pos = existeVariable($1);
      dtipo td = TS[pos].tipoDato; 

      $$.codigo = catf("\tscanf(\"%s\", &%s);\n", tipoImprimir(td), $1.lexema);
    } 
  | lista_vars COMA ID
    { 
      int pos = existeVariable($3);
      dtipo td = TS[pos].tipoDato; 

      char *c = catf("\tscanf(\"%s\", &%s);\n", tipoImprimir(td), $3.lexema);
      $$.codigo = concatena($1.codigo, c); 
    }
  ;


sentencia_salida
  : ESCRITURA lista_exp_o_cad_escritura
    {
      $$.codigo = $2.codigo;
    }
  ;

lista_exp_o_cad_escritura
  : exp_o_cad 
    {
      $$.codigo = catf("\tprintf(\"%s\", %s);\n", tipoImprimir($1.tipo), $1.nombre);
    }
  | lista_exp_o_cad_escritura COMA exp_o_cad 
    { 
      $$.codigo = concatena($1.codigo, catf("\tprintf(\"%s\", %s);\n", tipoImprimir($3.tipo), $3.nombre)); 
    }
  ;

lista_exp_o_cad
  : exp_o_cad
  {
    $$.tipo  = $1.tipo;  
    $$.atrib = 1;          /* nº de elementos */
    $$.codigo = $1.codigo;
  }
  | lista_exp_o_cad COMA exp_o_cad
  {
    $$.atrib = $1.atrib + 1;  /* nº de elementos */

    $$.codigo = concatena($1.codigo, concatena(",", $3.codigo));

    /* comprobamos que no mezclas tipos raros en la lista */
    if ($1.tipo == desconocido || $3.tipo == desconocido) {
      $$.tipo = desconocido;
    } else if ($1.tipo == $3.tipo) {
      $$.tipo = $1.tipo;
    } else {
      fprintf(stderr,
              "*** [Linea %d] Error semántico: elementos con tipos incompatibles en inicialización de vector\n",
              yylineno);
      $$.tipo = desconocido;
    }
  }
  ;

exp_o_cad
  : expresion   
  { 
    $$.tipo = $1.tipo; 
    $$.dimensiones = $1.dimensiones;
    $$.tam1 = $1.tam1;
    $$.tam2 = $1.tam2;

    $$.codigo = $1.codigo;
    $$.nombre = $1.nombre;
  }
  | CADENA
  {
    $$.tipo = caracter;
    $$.dimensiones = 1;   /* tratamos la cadena como vector de char */
    $$.tam1 = 0;
    $$.tam2 = 0;

    $$.codigo = strdup("");
    $$.nombre = strdup($1.lexema);
  }
  ;


/* ---------------- IF / WHILE / CASE ---------------- */

sentencia_if
  : SI PIZQ expresion PDCHA sentencia %prec MENOR_QUE_SINO  
  {
      expresionBooleana($3.tipo);

      char *Lelse   = nuevaEtiqueta();
      char *Lsalida = nuevaEtiqueta();

      /* if (!cond) goto Lelse; */
      char *c1 = catf("\tif (!%s) goto %s;\n", $3.nombre, Lelse);

      char *cThen = $5.codigo;
      char *c2 = catf("\tgoto %s;\n", Lsalida);
      char *c3 = catf("%s:\n", Lelse);
      char *c4 = catf("%s:\n", Lsalida);

      $$.codigo = concatena($3.codigo,
                     concatena(c1,
                       concatena(cThen,
                         concatena(c2,
                           concatena(c3, c4)))));
  }               
  | SI PIZQ expresion PDCHA sentencia SINO sentencia
  {
      expresionBooleana($3.tipo);

      char *Lelse   = nuevaEtiqueta();
      char *Lsalida = nuevaEtiqueta();

      char *c1 = catf("\tif (!%s) goto %s;\n", $3.nombre, Lelse);
      char *cThen = $5.codigo;
      char *c2 = catf("\tgoto %s;\n", Lsalida);
      char *c3 = catf("%s:\n", Lelse);
      char *cElse = $7.codigo;
      char *c4 = catf("%s:\n", Lsalida);

      $$.codigo = concatena($3.codigo,
                     concatena(c1,
                       concatena(cThen,
                         concatena(c2,
                           concatena(c3,
                             concatena(cElse, c4))))));
  }
  ;

sentencia_while
  : MIENTRAS PIZQ expresion PDCHA HACER sentencia
  {
      expresionBooleana($3.tipo);

      char *Lentrada = nuevaEtiqueta();
      char *Lsalida  = nuevaEtiqueta();

      char *c1 = catf("%s:\n", Lentrada);
      char *c2 = concatena($3.codigo,
                    catf("\tif (!%s) goto %s;\n", $3.nombre, Lsalida));
      char *cCuerpo = $6.codigo;
      char *c3 = catf("\tgoto %s;\n", Lentrada);
      char *c4 = catf("%s:\n", Lsalida);

      $$.codigo = concatena(c1,
                     concatena(c2,
                       concatena(cCuerpo,
                         concatena(c3, c4))));
  }
  ;

/* switch*/
cabecera_case
  : PIZQ expresion PDCHA LLAVEI
  {
      if ($2.tipo != entero) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: expresión del 'case' debe ser entera\n",
                yylineno);
      }

      char *tempDisc = nuevaTemporal();
      char *decl = catf("\tint %s;\n", tempDisc);
      char *asig = catf("\t%s = %s;\n", tempDisc, $2.nombre);

      /* guardamos el código de la expresión + asignación a la temporal */
      $$.codigo = concatena($2.codigo, concatena(decl, asig));

      NombreVarControlActual   = tempDisc;         /* variable de control */
      EtiquetaSalidaCaseActual = nuevaEtiqueta();  /* etiqueta de salida global */
  }
  ;

sentencia_case
  : SEGUN cabecera_case lista_casos LLAVED
  {
      /* código = cabecera (variable switch) + código de todos los casos + etiqueta de salida */
      char *cSalida = catf("%s:\n", EtiquetaSalidaCaseActual);

      $$.codigo = concatena($2.codigo,
                     concatena($3.codigo, cSalida));

      /* Hemos acabado, ponemos a NULL por si acaso */
      NombreVarControlActual   = NULL;
      EtiquetaSalidaCaseActual = NULL;
  }
  ;

lista_casos
  : caso
      { $$.codigo = $1.codigo; }
  | lista_casos caso
      { $$.codigo = concatena($1.codigo, $2.codigo); }
  ;


caso
  /* caso con PARAR (break) */
  : CASO CONSTANTE_ENTERA DOSPUNTOS sentencia PARAR PUNCOMA
  {
      char *LsalidaCaso = nuevaEtiqueta();      /* etiqueta de salida del bloque */

      /* expresión del caso (constante entera) y comparación */
      char *tmpCmp = nuevaTemporal();
      char *decl = catf("\tint %s;\n", tmpCmp);
      char *cmp = catf("\t%s = (%s == %s);\n",
                        tmpCmp, NombreVarControlActual, $2.lexema);

      /* if (!tmpCmp) goto LsalidaCaso; */
      char *salto = catf("\tif (!%s) goto %s;\n", tmpCmp, LsalidaCaso);

      /* bloque de sentencias del caso */
      char *cuerpo = $4.codigo;

      /* PARAR => salto a la etiqueta de salida global del CASE */
      char *gotoFinCase = catf("\tgoto %s;\n", EtiquetaSalidaCaseActual);

      /* etiqueta de salida del bloque de caso */
      char *etiqSalidaCaso = catf("%s:\n", LsalidaCaso);

      $$.codigo = concatena(decl,
                      concatena(cmp,
                        concatena(salto,
                          concatena(cuerpo,
                            concatena(gotoFinCase, etiqSalidaCaso)))));
  }

  /* caso sin PARAR (sigue probando otros casos) */
  | CASO CONSTANTE_ENTERA DOSPUNTOS sentencia
  {
      char *LsalidaCaso = nuevaEtiqueta();

      char *tmpCmp = nuevaTemporal();
      char *decl   = catf("\tint %s;\n", tmpCmp);
      char *cmp    = catf("\t%s = (%s == %s);\n",
                          tmpCmp, NombreVarControlActual, $2.lexema);
      char *salto  = catf("  if (!%s) goto %s;\n", tmpCmp, LsalidaCaso);

      char *cuerpo = $4.codigo;
      char *etiqSalidaCaso = catf("%s:\n", LsalidaCaso);

      /* Si la condición es verdadera, se ejecuta el bloque y luego
         se continúa con el siguiente caso (no hay goto al final). */
      $$.codigo = concatena(decl,
                      concatena(cmp,
                        concatena(salto,
                          concatena(cuerpo, etiqSalidaCaso))));
  }

  /* OTRO (default) sin PARAR */
  | OTRO DOSPUNTOS sentencia
  {
      /* No se comprueba nada, se llega aquí si ningún caso anterior ha hecho goto fin */
      $$.codigo = $3.codigo;
  }

  /* OTRO con PARAR (default + break) */
  | OTRO DOSPUNTOS sentencia PARAR PUNCOMA
  {
      char *gotoFinCase = catf("\tgoto %s;\n", EtiquetaSalidaCaseActual);
      $$.codigo = concatena($3.codigo, gotoFinCase);
  }
  ;



/* ---------------- PROCEDIMIENTOS ---------------- */

/* llamada a procedimiento */
llamada_proc
  : ID PIZQ 
  {
    nParam = 0;
    subprogLlamada = TS_Buscar($1.lexema);
    if (subprogLlamada < 0) {
      fprintf(stderr,
              "*** [Linea %d] Error semántico: subprograma '%s' no declarado\n",
              yylineno, $1.lexema);
    } else if (TS[subprogLlamada].entrada != procedimiento) {
      fprintf(stderr,
              "*** [Linea %d] Error semántico: '%s' no es un procedimiento\n",
              yylineno, $1.lexema);
      subprogLlamada = -1;
    }
  }
  lista_arg_call_opt PDCHA
  { 
      if (subprogLlamada >= 0) {
        if (nParam != (int)TS[subprogLlamada].parametros) {
          fprintf(stderr,
                  "*** [Linea %d] Error semántico: número de argumentos incorrecto en llamada a '%s' (esperados %u, encontrados %d)\n",
                  yylineno, TS[subprogLlamada].nombre,
                  TS[subprogLlamada].parametros, nParam);
        }
      }
      subprogLlamada = -1;   /* limpiar por si acaso */

      /* esto no lo pide la practica */
      $$.codigo = strdup("  /* llamada a procedimiento no traducida a codigo intermedio */\n");
  }
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
  {
    if (subprogLlamada >= 0) {
        /* comprobar si nos pasamos de parametros */
        if (nParam>= (int)TS[subprogLlamada].parametros) {
          fprintf(stderr,
                  "*** [Linea %d] Error semántico: demasiados argumentos en llamada a '%s'\n",
                  yylineno, TS[subprogLlamada].nombre);
        } else {
          int idxFormal = subprogLlamada + 1 + nParam;
          dtipo tipoFormal = TS[idxFormal].tipoDato;

          /* Comprobamos tipos */
          if ($1.tipo == desconocido || $1.tipo == no_asignado || $1.tipo == vacio ||
              tipoFormal == desconocido || tipoFormal == no_asignado || tipoFormal == vacio ||
              $1.tipo != tipoFormal) {
            fprintf(stderr,
                    "*** [Linea %d] Error semántico: tipo incorrecto en argumento %d de llamada a '%s'\n",
                    yylineno, nParam+1, TS[subprogLlamada].nombre);
          }
        }
    }
    nParam++; 
  }
  ;


/* ---------------- EXPRESIONES ---------------- */

primario
  : PIZQ expresion PDCHA
      {
        $$.tipo = $2.tipo; 
        $$.dimensiones = $2.dimensiones; 
        $$.tam1 = $2.tam1;
        $$.tam2 = $2.tam2; 

        $$.codigo = $2.codigo;
        $$.nombre = $2.nombre;
      }
  | CONSTANTE_ENTERA
      {
        $$.tipo = entero; 
        $$.dimensiones = 0;
        $$.tam1 = 0;
        $$.tam2 = 0; 

        $$.codigo = strdup("");
        $$.nombre = strdup($1.lexema);
      }
  | CONSTANTE_REAL
      {
        $$.tipo = real; 
        $$.dimensiones = 0; 
        $$.tam1 = 0;
        $$.tam2 = 0; 

        $$.codigo = strdup("");
        $$.nombre = strdup($1.lexema);
      }
  | TRUE
      {
        $$.tipo = booleano; 
        $$.dimensiones = 0; 
        $$.tam1 = 0;
        $$.tam2 = 0;

        $$.codigo = strdup("");
        $$.nombre = strdup("1");
      }
  | FALSE
      {
        $$.tipo = booleano; 
        $$.dimensiones = 0; 
        $$.tam1 = 0;
        $$.tam2 = 0;

        $$.codigo = strdup("");
        $$.nombre = strdup("0");
      }
  | ID
      {
        int pos=existeVariable($1); 
        if (pos >= 0) {
          $$.tipo = TS[pos].tipoDato;
          $$.dimensiones = TS[pos].dimensiones;
          $$.tam1 = TS[pos].TamDimen1;
          $$.tam2 = TS[pos].TamDimen2; 
        }

        $$.codigo = strdup("");
        $$.nombre = strdup($1.lexema);
      }
  ;

/* acceso indexado a vectores/matrices*/
postfijo
  : primario              
  { 
    $$.tipo = $1.tipo;
    $$.dimensiones = $1.dimensiones; 
    $$.tam1 = $1.tam1;
    $$.tam2 = $1.tam2;

    $$.codigo = $1.codigo;
    $$.nombre = $1.nombre;
  }
  | ID indices            
  { 
    int pos=existeVariable($1); 
    if(TS[pos].dimensiones == 0 ){
      fprintf(stderr,
              "*** [Linea %d] Error semántico: Indexación sobre una variable que no es un array\n",
              yylineno);
    } else if(TS[pos].dimensiones==$2.atrib){
      $$.tipo=TS[pos].tipoDato; /*No devuelve el array sino el elemento*/
    } else {
      fprintf(stderr,
              "*** [Linea %d] Error semántico: Acceso incorrecto a '%s'. Esperados %d índices, encontrados %d.\n",
              yylineno, $1.lexema, TS[pos].dimensiones, $2.atrib);
    }

    /* Para la practica 5, tratamos el acceso como si fuese la variable base */
    $$.codigo = strdup("");
    $$.nombre = strdup($1.lexema);
  }
  ;

indices
  : indice
      { $$.atrib=1; }
  | indice indice
      { $$.atrib=2; }
  ;

indice
  : CORI expresion CORD
      { expresionEntera($2.tipo); }
  ;

unario 
  : postfijo
      {
        $$.tipo=$1.tipo;
        $$.dimensiones = $1.dimensiones; 
        $$.tam1 = $1.tam1;
        $$.tam2 = $1.tam2;

        $$.codigo = $1.codigo;
        $$.nombre = $1.nombre;
      }
  | OPERUNI unario
      {
        if(($2.tipo==real || $2.tipo==entero || $2.tipo == caracter) && $1.atrib != 0){
          $$.tipo=$2.tipo;
        }
        else if($2.tipo==booleano  && $1.atrib == 0){
          $$.tipo=$2.tipo;
        } else {
          fprintf(stderr,
                  "[Linea %d] Error semántico: Tipos de datos no concordantes. Esperados: entero o real. Encontrado: %d",
                  yylineno, $2.tipo);
        }

        /* P5: solo implementamos !x de forma explícita */
        if ($1.atrib == OU_NEG) {
          char *tmp = nuevaTemporal();
          char *decl = catf("\tint %s;\n", tmp);
          char *asig = catf("\t%s = !(%s);\n", tmp, $2.nombre);

          $$.codigo = concatena($2.codigo, concatena(decl, asig));
          $$.nombre = tmp;
        } else {
          /* el resto de operadores unarios los propagamos tal cual */
          $$.codigo = $2.codigo;
          $$.nombre = $2.nombre;
        }
      }
  | unario OPERUNI
      {
        if(($1.tipo==real || $1.tipo==entero || $1.tipo==caracter) &&
           $1.atrib != 0 && $1.atrib != 3 && $1.atrib != 4){
          $$.tipo=$1.tipo;
        }
        else{
          fprintf(stderr,
                  "[Linea %d] Error semántico: Tipos de datos no concordantes. Esperados: entero o real. Encontrado: %d",
                  yylineno, $1.tipo);
        }

        /* Para la parte obligatoria de la P5 no hay que generar código especial */
        $$.codigo = $1.codigo;
        $$.nombre = $1.nombre;
      }
  ;

expresion
  : unario
      {
        $$.tipo = $1.tipo;
        $$.dimensiones=$1.dimensiones;
        $$.tam1=$1.tam1;
        $$.tam2=$1.tam2;

        $$.codigo = $1.codigo;
        $$.nombre = $1.nombre;
      }
  | exp_o_cad OPERBIN exp_o_cad
      {   
        if ($1.tipo == $3.tipo    /* valor con valor */
            && ($1.tipo==real || $1.tipo==caracter|| $1.tipo==entero) 
            && ($1.dimensiones==0 && $3.dimensiones==0)){ 
              
          if($2.atrib>=6 && $2.atrib <=9){
            $$.tipo=booleano; 
            $$.dimensiones=0; 
            $$.tam1=0; 
            $$.tam2=0;
          }
          else {
            $$.tipo=$1.tipo;
            $$.dimensiones = 0;
            $$.tam1 = 0;
            $$.tam2 = 0;
          }
        }

        else if($1.tipo == $3.tipo    /* vector1D/vector2D con vector1D/vector2D */
                && ($1.tipo==real || $1.tipo==caracter|| $1.tipo==entero) 
                && ($1.dimensiones == $3.dimensiones)){

          if($2.atrib>=5 && $2.atrib <=13){
            fprintf(stderr,
                    "[Linea %d] Error semántico: No se admite operador binario con array",
                    yylineno);
          }
          else if($2.atrib==3){ /* Operador ** */
            if($1.tam2 == $3.tam1) {
              $$.tipo=$1.tipo; 
              $$.tam1 = $1.tam1; 
              $$.tam2 = $2.tam2;
              if($$.tam1==1 && $$.tam2==1){
                $$.dimensiones=0;
                $$.tam1=0;
                $$.tam2=0;
              } 
              else if($$.tam1==1){
                $$.dimensiones=1;
                $$.tam2=0;
              } else if($$.tam2==1){
                $$.dimensiones=1;
                $$.tam1=0;
              } else $$.dimensiones=2;
            }
            else{
              fprintf(stderr,
                      "*** [Linea %d] Error semántico: Las dimensiones de los arrays no satisfacen las condiciones para multiplicarlos",
                      yylineno);
            }
          }
          else {  /* Operadores +, -, *, / */
            if(($1.tam1 == $3.tam1) && ($1.tam2 == $3.tam2)) {
              $$.tipo=$1.tipo;
              $$.dimensiones = $1.dimensiones;
              $$.tam1 = $1.tam1;
              $$.tam2 = $1.tam2;
            }
            else{
              fprintf(stderr,
                      "[Linea %d] Error semántico: Los arrays no tienen las mismas dimensiones",
                      yylineno);
            }
          }
        }
  
        else if ($1.tipo == $3.tipo    /* valor con vector1D/vector2D o vector1D/vector2D con valor*/
                 && ($1.tipo==real || $1.tipo==caracter|| $1.tipo==entero) 
                 && ($1.dimensiones==0 || $3.dimensiones==0)){ 
              
          if(($2.atrib>=5 && $2.atrib <=13) || $2.atrib==3){
            fprintf(stderr,
                    "[Linea %d] Error semántico: No se admite operador binario con array y valor",
                    yylineno);
          }
          else {
            $$.tipo=$1.tipo;
            if ($1.dimensiones > 0) {
              $$.dimensiones = $1.dimensiones;
              $$.tam1 = $1.tam1;
              $$.tam2 = $1.tam2;
            }
            else if ($3.dimensiones > 0) {
              $$.dimensiones = $3.dimensiones;
              $$.tam1 = $3.tam1;
              $$.tam2 = $3.tam2;
            }
          }
        }
        else {
          fprintf(stderr,
                  "*** [Linea %d] Error semántico: Tipos de datos no concordantes",
                  yylineno);
        }

        
        if ($1.dimensiones == 0 && $3.dimensiones == 0 &&
            $1.tipo != desconocido && $3.tipo != desconocido) {

          char *tmp = nuevaTemporal();
          const char *op = "";

          switch ($2.atrib) {
            case OB_SUMA:    op = "+";  break;
            case OB_RESTA:   op = "-";  break;
            case OB_MULT:    op = "*";  break;
            case OB_DIV:     op = "/";  break;
            case OB_MOD:     op = "%";  break;
            case OB_MAYOR:   op = ">";  break;
            case OB_MAYORIG: op = ">="; break;
            case OB_MENOR:   op = "<";  break;
            case OB_MENORIG: op = "<="; break;
            case OB_MASIGUAL: op = "+="; break;
            case OB_MENOSIGUAL: op = "-="; break;
            case OB_PORIGUAL: op = "*="; break;
            case OB_DIVIGUAL: op = "/="; break;
            default:
              op = "/*op*/";
              break;
          }

          char *decl = catf("\tint %s;\n", tmp); 
          char *asig = catf("\t%s = %s %s %s;\n", tmp, $1.nombre, op, $3.nombre);

          $$.codigo = concatena($1.codigo,
                         concatena($3.codigo,
                                   concatena(decl, asig)));
          $$.nombre = tmp;
        } else {
          /* casos con arrays o errores */
          $$.codigo = concatena($1.codigo, $3.codigo);
          $$.nombre = strdup("/*exp*/");
        }
      }
  | exp_o_cad OPERLOG exp_o_cad
      {
        if ($1.tipo == $3.tipo && ($1.tipo==booleano) &&
            ($2.atrib == 0 || $2.atrib==1) &&
            $1.dimensiones ==0 && $3.dimensiones==0){ /* AND/OR */
          $$.tipo=booleano;
        }
        else if($1.tipo == $3.tipo &&
                $1.dimensiones ==0 && $3.dimensiones==0 &&
                ($2.atrib == 4 || $2.atrib==3)) { /* ==/!= */
          $$.tipo=booleano;
        }
        else {
          fprintf(stderr,
                  "*** [Linea %d] Error semántico: Tipos de datos no concordantes",
                  yylineno);
        }

        
        if ($1.dimensiones == 0 && $3.dimensiones == 0 &&
            $1.tipo != desconocido && $3.tipo != desconocido) {

          char *tmp = nuevaTemporal();
          const char *op = "";

          switch ($2.atrib) {
            case OL_Y:        op = "&&"; break;
            case OL_O:        op = "||"; break;
            case OL_IGUAL:    op = "=="; break;
            case OL_DISTINTO: op = "!="; break;
            default:
              op = "/*oplog*/"; break;
          }

          char *decl = catf("\tint %s;\n", tmp);
          char *asig = catf("\t%s = (%s %s %s);\n",
                            tmp, $1.nombre, op, $3.nombre);

          $$.codigo = concatena($1.codigo,
                         concatena($3.codigo,
                                   concatena(decl, asig)));
          $$.nombre = tmp;
        } else {
          $$.codigo = concatena($1.codigo, $3.codigo);
          $$.nombre = strdup("/*explog*/");
        }
      }
  ;

%%



/* para ejecutar el programa: ./semantico < "nombre del fichero con el main" */
int main(void){
  
  iniciarCodigo("salida.c");

  int res = yyparse();

  cerrarCodigo();
  return res;
}
