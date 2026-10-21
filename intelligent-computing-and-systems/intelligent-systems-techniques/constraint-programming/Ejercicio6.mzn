% Ejercicio6: Problema de conformación de tribunales de defensa de TFG
% TSI - Practica2 
% Autor: Jaime Corzo Galdó

include "globals.mzn";

% Conjuntos para indexar las distintas estructuras
set of int: PROF = 1..10;
set of int: TFG = 1..12;
set of int: TRIB = 1..3;
set of int: DEP = 1..3;      % 1=DECSAI, 2=LSI, 3=ICAR
set of int: DIA = 1..5;      % 1=Lun, 2=Mar, 3=Mie, 4=Jue, 5=Vie
set of int: SES = 1..2;      % 1=Manana, 2=Tarde

% Nombres para la salida
array[DEP] of string: nombre_dep = ["DECSAI", "LSI", "ICAR"];
array[DIA] of string: nombre_dia = ["Lunes", "Martes", "Miercoles", "Jueves", "Viernes"];
array[SES] of string: nombre_ses = ["Manana", "Tarde"];

% Establecemos el departamento de cada profesor
% 1..4 DECSAI, 5..7 LSI, 8..10 ICAR
array[PROF] of int: depto = [
    1, 1, 1, 1, 2, 2, 2, 3, 3, 3       
];

% Establecemos la disponiblidad que tiene cada profesor
array[PROF, DIA] of bool: disp_dia =
%  L,      M,     X,     J,     V
[|false, true,  false, false, false,  % p1
 |true,  false, true,  false, true,   % p2
 |true,  false, true,  false, false,  % p3
 |false, false, true,  true,  false,  % p4
 |true,  false, false, true,  false,  % p5
 |true,  true,  true,  true,  true,   % p6
 |false, false, false, false, true,   % p7
 |false, false, true,  true,  false,  % p8
 |false, false, false, true,  false,  % p9
 |true,  true,  false, false, true   % p10
|];

% Establecemosla disponibilidad por sesion de cada profesor
% 1,5,8 solo mañana
% 2,4,6 solo tarde
% resto ambas
array[PROF, SES] of bool: disp_ses =
%  M,      T
[|true,  false,   % p1
 |false, true,    % p2
 |true,  true,    % p3
 |false, true,    % p4
 |true,  false,   % p5
 |false, true,    % p6
 |true,  true,    % p7
 |true,  false,   % p8
 |true,  true,    % p9
 |true,  true,   % p10
|];

% Nos queda almacenar a informacion relativa a si un profesor ha dirigido un TFG, ya que en
% ese caso no puede formar parte del tribunal que lo evalúe. 
% director[p, t] = true si el profesor p ha dirigido/codirigido el TFG t
%
% NOTA (USO DE IA): Para crear estas matrices, debido a que se trata de un trabajo simple pero costoso
% se ha delegado en una IA
array[PROF, TFG] of bool: dirige =
% TFG 1..12
[|false, false, false, false, false, false, false, false, true,  true,  true,  false, % p1
 |true,  false, false, false, false, false, false, false, false, true,  false, false, % p2
 |false, false, false, false, false, false, true,  true,  false, false, false, false, % p3
 |false, false, false, true,  false, true,  false, false, false, false, false, true,  % p4
 |false, true,  false, false, false, false, false, false, false, false, false, false, % p5
 |false, false, true,  false, true,  false, false, false, false, false, false, false, % p6
 |false, false, true,  false, true,  false, false, false, false, false, false, false, % p7
 |false, false, false, true,  false, false, true,  false, false, false, false, false, % p8
 |false, false, false, false, false, false, false, false, false, false, false, false, % p9
 |false, false, false, false, false, false, false, false, true,  false, true,  false, % p10
|];

% Necesitamos almacenar los miembros de cada uno de lo 3 tribunales. Para asegurarnos que haya uno de cada 
% departamento, usamos una matriz Tribuna y  Departamento, de esta manera garantizamos que haya exactamente un asiento 
% para DECSAI (d=1), uno para LSI (d=2) y uno para ICAR (d=3)
% miembros[t,d] = profesor del tribunal t del departamento d
array[TRIB, DEP] of var PROF: miembros;

% Asgnamos el dia y sesion de cada tribunal. Para no tener que añadir restricciones extra para obligar a que los tres 
% profesores del mismo tribunal coincidan en el mismo dia y sesion, asignamos la hora y sesion por tribunal, y los profesores
% tienen que estar disponibles ese dia y sesion
array[TRIB] of var DIA: dia_trib;
array[TRIB] of var SES: ses_trib;

% Por ultimo tenemos que almacenar a que tribunal corresponde cada TFG, hemos preferido crear un array donde 
% tribunal_tfg[x] = tribunal que evalua el TFG x, ya que asi estamos ya estableciendo que cada TFG tiene 
% asociado un unico tribunal
array[TFG] of var TRIB: tribunal_tfg;

% Para evitar simetrias y establecer correctamente que si estamos en la columna 1,
% corresponde con el departmento DECSAI, al que pertenecen los profesores 1-4, si estamos en la columna 2,
% corresponde con el departmento LSI, al que pertenecen los profesores 5-7 y por ultimo, si estamos en 
% la columna 3, corresponde con el departmento ICAR, al que pertenecen los profesores 8-10, restringimos
% el espacio de busqueda, de esta manera, el solver no intentara poner al profesor 10 (ICAR) en el asiento 1 (DECSAI)
constraint forall(t in TRIB)(
    miembros[t,1] in 1..4 /\
    miembros[t,2] in 5..7 /\
    miembros[t,3] in 8..10
);

