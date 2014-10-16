BIN :=cell_test
CFLAGS :=-W -Wall -g -MMD -MP -std=c++11 -I../../sort/
LDFLAGS= `pkg-config --libs libcurl` `pkg-config --libs opencv`
CC=g++

SRCS := Cells.cpp test_main.cpp BinaryTree.cpp BinaryTreeTest.cpp
DEPS := $(SRCS:%.cpp=%.d)

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

cell_test:Cells.o ../../sort/PosData.o ../../sort/util.o ../../sort/Pos.o
	$(CC) -o $@ $^ $(LDFLAGS)
