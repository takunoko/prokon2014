CC  = g++
CFLAGS = -Wall -O2
TARGET  = a.out
SRCS= util.cpp Pos.cpp Dataset.cpp ProcessBase.cpp $(wildcard Process?.cpp) sort.cpp
OBJS= $(SRCS:%.cpp=.%.o)

#.SUFFIXES: .cpp .o
$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

#$(OBJS): $(SRCS)
.%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

#.cpp.o:
#	$(CC) $(CFLAGS) -c $^

