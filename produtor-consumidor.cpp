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
#define N   10

// Número de threads de produtores
#define Np  4

// Número de threads de consumidores
#define Nc  4

// -------------- Variáveis globais --------------

sem_t mutex, empty, full;
bool consumo_em_andamento = true;
std::queue<uint32_t> buffer;
uint32_t numero_produtos_consumidos = 0;
uint32_t numero_max_produtos_consumidos = pow(10,5);

// -------------- Funções dos produtores e consumidores --------------

uint32_t produz_inteiro() 
{
    srand((unsigned) time(NULL));
    return (uint32_t)((rand() / RAND_MAX) * (pow(10,7)-1) + 1);
}

void checar_primo(int numero) {
    if (numero == 1) {
        std::cout << numero << "nao eh primo" << std::endl;
        return;
    }
    for (int idx = 2; idx <= numero/2; idx++)
        if (!(numero % idx)) {
            std::cout << numero << "nao eh primo" << std::endl;
            return;
        }
    std::cout << numero << "eh primo" << std::endl;
    return;
}

void colocar_no_buffer(uint32_t produto) 
{
    buffer.push(produto);
}

uint32_t retirar_do_buffer() 
{
    uint32_t produto = buffer.front();
    buffer.pop();
    numero_produtos_consumidos++;
    return produto;
}

void tarefa_produtor() 
{
    while(1) {        
        uint32_t produto = produz_inteiro();
        sem_wait(&empty);
        sem_wait(&mutex);
        if(numero_produtos_consumidos == numero_max_produtos_consumidos) {
            sem_post(&mutex);
            sem_post(&full);
            std::terminate();
        }
        colocar_no_buffer(produto);
        sem_post(&mutex);
        sem_post(&full);
    }
}

void tarefa_consumidor() 
{
    while(1) {
        sem_wait(&full);
        sem_wait(&mutex);
        if(numero_produtos_consumidos == numero_max_produtos_consumidos) {
            sem_post(&mutex);
            sem_post(&empty);
            std::terminate();
        }
        uint32_t produto = retirar_do_buffer();
        sem_post(&mutex);
        sem_post(&empty);
        checar_primo(produto);
        if(numero_produtos_consumidos == numero_max_produtos_consumidos)
            std::terminate();
    }
}

// -------------- Funções básicas --------------

int main() 
{
    // Inicializa semáforos
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    // Cria threads
    std::vector<std::thread> vetor_consumidores;
    std::vector<std::thread> vetor_produtores;
    srand((unsigned) time(NULL));
    while((vetor_produtores.size() < Np) && (vetor_consumidores.size() < Nc)) 
    {
        // Decide aleatoriamente se vai criar produor (0) ou consumidor (1)
        int zero_ou_um = rand() % 2;
        if(vetor_produtores.size() == Np) zero_ou_um = 1;
        if(vetor_consumidores.size() == Nc) zero_ou_um = 0;

        if(zero_ou_um == 0) 
            vetor_produtores.push_back(std::thread(tarefa_produtor));
        else
            vetor_consumidores.push_back(std::thread(tarefa_consumidor));
    }

    // Espera threads terminarem
    for(std::thread &thread_produtor : vetor_produtores) 
        thread_produtor.join();
    for(std::thread &thread_consumidor : vetor_consumidores) 
        thread_consumidor.join();

    return 0;
}