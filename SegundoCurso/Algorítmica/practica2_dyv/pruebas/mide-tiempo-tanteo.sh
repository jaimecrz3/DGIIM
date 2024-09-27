#!/bin/bash 
#echo "" >> salida.dat
i=50000
while [ "$i" -le 1250000 ]
do
    ./dyv_con_umbral $i >> ./umbral_tanteo/salida_tanteo_1000.dat
      i=$(( $i + 50000))
done
      
