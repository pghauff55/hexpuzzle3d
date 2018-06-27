CC = g++
CFLAGS = -g -Wall -Wno-deprecated `pkg-config --cflags IlmBase` 

	
OPENCV = `pkg-config opencv --cflags --libs`
LIBS =  -L. $(OPENCV) `pkg-config --libs IlmBase` -lglut -lGLU -lGL -lm -lSOIL 

hexp_main: hexp_main.o hexplanet.o
	$(CC) $(CFLAGS) -o $@ hexplanet.o $<  $(LIBS)

.o:.cpp
	$(CC) $(CFLAGS) -c $<

clean: FORCE
	-rm -f *.o *~ sample *core*

FORCE:

