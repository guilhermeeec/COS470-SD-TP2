CC = g++
CFLAGS = -Wall

TIMERS_SRCS = timer.cpp example_timer.cpp
SOMADOR_SRCS = somador.cpp spinlock.cpp

all: timer somador

timer: $(TIMERS_SRCS:.cpp=.o)
	$(CC) -o test_timer example_timer.o timer.o

somador: $(SOMADOR_SRCS:.cpp=.o)
	$(CC) -std=c++11 -pthread -o somador somador.o spinlock.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f test_timer somador $(wildcard *.o)