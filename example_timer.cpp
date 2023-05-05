#include "timer.h"
#include <math.h>

void nome_da_funcao() {
    Timer("nome_da_funcao");

    for(int i=0;i<pow(10,5);i++);
}

int main () {
    // Colocar no início de qualquer função que vá ter o tempo medido
    Timer("main()");

    for (int i=0; i<10; i++)
        nome_da_funcao();

    // Pode ser chamado EM QUALQUER LUGAR (não precisa de nenhuma "variável")
    Timer::s_print_times();
}