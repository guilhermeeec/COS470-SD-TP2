import matplotlib.pyplot as plt
import numpy as numpy

def main():
    n = ["10^0", "10^1", "10^2", "10^3"]
    nc = [1, 2, 4, 8] # quando np=1
    tempo_np_1 = [[30.26,15.45,8.46,5.03],
                  [29.75,15.65,8.51,5.05],
                  [29.98,15.75,8.51,5.11],
                  [30.04,15.63,8.59,5.08]]

    # Define uma paleta de cores para as linhas do gráfico
    cores = plt.cm.get_cmap('tab10')

    for i in range(len(nc)):
        y = numpy.array(tempo_np_1[i])
        plt.plot(nc, y, color=cores(i), label='N = ' + n[i])

    # Define os limites para os eixos x e y
    plt.xlim(1, 8)
    plt.ylim(0, 35)

    # Adiciona legendas e títulos ao gráfico
    plt.legend()
    plt.xlabel('Número de Consumidores')
    plt.ylabel('Tempo Médio (s)')
    plt.title('Tempo Médio de Execução com 1 produtor')

    # Adiciona grades ao gráfico
    plt.grid(axis='both', linestyle='--', alpha=0.5)

    # Ajusta o tamanho da figura
    fig = plt.gcf()
    fig.set_size_inches(8, 6)

    # Exibe o gráfico
    plt.savefig('np=1.png', dpi=300, bbox_inches='tight')
    plt.show()



    np = [1, 2, 4, 8] # quando nc=1
    tempo_nc_1 = [[30.26,30.89,32.09,32.65],
                  [29.75,30.39,30.48,30.33],
                  [29.98,30.52,30.23,30.76],
                  [30.04,30.21,30.52,30.41]]
    
    cores = plt.cm.get_cmap('tab10')

    for i in range(len(nc)):
        y = numpy.array(tempo_nc_1[i])
        plt.plot(nc, y, color=cores(i), label='N = ' + n[i])

    plt.xlim(1, 8)
    plt.ylim(20, 40)

    plt.legend()
    plt.xlabel('Número de Produtores')
    plt.ylabel('Tempo Médio (s)')
    plt.title('Tempo Médio de Execução com 1 Consumidor')

    plt.grid(axis='both', linestyle='--', alpha=0.5)

    fig = plt.gcf()
    fig.set_size_inches(8, 6)

    plt.savefig('nc=1.png', dpi=300, bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    main()
