% Ejercicio1: Problema de la fórmula de una bebida isotónica
% Apartado b
% TSI - Practica2 
% Autor: Jaime Corzo Galdó

% Parametro de entrada: volumen total buscado (en ml)
% Dejar solo un opcion descomentada cada vez
int: VOLUMEN_TOTAL_ML = 200;
%int: VOLUMEN_TOTAL_ML = 1000;
%int: VOLUMEN_TOTAL_ML = 5000;
%int: VOLUMEN_TOTAL_ML = 10000;

% Ingredientes, elegimos como estructura un enum ya que son
% valores que no se van a poder modificar, y además nos va permitir
% crear arrays indexados por este enum.
enum ING = {
  AGUA_BASE, JARABE_A, JARABE_B,
  CONCENTRADO_SALINO, EXTRACTO_ENERGETICO, AROMA_CITRICO
};

% Como cada cantidad de ingrediente debe expresarse en multiplos de 10ml,
% vamos a trabajar de la siguiente manera, si una cantidad de ingrediente vale 1,
% quiere decir que vale 10ml, si vale 2, quiere decir que vale 20ml, etc.
% Es decir, que tenemos que dividir por 10.
% Además como las cantidades totales son multiplos de 10 no hay problema
int: V10 = VOLUMEN_TOTAL_ML div 10;

% En este array llamado x, es donde se estable para cada ingrediente la cantidad que va a tener
array[ING] of var 0..V10: x;

% Establecemos ahora la composicion nutricional por cada 100ml, como hemos establecido que cada 
% variable representa 10ml, tenemos que dividir cada valor de la tabla del enunciado por 10, para quedarnos
% con el valor por unidadpero como nos piden usar solos valores int, vamos a usar, para el coste: decimas de centimos,
% para el azucar: decimas de gramos y para el sodio y la cafeina: decimas de mg
array[ING] of int: coste = [
 2, 12, 10, 8, 20, 6
];

array[ING] of int: azucar = [
 0, 18, 12, 0, 4, 2
];

array[ING] of int: sodio = [
 0, 40, 20, 180, 10, 5
];

array[ING] of int: cafeina = [
 0, 0, 0, 0, 30, 0
];

% Apartir de estos valores, obtenemos el coste total (decimas de centimo), azucar total(decimas de gramos) y 
% sodio total y cafeina total (decimas de mg) multiplicando los costes anteriores por las respectivas cantidades
% de cada ingrediente con valores entre 0 y V10
var int: coste_total = sum(i in ING)(coste[i] * x[i]);
var int: azucar_total = sum(i in ING)(azucar[i] * x[i]);
var int: sodio_total = sum(i in ING)(sodio[i] * x[i]);
var int: cafeina_total = sum(i in ING)(cafeina[i] * x[i]);

% Establecemos ahora la restriccion del volumen total
constraint sum(i in ING)(x[i]) = V10;

% Para establecer las restricciones nutricionales, como se nos dan valores para cada 1000ml, 
% eso correponde con valores cada 100 unidades, ya que hmos dicho que si una cantidad de ingrediente
% vale 1, corresponde a 10 ml, ahora tomando como ejemplo el azucar, sabemos que en la variable azucar_total
% tenemos almacenado cuanto azucar usamos en decimas de gramos. Luego queremos que:
% ((70*10) * V10) / 100 <= azucar_total <= ((85*10) * V10) / 100
% Analogo con sodio y cafeina
constraint 700 * V10 <= 100 * azucar_total;
constraint 100 * azucar_total <= 850 * V10;

constraint 4000 * V10 <= 100 * sodio_total;
constraint 100 * sodio_total <= 4750 * V10;

constraint 600 * V10 <= 100 * cafeina_total;
constraint 100 * cafeina_total <= 900 * V10;

% Nos faltaria añadir las siguientes restricciones

% El aroma citrico debe usarse obligatoriamente entre el 3% y el 20%
constraint 100 * x[AROMA_CITRICO] >= 3 * V10;
constraint 100 * x[AROMA_CITRICO] <= 20 * V10;

% El extracto energetico no puede superar el 40%
constraint 100 * x[EXTRACTO_ENERGETICO] <= 40 * V10;

% Debe utilizarse exactamente uno de los dos jarabes
% NOTA (USO DE IA): para comprobar que se podia usar xor he pedido ayuda a la IA
constraint (x[JARABE_A] > 0) xor (x[JARABE_B] > 0);

% Si se usa Jarabe A, al menos el 5% del volumen total
constraint x[JARABE_A] = 0 \/ (100 * x[JARABE_A] >= 5 * V10);

% Si se usa concentrado salino, al menos el 3% del volumen total
constraint x[CONCENTRADO_SALINO] = 0 \/ (100 * x[CONCENTRADO_SALINO] >= 3 * V10);

% Como mucho pueden usarse 4 ingredientes distintos
constraint sum(i in ING)(x[i] > 0) <= 4;

% Como ahora queremos minimizar el coste total, estamos en un problema COP
solve minimize coste_total;

% Salida
% NOTA (USO DE IA): para ajustar el formato de salida al que se pide en el guion he pedido ayuda a la IA
output
[
    "Cantidades usadas:\n",
    "------------------\n"
]
++
[
    show(i) ++ ": " ++ show(10 * x[i]) ++ " ml\n"
    | i in ING
]
++
[
    "Coste total = " ++ show(coste_total div 10) ++ "." ++ show(coste_total mod 10) ++ " centimos\n",
    "Azucar total = " ++ show(azucar_total div 10) ++ "." ++ show(azucar_total mod 10) ++ " g\n",
    "Sodio total = " ++ show(sodio_total div 10) ++ "." ++ show(sodio_total mod 10) ++ " mg\n",
    "Cafeina total = " ++ show(cafeina_total div 10) ++ "." ++ show(cafeina_total mod 10) ++ " mg\n"
];




