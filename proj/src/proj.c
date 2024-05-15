#include <stdio.h>
#include "controllers/graphics.h"
#include "controllers/keyboard.h"
#include "controllers/mouse.h"
#include "controllers/timer.h"
#include "game/game.h"
#include <lcom/lcf.h>
#include <lcom/vbe.h>
#include <stdint.h>
uint8_t kbd_value;
extern uint8_t cur;
extern double x;
extern double y;

// Any header files included below this line should have been created by you
extern RotationState rotation_state;
extern uint8_t kbd_value;
extern uint8_t byte_index;
extern struct packet mouse_packet;
extern int timerCounter;
extern vbe_mode_info_t mode_info;
double delta=0;

int main(int argc, char *argv[]) {
  lcf_set_language("EN-US");
  lcf_trace_calls("/home/lcom/labs/g1/proj/src/log/trace.txt");
  lcf_log_output("/home/lcom/labs/g1/proj/src/log/output.txt");
  if (lcf_start(argc, argv))
    return 1;
  lcf_cleanup();
  return 0;
}

int(proj_main_loop)(int argc, char *argv[]) {
  rotation_state = STOPPED;
  int ipc_status;
  int receiver;
  x=0;
  y=0;
  uint16_t irq_set_mouse;
  uint8_t irq_set_timer;
  uint16_t irq_set_kbd;
  message msg;
  
  if (mouse_subscribe_int(&irq_set_mouse)) {
    printf("Error subscribing to mouse!\n");
    return 1;
  }
  if(kbd_subscribe_int(&irq_set_kbd)) return 1;
  
  if (mouse_send_command(0xF4)) {
    printf("Error enabling data reporting!\n");
    return 1;
  }
  
  if (setFrameBuffer(0x14C)) {
    printf("Error setting frame buffer!\n");
    return 1;
  }
  
  if (setGraphicsMode(0x14C)) {
    printf("Error setting graphics mode!\n");
    return 1;
  }
  
  if (timer_set_frequency(0, 20)) {
    printf("Error setting the frequency!\n");
    return 1;
  }
  
  if (timer_subscribe_int(&irq_set_timer)) {
    printf("Error subscribing to timer!\n");
    return 1;
  }

  while (!mouse_packet.rb) {
    if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
      printf("error driver_receive");
      return 1;
    }
    if (is_ipc_notify(ipc_status)) {
      switch (_ENDPOINT_P(msg.m_source)) {
        case HARDWARE:
          if (msg.m_notify.interrupts & irq_set_kbd) {
            kbd_ih();
            printf("kbd_value: %x\n", kbd_value);
            cur=kbd_value;
           double x_changer = 0;
          double y_changer = 0;
          if (cur == 0x11) {
                      x_changer = cos(delta);
                      y_changer = sin(delta);
          }else if(cur == 0x1f){
                      x_changer = -cos(delta);
                      y_changer = -sin(delta);
                    
          }else if(cur == 0x1e){
                    x_changer = -sin(delta);
                      y_changer = cos(delta);
                    
          }
          else if (cur == 0x20) {
                      x_changer = sin(delta);
                      y_changer = -cos(delta);
                      
          }
          x+=x_changer*10;
          y-=y_changer*10;
          }
          if (msg.m_notify.interrupts & irq_set_mouse) {
            mouse_ih();
            sync_bytes();
            if (byte_index == 3) {
              byte_index = 0;
              create_packet();
   
            }
          }
          if (msg.m_notify.interrupts & irq_set_timer) {
                      
              //double interval = atan2(mouse_packet.delta_y, mouse_packet.delta_x);
              
          if (mouse_packet.lb) {
              if (mouse_packet.delta_x > 0) {
                  rotation_state = CLOCKWISE;
              } else if (mouse_packet.delta_x < 0) {
                  rotation_state = COUNTER_CLOCKWISE;
              } else {
                  rotation_state = STOPPED;
              }
          } else {
              rotation_state = STOPPED;
          }


            main_loop();
          }
          break;
        default:
          break;
      }
    }
  }
  if(kbd_unsubscribe_int()){
    printf("Error subscribing to keyboard!\n");
    return 1;
  }
  if (mouse_send_command(0xF5)) {
    printf("Error disabling data reporting!\n");
    return 1;
  }
  
  if (mouse_unsubscribe_int()) {
    printf("Error unsubscribing to mouse!\n");
    return 1;
  }
  
  if (timer_unsubscribe_int()) {
    printf("Error unsubscribing to timer!\n");
    return 1;
  }

  return vg_exit();
}
