#OpenGL 

OpenGL_OPTION = -framework OpenGL -framework GLUT -framework Foundation -Wno-deprecated
# -framework hoge OpenGLを使うために必要なもの(Mac)
# -Wno-deprecated を使うと、非推奨に対する警告を表示しなくなる

gl_test.out : gl_test.cpp my_gl.o
	g++ -o gl_test.out my_gl.o gl_test.cpp $(OpenGL_OPTION)

my_gl.o : my_gl.cpp
	g++ -c my_gl.cpp
