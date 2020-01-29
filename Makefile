TARGET		= xstation68
OBJS_TARGET	= xstation68.o oga.o \
			e68000/cc.o e68000/disasm.o e68000/e68000.o e68000/ea.o e68000/opcodes.o e68000/ops-020.o

CFLAGS += -O2 -g `sdl-config --cflags`
LIBS += `sdl-config --libs` -lm -lc

include Makefile.in

additional_clean :
	-rm e68000/*.o
	-rm fw.rom
