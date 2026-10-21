#!/bin/bash 
#echo "" >> salida.dat
i=50
while [ "$i" -le 2000 ]
do
    ./generador $i
    ./prim "ficheroDeSalida.txt" >> ./salida_prim_2000.dat
      i=$(( $i + 10 ))
done
      
