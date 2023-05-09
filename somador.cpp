#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <iomanip>
#include "spinlock.h"

double acumulador_compartilhado = 0;
double acumulador_checagem = 0;
float tempo_total = 0;

SpinLock somador_lock;
SpinLock tempo_lock;

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

void somar_parcela(std::vector<int8_t> parcela)
{
    double temp = 0;
    auto inicio = std::chrono::high_resolution_clock::now();

    // Faz a soma da parcela correspondente àquela thread
    for (auto& num: parcela)
    {
        temp += num;
    }
    // Adquire controle da região crítica, adiciona ao acumulador e libera a região crítica
    somador_lock.acquire();
    acumulador_compartilhado += temp;
    auto fim = std::chrono::high_resolution_clock::now();
    somador_lock.release();

    tempo_lock.acquire();
    tempo_total += std::chrono::duration<double, std::milli>(fim - inicio).count();
    tempo_lock.release();
}

void somar_checagem(std::vector<int8_t> total)
{
    // Soma todos os valores para a checagem
    for (auto& num: total)
    {
        acumulador_checagem += num;
    }
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
            // Gerar o vetor de threads para as k threads
            std::vector<std::thread> threads(k + 1);
            std::cout << k << " Threads:" << std::endl;
            for (int i = 0; i < 10; i++) {
                // Apontar para cada thread a parcela específica a ser somada
                for (int idx = 0; idx < k; idx++) {
                    threads[idx] = std::thread(somar_parcela,
                                               std::vector<int8_t>(numeros.begin() + idx * (n / k),
                                                                   numeros.begin() + (idx + 1) * (n / k)));
                }
                // Criar a thread para checagem
                threads[threads.size() - 1] = std::thread(somar_checagem, numeros);

                // Esperar que todas as threads terminem
                for (auto& thread: threads) {
                    thread.join();
                }
                
                if (acumulador_compartilhado != acumulador_checagem) {
                    std::cout << "Valores somados inconsistentes com a checagem." << std::endl;
                    return 1;
                }
                //std::cout << "Soma Compartilhada: " << acumulador_compartilhado << std::endl;
                //std::cout << "Soma para Checagem: " << acumulador_checagem << std::endl;

                acumulador_compartilhado = 0;
                acumulador_checagem = 0;
            }
            int tempo_total_medio = round(tempo_total/10);

            tempo_total = 0;
            if (arquivo_saida.is_open()) {
                arquivo_saida << tempo_total_medio << " ";
            }
            std::cout << std::endl;
            std::cout << "Tempo Médio:" << tempo_total_medio << "ms" << std::endl;
            std::cout << std::endl;
        }
        arquivo_saida << "\n";
    }
    arquivo_saida.close();
    return 0;
}