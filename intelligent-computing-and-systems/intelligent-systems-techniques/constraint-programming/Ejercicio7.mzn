% Ejercicio7: Planificación de horarios de juego en una semana de la NBA
% TSI - Practica2 
% Autor: Jaime Corzo Galdó

include "globals.mzn";

% El problema se modela como un CSP:
% - Variables: dia y franja de cada partido
% - Dominios: dias 1..7 y franjas {Tarde, Noche, Prime Time}
% - Restricciones: todas las condiciones del enunciado

% Conjuntos que nos van a servir para definir los indices de distintas estructuras
set of int: PARTIDO = 1..12;
set of int: EQUIPO = 1..10;
set of int: DIA = 1..7;
set of int: FRANJA = 1..3; % 1=Tarde, 2=Noche, 3=Prime Time

% Constantes para hacer el codigo mas legible
int: TARDE = 1;
int: NOCHE = 2;
int: PRIME = 3;

% Establecemos qué equipo juega como local y cuál como visitante en cada partido
% Codificacion de equipos:
% 1=BOS, 2=NYK, 3=PHI, 4=MIA, 5=MIL, 6=LAL, 7=GSW, 8=PHX, 9=DEN, 10=DAL
array[PARTIDO] of int: local = [
    1, 4, 5, 6, 8, 10, 7, 2, 3, 9, 1, 4       
];

array[PARTIDO] of int: visitante = [
    2, 3, 1, 7, 9, 6, 8, 4, 5, 10, 6, 7
];

% Este array nos va a servir para mostrar por pantalla los resultados
array[PARTIDO] of string: cod_partido = [
    "BOS-NYK", "MIA-PHI", "MIL-BOS", "LAL-GSW", "PHX-DEN", "DAL-LAL",
    "GSW-PHX", "NYK-MIA", "PHI-MIL", "DEN-DAL", "BOS-LAL", "MIA-GSW"
];

% participa[e,p] indica si el equipo e juega el partido p.
%
% No es una variable de decision, sino un dato derivado de local y visitante.
% Por eso lo declaramos como bool normal, no como var bool
%
% En lugar de introducir estos datos a mano, los calculamos por comprensión
% comprobando si el equipo actúa como local o como visitante. 
% Dado que esta operación nos devuelve una lista plana (de una sola dimensión), 
% empleamos la función array2d para reestructurar esos resultados y darle 
% la forma de matriz bidimensional (EQUIPO x PARTIDO) que necesitamos
%
% NOTA (USO DE LA IA): Iba a hacer esto con matrices (por extensión) como hice con el ejercicio 6,
% pero como queda una matriz muy grande la IA me ha sugerido que una manera alternativa es hacerlo por compresión, 
% es decir, dando una regla o fórmula para que el solver lo calcule.
array[EQUIPO, PARTIDO] of bool: participa =
    array2d(EQUIPO, PARTIDO,
        [ local[p] = e \/ visitante[p] = e | e in EQUIPO, p in PARTIDO ]
    );

% Variables de decision:
%
% Asignamos el dia y franja de cada partido. De esta manera garantizamos que cada 
% partido tiene asociado exactamente un unico dia y una unica franja.
array[PARTIDO] of var DIA: dia_partido;
array[PARTIDO] of var FRANJA: franja_partido;

% Necesitamos almacenar si un equipo juega un día concreto y si tiene un back-to-back
% (jugar dos días seguidos). Usamos variables auxiliares.
%
% juega[e,d] sera true si el equipo e juega algun partido el dia d.
%
% b2b[e,d] sera true si el equipo e juega los dias d y d+1.
array[EQUIPO, DIA] of var bool: juega;
array[EQUIPO, 1..6] of var bool: b2b;

constraint forall(e in EQUIPO, d in DIA)(
    juega[e,d] <-> exists(p in PARTIDO where participa[e,p]) (
        dia_partido[p] = d
    )
);

constraint forall(e in EQUIPO, d in 1..6)(
    b2b[e,d] <-> (juega[e,d] /\ juega[e,d+1])
);

% Imponemos la restriccion de que un equipo no puede jugar más de un partido el mismo día.
% Establecemos el orden p1 < p2 para evitar comprobaciones redundantes.
constraint forall(e in EQUIPO, p1, p2 in PARTIDO
                  where p1 < p2 /\ participa[e,p1] /\ participa[e,p2]) (
    dia_partido[p1] != dia_partido[p2]
);

