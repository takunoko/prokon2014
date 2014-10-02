BIN :=client_test
CFLAGS :=-W -Wall -g -MMD -MP -std=c++11
LDFLAGS= `pkg-config --libs libcurl` `pkg-config --libs opencv`
CC=g++

SRCS := client.c test.c
DEPS := $(SRCS:%.c=%.d)

.PHONY:all clean remake
.SUFFIXES: .cpp

all:$(BIN)

-include $(DEPS)

clean:
	rm -vf $(BIN)
	rm -vf *.o

.cpp.o:
	$(CC) -o $@ -c $< $(CFLAGS)

remake:
	make clean
	make all

client_test:client.o test.o
	$(CC) -o $@ $^ $(LDFLAGS)
