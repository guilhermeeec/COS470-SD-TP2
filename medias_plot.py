import matplotlib.pyplot as plt
import numpy as np

def main():
    n = ["10^7", "10^8", "10^9"]
    x = [1, 2, 4, 8, 16, 32, 64, 128, 256]
    dados = np.loadtxt('medias.txt')

    # Itera sobre as linhas do arquivo e plota cada uma
    for i in range(len(dados)):
        y = dados[i]
        plt.plot(x, y, label='N = ' + n[i])

    # Adiciona legendas e títulos ao gráfico
    plt.legend()
    plt.xlabel('Número de Threads')
    plt.ylabel('Tempo Médio (ms)')
    plt.title('Tempo Médio de Execução para Diferentes Números de Threads')

    # Exibe o gráfico
    plt.savefig('grafico.png')
    plt.show()

if __name__ == "__main__":
    main()
