#!/bin/bash 
#echo "" > salida_backt_25_c1.dat

i=2
while [ "$i" -le 18 ]
do
    ./branchandbound 2 $i >> ./salida_branch_c2.dat
      i=$(( $i + 1 ))
done
      
