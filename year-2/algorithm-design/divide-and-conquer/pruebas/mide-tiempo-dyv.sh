#!/bin/bash 
#echo "" >> salida.dat
i=1
while [ "$i" -le 59 ]
do
    ./dyv $i >> ./salida_dyv_59.dat
      i=$(( $i + 1 ))
done
      
