%{
  #include <stdio.h>
  #include <stdlib.h>
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

/*  Precedencias y asociatividad */
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
  : LLAVEI               { TS_InsertaMARCA(); } 
    cabecera_bloque 
    lista_sentencias 
    LLAVED               { TS_VaciarENTRADAS(); }
  ;



cabecera_bloque
  :
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
  /* escalar solo */
  : ID
      {
        dimTmp = 0;
        tam1Tmp = tam2Tmp = 0;
        TS_InsertaIDENT($1);
      }

  /* escalar + más cosas detrás */
  | ID COMA lista_id
      {
        dimTmp = 0;
        tam1Tmp = tam2Tmp = 0;
        TS_InsertaIDENT($1);
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

  /* array 2D estilo C: ID [tam] [tam] */
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

decl
  : TIPO                   { tipoTmp = $1.tipo; }
    lista_id 
    PUNCOMA 
  | error PUNCOMA          { yyerror("declaración"); yyerrok; }
  ;

tam
  : CONSTANTE_ENTERA        
  ;

seccion_procs
  : declar_proc
  | seccion_procs declar_proc
  ;


declar_proc
  : cabecera_proc          { subProg=1; } 
    bloque                 { subProg=0; }
  | cabecera_proc error    { yyerror("bloque de procedimiento"); yyerrok; }
  ;

cabecera_proc
  : PROCD ID               { TS_InsertaSUBPROG($2); }
    PIZQ lista_param PDCHA  
  ;

lista_param
  : 
  | parametros
  | parametros COMA error  { yyerror("parámetro"); yyerrok; } 
  ;

parametros
  : parametro
  | parametros COMA parametro
  ;


parametro
  /* parámetro escalar por valor */
  : TIPO ID
    {
      tipoTmp = $1.tipo;   /* tipo del parámetro */
      dimTmp  = 0;
      TS_InsertaPARAMF($2);
    }

  /* parámetro escalar por referencia (OPERUNI ID) */
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
  : 
  | sentencia lista_sentencias
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
  { 
    comprobarAsignacion($1, $3.tipo);
    int pos = existeVariable($1); 
    if(TS[pos].dimensiones != 0){
      if((TS[pos].dimensiones != $3.dimensiones) || (TS[pos].TamDimen1 < $3.tam1) || (TS[pos].TamDimen2 < $3.tam2)){
        fprintf(stderr,
                  "*** [Linea %d] Error semántico: asignación de vectores de distinto tamaño\n",
                  yylineno);
      }
    }
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
  }

asignacion2D
  : LLAVEI lista_exp_o_cad LLAVED
  {
    int numCols = $2.atrib;
    filasInit2D++;

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
  ;

lista_vars_opt
  : lista_vars
  ;

lista_vars
  :
  | ID                     { existeVariable($1); } 
  | lista_vars COMA ID     { existeVariable($3); }
  ;

/* escritura admite expresiones y cadenas */
sentencia_salida
  : ESCRITURA lista_exp_o_cad_escritura
  ;

lista_exp_o_cad_escritura
  : exp_o_cad
  | lista_exp_o_cad_escritura COMA exp_o_cad
  ;

lista_exp_o_cad
  : exp_o_cad
  {
      $$.tipo  = $1.tipo;  
      $$.atrib = 1;        /* número de elementos */
  }
  | lista_exp_o_cad COMA exp_o_cad
  {
      $$.atrib = $1.atrib + 1; 

      /* Comprobamos que no mezclas tipos raros en la lista */
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
    $$.dimensiones=$1.dimensiones;
    $$.tam1=$1.tam1;
    $$.tam2=$1.tam2;
  }
  | CADENA      { $$.tipo = caracter; }
  ;

sentencia_if
  : SI PIZQ expresion PDCHA sentencia %prec MENOR_QUE_SINO  
  {
      expresionBooleana($3.tipo);
  }               
  | SI PIZQ expresion PDCHA sentencia SINO sentencia
  {
      expresionBooleana($3.tipo);
  }
  ;

sentencia_while
  : MIENTRAS PIZQ expresion PDCHA HACER sentencia
  {
      expresionBooleana($3.tipo);
  }
  ;

/* switch*/
sentencia_case
  : SEGUN PIZQ expresion PDCHA LLAVEI lista_casos LLAVED
  {
      if ($3.tipo != entero) {
        fprintf(stderr,
                "*** [Linea %d] Error semántico: expresión del 'case' debe ser entera\n",
                yylineno);
      }
  }
  ;

lista_casos
  : caso
  | lista_casos caso
  ;

caso
  : CASO CONSTANTE_ENTERA DOSPUNTOS sentencia PARAR PUNCOMA
  | CASO CONSTANTE_ENTERA DOSPUNTOS sentencia
  | OTRO DOSPUNTOS sentencia
  | OTRO DOSPUNTOS sentencia PARAR PUNCOMA
  ;


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
  }
  ;

lista_arg_call_opt
  :
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
  : PIZQ expresion PDCHA  { $$.tipo = $2.tipo; 
                            $$.dimensiones = $2.dimensiones; 
                            $$.tam1 = $2.tam1; $$.tam2 = $2.tam2; 
                          }
  | CONSTANTE_ENTERA      { $$.tipo = entero; 
                            $$.dimensiones = 0; /* Es un escalar */
                            $$.tam1 = 0; $$.tam2 = 0; 
                          }
  | CONSTANTE_REAL        { $$.tipo = real; 
                            $$.dimensiones = 0; 
                            $$.tam1 = 0; $$.tam2 = 0; 
                          }
  | TRUE                  { $$.tipo = booleano; 
                            $$.dimensiones = 0; 
                            $$.tam1 = 0; $$.tam2 = 0;
                          }
  | FALSE                 { $$.tipo = booleano; 
                            $$.dimensiones = 0; 
                            $$.tam1 = 0; $$.tam2 = 0;
                          }
  | ID                    { int pos=existeVariable($1); 
                            if (pos >= 0) {
                              $$.tipo = TS[pos].tipoDato;
                              $$.dimensiones = TS[pos].dimensiones;
                              $$.tam1 = TS[pos].TamDimen1;
                              $$.tam2 = TS[pos].TamDimen2;; 
                            }
                          }
  ;

/* acceso indexado a vectores/matrices*/
postfijo
  : primario              { $$.tipo = $1.tipo;
                            $$.dimensiones = $1.dimensiones; 
                            $$.tam1 = $1.tam1; $$.tam2 = $1.tam2;
                          }
  | ID indices            
  { int pos=existeVariable($1); 
    if(TS[pos].dimensiones == 0 ){
      fprintf(stderr,
                  "*** [Linea %d] Error semántico: Indexación sobre una variable que no es un array\n",
                  yylineno);
    }else if(TS[pos].dimensiones==$2.atrib){
      $$.tipo=TS[pos].tipoDato; /*No devuelve el array sino de lo que es el array*/
    }else{
      fprintf(stderr, "*** [Linea %d] Error semántico: Acceso incorrecto a '%s'. Esperados %d índices, encontrados %d.\n", yylineno,
                      $1.lexema, TS[pos].dimensiones, $2.atrib);
    } 
  }
  ;

indices
  : indice {$$.atrib=1;}
  | indice indice {$$.atrib=2;}
  ;

indice
  : CORI expresion CORD   { expresionEntera($2.tipo); }
  ;

unario 
  : postfijo {  $$.tipo=$1.tipo;
                $$.dimensiones = $1.dimensiones; 
                $$.tam1 = $1.tam1; $$.tam2 = $1.tam2;
              }
  | OPERUNI unario {if(($2.tipo==real || $2.tipo==entero || $2.tipo == caracter) && $1.atrib != 0){
  			              $$.tipo=$2.tipo;
                    }
                    else if($2.tipo==booleano  && $1.atrib == 0){
                      $$.tipo=$2.tipo;
                    }else{
                      fprintf(stderr, "[Linea %d] Error semántico: Tipos de datos no concordantes. Esperados: entero o real. Encontrado: %d", yylineno, $2.tipo);
                    }
  		    }
  | unario OPERUNI {if(($1.tipo==real || $1.tipo==entero || $1.tipo==caracter) &&  $1.atrib != 0 && $1.atrib != 3 && $1.atrib != 4){
  			              $$.tipo=$1.tipo;
                    }
                    else{
                      fprintf(stderr, "[Linea %d] Error semántico: Tipos de datos no concordantes. Esperados: entero o real. Encontrado: %d", yylineno, $1.tipo);
                    }}
  ;

expresion
  : unario {$$.tipo = $1.tipo;
            $$.dimensiones=$1.dimensiones;
            $$.tam1=$1.tam1;
            $$.tam2=$1.tam2;}
  | exp_o_cad OPERBIN exp_o_cad {   
                                if ($1.tipo == $3.tipo    /* valor con valor */
                                    && ($1.tipo==real || $1.tipo==caracter|| $1.tipo==entero) 
                                    && ($1.dimensiones==0 && $3.dimensiones==0)){ 
                                      
                                  if($2.atrib>=6 && $2.atrib <=9){
                                    $$.tipo=booleano; 
                                    $$.dimensiones=0; 
                                    $$.tam1=0; 
                                    $$.tam2=0;}
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

                                  if($2.atrib>=5 && $2.atrib <=13){ fprintf(stderr, "[Linea %d] Error semántico: No se admite operador binario con array", yylineno);}
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
                                      }else if($$.tam2==1){
                                        $$.dimensiones=1;
                                        $$.tam1=0;
                                      } 
                                      else $$.dimensiones=2;
                                    }
                                    else{
                                      fprintf(stderr, "*** [Linea %d] Error semántico: Las dimensiones de los arrays no satisfacen las condiciones para multiplicarlos", yylineno);
                                    }
                                  }
                                  else { /* Operador +, -, *, / */
                                    if(($1.tam1 == $3.tam1) && ($1.tam2 == $3.tam2)) {
                                      $$.tipo=$1.tipo;
                                      $$.dimensiones = $1.dimensiones;
                                      $$.tam1 = $1.tam1;
                                      $$.tam2 = $1.tam2;
                                    }
                                    else{
                                       fprintf(stderr, "[Linea %d] Error semántico: Los arrays no tienen las mismas dimensiones", yylineno);
                                    }
                                  }
                                }
                      
                                else if ($1.tipo == $3.tipo    /* valor con vector1D/vector2D o vector1D/vector2D con valor*/
                                         && ($1.tipo==real || $1.tipo==caracter|| $1.tipo==entero) 
                                         && ($1.dimensiones==0 || $3.dimensiones==0)){ 
                                      
                                  if(($2.atrib>=5 && $2.atrib <=13) || $2.atrib==3){ fprintf(stderr, "[Linea %d] Error semántico: No se admite operador binario con array y valor", yylineno);}
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
                                  fprintf(stderr, "*** [Linea %d] Error semántico: Tipos de datos no concordantes", yylineno);
                                } 
  				                    }

  | exp_o_cad OPERLOG exp_o_cad {
                                  if ($1.tipo == $3.tipo && ($1.tipo==booleano) && ($2.atrib == 0 || $2.atrib==1) && $1.dimensiones ==0 && $3.dimensiones==0){ // AND/OR
                                    $$.tipo=booleano;
                                  }
                                  else if($1.tipo == $3.tipo && $1.dimensiones ==0 && $3.dimensiones==0 && ($2.atrib == 4 || $2.atrib==3)) { // ==/!=
                                    $$.tipo=booleano;
                                  }
                                  else {
                                    fprintf(stderr, "*** [Linea %d] Error semántico: Tipos de datos no concordantes", yylineno);
                                  }
                                }  
  ;

%%


// para ejecutar: ./semantico < <nombre del fichero que queremos pasarle>

int main(void){
  return yyparse();
}
