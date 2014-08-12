CFLAGS = -Wall -O3
TARGET = a.out
HEADERS = util.h Pos.h PosData.h Dataset.h ProcessBase.h $(wildcard Process?.h)
SRCS = $(HEADERS:%.h=%.cpp) sort.cpp
OBJS = $(SRCS:%.cpp=.%.o)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $(OBJS)

.%.o: %.cpp
	$(CXX) $(CFLAGS) -o $@ -c $<

$(OBJS): $(HEADERS)
