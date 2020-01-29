#include <stdio.h>
#include "text.h"

struct CONSOLE g_cons;
struct CONSOLE *cons;

int bayerpattern8[64] = {
	 1, 33,  9, 41,  3, 35, 11, 43,
	49, 17, 57, 25, 51, 19, 59, 27,
	13, 45,  5, 37, 15, 47,  7, 39,
	61, 29, 53, 21, 63, 31, 55, 23,
	 4, 36, 12, 44,  2, 34, 10, 42,
	52, 20, 60, 28, 50, 18, 58, 26,
	16, 48,  8, 40, 14, 46,  6, 38,
	64, 32, 56, 24, 62, 30, 54, 22,
};

int bayerpattern[16] = {
	  0,  8,  2, 10,
	 12,  4, 14,  6,
	  3, 11,  1,  9,
	 15,  7, 13,  5
};

#define cutoff(__n) ((__n) > 255 ? 255 : ((__n) < 0 ? 0 : (__n)))

unsigned char rgb2pal(int r, int g, int b, int x, int y)
{
	//int bias = bayerpattern[(y%4)*4 + (x%4)]*4;
	int bias = bayerpattern8[(y%8) * 8 + (x%8)];
	//int bias = 0;
	
	int r1 = cutoff(r + bias / 2) >> 5;
	int g1 = cutoff(g + bias / 2) >> 5;
	int b1 = cutoff(b + bias) >> 6;
	
	return (r1 << 5) | (g1 << 2) | b1;
}

void entry(void)
{
	*((unsigned char *)0x800000) = 0;
	
	cons = &g_cons;
	
	cons->cur_x = 0;
	cons->cur_y = 0;
	
	cons_putstr0(cons, "[xstation68] boot finished\n");
	
	char s[256];
	sprintf(s,"PRINTF TEST (%d)",1234);
	
	cons_putstr0(cons, s);
	
	/*int r, g, b;
	
	*((char *)(8*1024*1024)) = 0x01;
	
	int x, y;
	for(y = 0; y < 256; y++) {
		for(x = 0; x < 256; x++) {
			r = 0;
			g = y;
			b = x;
			
			((unsigned char *)VRAM_TEXTMODE)[y * 640 + x] = rgb2pal(r, g, b, x, y);
		}
	}*/
}
