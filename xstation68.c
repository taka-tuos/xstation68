#include "xstation68.h"

#define FPS 60

int poll_event(SDL_Event *sdl_event);
void adjust_fps(void);
void print_fps(void);

unsigned char *m68k_wram;


unsigned char m68k_get_uint8(void *ext, unsigned long addr);
unsigned short m68k_get_uint16(void *ext, unsigned long addr);
unsigned long m68k_get_uint32(void *ext, unsigned long addr);

void m68k_set_uint8(void *ext, unsigned long addr, unsigned char val);
void m68k_set_uint16(void *ext, unsigned long addr, unsigned short val);
void m68k_set_uint32(void *ext, unsigned long addr, unsigned long val);

void m68k_execute_cycle(e68000_t *c, int n)
{
	int o = 0;

	while (n >= c->delay) {
		n -= c->delay;
		o += c->delay;

		c->clkcnt += c->delay;
		c->delay = 0;

		oga_setvsync((float)n * (480.0f / 525.0f) < o ? 1 : 0);

		e68_execute(c);

		if (c->delay == 0) {
			fprintf(stderr, "warning: delay == 0 at %08lx\n",
				(unsigned long)e68_get_pc(c)
				);
			fflush(stderr);
			break;
		}
	}

	c->clkcnt += n;
	c->delay -= n;
}

int main(int argc, char *argv[])
{
	e68000_t *m68k = e68_new();

	m68k_wram = (unsigned char *)malloc(8 * 1024 * 1024 - 1);
	memset(m68k_wram, 0, 8 * 1024 * 1024 - 1);

	e68_init(m68k);
	e68_set_ram(m68k, m68k_wram, 8 * 1024 * 1024 - 1);
	e68_set_68020(m68k);

	m68k->get_uint8 = m68k_get_uint8;
	m68k->get_uint16 = m68k_get_uint16;
	m68k->get_uint32 = m68k_get_uint32;

	m68k->set_uint8 = m68k_set_uint8;
	m68k->set_uint16 = m68k_set_uint16;
	m68k->set_uint32 = m68k_set_uint32;

	FILE *fp = fopen("fw.rom", "rb");
	fread(m68k_wram, 1, 128 * 1024, fp);
	fclose(fp);

	e68_reset(m68k);

	SDL_Event sdl_event;
	SDL_Surface *sdl_screen;

	SDL_Init(SDL_INIT_VIDEO);

	sdl_screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);

	oga_init();

	uint32_t *oga_work = (uint32_t *)malloc(640 * 480 * 4);

	while (!poll_event(&sdl_event)) {
		adjust_fps();
		m68k_execute_cycle(m68k, (int)(33.3333f * 1000.0f * 1000.0f / 60.0f));

		int cga_w, cga_h;
		int x, y;

		uint32_t *oga_32bpp = oga_bilt(&cga_w, &cga_h, m68k_wram + 0x40000, oga_work);

		float a = (float)cga_w / (float)sdl_screen->w;
		float b = (float)cga_h / (float)sdl_screen->h;

		for (y = 0; y < sdl_screen->h; y++) {
			for (x = 0; x < sdl_screen->w; x++) {
				((uint32_t *)sdl_screen->pixels)[y * (sdl_screen->pitch >> 2) + x] = oga_32bpp[(int)((float)y*b) * cga_w + (int)((float)x*a)];
			}
		}

		SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

		print_fps();
	}

	SDL_Quit();

	return 0;
}

void print_fps(void)
{
	static int frame = 0;
	static unsigned int backtime =0;
	
	if(!backtime) backtime = SDL_GetTicks();

	if (SDL_GetTicks() - backtime > 1000) {
		char sz[256];
		sprintf(sz, "%d fps\n", frame);
		SDL_WM_SetCaption(sz, NULL);
		frame = 0;
		backtime = SDL_GetTicks();
	}
	else {
		frame++;
	}
}

void adjust_fps(void)
{
	static unsigned long backtime = 0;
	static int frame = 0;
	long sleeptime;
	if(!backtime) backtime = SDL_GetTicks();
	frame++;
	sleeptime = (frame < FPS) ?
		(backtime + (long)((double)frame * (1000.0 / FPS)) - SDL_GetTicks()) :
		(backtime + 1000 - SDL_GetTicks());
	if(sleeptime > 0) SDL_Delay(sleeptime);
	if(frame >= FPS) {
		frame = 0;
		backtime = SDL_GetTicks();
	}
}

int poll_event(SDL_Event *sdl_event)
{
	if (SDL_PollEvent(sdl_event)) {
		switch (sdl_event->type) {
		case SDL_QUIT:
			return 1;
		}
	}

	return 0;
}

//--------------------------------------------------------------------------------

unsigned char m68k_get_uint8(void *ext, unsigned long addr)
{
	switch (addr - 0x800000) {
	case 0x00:
		return oga_getstareg();
	}

	return 0xff;
}

unsigned short m68k_get_uint16(void *ext, unsigned long addr)
{
	return ((m68k_get_uint8(ext, addr) << 8) | m68k_get_uint8(ext, addr + 1));
}

unsigned long m68k_get_uint32(void *ext, unsigned long addr)
{
	unsigned long val;

	val = m68k_get_uint8(ext, addr);
	val = (val << 8) | m68k_get_uint8(ext, addr + 1);
	val = (val << 8) | m68k_get_uint8(ext, addr + 2);
	val = (val << 8) | m68k_get_uint8(ext, addr + 3);
	return val;
}

void m68k_set_uint8(void *ext, unsigned long addr, unsigned char val)
{
	switch (addr - 0x800000) {
	case 0x00:
		oga_setvmode(val);
		break;
	}

	return;
}

void m68k_set_uint16(void *ext, unsigned long addr, unsigned short val)
{
	m68k_set_uint8(ext, addr, (val >> 8) & 0xff);
	m68k_set_uint8(ext, addr + 1, val & 0xff);
}

void m68k_set_uint32(void *ext, unsigned long addr, unsigned long val)
{
	m68k_set_uint8(ext, addr, (val >> 24) & 0xff);
	m68k_set_uint8(ext, addr + 1, (val >> 16) & 0xff);
	m68k_set_uint8(ext, addr + 2, (val >> 8) & 0xff);
	m68k_set_uint8(ext, addr + 3, val & 0xff);
}