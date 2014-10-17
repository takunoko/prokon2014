
CC = g++
TARGET = cv_disp_img_tuple
OBJECT = PPMFILE.o cv_disp_img_tuple.o ../sort/PosData.o ../sort/Pos.o ../sort/util.o
HEADER = PPMFILE.hpp

# OpenCVまでのパス
# プログラム上では<opencv2/core/core.hpp>のように書く
#	以下で勝手にパスをもってきてくれる。

OPEN_CV_HEADER = `pkg-config --cflags opencv`
OPEN_CV_LIBS = `pkg-config --libs opencv`

# FLAGS = -lm
# デバッグのためにO0して-gをつける
# -O3は最適化
#  -std=c++11, -stdlib=libc++ はMacでコンパイルする際にC++11が使われないのの対策
#  これを入れないとtupleが使えない。
CFLAGS=-g -O0 --std=c++11 -Wall ${OPEN_CV_HEADER}
LDFLAGS= -lm ${OPEN_CV_LIBS} --stdlib=libc++

.SUFFIXES: .cpp .o

.cpp.o:
	${CC} -c -o $@ ${CFLAGS} $<

all : cv_disp_img_tuple

cv_disp_img_tuple : ${OBJECT} ${HEADER}
	${CC} ${LDFLAGS} -o $@ ${OBJECT}

clean:
	rm -vf ${TARGET}
	rm -vf *.o
