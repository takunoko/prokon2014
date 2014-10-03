
CC = g++
TARGET = cv_disp_img

# OpenCVまでのパス
# プログラム上では<opencv2/core/core.hpp>のように書く
#	以下で勝手にパスをもってきてくれる。

OPEN_CV_HEADER = `pkg-config --cflags opencv`
OPEN_CV_LIBS = `pkg-config --libs opencv`

FLAGS = -lm

# -O3は最適化
#  -std=c++11, -stdlib=libc++ はMacでコンパイルする際にC++11が使われないのの対策
#  これを入れないとtupleが使えない。
OPTION = -O3 -std=c++11 -stdlib=libc++

all : cv_disp_img stl_test stl_test2 stl_test3

cv_disp_img : cv_disp_img.cpp
	${CC} ${OPTION} ${OPEN_CV_HEADER} ${OPEN_CV_LIBS} ${FLAGS} -o $@ $<

stl_test : stl_test.cpp
	${CC} ${OPTION} ${OPEN_CV_HEADER} ${OPEN_CV_LIBS} ${FLAGS} -o $@ $<

stl_test2 : stl_test2.cpp
	${CC} ${OPTION} ${OPEN_CV_HEADER} ${OPEN_CV_LIBS} ${FLAGS} -o $@ $<

stl_test3 : stl_test3.cpp
	${CC} ${OPTION} ${OPEN_CV_HEADER} ${OPEN_CV_LIBS} ${FLAGS} -o $@ $<
