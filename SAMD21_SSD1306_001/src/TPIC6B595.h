#ifndef TPIC6B595_H
#define TPIC6B595_H

#include <asf.h>


void shftreg_init(void);
void shftreg_display(uint8_t numerals[4]);
void shftreg_blank(void);
void shftreg_bright(uint8_t brightness);

#endif
