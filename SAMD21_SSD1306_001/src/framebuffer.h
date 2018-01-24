#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <bitmap_db.h>
#include <fonts.h>




uint8_t Punctuate(char mark);
void buffer_WriteText(uint8_t *buffer, const FONT_INFO *fontp, const char text[], uint8_t x, uint8_t y, uint8_t orient );
void buffer_WriteTextToConsole(uint8_t *buffer,const FONT_INFO *fontp, const char *text);
void Byte2String (char* outstr ,uint8_t u8t);
void scrollRows(uint8_t *buffer, uint8_t rows);
void scrollPage(uint8_t *buffer, uint8_t pages);

void buffer_clear(uint8_t *buffer);
void buffer_drawPixel(uint8_t *buffer, uint8_t pos_x, uint8_t pos_y, uint8_t pixel_status);
void buffer_drawVLine(uint8_t *buffer, uint8_t x, uint8_t y, uint8_t length);
void buffer_drawHLine(uint8_t *buffer, uint8_t x, uint8_t y, uint8_t length);
void buffer_drawRectangle(uint8_t *buffer, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void buffer_drawBitmap(uint8_t *buffer, const uint8_t *bitmap, uint8_t height, uint8_t width, uint8_t pos_x, uint8_t pos_y);

void buffer_drawButtonOutlines(uint8_t *buffer);
void buffer_drawButtons(uint8_t *buffer, uint16_t buttonstates);

#endif