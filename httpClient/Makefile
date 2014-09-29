BIN=client_test
CFLAGS=-W -Wall 
LDFLAGS= `pkg-config --libs libcurl`
CC=g++

.PHONY:all clean remake
.SUFFIXES: .cpp

all:$(BIN)

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
