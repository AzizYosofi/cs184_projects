CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX
	LDFLAGS = -framework GLUT -framework OpenGL -L./lib/mac/ \
    	-L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
    	-lGL -lGLU -lm -lstdc++ -lGLEW
else
	CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11R6/include -I/sw/include \
					 -I/usr/sww/include -I/usr/sww/pkg/Mesa/include
	LDFLAGS = -L./lib/nix -L/usr/X11R6/lib -L/sw/lib -L/usr/sww/lib \
						-L/usr/sww/bin -L/usr/sww/pkg/Mesa/lib -lglut -lGLU -lGL -lX11 -lGLEW
endif
INCFLAGS = 
RM = /bin/rm -f
all: curves
curves: main.o curves2.o Curve.o Point.o Scene.o shaders.o Bezier2.h Bezier.h Bspline.h Curve.h Point.h Scene.h WorkingScene.h
	$(CC) $(CFLAGS) -o curves main.o curves2.o Curve.o Point.o Scene.o shaders.o $(INCFLAGS) $(LDFLAGS) 
main.o: main.cpp WorkingScene.h Scene.h Point.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c main.cpp
Curve.o: Curve.cpp Curve.h Point.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Curve.cpp
curves2.o: curves2.cpp WorkingScene.h Scene.h Point.h Bezier.h Bspline.h Bezier2.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c curves2.cpp
Point.o: Point.cpp Point.h 
	$(CC) $(CFLAGS) $(INCFLAGS) -c Point.cpp  
Scene.o: Scene.cpp Scene.h Point.h Bezier.h Bspline.h Bezier2.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c Scene.cpp  
shaders.o: shaders.cpp shaders.h
	$(CC) $(CFLAGS) $(INCFLAGS) -c shaders.cpp
clean: 
	$(RM) *.o curves

