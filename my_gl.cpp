#include <OpenGL/gl.h>
#include <GLUT/glut.h>

#define WINDOW_WIDTH 1024
#define WINDOW_HEIGHT 768

void init(void){
	glClearColor(0.0, 1.0, 0.0, 1.0);
	glOrtho(-1, WINDOW_WIDTH, WINDOW_HEIGHT, -1, -1, 1);
}

void resize(int w, int h){
	glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	//glLoadIdentity();	
	//glOrtho( -(double)WINDOW_WIDTH / w, (double)WINDOW_WIDTH / w, -(double)WINDOW_WIDTH / h, (double)WINDOW_HEIGHT / h, -1.0, 1.0);
}

// 指定された座標にドットを打つ
void Point(int x, int y, int size, int r, int g, int b){
	glColor3ub( r, g, b);
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2i( x, y);
	glEnd();
}
