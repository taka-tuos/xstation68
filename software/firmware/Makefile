TARGET = fw
OBJS_TARGET = fw.o text.o
CFLAGS = -fno-builtin -msoft-float -I. -I./golibc/include -m68020 -g -O3
LFLAGS = -Txstation68_fw.x

include Makefile.in

post : $(TARGET).rom
	cp $(TARGET).rom ../..
