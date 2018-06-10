#!/bin/bash

if [ $# != 1 ]
then
    echo "Mauvaise synthaxe: ./exec.sh nom_du_fichier"
    exit
fi

VAR=$1

if [ $VAR = *.s ]
then
    ./asm_to_hex $VAR
    VAR=`echo ${VAR//.s/.hex}`
fi

./Simulateur2000 $VAR
