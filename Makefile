C = g++
CFLAGS = -Wall

TIMERS_SRCS = timer.cpp example_timer.cpp
PROD_CONS_SRCS = produtor-consumidor.cpp

all: timer prod_cons

timer: $(TIMERS_SRCS:.c=.o)
	$(CC) -o test_timer example_timer.o timer.o

prod_cons: $(PROD_CONS_SRCS:.c=.o)
	$(CC) -o prod_cons prod_cons.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f test_timer prod_cons $(wildcard *.o)