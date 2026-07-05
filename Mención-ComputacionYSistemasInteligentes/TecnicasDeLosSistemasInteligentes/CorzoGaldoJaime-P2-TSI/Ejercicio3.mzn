% Ejercicio3: Problema de la mochila multiobjetivo con optimización lexicográfica
% TSI - Practica2 
% Autor: Jaime Corzo Galdó

% Objetos, elegimos como estructura un enum ya que son
% valores que no se van a poder modificar, y además nos va permitir
% crear arrays indexados por este enum
enum OBJ = {
    GPS,CAMARA,ORDENADOR,BATERIA_EXTRA,SENSOR_TERMICO,
    BOTIQUIN,RADIO,PANEL_SOLAR
};

% Necesitamos almacenar para cada objeto, su peso, coste y utilidad,
% luego creamos un array para cada uno de ellos indexado por el enum OBJ
array[OBJ] of int: peso = [
    3,  % GPS
    2,  % Camara
    4,  % Ordenador
    2,  % Bateria extra
    5,  % Sensor termico
    3,  % Botiquin
    4,  % Radio
    6   % Panel solar
];

array[OBJ] of int: coste = [
    8,   % GPS
    6,   % Camara
    10,  % Ordenador
    4,   % Bateria extra
    12,  % Sensor termico
    5,   % Botiquin
    7,   % Radio
    9    % Panel solar
];

array[OBJ] of int: utilidad = [
    10,  % GPS
    9,   % Camara
    14,  % Ordenador
    7,   % Bateria extra
    16,  % Sensor termico
    8,   % Botiquin
    11,  % Radio
    13   % Panel solar
];

% Para resolver el problema tenemos que saber que objetos se han seleccionado
% y cuales no, para ello creamos un array donde cada posicion indica si el objeto 
% ha sido seleccionado si x[o] = 1, 0 si no se ha seleccionado
array[OBJ] of var 0..1: x;

% Para algunas restricciones y para los resultados finales necesitamos almacenar el peso total
% la utilidad total y el coste total de la solucion
var int: utilidad_total = sum(o in OBJ)(utilidad[o] * x[o]);
var int: peso_total     = sum(o in OBJ)(peso[o] * x[o]);
var int: coste_total    = sum(o in OBJ)(coste[o] * x[o]);

% Ya podemos establecer las restricciones del problema

% Peso maximo
constraint peso_total <= 15;

% Dependencia tecnica:
% si se selecciona el Ordenador, tambien la Bateria extra
% Lo que nos dice esta restriccion es que si x[ORDENADOR] = 1, entonces x[BATERIA_EXTRA] = 0, 
% pero se puede seleccionar la bateria extra sin necesidad de seleccionar el ordenador 
constraint x[ORDENADOR] <= x[BATERIA_EXTRA];

% Compatibilidad energetica:
% si se selecciona el Sensor termico, tambien el Panel solar
% Mismo razonamiento que en la anterior restriccion
constraint x[SENSOR_TERMICO] <= x[PANEL_SOLAR];

% Seguridad minima:
% el Botiquin es obligatorio
constraint x[BOTIQUIN] = 1;

% El problema pide diseñar una funcion ponderada para aplicar el siguiente criterio
% de optimizacion lexicografica:
% Primero, maximizar la utilidad total; entre todas las soluciones de utilidad
% máxima, minimizar el peso total; y, entre todas las soluciones anteriores, minimizar el coste total.
%
% Para ello, creamos una funcion ponderda para convertir este criterio lexicografico en una funcion objetivo
% a maximizar como parte de un COP, es decir queremos maximizar:
%   1) utilidad_total
%   2) minimizar peso_total
%   3) minimizar coste_total
%
% Para ello construimos la siguiente funcion:
%   objetivo = A * utilidad_total - B * peso_total - coste_total
%
% Lo que hemos hecho es restar el coste porque queremos minimizarlo (el coste maximo de llevarlo todo es 61),
% luego restamos el peso, ya que tambien queremos minimizarlo, pero lo multiplicamos por B = MAX_COSTE_TOTAL + 1 = 62,
% de esta manera nos aseguramos de que ahorrar 1 sola unidad de peso (que valdrá 62 puntos) siempre será mejor 
% matemáticamente que ahorrar todo el coste del mundo (que como máximo vale 61). Por último, se suma la utilidad total,
% ya que queremos maximizarla, y multiplicamos por A = MAX_PESO_TOTAL * B + MAX_COSTE_TOTAL + 1 = 1860, de esta manera nos
% aseguramos que ganar 1 sola unidad de utilidad otorgará 1860 puntos, superando cualquier posible penalización combinada 
% de peso y coste. 
%
% NOTA (USO DE IA): Para saber como funciona una función ponderada para optimizacion lexicografica, se ha pedido ayuda de la
% IA, ya que desconocía este término.
int: MAX_PESO_TOTAL = sum(o in OBJ)(peso[o]);         % 29
int: MAX_COSTE_TOTAL = sum(o in OBJ)(coste[o]);       % 61

% Una unidad menos de peso debe compensar cualquier cambio de coste
int: B = MAX_COSTE_TOTAL + 1;                         % 62

% Una unidad mas de utilidad debe compensar cualquier diferencia
% posible en el termino de peso y coste
int: A = MAX_PESO_TOTAL * B + MAX_COSTE_TOTAL + 1;    % 1860

var int: objetivo = A * utilidad_total - B * peso_total - coste_total;

% Como estamos en un problema de COP, usamos
solve maximize objetivo;

% Salida
output
[
    "Objetos seleccionados:\n"
]
++
[
    if fix(x[o]) = 1 then
        "- " ++ show(o) ++
        " (peso=" ++ show(peso[o]) ++
        ", coste=" ++ show(coste[o]) ++
        ", utilidad=" ++ show(utilidad[o]) ++ ")\n"
    else
        ""
    endif
    | o in OBJ
]
++
[
    "Utilidad total = " ++ show(utilidad_total) ++ "\n",
    "Peso total = " ++ show(peso_total) ++ "\n",
    "Coste total = " ++ show(coste_total) ++ "\n"
];