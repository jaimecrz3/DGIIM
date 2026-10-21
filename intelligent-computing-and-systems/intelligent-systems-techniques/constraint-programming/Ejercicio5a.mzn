% Ejercicio5: Problema de planificación de tareas
% Apartado a
% TSI - Practica2 
% Autor: Jaime Corzo Galdó

include "globals.mzn"; % Para forall, max y sum

% Definimos un enumerado para las tareas, que nos va a servir tambien 
% para indexar distintas estructuras 
enum TAREA = {A, B, C, D, E, F, G, H, I, J, K};

% Descripciones de tareas para la salida
array[TAREA] of string: desc = [
    "Montar chasis",
    "Instalar ruedas",
    "Cableado electrico",
    "Motor de fusion nuclear",
    "Tablero de control",
    "Instalacion y configuracion del condensador de fluzo (flux capacitor)",
    "Ajuste aerodinamico",
    "Instalacion de puertas de ala de gaviota",
    "Panel de tiempo",
    "Implementacion y validacion de algoritmos de busqueda heuristica para optimizacion de caminos",
    "Prueba de plutonio"
];

% Duraciones de cada tarea para cada uno de los dos trabajadores
array[TAREA] of int: dur_marty = [10, 1, 5, 15, 12, 13, 7, 4, 2, 4, 4];
array[TAREA] of int: dur_doc   = [16, 2, 4,  5,  4,  5, 6, 3, 2, 2, 3];

% Biff no puede apoyar en las tareas D y F, por lo que su ayuda la indicamos 
% como 0, en la tarea A, se reduce en 3 dias, en la tarea H, en 2 dias, y en el resto,
% en 1 dia
array[TAREA] of int: red_biff = [3, 1, 1, 0, 1, 0, 1, 2, 1, 1, 1];

% Similar a un ejemplo que viene en las diapositivas, establecemos las tareas predecesoras a cada tarea
array[TAREA] of set of TAREA: preds = [
    {},         % A
    {A},        % B
    {B},        % C
    {A, C},     % D
    {C},        % E
    {C, D, E},  % F
    {C, D},     % G
    {E, G},     % H
    {F, H},     % I
    {G, I},     % J
    {H, I}      % K
];

% Definimos el peor escenario posible para acotar el dominio. Seria el caso
% en que cada tarea se hiciese en el mayor tiempo y sin apoyos. De esta manera
% reducimos el espacio de búsqueda
int: T = 84;

% Para codificar si una tarea la realiza marty o doc, lo hacemos con un booleano
% de esta manera solo necesitamos una variable, que cuando sea true quiere decir que la tarea
% correspondiente la realiza marty y si es false doc
array[TAREA] of var bool: es_marty;      
array[TAREA] of var bool: usa_biff;
array[TAREA] of var bool: usa_george;

% Tiempo de inicio de cada tarea, acotamos su dominio con la constante T previamente creada
array[TAREA] of var 1..T: inicio;
% Creamos una variable para la duracion de cada tarea y la duracion base, que es la duracion de la tarea
% si no hay apoyo, esta es necesaria ya que Biff solo puede apoyar en tareas cuya duración sea al menos de
% 3 días y George solo puede ayudar en tareas asignadas a Marty cuya duración original para Marty sea
% de al menos 2 días, lugo necesitamos saber esa duracion original
% Como la tarea que tarda mas tiempo de todas es montar chasis que son 16 dias, acotamos el dominio usando ese valor
array[TAREA] of var 1..16: duracion;
array[TAREA] of var 1..16: duracion_base;
% Tiempo de fin de cada tarea
array[TAREA] of var 1..T: fin;

% Esta va a ser la variable a minimizar, el tiempo total en montar el DeLorean
var 1..T: tiempo_total;

% Impoemos la estriccion de que la tarea A comienza en el dia 1
constraint inicio[A] = 1;

% Establecemos la duracion base de las tareas segun el trabajador que las realize.
% En vez de declararlo con un if-else, lo linealizamos usando que es_marty es booleano,
% de esta manera para el solver es mucho mas eficiente
%
% NOTA (USO DE IA): Para hacer esta restriccion mas optima se ha pedido ayuda a la IA 
constraint forall(t in TAREA)(
    duracion_base[t] =
        dur_doc[t] + (dur_marty[t] - dur_doc[t]) * es_marty[t]
);

% Ahora establecemos la duracion real de las tareas, que va a depender de si marty o doc reciben ayuda de george o Biff
constraint forall(t in TAREA)(
    duracion[t] = duracion_base[t] - red_biff[t] * (usa_biff[t]) - (usa_george[t])
);

constraint forall(t in TAREA)(
    fin[t] = inicio[t] + duracion[t] - 1
);

