#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include "graphics.h"

extern vbe_mode_info_t mode_info;

int main(int argc, char *argv[]) {
  // sets the language of LCF messages (can be either EN-US or PT-PT)
  lcf_set_language("EN-US");

  // enables to log function invocations that are being "wrapped" by LCF
  // [comment this out if you don't want/need it]
  lcf_trace_calls("/home/lcom/labs/g1/lab5/trace.txt");

  // enables to save the output of printf function calls on a file
  // [comment this out if you don't want/need it]
  lcf_log_output("/home/lcom/labs/g1/lab5/output.txt");

  // handles control over to LCF
  // [LCF handles command line arguments and invokes the right function]
  if (lcf_start(argc, argv))
    return 1;

  // LCF clean up tasks
  // [must be the last statement before return]
  lcf_cleanup();

  return 0;
}

int(video_test_init)(uint16_t mode, uint8_t delay) {
  if (setGraphicsMode(mode))
    return 1;
  sleep(delay);
  return vg_exit();
}

int(video_test_rectangle)(uint16_t mode, uint16_t x, uint16_t y,
                          uint16_t width, uint16_t height, uint32_t color) {
  if (setFrameBuffer(mode)) {
    printf("Error setting frame buffer\n");
    return 1;
  }
  if (setGraphicsMode(mode)) {
    printf("Error setting graphics mode\n");
    return 1;
  }
  uint32_t newColor;
  if (normalizeColor(color, &newColor)) {
    printf("Error normalizing the color\n");
    return 1;
  }

  if (vg_draw_rectangle(x, y, width, height, newColor)) {
    printf("Error drawing the rectangle\n");
    return 1;
  }

  if (waitForESC()) {
    printf("Keyboard error\n");
    return 1;
  }
  return vg_exit();
}

int(video_test_pattern)(uint16_t mode, uint8_t no_rectangles, uint32_t first, uint8_t step) {
  if (setFrameBuffer(mode)) {
    printf("Error setting frame buffer\n");
    return 1;
  }
  if (setGraphicsMode(mode)) {
    printf("Error setting graphics mode\n");
    return 1;
  }

  int height = mode_info.YResolution / no_rectangles;
  int width = mode_info.XResolution / no_rectangles;

  for (int i = 0; i < no_rectangles; i++) {
    for (int j = 0; j < no_rectangles; j++) {
      uint32_t color;
      // If indexed color model
      if (mode_info.MemoryModel == 0x06) {
        uint32_t Rfirst = ((1 << mode_info.RedMaskSize) - 1) & (first >> mode_info.RedFieldPosition);
        uint32_t Gfirst = ((1 << mode_info.GreenMaskSize) - 1) & (first >> mode_info.GreenFieldPosition);
        uint32_t Bfirst = ((1 << mode_info.BlueMaskSize) - 1) & (first >> mode_info.BlueFieldPosition);
        uint32_t R = (Rfirst + j * step) % (1 << mode_info.RedMaskSize);
        uint32_t G = (Gfirst + i * step) % (1 << mode_info.GreenMaskSize);
        uint32_t B = (Bfirst + (j + i) * step) % (1 << mode_info.BlueMaskSize);
        color = R << mode_info.RedFieldPosition | G << mode_info.GreenFieldPosition | B << mode_info.BlueFieldPosition;
      }
      else {
        color = (first + (i * no_rectangles + j) * step) % (1 << mode_info.BitsPerPixel);
      }
      if (vg_draw_rectangle(j * width, i * height, width, height, color)) {
        printf("Error drawing the rectangle\n");
        return 1;
      }
    }
  }
  if (waitForESC()) {
    printf("Keyboard error\n");
    return 1;
  }
  return vg_exit();
  return 1;
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  /* To be completed */
  printf("%s(%8p, %u, %u): under construction\n", __func__, xpm, x, y);

  return 1;
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  /* To be completed */
  printf("%s(%8p, %u, %u, %u, %u, %d, %u): under construction\n",
         __func__, xpm, xi, yi, xf, yf, speed, fr_rate);

  return 1;
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
