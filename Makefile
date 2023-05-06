CC = g++
CFLAGS = -Wall

TIMERS_SRCS = timer.cpp example_timer.cpp

all: timer

timer: $(TIMERS_SRCS:.cpp=.o)
	$(CC) -o test_timer example_timer.o timer.o

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f test_timer $(wildcard *.o)