% Como hay tareas que se pueden hacer en paralelo, no podemos minimizar el tiempo total
% de realizar todas las tareas, lo que queremos es acabar el proyecto lo antes posible, y de
% esta manera el solver probara todas las combinaciones de turnos y trabajadores, y se quedara con la
% organizacion que haga que el tiempo en el que se acaba la ultima tarea sea el mas bajo
constraint tiempo_total = max([fin[t] | t in TAREA]);

% Imponemos la restriccion de que para realizar una tarea, primero se tienen que haber completado
% sus predecesoras, esto se traduce a que si una tarea predecesora empieza en el dia x y dura d dias,
% la siguiente tarea puede empezar en x+d
constraint forall(t in TAREA, p in preds[t])(
    inicio[t] >= inicio[p] + duracion[p]
);

% Se pide que ningun trabajador puede realizar tareas simultaneamente, luego se tiene que cumplir que
% [inicio_tarea1, inicio_tarea1+duracion_tarea1-1] y [inicio_tarea2, inicio_tarea2+duracion_tarea2-1] tengan interseccion vacia
% Establecemos el orden tarea1 < tarea2 para evitar que se compruebe la pareja (B, A) si ya se ha comprobado la pareja (A, B)
% Marty
constraint forall(t1, t2 in TAREA where t1 < t2)( 
    (es_marty[t1] /\ es_marty[t2]) -> (inicio[t1] + duracion[t1] <= inicio[t2]) \/ (inicio[t2] + duracion[t2] <= inicio[t1])
);

% Doc
constraint forall(t1, t2 in TAREA where t1 < t2)(
    ((not es_marty[t1]) /\ (not es_marty[t2])) -> (inicio[t1] + duracion[t1] <= inicio[t2]) \/ (inicio[t2] + duracion[t2] <= inicio[t1])
);


% Restricciones de Biff
% Maximo 4 tareas en total
constraint sum(t in TAREA)(usa_biff[t]) <= 4;

% No puede ayudar en D ni F
constraint usa_biff[D] = false;
constraint usa_biff[F] = false;

% Solo en tareas cuya duracion base para el trabajador asignado sea al menos 3
constraint forall(t in TAREA)(
    usa_biff[t] -> duracion_base[t] >= 3
);

% Como mucho 2 apoyos a Marty y 2 a Doc
constraint sum(t in TAREA)(usa_biff[t] /\ es_marty[t]) <= 2;
constraint sum(t in TAREA)(usa_biff[t] /\ not es_marty[t]) <= 2;

% Biff no puede apoyar tareas simultaneas
constraint forall(t1, t2 in TAREA where t1 < t2)(
    (usa_biff[t1] /\ usa_biff[t2]) -> (inicio[t1] + duracion[t1] <= inicio[t2]) \/ (inicio[t2] + duracion[t2] <= inicio[t1])
);

% Restricciones de George
% George solo ayuda a Marty
constraint forall(t in TAREA)(
    usa_george[t] -> es_marty[t]
);

% George solo puede ayudar en tareas asignadas a Marty cuya duracion original para Marty sea al menos 2 dias
constraint forall(t in TAREA where dur_marty[t] < 2)(
    usa_george[t] = false
);

% George no puede coincidir con Biff en la misma tarea
constraint forall(t in TAREA)(
    not (usa_biff[t] /\ usa_george[t])
);

% George no puede apoyar tareas simultaneas
constraint forall(t1, t2 in TAREA where t1 < t2)(
    (usa_george[t1] /\ usa_george[t2]) -> (inicio[t1] + duracion[t1] <= inicio[t2]) \/ (inicio[t2] + duracion[t2] <= inicio[t1])
);

% Como estamos en un problema de COP, usamos
solve minimize tiempo_total;

% Salida
% NOTA (USO DE IA): Para dejar el codigo de la salida mas limpio se ha pedido ayuda a la IA
function string: nombre_trab(bool: b) =
    if b then "Marty" else "Doc" endif;

function string: texto_apoyo(bool: biff, bool: george) =
    if biff then
        " con apoyo de Biff"
    elseif george then
        " con apoyo de George"
    else
        ""
    endif;

output
[
    "Duracion minima = " ++ show(tiempo_total) ++ " dias\n\n"
]
++
[
    "\"" ++ show(t) ++ " - " ++ desc[t] ++ "\" inicio dia \"" ++ show(inicio[t]) ++
    "\" fin dia \"" ++ show(fin[t]) ++ "\". Asignada a \"" ++
    nombre_trab(fix(es_marty[t])) ++ "\"" ++
    texto_apoyo(fix(usa_biff[t]), fix(usa_george[t])) ++ "\n"
    | t in TAREA
];