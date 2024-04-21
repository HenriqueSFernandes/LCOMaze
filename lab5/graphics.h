#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>

int set_frame_buffer(uint16_t mode);
int(setGraphicsMode)(uint16_t mode);
int(normalize_color)(uint32_t color, uint32_t *new_color);
int paint_pixel(uint16_t x, uint16_t y, uint32_t color);
uint32_t(pixelIndex)(uint16_t x, uint16_t y);
int (draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int (getHeight)();
int (getWidth)();
#endif
