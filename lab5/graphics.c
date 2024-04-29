#include <graphics.h>
#include <keyboard.h>

extern int kbd_hook_id;
extern uint8_t kbd_value;

int setFrameBuffer(uint16_t mode) {
  // Set mode_info to 0
  memset(&mode_info, 0, sizeof(mode_info));
  // Get the info
  if (vbe_get_mode_info(mode, &mode_info))
    return 1;

  // Calculating the bytes per pixel, by rounding up the bits per pixel and dividing it by 8.
  uint32_t bytesPerPixel = (mode_info.BitsPerPixel + 7) / 8;
  // Calculate the frame size (width * height * bytes per pixel).
  uint32_t frameSize = mode_info.XResolution * mode_info.YResolution * bytesPerPixel;

  // Allocate physical addresses
  struct minix_mem_range physicAddresses;
  // The start of the physical address.
  physicAddresses.mr_base = mode_info.PhysBasePtr;
  // The end of the physical address, which is the start + size.
  physicAddresses.mr_limit = physicAddresses.mr_base + frameSize;

  // TODO wtf is this
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physicAddresses))
    return 1;

  // Allocate virtual addresses.
  frame_buffer = vm_map_phys(SELF, (void *) physicAddresses.mr_base, frameSize);
  return frame_buffer == NULL;
}

int setGraphicsMode(uint16_t mode) {
  // Initialize the struct.
  reg86_t reg86;
  // Set the struct to 0.
  memset(&reg86, 0, sizeof(reg86));
  reg86.intno = 0x10;         // BIOS video service.
  reg86.ah = 0x4F;            // ax higher bits.
  reg86.al = 0x02;            // ax lower bits.
  reg86.bx = mode | BIT(14);  // Set the mode with bit 14 (linear memory mapping).
  // Send the command.
  if (sys_int86(&reg86) != 0)
    return 1;
  return 0;
}

int(waitForESC)() {
  kbd_value = 0x00;
  int ipc_status;  // Indicates if a message has been received.
  int receiver;    // Indicates if there was an error receiving the message.
  uint8_t irq_set; // Value that indicates which interrupts the program should care about.
  message msg;     // The message itself.
  uint8_t keys[2] = {0x00, 0x00};

  // Subscribe to the interruptions.
  if (kbd_subscribe_int(&irq_set) != 0)
    return 1;

  // Loop until the key is ESC.
  while (kbd_value != 0x81) {
    // Check if there is a message available
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // If the interrupt is for the keyboard, call the interrupt handler.
          if (msg.m_notify.interrupts & irq_set) {
            // Call the interrupt handler.
            kbd_ih();
            // If the previous key was 0xE0, then it means the makecode and breakcode for that key has size 2.
            if (keys[0] == 0xE0) {
              keys[1] = kbd_value;
            }
            else {
              keys[0] = kbd_value;
            }
            if (kbd_value != 0xE0) {
              keys[0] = 0x00;
            }
          }
          break;
        default:
          break;
      }
    }
  }
  return kbd_unsubscribe_int();
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
  return memcpy(&frame_buffer[index], &color, bytesPerPixel) == NULL;
}

int(vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for (uint16_t i = x; i < width + x; i++) {
    for (uint16_t j = y; j < height + y; j++) {
      if (vg_draw_pixel(i, j, color)) {
        vg_exit();
        return 1;
      }
    }
  }
  return 0;
}
