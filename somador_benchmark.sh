#!/bin/bash

for N in 10000000 100000000 1000000000; do
    for K in 1 2 4 8 16 32 64 128 256; do
        TEMPO=$(./somador $N $K)
        MEDIA=$((TEMPO/10))
        echo "Numeros Gerados: $N Numero de Threads:$K Media: $MEDIA"
        echo -n "$MEDIA " >> medias2.txt
    done
    echo "" >> medias2.txt
done