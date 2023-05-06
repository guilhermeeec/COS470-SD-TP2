// -------------- Bibliotecas --------------

#include <thread>
#include <cstdlib>
#include <csignal>
#include <iostream>
#include <unistd.h>
#include <cmath>
#include <semaphore.h>
#include <queue>
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
}

void tarefa_produtor() 
{
    uint32_t produto = produz_inteiro();
    sem_wait(&empty);
    sem_wait(&mutex);
    colocar_no_buffer(produto);
    sem_post(&mutex);
    sem_post(&full);
}

void tarefa_consumidor() 
{
    sem_wait(&full);
    sem_wait(&mutex);
    uint32_t produto = retirar_do_buffer();
    sem_post(&mutex);
    sem_post(&empty);
    checar_primo(produto);
    
    if(numero_produtos_consumidos == numero_max_produtos_consumidos)
        raise(SIGUSR1);
}

// -------------- Funções básicas --------------

void inicializa_semaforos() 
{
    sem_init(&mutex, 0, 1);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);
}

void cria_produtores_consumidores() 
{
    unsigned num_produtores_criados, num_consumidores_criados = 0;
	srand((unsigned) time(NULL));
    while((num_produtores_criados < Np) && (num_consumidores_criados < Nc)) 
    {
        // Decide aleatoriamente se vai criar produor (0) ou consumidor (1)
        int zero_ou_um = rand() % 2;
        if(num_produtores_criados == Np) zero_ou_um = 1;
        if(num_consumidores_criados == Nc) zero_ou_um = 0;

        if(zero_ou_um == 0) {
            std::thread thread_produtor(tarefa_produtor);
            num_produtores_criados++;
        }
        else {
            std::thread thread_consumidor(tarefa_consumidor);
            num_consumidores_criados++;
        }
    }
}

// Depois, coletar/exibir algumas informações
void signal_handler(int) {
    std::cout << "Terminou" << std::endl;
}

void espera_criterio_parada()
{
    // Sinalizado por um consumidor (o primeiro a consumir 10^5)
    signal(SIGUSR1, signal_handler);
    while(consumo_em_andamento)
        pause();
}

int main() 
{
    inicializa_semaforos();
    cria_produtores_consumidores();
    espera_criterio_parada();
    return 0;
}

// Encerrar thread: std::terminate() dentro da thread
// Disparar sinal local raise(SIGUSR1)