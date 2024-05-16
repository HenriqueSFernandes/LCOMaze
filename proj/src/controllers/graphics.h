#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <lcom/lcf.h>
#include <stdint.h>
#include <stdio.h>
#include <lcom/vbe.h>
vbe_mode_info_t mode_info;
xpm_image_t img; // pixmap and metadata
uint8_t *frame_buffer;
uint8_t *back_buffer;

int setFrameBuffer(uint16_t mode);

int setGraphicsMode(uint16_t mode);

int(normalizeColor(uint32_t color, uint32_t *newColor));

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

int (fill_color)(uint32_t color);

int (swap)();

int (draw_xpm_at_pos)(xpm_map_t xpm, uint16_t x, uint16_t y);

int draw_xpm_at_pos_at_delta(xpm_map_t xpm, uint16_t x, uint16_t y, double theta) ;

#endif
