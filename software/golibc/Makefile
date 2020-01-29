include ../Makefile.ref

OBJS_GOLIBC	=	vsprintf.o abs.o atoi.o errno.o memchr.o memcmp.o memcpy.o memmove.o \
				memset.o qsort.o rand.o sprintf.o strcat.o strchr.o strcmp.o strcpy.o \
				strcspn.o strlen.o strncat.o strncmp.o strncpy.o strpbrk.o strrchr.o \
				strspn.o strstr.o strtol.o strtoul.o strtoul0.o 

CFLAGS		= -Iinclude -fno-builtin -msoft-float -m68020 -g -O2
ASFLAGS		= -Iinclude

CC			= $(PREFIX)gcc
AR			= $(PREFIX)ar
AS			= $(PREFIX)as

# default

default :
	$(MAKE) golibc.a

# rules

golibc.a : $(OBJS_GOLIBC) Makefile
	$(AR) rcs golibc.a $(OBJS_GOLIBC)

# normal rules

%.o : %.c Makefile
	$(CC) $(CFLAGS) -o $*.o -c $*.c

%.o : %.s Makefile
	$(AS) $(CFLAGS) -o $*.o -c $*.s

# commands

clean :
	-rm *.o
	-rm golibc.a
