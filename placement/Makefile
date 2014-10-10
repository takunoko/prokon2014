
CC = g++
TARGET = cv_disp_img_tuple

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

all : cv_disp_img_tuple

cv_disp_img_tuple : PPMFILE.cpp cv_disp_img_tuple.cpp
	${CC} ${OPTION} ${OPEN_CV_HEADER} ${OPEN_CV_LIBS} ${FLAGS} -o $@ $^

clean:
	rm -vf ${TARGET}
