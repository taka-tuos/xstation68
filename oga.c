#include "oga.h"
#include "oga_font.h"

#define rgb332(r, g, b) ((((r) >> 5) << 5) | (((g) >> 5) << 2) | ((b) >> 6))

uint8_t __oga_pallete16[16];
uint32_t __oga_pallete256[256];

uint32_t __oga_fc, __oga_bc;

int __oga_vmode = 0;
int __oga_vsync = 0;

void oga_setvsync(int oga_vsync)
{
	__oga_vsync = oga_vsync;
}

void oga_setvmode(int oga_vmode)
{
	__oga_vmode = oga_vmode;
}

uint8_t oga_getstareg(void)
{
	return __oga_vsync;
}

void oga_init(void)
{
	int r, g, b, a, i;

	for (r = 0; r < 8; r++) {
		for (g = 0; g < 8; g++) {
			for (b = 0; b < 4; b++) {
				int r8, g8, b8;

				r8 = (r << 5) | (r << 2) | (r >> 1);
				g8 = (g << 5) | (g << 2) | (g >> 1);
				b8 = (b << 6) | (b << 4) | (b << 2) | b;

				__oga_pallete256[(r << 5) | (g << 2) | b] = (r8 << 16) | (g8 << 8) | b8;
			}
		}
	}

	for (i = 0; i < 16; i++) {
		r = ((i & 7) >> 2) & 1;
		g = ((i & 7) >> 1) & 1;
		b = ((i & 7) >> 0) & 1;
		a = i >> 3;

		__oga_pallete16[i] = rgb332(((r * 0xaa) | (0x55 * a)), ((g * 0xaa) | (0x55 * a)), ((b * 0xaa) | (0x55 * a)));

		printf("%d %d %d %d %06x\n",a,r,g,b,__oga_pallete256[__oga_pallete16[i]]);
	}
}

void oga_cgout8(uint32_t *bmp, int x, int y, uint8_t *font)
{
	int i;
	unsigned int *p, d;

	for (i = 0; i < 8; i++) {
		p = bmp + (y + i) * 640 + x;
		d = font[i];
		if ((d & 0x80) != 0) { p[0] = __oga_fc; }
		else { p[0] = __oga_bc; }
		if ((d & 0x40) != 0) { p[1] = __oga_fc; }
		else { p[1] = __oga_bc; }
		if ((d & 0x20) != 0) { p[2] = __oga_fc; }
		else { p[2] = __oga_bc; }
		if ((d & 0x10) != 0) { p[3] = __oga_fc; }
		else { p[3] = __oga_bc; }
		if ((d & 0x08) != 0) { p[4] = __oga_fc; }
		else { p[4] = __oga_bc; }
		if ((d & 0x04) != 0) { p[5] = __oga_fc; }
		else { p[5] = __oga_bc; }
		if ((d & 0x02) != 0) { p[6] = __oga_fc; }
		else { p[6] = __oga_bc; }
		if ((d & 0x01) != 0) { p[7] = __oga_fc; }
		else { p[7] = __oga_bc; }
	}

	return;
}

#define OGA_UNDEFMODE_MSG0 "UNDEFINED MODE"
#define OGA_UNDEFMODE_MSG1 "Open Graphics Array 0.1.0-rc1"

uint32_t *oga_bilt(int *oga_w, int *oga_h, uint8_t *oga_gram, uint32_t *oga_work)
{
	int x, y, i, j;
	int bilt_w, bilt_h;


	switch (__oga_vmode) {
	case 0x00:
		bilt_w = 640;
		bilt_h = 480;

		for (y = 0; y < 60; y++) {
			for (x = 0; x < 80; x++) {
				__oga_fc = __oga_pallete256[__oga_pallete16[oga_gram[(y * 80 + x) * 2 + 1] & 15]];
				__oga_bc = __oga_pallete256[__oga_pallete16[oga_gram[(y * 80 + x) * 2 + 1] >> 4]];
				oga_cgout8(oga_work, x * 8, y * 8, __oga_cgrom + oga_gram[(y * 80 + x) * 2] * 8);
			}
		}

		break;
	case 0x01:
		bilt_w = 640;
		bilt_h = 480;

		for (y = 0; y < 480; y++) {
			for (x = 0; x < 640; x++) {
				oga_work[y * 640 + x] = __oga_pallete256[oga_gram[y * 640 + x]];
			}
		}

		break;
	default:
		bilt_w = 640;
		bilt_h = 480;

		memset(oga_work, 0, 640 * 480 * 4);

		for (i = 0; OGA_UNDEFMODE_MSG0[i] != 0; i++) oga_cgout8(oga_work, i * 8,       0, __oga_cgrom + OGA_UNDEFMODE_MSG0[i] * 8);
		for (i = 0; OGA_UNDEFMODE_MSG1[i] != 0; i++) oga_cgout8(oga_work, i * 8, 480 - 8, __oga_cgrom + OGA_UNDEFMODE_MSG1[i] * 8);

		break;
	}

	*oga_w = bilt_w;
	*oga_h = bilt_h;

	return oga_work;
}

