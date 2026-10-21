#!/bin/bash 

./generador 20
./backtracking ficheroDeSalida.txt 1 >> ./salida_back_24_c1.dat
./backtracking ficheroDeSalida.txt 2 >> ./salida_back_24_c2.dat
./backtracking ficheroDeSalida.txt 3 >> ./salida_back_24_c3.dat
      
