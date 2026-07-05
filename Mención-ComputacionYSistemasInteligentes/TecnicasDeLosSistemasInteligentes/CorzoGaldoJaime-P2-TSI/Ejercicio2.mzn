% Ejercicio2: Puzzle logico
% TSI - Practica2 
% Autor: Jaime Corzo Galdó

include "globals.mzn"; % Para all_different e inverse

% Para definir los arrays del proyecto, el aula y
% la hora de presentacion de cada estudiante necesitamos establecer el 
% conjunto de posiciones de cada array, para que no haya problemas, definimos sets
set of int: EST = 1..6;
set of int: PROY = 1..6;
set of int: AULA = 1..6;
set of int: HORA = 1..6;

% Definimos las constantes simplemente para cuando escribamos las restricciones, hacerlo
% mas legible ya que nos permitiria escrbir hora[ALBA] en vez de hora[1]
% Constantes para los estudiantes
int: ALBA  = 1;
int: BRUNO = 2;
int: CARLA = 3;
int: DIEGO = 4;
int: ELENA = 5;
int: PABLO = 6;

% Constantes para las aulas
int: A1 = 1;
int: A2 = 2;
int: B1 = 3;
int: B2 = 4;
int: C1 = 5;
int: C2 = 6;

% Como las horas no se usan directamente en las restricciones, salvo en las últimas dos, si no 
% que se usa como comparaciones entre horarios de proyectos, simplemente tenemos en cuenta que:
% 1 -> 15:00
% 2 -> 16:00
% 3 -> 17:00
% 4 -> 18:00
% 5 -> 19:00
% 6 -> 20:00

% El formato que se pide es: "Nombre": "Proyecto", "Aula", "Hora", luego necesitamos establecer
% estos strings para que salga bien la salida. Aquí podemos ver que ya usamos como posiciones de los arrays
% los sets definidos previamente
array[EST] of string: nombre_est = [
    "Alba",
    "Bruno",
    "Carla",
    "Diego",
    "Elena",
    "Pablo"
];

array[PROY] of string: nombre_proy = [
    "A* cuantico",
    "Demostracion matematica de que RTA* es un algoritmo absurdo",
    "Modelado de la incertidumbre a la hora de aprobar TSI sin estudiar nada",
    "Poda alfa-beta aplicada al encaje de bolillos",
    "Planificacion automatica jerarquica hibrida dedicada a la yuxtaposicion de los mundos paralelos",
    "Brazo robotico para jugar a la pocha"
];

array[AULA] of string: nombre_aula = [
    "A1", "A2", "B1", "B2", "C1", "C2"
];

array[HORA] of string: nombre_hora = [
    "15:00", "16:00", "17:00", "18:00", "19:00", "20:00"
];

% Para cada estudiante, el solver debe decidir: qué proyecto hace, en qué aula y a qué hora,
% luego creamos un array para cada uno
array[EST] of var PROY: proyecto;
array[EST] of var AULA: aula;
array[EST] of var HORA: hora;

% Cada proyecto, aula y hora se usa exactamente una vez
constraint all_different(proyecto);
constraint all_different(aula);
constraint all_different(hora);

% Hay restricciones que hablan desde la perspectiva del estudiante, como: Bruno es el primero en presentar.
% Sin embargo, para otras como por ejemplo: El proyecto 4 se presenta en el aula B2, si solo tuviesemos las 
% variables proyecto, aula y hora, seria mas ineficiente buscar que estudiante tiene el proyecto 4, ya que tendrias
% que hacer un forall de todos los estudiantes, mientras que con inverse lo que hacemos es que si se establece 
% que proyecto[ALBA] = 4, entonces se fuerza a que est_de_proy[4] = ALBA
% NOTA (USO DE IA): Para ver que se podía usar para establecer de forma más sencilla las restricciones que no
% implicaban directamente al estudiante, como el caso anterior, se ha hecho uso de la IA que ha sugerido el uso de inverse
array[PROY] of var EST: est_de_proy;
array[AULA] of var EST: est_de_aula;
array[HORA] of var EST: est_de_hora;

constraint inverse(proyecto, est_de_proy);
constraint inverse(aula, est_de_aula);
constraint inverse(hora, est_de_hora);

% Establecemos las restricciones

% Bruno es el primero en presentar
constraint hora[BRUNO] = 1;

% El proyecto 4 se presenta en el aula B2 y antes que el proyecto 1
constraint aula[est_de_proy[4]] = B2;
constraint hora[est_de_proy[4]] < hora[est_de_proy[1]];

% El proyecto 1 se presenta inmediatamente antes que el proyecto 3,
% en un aula que no es ni B2 ni A1
constraint hora[est_de_proy[1]] + 1 = hora[est_de_proy[3]];
constraint aula[est_de_proy[1]] != B2;
constraint aula[est_de_proy[1]] != A1;

% Diego presenta exactamente una hora despues que Alba
constraint hora[DIEGO] = hora[ALBA] + 1;

% Carla presenta exactamente una hora despues que Diego
constraint hora[CARLA] = hora[DIEGO] + 1;

% Elena presenta en C1, despues de Carla y antes de Pablo
constraint aula[ELENA] = C1;
constraint hora[ELENA] > hora[CARLA];
constraint hora[ELENA] < hora[PABLO];

% El proyecto 2 se presenta exactamente una hora despues que el proyecto 3,
% en un aula que no es A2
constraint hora[est_de_proy[2]] = hora[est_de_proy[3]] + 1;
constraint aula[est_de_proy[2]] != A2;

% El proyecto 3 se presenta en el aula A2
constraint aula[est_de_proy[3]] = A2;

% El proyecto 5 se presenta a las 19:00
constraint hora[est_de_proy[5]] = 5;

% El proyecto 6 se presenta en C2 a ultima hora
constraint aula[est_de_proy[6]] = C2;
constraint hora[est_de_proy[6]] = 6;

% Como estamos en un problema de CSP, usamos
solve satisfy;

% Salida
% Usamos fix para recuperar el valor asignado a las variables
% NOTA (USO DE IA): Para ver como incluir las comillas en el output se ha pedido ayuda a la IA
output
[
    "\"" ++ nombre_est[e] ++ "\": \"" ++ nombre_proy[fix(proyecto[e])] ++
    "\", \"" ++ nombre_aula[fix(aula[e])] ++
    "\", \"" ++ nombre_hora[fix(hora[e])] ++ "\"\n"
    | e in EST
];