// -------------- Bibliotecas --------------

#include <thread>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <semaphore.h>
#include <queue>
#include <vector>
//#include "utils.h" // Trabalho anterior

// -------------- Macros --------------

// Tamanho do vetor de inteiros
#ifndef N
#define N   10
#endif

// Número de threads de produtores
#ifndef Np
#define Np  4
#endif

// Número de threads de consumidores
#ifndef Nc
#define Nc  4
#endif

// -------------- Variáveis globais --------------

sem_t mutex, empty, full;
std::queue<uint32_t> buffer;
uint32_t numero_produtos_consumidos = 0;
uint32_t numero_produtos_produzidos = 0;
uint32_t numero_max_produtos = pow(10,5);

// -------------- Funções dos produtores e consumidores --------------

uint32_t produz_inteiro(unsigned id) 
{
    return (uint32_t)(rand() % (int)pow(10,7))+1;
}

void checar_primo(int numero, unsigned id) {
    if (numero == 1) {
        std::cout << "[CONS" << id << "] (produto " << numero_produtos_consumidos << ") " << numero << " nao eh primo" << std::endl << std::flush;
        return;
    }
    for (int idx = 2; idx <= numero/2; idx++)
        if (!(numero % idx)) {
            std::cout << "[CONS" << id << "] " << numero << " nao eh primo" << std::endl << std::flush;
            return;
        }
    std::cout << "[CONS" << id << "] " << numero << " eh primo" << std::endl << std::flush;
    return;
}

void colocar_no_buffer(uint32_t produto) 
{
    buffer.push(produto);
    numero_produtos_produzidos++;
}

uint32_t retirar_do_buffer() 
{
    uint32_t produto = buffer.front();
    buffer.pop();
    numero_produtos_consumidos++;
    return produto;
}

void tarefa_produtor(unsigned id) 
{
    bool continua_producao = true;
    srand((unsigned) time(NULL)+id);
    while(continua_producao) {        
        uint32_t produto = produz_inteiro(id);
        sem_wait(&empty);
        sem_wait(&mutex);
        if(numero_produtos_produzidos == numero_max_produtos)
            continua_producao = false;
        else {
            colocar_no_buffer(produto);
            //std::cout << "[PROD" << id << "] colocou " << produto << " no buffer" << std::endl << std::flush;
        }
        sem_post(&mutex);
        sem_post(&full);
    }
}

void tarefa_consumidor(unsigned id) 
{
    bool continua_consumo = true;
    uint32_t produto = 0;
    while(continua_consumo) {
        sem_wait(&full);
        sem_wait(&mutex);
        if(numero_produtos_consumidos == numero_max_produtos)
            continua_consumo = false;
        else
            produto = retirar_do_buffer();
        sem_post(&mutex);
        sem_post(&empty);
        if(continua_consumo) checar_primo(produto, id);
    }
}

// -------------- Funções básicas --------------

int main() 
{
    std::cout << "[MAIN] Tamanho do buffer: " << N << std::endl << std::flush;
    std::cout << "[MAIN] Inicializando semaforos" << std::endl << std::flush;
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    std::cout << "[MAIN] Criando threads" << std::endl << std::flush;
    std::vector<std::thread> vetor_consumidores;
    std::vector<std::thread> vetor_produtores;
    srand((unsigned) time(NULL));
    while((vetor_produtores.size() < Np) || (vetor_consumidores.size() < Nc)) 
    {
        // Decide aleatoriamente se vai criar produor (0) ou consumidor (1)
        int zero_ou_um = rand() % 2;
        if(vetor_produtores.size() == Np) zero_ou_um = 1;
        if(vetor_consumidores.size() == Nc) zero_ou_um = 0;

        if(zero_ou_um == 0) 
            vetor_produtores.push_back(std::thread(tarefa_produtor,vetor_produtores.size()));
        else
            vetor_consumidores.push_back(std::thread(tarefa_consumidor,vetor_consumidores.size()));
    }

    // Espera threads terminarem
    std::cout << "[MAIN] Esperando " << vetor_produtores.size() << " produtores e " << vetor_consumidores.size() << " consumidores" << std::endl << std::flush;
    for(std::thread &thread_produtor : vetor_produtores) 
        thread_produtor.join();
    for(std::thread &thread_consumidor : vetor_consumidores) 
        thread_consumidor.join();

    std::cout << "[MAIN] Terminou" << std::endl << std::flush;
    return 0;
}