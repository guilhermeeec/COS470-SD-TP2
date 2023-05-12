#include <time.h>
#include <stdlib.h>
#include "utils.h"

int checar_primo(int numero) {
    if (numero == 1) {
        return NAO_PRIMO;
    }
    for (int idx = 2; idx <= numero/2; idx++) {
        if (!(numero % idx)) {
            return NAO_PRIMO;
        }
    }
    return PRIMO;
}

int gerar_numero(int ultimo_numero) {
    int r = rand() % 100;
    return ultimo_numero + r;
}