% Imponemos la restriccion de que un profesor solo puede aparecer en un tribunal,
% salvo el profesor 10, que puede aparecer en 2 como maximo
constraint forall(p in 1..9)(
    sum(t in TRIB, d in DEP)(miembros[t,d] = p) <= 1
);

constraint sum(t in TRIB, d in DEP)(miembros[t,d] = 10) <= 2;

% Imponemos que cada tribunal tenga miembros compatibles con su dia y sesion
constraint forall(t in TRIB, d in DEP)(
    disp_dia[miembros[t,d], dia_trib[t]] /\ disp_ses[miembros[t,d], ses_trib[t]]
);

% Imponemos que un mismo profesor no puede tener dos tribunales el mismo día
% Lo que hacemos es que dados cualesquiera dos tribunales cuyos dias coincidan, ninguno de sus miembros pueden
% coincidir, como sabemos cada uno de los tres miembros del tribunal a que departamento pertenecen, solo
% tenemos hacer tres comprobaciones por cada dos tribunales que tengan el mismo dia
% Establecemos el orden t1 < t2 para evitar que se compruebe que si el dia de t1 = dia de t2 y luego al reves, 
% ya que se trata de la misma comprobacion
constraint forall(t1, t2 in TRIB where t1 < t2)(
    (dia_trib[t1] = dia_trib[t2]) -> forall(d1 in DEP, d2 in DEP)(miembros[t1,d1] != miembros[t2,d2])
);

% Cada tribunal evalua exactamente 4 TFGs. Aunque el enunciado dice que pueden evaluar entre 4 y 6 TFGs,
% al haber 3 tribunales y 12 TFGs, es imposible que un tribunal evalue mas de 4 TFGs, si todos tienen que
% evaluar 4 TFGs como minimo.
constraint forall(t in TRIB)(
    count(tribunal_tfg, t) = 4
);

% TFG 6 y 12 en el mismo tribunal
% TFG 1 y 10 en el mismo tribunal
constraint tribunal_tfg[6] = tribunal_tfg[12];
constraint tribunal_tfg[1] = tribunal_tfg[10];

% Incompatibilidad director / codirector con el tribunal
% Ningun profesor de un tribunal puede haber dirigido un TFG asignado a ese tribunal
constraint forall(x in TFG, t in TRIB, d in DEP)(
    (tribunal_tfg[x] = t) -> not dirige[miembros[t,d], x]
);

% Para romper simetris entre tribunales, vamos a realizar una idea parecida a la
% implementada en el Ejercicio4.mzn. Por un lado, el problema de que se permuten los profesores
% dentro de un mismo tribunal esta ya solucionado al fijar que cada posicion corresponde a un departamento.
% Algo que hemos hecho con la variables miembros y con el constraint de restringir el dominio
% de los profesores que pueden estar en el asiento de cada departamento a sus valores correspondiente.
%
% Por otro lado, seguimos teniendo la siguiente simetría: Sol1: Tribunal 1 evalúa el lunes, Tribunal 2 el marte y
% Sol2: Tribunal 1 pasa a ser el martes y Tribunal 2 el lunes. Para nostros es la misma solución, pero para el ordenador 
% son ramas de búsqueda diferentes, multiplicando el tiempo de ejecución innecesariamente.
%
% Por ello, forzamos a que el Tribunal 1 sea "menor o igual" al Tribunal 2 basándose en su día, sesión y miembros, 
% obligando al solver a ordenar las soluciones.
% Lo que hacemos es comparar los dias de los dos tribunales, y nos lo quedamos como solucion si el dia del primero es menor
% que el del segundo, si son iguales, pasamos a comparar las sesiones, y asi sucesivamente.
constraint forall(t in 1..2)(
    lex_lesseq(
        [dia_trib[t], ses_trib[t], miembros[t,1], miembros[t,2], miembros[t,3]],
        [dia_trib[t+1], ses_trib[t+1], miembros[t+1,1], miembros[t+1,2], miembros[t+1,3]]
    )
);

% Como estamos en un problema de CSP, usamos
solve satisfy;

% Salida
% NOTA (USO DE IA): para ajustar el formato de salida al que se pide en el guion y dejarlo bonito he pedido ayuda a la IA
output
[
    "Tribunal " ++ show(t) ++ ": \"" ++ show(miembros[t,1]) ++ "\"(\"" ++ nombre_dep[depto[fix(miembros[t,1])]] ++ "\"), " ++
    "\"" ++ show(miembros[t,2]) ++ "\"(\"" ++ nombre_dep[depto[fix(miembros[t,2])]] ++ "\"), " ++
    "\"" ++ show(miembros[t,3]) ++ "\"(\"" ++ nombre_dep[depto[fix(miembros[t,3])]] ++ "\") " ++
    "(Dia: \"" ++ nombre_dia[fix(dia_trib[t])] ++ "\", Sesion: \"" ++ nombre_ses[fix(ses_trib[t])] ++ "\")\n" ++
    "TFGs a evaluar: \"" ++
    concat([ if fix(tribunal_tfg[x]) = t
             then show(x) ++ if exists(y in (x+1)..12)(fix(tribunal_tfg[y]) = t) then ", " else "" endif
             else ""
             endif
             | x in TFG ]) ++ "\"\n"
    | t in TRIB
];