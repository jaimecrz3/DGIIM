#!/bin/bash 
#echo "" >> salida.dat
i=1
while [ "$i" -le 59 ]
do
    ./especifico $i >> ./salida_especifico_59.dat
      i=$(( $i + 1 ))
done
      
