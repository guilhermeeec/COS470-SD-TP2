CC = g++
CFLAGS = -Wall

PROD_CONS_N=10
PROD_CONS_Np=4
PROD_CONS_Nc=4
PROD_CONS_FILENAME=ocupacao_buffer.txt

TIMERS_SRCS = timer.cpp example_timer.cpp
PROD_CONS_SRCS = produtor-consumidor.cpp
SOMADOR_SRCS = somador.cpp spinlock.cpp

all: timer prod_cons somador

timer: $(TIMERS_SRCS:.cpp=.o)
	$(CC) -o test_timer example_timer.o timer.o

prod_cons: $(PROD_CONS_SRCS:.cpp=.o)
	$(CC) -pthread -o prod_cons produtor-consumidor.o

produtor-consumidor.o: produtor-consumidor.cpp
	$(CC) ${CFLAGS} -DFILENAME="\"${PROD_CONS_FILENAME}\"" -DN=${PROD_CONS_N} -DNc=${PROD_CONS_Nc} -DNp=${PROD_CONS_Np} -c produtor-consumidor.cpp -o produtor-consumidor.o

somador: $(SOMADOR_SRCS:.cpp=.o)
	$(CC) -std=c++11 -pthread -o somador somador.o spinlock.o
  
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f test_timer prod_cons somador $(wildcard *.o)