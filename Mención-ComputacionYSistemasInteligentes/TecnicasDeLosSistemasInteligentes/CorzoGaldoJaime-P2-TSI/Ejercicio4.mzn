% Ejercicio4: Problema del cuadrado mágico
% TSI - Practica2 
% Autor: Jaime Corzo Galdó

include "globals.mzn"; % Para forall, nvalue y lex_lesseq

% Parametro de entrada: N, representa el tamaño de la matriz cuadrada NxN
% Dejar solo una opcion descomentada
int: N = 3;
%int: N = 4;
%int: N = 5;
% int: N = 6;
%int: N = 7;

% Valor maximo permitido en cada celda
int: MAXV = 100;

% Conjuntos que nos van a servir para definir los indices de distintas estructuras
set of int: S = 1..N;
set of int: V = 1..MAXV;

% El enunciado nos dice que si
% -> N es par, todos los valores deben ser pares.
% -> N es impar, todos los valores deben ser impares.
% Luego podemos hacer una reducción de dominio, de esta manera, reducimos el
% espacio de búsqueda a la mitad
%
% NOTA (USO DE IA): Para ver como codificar esta idea en Minizinc se ha hecho uso de la IA
set of int: VALORES_VALIDOS =
    if N mod 2 = 0 then
        {v | v in V where v mod 2 = 0}
    else
        {v | v in V where v mod 2 = 1}
    endif;

% Establecemos el minimo numero de valores distintos que tiene que haber en la matriz,
% que son al menos la mitad de todos los valores. Si el número de valores es impar, por ejemplo 
% en una matriz 3x3 tenemos 9 valores, tomamos el entero inmediatamente superior, y para evitar 
% decimales, sumamos 1 al tamaño de la matriz antes de dividir entre 2.
int: MIN_DISTINTOS = (N * N + 1) div 2;

% Definimos el cuadrado magico como una matriz
array[S, S] of var VALORES_VALIDOS: matriz;

% Se pide que la suma de los elementos de cada fila y de cada columna, 
% y la suma de los elementos de la diagonal principal y de la diagonal secundaria,
% proporcionen el mismo valor, para ello, en vez de establecer una restriccion 
% Como los valores de la matriz ya estan fijados, hacemos uso de ello para reducir el
% espacio de busqueda del solver, estableciendo la suma comun a la suma de los elementos 
% de la primera fila (por ejemplo). De esta manera nos evitamos tener que definir la
% suma comun como una variable libre y que el solver tenga que probar valores que no 
% estan permitidos
var int: suma_comun = sum(j in S)(matriz[1, j]);
% La suma total es la suma de todos los valores de la matriz
var int: suma_total = N * suma_comun;

% Establecemos ahora las restricciones
% Filas y columnas con la misma suma
constraint forall(i in S)(
    sum(j in S)(matriz[i, j]) = suma_comun
);

constraint forall(j in S)(
    sum(i in S)(matriz[i, j]) = suma_comun
);

% Diagonales con la misma suma
constraint sum(i in S)(matriz[i, i]) = suma_comun;
constraint sum(i in S)(matriz[i, N - i + 1]) = suma_comun;

% Establecemos el numero de valores distintos en la matriz debe ser mayor o igual que
% MIN_DISTINTOS, para ello he encontrado una funcion nativa que cuenta el numero de valores
% distintos en un array, y  para que se cumpla la restriccion de que este numero tiene que ser mayor 
% o igual que MIN_DISTINTOS, simplemente acotamos el dominio de valores que puede tomar la variable,
% descaratando cualquier otro caso
var MIN_DISTINTOS..(N*N): num_distintos;
constraint nvalue(num_distintos, [matriz[i, j] | i in S, j in S]);

% Por ultimo nos queda romper las posibles simetria que pueden aparecer, ya que de lo contrario,
% estaríamos realizando un proceso de buqueda excesivamente complejo.
% En este caso, tenemos 8 variaciones geométricas equivalentes (la matriz original, 3 rotaciones y 4 reflejos).
% Para romper la simetría, tenemos que fijar un orden, que va a ser el siguiente:
% Si teenmos dos matrices que son cada una, una varacion geometrica de la otra, vamos a quedarnos con aquella
% que concatenando sus filas para obtener un array 1D, tenga un valor mas pequeño. Ejemplo:
% Si tenemos la matriz A (Original): [8, 1, 6, 3, 5, 7, 4, 9, 2] y la Matriz B (Rotada): [6, 7, 2, 1, 5, 9, 8, 3, 4]  
% La matriz B va a ser mas pequeña ya que 6 < 8, luego descartamos la A. Si ambos valores hubiesen sido iguales,
% se pasa a comparar la siguiente posicion.
% Reptiendo este procedimiento para cada una de las 8 variaciones geometricas nos aseguramos con quedarnos
% con solo una matriz representante de la clase
% 
% NOTA (USO DE IA): Para crear las 7 trasformaciones geometricas equivalentes a partir de la matriz original
% se ha hecho uso de la IA, además de para ver como establecer la restriccion de quedarnos con la menor
% de todas las variaciones geometricas, para lo que nos ha proporcionado la funcion lex_lesseq,que hace
% precisamente eso: compara dos listas de números posición por posición y comprueba que la primera sea
% menor que la segunda
array[1..N*N] of var VALORES_VALIDOS: orig =
    [matriz[i, j] | i in S, j in S];

array[1..N*N] of var VALORES_VALIDOS: rot90 =
    [matriz[N - j + 1, i] | i in S, j in S];

array[1..N*N] of var VALORES_VALIDOS: rot180 =
    [matriz[N - i + 1, N - j + 1] | i in S, j in S];

array[1..N*N] of var VALORES_VALIDOS: rot270 =
    [matriz[j, N - i + 1] | i in S, j in S];

array[1..N*N] of var VALORES_VALIDOS: refl_h =
    [matriz[N - i + 1, j] | i in S, j in S];

array[1..N*N] of var VALORES_VALIDOS: refl_v =
    [matriz[i, N - j + 1] | i in S, j in S];

array[1..N*N] of var VALORES_VALIDOS: refl_diag_princ =
    [matriz[j, i] | i in S, j in S];

array[1..N*N] of var VALORES_VALIDOS: refl_diag_sec =
    [matriz[N - j + 1, N - i + 1] | i in S, j in S];

constraint lex_lesseq(orig, rot90);
constraint lex_lesseq(orig, rot180);
constraint lex_lesseq(orig, rot270);
constraint lex_lesseq(orig, refl_h);
constraint lex_lesseq(orig, refl_v);
constraint lex_lesseq(orig, refl_diag_princ);
constraint lex_lesseq(orig, refl_diag_sec);

% Como estamos en un problema de COP, usamos
solve minimize suma_total;

% Salida 
% NOTA (USO DE IA): Para mostrar por pantalla la matriz se ha pedido ayuda a la IA
output [
    "N = " ++ show(N) ++ "\n" ++
    "MAXV = " ++ show(MAXV) ++ "\n" ++
    "Minimo de valores distintos exigido = " ++ show(MIN_DISTINTOS) ++ "\n" ++
    "Numero de valores distintos = " ++ show(num_distintos) ++ "\n" ++
    "Suma comun de filas/columnas/diagonales = " ++ show(suma_comun) ++ "\n" ++
    "Suma total minimizada = " ++ show(suma_total) ++ "\n" ++
    "Matriz:\n"
] ++
[
    show(matriz[i, j]) ++ if j < N then " " else "\n" endif
    | i in S, j in S
];