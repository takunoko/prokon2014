# binary name here
BIN := prokon25

all: $(BIN)

CC=g++
# 全警告, デバッグ, ヘッダファイル依存 C++11仕様
CFLAGS=-Wall -O3 -MMD -MP -std=c++11 `pkg-config --cflags libcurl` `pkg-config --cflags opencv`
LDFLAGS= `pkg-config --libs libcurl` `pkg-config --libs opencv`

.PHONY: all clean
.SUFFIXES: .cpp .o

prokon25_SRCS= main.cpp httpClient/client.cpp sort/Process5.cpp sort/ProcessBase.cpp sort/Dataset.cpp sort/PosData.cpp sort/Pos.cpp sort/util.cpp placement/PPMFILE.cpp
# FIXME: Makefileの置換
prokon25_DEPS= $(prokon25_SRCS:.cpp=.d)
prokon25_OBJS= $(prokon25_SRCS:.cpp=.o)

.cpp.o:
	$(CC) -o $@ -c $< $(CFLAGS)

-include $(DEPS)

prokon25: $(prokon25_OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

#OpenGL
OpenGL_OPTION = -framework OpenGL -framework GLUT -framework Foundation -Wno-deprecated
# -framework hoge OpenGLを使うために必要なもの(Mac)
# -Wno-deprecated を使うと、非推奨に対する警告を表示しなくなる

gl_test.out : gl_test.cpp my_gl.o
	g++ -o gl_test.out my_gl.o gl_test.cpp $(OpenGL_OPTION)

my_gl.o : my_gl.cpp
	g++ -c my_gl.cpp

clean:
	rm -vf $(BIN)
	rm -vf *.o
