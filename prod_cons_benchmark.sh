#!/bin/bash

file_name="prod_cons_result_$(date +"%Y-%m%d_%T")"
TIMEFORMAT=%R
array_cons=("PROD_CONS_Nc=1" "PROD_CONS_Nc=2" "PROD_CONS_Nc=4" "PROD_CONS_Nc=8" \
            "PROD_CONS_Nc=1" "PROD_CONS_Nc=1" "PROD_CONS_Nc=1")
array_prod=("PROD_CONS_Np=1" "PROD_CONS_Np=1" "PROD_CONS_Np=1" "PROD_CONS_Np=1" \
            "PROD_CONS_Np=2" "PROD_CONS_Np=4" "PROD_CONS_Np=8")

for N in 1 10 100 1000; do
    echo "N=$N" >> $file_name
    for config in $(seq 0 6); do
        echo "conf=${array_prod[$config]} ${array_cons[$config]}" >> $file_name
        make PROD_CONS_N=$N ${array_cons[$config]} ${array_prod[$config]} prod_cons
        for round in $(seq 0 9); do 
            { time ./prod_cons ; } 2>> $file_name
        done;
        make clean
    done
done