% Imponemos que en una misma franja horaria puede haber como mucho 2 partidos.
constraint forall(d in DIA, f in FRANJA)(
    sum(p in PARTIDO)(bool2int(dia_partido[p] = d /\ franja_partido[p] = f)) <= 2
);

% En un mismo día puede haber como mucho 4 partidos.
constraint forall(d in DIA)(
    sum(p in PARTIDO)(bool2int(dia_partido[p] = d)) <= 4
);

% Imponemos que el dia 4 no puede haber partidos y que el dia 7 no puede haber partidos en Prime Time.
constraint forall(p in PARTIDO)(
    dia_partido[p] != 4 /\ 
    not(dia_partido[p] = 7 /\ franja_partido[p] = PRIME)
);

% Lakers (6), Celtics (1) y Warriors (7) no pueden jugar en Tarde.
constraint forall(p in PARTIDO)(
    (participa[1,p] \/ participa[6,p] \/ participa[7,p]) -> franja_partido[p] != TARDE
);

% Imponemos el numero de partidos por franja.
% Al ser 12 partidos, si Tarde > Noche > Prime, y Prime >= 3,
% matematicamente solo puede ser 5, 4 y 3.
constraint count(franja_partido, TARDE) = 5;
constraint count(franja_partido, NOCHE) = 4;
constraint count(franja_partido, PRIME) = 3;

% Partidos fijados por la liga y disponibilidad de pabellones

% Partido 4: Lakers vs Warriors, dia 2 en Prime Time
constraint dia_partido[4] = 2 /\ franja_partido[4] = PRIME;

% Partido 11: Celtics vs Lakers, dia 6 en Prime Time
constraint dia_partido[11] = 6 /\ franja_partido[11] = PRIME;

% Partido 3: Bucks vs Celtics, tambien en Prime Time
constraint franja_partido[3] = PRIME;

% Knicks vs Heat no puede jugarse el dia 5
constraint dia_partido[8] != 5;

% Suns vs Nuggets no puede jugarse el dia 2
constraint dia_partido[5] != 2;

% Bucks vs Celtics no puede jugarse el dia 1
constraint dia_partido[3] != 1;

% Nuggets vs Mavericks no puede jugarse el dia 6
constraint dia_partido[10] != 6;

% Restricciones de back-to-backs.
% Un equipo como mucho tiene 1 b2b.
constraint forall(e in EQUIPO)(
    sum(d in 1..6)(bool2int(b2b[e,d])) <= 1
);

% Calculamos el numero total de back-to-backs, para restringirlo y mostrarlo en la salida.
var int: back_to_backs_totales =
    sum(e in EQUIPO, d in 1..6)(bool2int(b2b[e,d]));

% Globalmente hay como mucho 2 back-to-backs.
constraint back_to_backs_totales <= 2;

% Celtics (1) y Warriors (7) no tienen b2b.
constraint sum(d in 1..6)(bool2int(b2b[1,d])) = 0;
constraint sum(d in 1..6)(bool2int(b2b[7,d])) = 0;

% Los dias 5 y 6 deben tener al menos 3 partidos cada uno.
constraint count(dia_partido, 5) >= 3;
constraint count(dia_partido, 6) >= 3;

% Incompatibilidad de franjas consecutivas.
% Si un equipo juega en Prime Time un día, no puede jugar por la Tarde al día siguiente.
constraint forall(e in EQUIPO, d in 1..6)(
    (exists(p in PARTIDO where participa[e,p]) (
        dia_partido[p] = d /\ franja_partido[p] = PRIME
    ))
    -> 
    not(exists(q in PARTIDO where participa[e,q]) (
        dia_partido[q] = d + 1 /\ franja_partido[q] = TARDE
    ))
);

% Como estamos en un problema de CSP, usamos solve satisfy.
solve satisfy;

% Salida
% NOTA (USO DE IA): para ajustar el formato de salida al que se pide en el guion y dejarlo bonito he pedido ayuda a la IA.

