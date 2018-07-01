CC = g++
CFLAGS = -g -Wall -Wno-deprecated  

	

LIBS =  -L.  -lglut -lGLU -lGL -lm -lSOIL 

hexp_main: hexp_main.o hexplanet.o
	$(CC) $(CFLAGS) -o $@ hexplanet.o $<  $(LIBS)

.o:.cpp
	$(CC) $(CFLAGS) -c $<

clean: FORCE
	-rm -f *.o *~ sample *core*

FORCE:

