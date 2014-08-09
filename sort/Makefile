CC  = g++
CFLAGS = -Wall -O2
TARGET  = a.out
HEADERS= util.h Pos.h Dataset.h ProcessBase.h $(wildcard Process?.h)
SRCS= $(HEADERS:%.h=%.cpp) sort.cpp
OBJS= $(SRCS:%.cpp=.%.o)

$(TARGET): $(OBJS)
	$(CC) -o $@ $(OBJS)

.%.o: %.cpp
	$(CC) $(CFLAGS) -o $@ -c $<

$(OBJS): $(HEADERS)
