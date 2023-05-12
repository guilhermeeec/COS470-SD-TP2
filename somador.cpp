// -------------- Bibliotecas --------------

#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "spinlock.h"


// -------------- Variáveis globais --------------

double acumulador_compartilhado = 0;
double acumulador_checagem = 0;
float tempo_total = 0;

SpinLock somador_lock;

std::vector<std::int8_t> gerar_numeros(double n)
{
    std::mt19937 gerador(time(NULL));                           // Gerador Mersenne Twister
    std::uniform_int_distribution<std::int8_t> dist(-100, 100); // Distribuição entre (-100, 100)
    std::vector<std::int8_t> numeros;

    // Gerar n números aleatórios e adicionar ao vetor de números
    for (double idx = 0; idx < n; idx++)
    {
        numeros.push_back(dist(gerador));
    }

    return numeros;
}

void somar_parcela(std::vector<std::int8_t>& numeros, int min, int max)
{
    double temp = 0;

    // Faz a soma da parcela correspondente àquela thread
    for (int idx = min; idx < max; idx++)
    {
        temp += numeros[idx];
    }

    // Adquire controle da região crítica, adiciona ao acumulador e libera a região crítica
    somador_lock.acquire();
    acumulador_compartilhado += temp;
    somador_lock.release();

}

void somar_checagem(std::vector<std::int8_t>& numeros)
{
    // Soma todos os valores para a checagem
    for (auto& num: numeros)
    {
        acumulador_checagem += num;
    }
}

void gerar_threads(std::vector<std::int8_t>& numeros, double n, int k) {
    std::vector<std::thread> threads(k);
    auto inicio = std::chrono::high_resolution_clock::now();

    // Apontar para cada thread a parcela específica a ser somada
    for (int idx = 0; idx < k; idx++) {
            threads[idx] = std::thread(somar_parcela, std::ref(numeros), idx * (n / k),
                                                                        (idx + 1) * (n / k));
    }

    // Esperar que todas as threads terminem
    for (auto& thread: threads) {
        thread.join();
    }

    auto fim = std::chrono::high_resolution_clock::now();
    auto dur = fim - inicio; 
    tempo_total += std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
}

int main(int argc, char *argv[])
{
    double N;
    int K;
    
    N = std::stod(argv[1]);
    K = std::stoi(argv[2]);

    std::vector<std::int8_t> numeros = gerar_numeros(N);
    for (int idx = 0; idx < 10; idx++) {
        // Criar k threads para a soma
        gerar_threads(std::ref(numeros), N, K);

        // Criar a thread para checagem
        std::thread check = std::thread(somar_checagem, std::ref(numeros));
        check.join();

        if (acumulador_compartilhado != acumulador_checagem) {
            std::cout << "Valores somados inconsistentes com a checagem." << std::endl;
            return 1;
        }

    }
    std::cout << tempo_total << std::endl;
    
    return 0;
}