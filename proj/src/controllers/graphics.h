#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../images/font/letter_a.xpm"
#include "../images/font/letter_b.xpm"
#include "../images/font/letter_c.xpm"
#include "../images/font/letter_d.xpm"
#include "../images/font/letter_e.xpm"
#include "../images/font/letter_f.xpm"
#include "../images/font/letter_g.xpm"
#include "../images/font/letter_h.xpm"
#include "../images/font/letter_i.xpm"
#include "../images/font/letter_j.xpm"
#include "../images/font/letter_k.xpm"
#include "../images/font/letter_l.xpm"
#include "../images/font/letter_m.xpm"
#include "../images/font/letter_n.xpm"
#include "../images/font/letter_o.xpm"
#include "../images/font/letter_p.xpm"
#include "../images/font/letter_q.xpm"
#include "../images/font/letter_r.xpm"
#include "../images/font/letter_s.xpm"
#include "../images/font/letter_t.xpm"
#include "../images/font/letter_u.xpm"
#include "../images/font/letter_v.xpm"
#include "../images/font/letter_w.xpm"
#include "../images/font/letter_x.xpm"
#include "../images/font/letter_y.xpm"
#include "../images/font/letter_z.xpm"
#include "../images/font/letter_arrow.xpm"
#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include <stdint.h>
#include <stdio.h>
vbe_mode_info_t mode_info;
xpm_image_t img; // pixmap and metadata
uint8_t *frame_buffer;
uint8_t *back_buffer;

int setFrameBuffer(uint16_t mode);

int setGraphicsMode(uint16_t mode);

int(normalizeColor(uint32_t color, uint32_t *newColor));

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, uint8_t *buffer);

int(vg_draw_rectangle_to_buffer)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color, uint8_t *buffer);

int(fill_color)(uint32_t color, uint8_t *buffer);

int(swap)();

int(clear)(uint8_t *buffer);

int(draw_xpm_at_pos)(xpm_map_t xpm, uint16_t x, uint16_t y);

int draw_xpm_at_pos_at_delta(xpm_map_t xpm, uint16_t x, uint16_t y, double theta);

int draw_text(char *text, uint16_t x, uint16_t y);
#endif
