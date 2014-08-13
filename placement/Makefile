
CC = g++
TARGET = cv_disp_img

# OpenCVまでのパス
# プログラム上では<opencv2/core/core.hpp>のように書く
#	以下で勝手にパスをもってきてくれる。
OPEN_CV_HEADER = `pkg-config --cflags opencv`
OPEN_CV_LIBS = `pkg-config --libs opencv`

cv_disp_img : cv_disp_img.cpp
	${CC} ${OPEN_CV_HEADER} ${OPEN_CV_LIBS} -o $@ $<