% FUNCIONES AUXILIARES PARA EL OUTPUT
%
% contenido_celda(f,d) devuelve un string con los partidos que se
% juegan en la franja f y dia d. Si no hay partidos, devuelve "-".
%
% concat:
% concat recibe una lista de strings y las concatena en un unico string.
% Aqui se usa solo para formatear la tabla de salida, no para imponer
% restricciones del problema.
%
% fix:
% fix se usa en el output para recuperar el valor concreto asignado
% por el solver a una variable de decision. Esto aparece explicado
% en la presentacion de la asignatura en la parte de formateo de salida.
%
% celda_dia:
% Como MiniZinc no permite calcular length(s) sobre strings, no podemos
% rellenar espacios en funcion de la longitud del texto. En su lugar,
% aprovechamos que una celda solo puede contener 0, 1 o 2 partidos
% porque el enunciado limita a 2 partidos por franja. Con ese numero
% decidimos cuantos espacios añadir para que la tabla quede alineada.
function string: contenido_celda(int: f, int: d) =
    if sum(p in PARTIDO)(bool2int(fix(dia_partido[p]) = d /\ fix(franja_partido[p]) = f)) = 0 then
        "-"
    else
        concat([
            if fix(dia_partido[p]) = d /\ fix(franja_partido[p]) = f then
                cod_partido[p] ++
                if exists(q in PARTIDO where q > p)(
                    fix(dia_partido[q]) = d /\ fix(franja_partido[q]) = f
                ) then
                    " /"
                else
                    ""
                endif
            else
                ""
            endif
            | p in PARTIDO
        ])
    endif;

% celda_dia(f,d) devuelve la celda completa de una franja y un dia.
% La anchura de cada celda de dia es fija:
% - si no hay partidos: "-";
% - si hay un partido: codigo de 7 caracteres, por ejemplo BOS-NYK;
% - si hay dos partidos: dos codigos separados por " /", por ejemplo NYK-MIA /PHI-MIL.
function string: celda_dia(int: f, int: d) =
    let {
        int: n = sum(p in PARTIDO)(
            bool2int(fix(dia_partido[p]) = d /\ fix(franja_partido[p]) = f)
        )
    } in
        if n = 0 then
            " -                |"
        elseif n = 1 then
            " " ++ contenido_celda(f,d) ++ "          |"
        else
            " " ++ contenido_celda(f,d) ++ " |"
        endif;

% Linea horizontal de la tabla.
string: LINEA_TABLA =
    "+------------+------------------+------------------+------------------+------------------+------------------+------------------+------------------+\n";

% Salida en formato de tabla, siguiendo el ejemplo del enunciado.
output [
    "CALENDARIO SEMANAL NBA (CSP)\n",
    "============================\n",
    "Back-to-backs totales = " ++ show(back_to_backs_totales) ++ "\n",
    LINEA_TABLA,
    "| Sesion     | Dia 1            | Dia 2            | Dia 3            | Dia 4            | Dia 5            | Dia 6            | Dia 7            |\n",
    LINEA_TABLA,
    "| Tarde      |" ++ celda_dia(TARDE,1) ++ celda_dia(TARDE,2) ++ celda_dia(TARDE,3) ++ celda_dia(TARDE,4) ++ celda_dia(TARDE,5) ++ celda_dia(TARDE,6) ++ celda_dia(TARDE,7) ++ "\n",
    LINEA_TABLA,
    "| Noche      |" ++ celda_dia(NOCHE,1) ++ celda_dia(NOCHE,2) ++ celda_dia(NOCHE,3) ++ celda_dia(NOCHE,4) ++ celda_dia(NOCHE,5) ++ celda_dia(NOCHE,6) ++ celda_dia(NOCHE,7) ++ "\n",
    LINEA_TABLA,
    "| Prime Time |" ++ celda_dia(PRIME,1) ++ celda_dia(PRIME,2) ++ celda_dia(PRIME,3) ++ celda_dia(PRIME,4) ++ celda_dia(PRIME,5) ++ celda_dia(PRIME,6) ++ celda_dia(PRIME,7) ++ "\n",
    LINEA_TABLA,
    "Leyenda:\n",
    "BOS=Celtics, NYK=Knicks, PHI=Sixers, MIA=Heat, MIL=Bucks,\n",
    "LAL=Lakers, GSW=Warriors, PHX=Suns, DEN=Nuggets, DAL=Mavericks\n"
];