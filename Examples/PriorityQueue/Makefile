# Very simple makefile illustrating how to use pkg-config to compile


All: PQ.c
	gcc PQ.c -Wall `pkg-config --cflags --libs lgm` -o PQ

clean:
	rm PQ
