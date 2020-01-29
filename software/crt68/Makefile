include ../Makefile.ref

AS			= $(PREFIX)as

default :
	$(MAKE) crt

crt : crt68.s Makefile
	$(AS) -o crt68.o -c crt68.s

clean : Makefile
	-rm crt68.o
