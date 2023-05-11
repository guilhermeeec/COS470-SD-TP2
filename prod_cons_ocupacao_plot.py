import matplotlib.pyplot as plt
import numpy as numpy

def main():
    f = open("ocupacao_buffer/1000_PROD_CONS_Np=8_PROD_CONS_Nc=1_2023-0511_13:54:55")
    linhas = f.readlines()
    t = numpy.linspace(1,len(linhas),len(linhas))
    cores = plt.cm.get_cmap('tab10')
    plt.plot(t, numpy.array(linhas), color=cores(0))

    plt.xlim(1, 2000)
    plt.ylim(0, 1000)

    plt.xlabel('Tempo (acessos aos buffer)')
    plt.ylabel('Ocupacao do buffer (unidades de produtos)')
    plt.title('Cenário: n=1000, np=1, nc=8')

    plt.grid(axis='both', linestyle='--', alpha=0.5)

    fig = plt.gcf()
    fig.set_size_inches(8, 6)

    plt.yticks(numpy.arange(0,1000, step=100))

    plt.savefig('ocup_.png', dpi=300, bbox_inches='tight')
    plt.show()

if __name__ == "__main__":
    main()
