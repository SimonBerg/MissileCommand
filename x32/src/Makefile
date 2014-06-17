OBJ = main.o cleanup.o missiles_logic.o drawing.o
LIBS_PATH = -L../lib
LIBS = -lallegro-static -lallegro_font-static -lallegro_ttf-static -lallegro_image-static -lxcb -lX11 -lGL -lallegro_dialog-static -lallegro_primitives-static 
PKG = `pkg-config --libs --static allegro-static-5 allegro_font-static-5 allegro_ttf-static-5 allegro_image-static-5 allegro_dialog-static-5 allegro_primitives-static-5`

MissileCommand: $(OBJ)
	g++ -o MissileCommand_32bit $(LIBS_PATH) $(OBJ) $(PKG)

-include dependencies

.PHONY: install depend clean cleanall

depend:
	g++ -MM *.cc > dependencies
clean:
	rm *.o
cleanall:
	rm MissileCommand_32bit *.o
