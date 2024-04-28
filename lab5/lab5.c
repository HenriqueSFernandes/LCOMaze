#include <lcom/lcf.h>

#include <lcom/lab5.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you

#include <graphics.h>
#include <keyboard.h>
#include <timer.h>

extern vbe_mode_info_t mode_info;
extern int timerHookId;
extern int kbd_hook_id;
extern uint8_t kbd_value;
extern int timerCounter;
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
        uint32_t R = ((first >> mode_info.RedFieldPosition) + j * step) % (1 << mode_info.RedMaskSize);
        uint32_t G = ((first >> mode_info.GreenFieldPosition) + i * step) % (1 << mode_info.GreenMaskSize);
        uint32_t B = ((first >> mode_info.BlueFieldPosition) + (i + j) * step) % (1 << mode_info.BlueMaskSize);
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
}

int(video_test_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
  if (setFrameBuffer(0x105)) {
    printf("Error setting frame buffer\n");
    return 1;
  }
  if (setGraphicsMode(0x105)) {
    printf("Error setting graphics mode\n");
    return 1;
  }
  xpm_image_t img;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (vg_draw_pixel(x + j, y + i, *colors))
        return 1;
      colors++;
    }
  }

  if (waitForESC()) {
    printf("Keyboard error\n");
    return 1;
  }
  return vg_exit();
}

int(video_test_move)(xpm_map_t xpm, uint16_t xi, uint16_t yi, uint16_t xf, uint16_t yf,
                     int16_t speed, uint8_t fr_rate) {
  bool horizontalMovement = true;
  bool running = true;
  uint16_t x = xi;
  uint16_t y = yi;
  if (xi == xf)
    horizontalMovement = false;
  if (timer_set_frequency(0, fr_rate)) {
    printf("Error setting the timer\n");
    return 1;
  }
  if (setFrameBuffer(0x105)) {
    printf("Error setting frame buffer\n");
    return 1;
  }
  if (setGraphicsMode(0x105)) {
    printf("Error setting graphics mode\n");
    return 1;
  }
  int ipc_status;
  int receiver;
  uint8_t irq_set_timer;
  uint8_t irq_set_kbd;
  message msg;
  kbd_value = 0x00;
  uint8_t keys[2] = {0x00, 0x00};

  if (timer_subscribe_int(&irq_set_timer) != 0) {
    printf("Error subscribing to timer\n");
    return 1;
  }
  if (kbd_subscribe_int(&irq_set_kbd) != 0) {
    printf("Error subscribing to keyboard\n");
    return 1;
  }
  xpm_image_t img;
  uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      if (vg_draw_pixel(x + j, y + i, *colors))
        return 1;
      colors++;
    }
  }
  int16_t waitingCounter = abs(speed);
  while (kbd_value != 0x81) {
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          // If the interrupt is for the keyboard, call the interrupt handler.
          if (msg.m_notify.interrupts & irq_set_kbd) {
            // Call the interrupt handler.
            if (kbd_ih())
              return 1;
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
          if (msg.m_notify.interrupts & irq_set_timer) {
            timer_int_handler();
            if (running && (speed > 0 || (speed < 0 && waitingCounter == 0))) {
              waitingCounter = abs(speed) + 1;

              printf("tick %d\n", timerCounter);
              xpm_image_t img;
              uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
              for (int i = 0; i < img.height; i++) {
                for (int j = 0; j < img.width; j++) {
                  if (vg_draw_pixel(x + j, y + i, 0))
                    return 1;
                }
              }
              if (horizontalMovement) {
                if (speed < 0) {
                  x++;
                }
                else {
                  x += speed;
                }
              }
              else {
                if (speed < 0) {
                  y++;
                }
                else {
                  y += speed;
                }
              }

              if (x > xf)
                x = xf;
              if (y > yf)
                y = yf;
              for (int i = 0; i < img.height; i++) {
                for (int j = 0; j < img.width; j++) {
                  if (vg_draw_pixel(x + j, y + i, *colors))
                    return 1;
                  colors++;
                }
              }
              if (x == xf && y == yf)
                running = false;
            }
            waitingCounter--;
          }
          break;
        default:
          break;
      }
    }
  }
  if (timer_unsubscribe_int())
    return 1;
  if (kbd_unsubscribe_int())
    return 1;
  return vg_exit();
}

int(video_test_controller)() {
  /* To be completed */
  printf("%s(): under construction\n", __func__);

  return 1;
}
