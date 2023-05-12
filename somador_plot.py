import matplotlib.pyplot as plt
import numpy as np

def main():
    n = ["10^7", "10^8", "10^9"]
    x = [1, 2, 4, 8, 16, 32, 64, 128, 256]
    dados = np.loadtxt('medias.txt')

    # Define uma paleta de cores para as linhas do gráfico
    cores = plt.cm.get_cmap('tab10')

    # Itera sobre as linhas do arquivo e plota cada uma
    for i in range(len(dados)):
        y = dados[i]
        plt.plot(x, y, color=cores(i), label='N = ' + n[i])

    # Define os limites para os eixos x e y
    plt.xlim(1, 256)
    plt.ylim(1, 10000)

    # Adiciona legendas e títulos ao gráfico
    plt.legend()
    plt.xlabel('Número de Threads')
    plt.ylabel('Tempo Médio (ms)')
    plt.title('Tempo Médio de Execução para Diferentes Números de Threads')

    # Adiciona grades ao gráfico
    plt.grid(axis='both', linestyle='--', alpha=0.5)

    # Ajusta o tamanho da figura
    fig = plt.gcf()
    fig.set_size_inches(8, 6)

    # Define o eixo x como logarítmico na base 2
    plt.xscale('log', base=2)
    plt.yscale('log', base=10)

    # Exibe o gráfico
    plt.savefig('grafico.png', dpi=300, bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    main()
