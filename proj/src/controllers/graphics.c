#include "graphics.h"
#include "keyboard.h"
#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include <stdint.h>
#include <stdio.h>
#include <math.h>

extern int kbd_hook_id;
extern uint8_t kbd_value;
 uint32_t bytesPerPixel;

int setFrameBuffer(uint16_t mode) {
  // Set mode_info to 0
  memset(&mode_info, 0, sizeof(mode_info));
  // Get the info
  if (vbe_get_mode_info(mode, &mode_info))
    return 1;

  // Calculating the bytes per pixel, by rounding up the bits per pixel and dividing it by 8.
  bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  // Calculate the frame size (width * height * bytes per pixel).
  uint32_t frameSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

  // Allocate physical addresses
  struct minix_mem_range physicAddresses;
  // The start of the physical address.
  physicAddresses.mr_base = mode_info.PhysBasePtr;
  // The end of the physical address, which is the start + size.
  physicAddresses.mr_limit = physicAddresses.mr_base + frameSize;

  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physicAddresses))
    return 1;

  // Allocate virtual addresses.
  frame_buffer = vm_map_phys(SELF, (void *) physicAddresses.mr_base, frameSize);
  back_buffer = (uint8_t *) malloc(frameSize);

  return frame_buffer == NULL;
}

int setGraphicsMode(uint16_t mode) {
  // Initialize the struct.
  reg86_t reg86;
  // Set the struct to 0.
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;        // BIOS video service.
  reg86.ah = 0x4F;           // ax higher bits.
  reg86.al = 0x02;           // ax lower bits.
  reg86.bx = mode | BIT(14); // Set the mode with bit 14 (linear memory mapping).
  // Send the command.
  if (sys_int86(&reg86) != 0)
    return 1;
  return 0;
}

int(normalizeColor(uint32_t color, uint32_t *newColor)) {
  // If each pixel is 32 bits, then just use the given color.
  if (mode_info.BitsPerPixel == 32) {
    *newColor = color;
  }
  // Otherwise, set the unnecessary bits to 0.
  else {
    *newColor = color & (BIT(mode_info.BitsPerPixel) - 1); // Imagine we want 5 bits, the mask should be 0...11111. BIT(5) = 0...100000, BIT(5) - 1 = 0...11111.
  }
  return 0;
}

int(vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x > mode_info.XResolution || y > mode_info.YResolution)
    return 1;

  uint32_t bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  uint32_t index = (mode_info.XResolution * y + x) * bytesPerPixel;
  return memcpy(&back_buffer[index], &color, bytesPerPixel) == NULL;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = x; i < width + x; i++) {
    for (uint16_t j = y; j < height + y; j++) {
      if (vg_draw_pixel(i, j, color)) {
        return 1;
      }
    }
  }
  return 0;
}

int (draw_xpm_at_pos)(xpm_map_t xpm, uint16_t x, uint16_t y){
    xpm_image_t img; // pixmap and metadataz
  uint32_t *map; // pixmap itself
    enum xpm_image_type image_type = XPM_8_8_8_8;
  map =(uint32_t *) xpm_load(xpm, image_type, &img);
  for(int i=0;i < img.height; i++){
    for(int j=0; j<img.width; j++){
      vg_draw_pixel(x+j, y+i,map[i*img.width+j]);
      
    }
  }return 0;
}


int draw_xpm_at_pos_at_delta(xpm_map_t xpm, uint16_t x, uint16_t y, double theta) {
    
    uint32_t *map; // pixmap itself
    enum xpm_image_type image_type = XPM_8_8_8_8;
    map = (uint32_t *)xpm_load(xpm, image_type, &img);

    // Calculate image center
    double center_x = img.width / 2.0;
    double center_y = img.height / 2.0;

    // Convert theta to radians and get cosine and sine values
    double cos_theta = cos(theta);
    double sin_theta = sin(theta);

    for (int i = 0; i < img.height; i++) {
        for (int j = 0; j < img.width; j++) {
            // Translate coordinates to be relative to center
            double translated_x = j - center_x;
            double translated_y = i - center_y;

            // Rotate coordinates
            int rotated_x = (int)(cos_theta * translated_x + sin_theta * translated_y);
            int rotated_y = (int)(-sin_theta * translated_x + cos_theta * translated_y);

            // Translate coordinates back to original position
            rotated_x += center_x;
            rotated_y += center_y;

            vg_draw_pixel(x + rotated_x, y + rotated_y, map[i * img.width + j]);
        }
    }
    return 0;
}
int(fill_color)(uint32_t color) {
  return vg_draw_rectangle(0, 0, mode_info.XResolution, mode_info.YResolution, color);
}

int (swap)(){
  memcpy(frame_buffer, back_buffer, mode_info.XResolution* mode_info.YResolution*bytesPerPixel);
  return 0;
}

