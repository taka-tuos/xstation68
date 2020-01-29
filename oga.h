#include <SDL/SDL.h>

void oga_init(void);
uint32_t *oga_bilt(int *oga_w, int *oga_h, uint8_t *oga_gram, uint32_t *oga_work);

void oga_setvsync(int oga_vsync);
void oga_setvmode(int oga_vmode);

uint8_t oga_getstareg(void);
