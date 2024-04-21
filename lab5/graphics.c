#include <graphics.h>
#include <lcom/lcf.h>

vbe_mode_info_t mode_info;
uint8_t *frame_buffer;

int set_frame_buffer(uint16_t mode) {

  // get mode info
  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info))
    return 1;

  // calculate the buffer size
  uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
  unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;

  // define the range of physical addresses
  struct minix_mem_range physic_addresses;
  physic_addresses.mr_base = mode_info.PhysBasePtr;
  physic_addresses.mr_limit = physic_addresses.mr_base + frame_size;

  // physical memory allocation
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) {
    printf("Physical memory allocation error\n");
    return 1;
  }

  // virtual memory allocation
  frame_buffer = vm_map_phys(SELF, (void *) physic_addresses.mr_base, frame_size);
  if (frame_buffer == NULL) {
    printf("Virtual memory allocation error");
    return 1;
  }

  return 0;
}

int (getHeight)() {
  return mode_info.YResolution;
}

int (getWidth)() {
  return mode_info.XResolution;
}

int(draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = x; i < x + width; i++) {
    for (uint16_t j = y; j < y + height; j++) {
      if (paint_pixel(i, j, color) != 0)
        return 1;
    }
  }
  return 0;
}

int(normalize_color)(uint32_t color, uint32_t *new_color) {
  if (mode_info.BitsPerPixel == 32) {
    *new_color = color;
  }
  else {
    *new_color = color & (BIT(mode_info.BitsPerPixel) - 1);
  }
  return 0;
}

int paint_pixel(uint16_t x, uint16_t y, uint32_t color) {

  // As coordenadas têm de ser válidas
  if (x >= mode_info.XResolution || y >= mode_info.YResolution)
    return 1;

  // Cálculo dos Bytes per pixel da cor escolhida. Arredondamento por excesso.
  unsigned BytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;

  // Índice (em bytes) da zona do píxel a colorir
  unsigned int index = (mode_info.XResolution * y + x) * BytesPerPixel;

  // A partir da zona de memória frame_buffer[index], copia @BytesPerPixel bytes da @color
  if (memcpy(&frame_buffer[index], &color, BytesPerPixel) == NULL)
    return 1;

  return 0;
}

int(setGraphicsMode)(uint16_t mode) {
  memset(&mode_info, 0, sizeof(mode_info));
  if (vbe_get_mode_info(mode, &mode_info))
    return 1;
  reg86_t reg;
  memset(&reg, 0, sizeof(reg));
  reg.intno = 0x10;
  reg.ax = 0x4F02;
  reg.bx = mode | BIT(14);
  if (sys_int86(&reg))
    return 1;

  return 0;
}

uint32_t(pixelIndex)(uint16_t x, uint16_t y) {
  return y * mode_info.YResolution + x;
}
