#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "spinlock.h"

#define PARCELA 0
#define CHECK 1

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

int main()
{
    std::vector<double> n_numeros = {10000000, 100000000, 1000000000};
    std::vector<int> k_threads = {1, 2, 4, 8, 16, 32, 64, 128, 256};

    std::ofstream arquivo_saida("medias.txt");

    for (auto& n: n_numeros) {
        std::vector<std::int8_t> numeros = gerar_numeros(n);
        std::cout << "Números Gerados: " << n << std::endl;
        for (auto& k: k_threads) {
            std::cout << k << " Threads:" << std::endl;
            for (int i = 0; i < 10; i++) {
                // Criar k threads para a soma
                gerar_threads(std::ref(numeros), n, k);

                // Criar a thread para checagem
                std::thread check = std::thread(somar_checagem, std::ref(numeros));
                check.join();

                if (acumulador_compartilhado != acumulador_checagem) {
                    std::cout << "Valores somados inconsistentes com a checagem." << std::endl;
                    return 1;
                }

                acumulador_compartilhado = 0;
                acumulador_checagem = 0;
            }
            int tempo_total_medio = round(tempo_total/10);
            std::cout << std::endl;
            std::cout << "Tempo Médio:" << tempo_total_medio << "ms" << std::endl;
            std::cout << std::endl;

            if (arquivo_saida.is_open()) {
                arquivo_saida << tempo_total_medio << " ";
            }

            tempo_total = 0;
        }
        arquivo_saida << "\n";
    }
    arquivo_saida.close();
    return 0;
}