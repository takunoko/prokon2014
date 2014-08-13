#SDL Make sample

CC      = gcc

CFLAGS  = -g -O2 -I/usr/local/include/SDL2 -I/usr/X11R6/include -D_THREAD_SAFE -I/usr/X11/include -DHAVE_OPENGL -g
LIBS	=  -lSDL2_test -L/usr/local/lib -lSDL2

test : test.c
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

clean :
	rm ./*.o
