#include <vector>
#include <random>
#include <ctime>
#include <iostream>
#include <thread>
#include "spinlock.h"

double acumulador_compartilhado = 0;
double acumulador_checagem = 0;
SpinLock somador_lock;

std::vector<int> gerar_numeros(double n) {
    std::mt19937 gerador(time(NULL)); // Gerador Mersenne Twister
    std::uniform_int_distribution<int> dist(-100, 100); // Distribuição entre (-100, 100)
    std::vector<int> numeros;

    // Gerar n números aleatórios e adicionar ao vetor de números
    for(double idx = 0; idx < n; idx++) {
        numeros.push_back(dist(gerador));
    }

    return numeros;
}

void somar_parcela(std::vector<int> parcela) {
    double temp = 0;
    
    // Faz a soma da parcela correspondente àquela thread
    for (double idx = 0; idx < parcela.size(); idx++) {
        temp += parcela[idx];
    }
    // Adquire controle da região crítica, adiciona ao acumulador e libera a região crítica
    somador_lock.acquire();
    acumulador_compartilhado += temp;
    somador_lock.release();

}

void somar_checagem (std::vector<int> total) {
    // Soma todos os valores para a checagem
    for (double idx = 0; idx < total.size(); idx++) {
        acumulador_checagem += total[idx];
    }
}

int main() {
    std::vector<double> n_numeros = {10000000, 100000000, 1000000000};
    std::vector<int> k_threads = {1, 2, 4, 8, 16, 32, 64, 128, 256};

    for (int n = 0; n < n_numeros.size(); n++) {
        std::vector<int> numeros = gerar_numeros(n_numeros[n]);
        std::cout << "Números Gerados: " << n_numeros[n] << std::endl;
        for (int k = 0; k < k_threads.size(); k++) {
            // Gerar o vetor de threads para as k threads
            std::vector<std::thread> threads(k_threads[k] + 1);
            // Apontar para cada thread a parcela específica a ser somada
            for (int idx = 0; idx < k_threads[k]; idx++) {
                threads[idx] = std::thread(somar_parcela,
                                           std::vector<int>(numeros.begin() + idx*(n_numeros[n]/k_threads[k]),
                                                            numeros.begin() + (idx+1)*(n_numeros[n]/k_threads[k])));
            }
            // Criar a thread para checagem
            threads[threads.size() - 1] = std::thread(somar_checagem, numeros);
        
            // Esperar que todas as threads terminem
            for (int idx = 0; idx < k_threads[k] + 1; idx++) {
                threads[idx].join();
            }
            std::cout << k_threads[k] << " Threads:" << std::endl;
            std::cout << "Soma Compartilhada: " << acumulador_compartilhado << std::endl;
            std::cout << "Soma para Checagem: " << acumulador_checagem << std::endl;

            acumulador_compartilhado = 0;
            acumulador_checagem = 0;
        }  
    }
    
}