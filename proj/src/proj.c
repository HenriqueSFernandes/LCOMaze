#include "proj.h"

int main(int argc, char *argv[]) {
    lcf_set_language("EN-US");
    lcf_trace_calls("/home/lcom/labs/g1/proj/src/log/trace.txt");
    lcf_log_output("/home/lcom/labs/g1/proj/src/log/output.txt");
    if (lcf_start(argc, argv))
        return 1;
    lcf_cleanup();
    return 0;
}

int binary_to_decimal(int bin_num) {
    // declaration of variables
    int decimal_num = 0, temp = 0, rem;
    while (bin_num != 0) {
        rem = bin_num % 10;
        bin_num = bin_num / 10;
        decimal_num = decimal_num + rem * pow(2, temp);
        temp++;
    }
    return decimal_num;
}

int(proj_main_loop)(int argc, char *argv[]) {
    if (argc < 1) {
        printf("Usage: proj [host|client]\n");
        return 1;
    }
    if (strncmp(argv[0], "host", 4) == 0) {
        host = 1;
    }
    else {
        host = 0;
    }
    int ipc_status;
    int receiver;
    uint16_t irq_set_mouse;
    uint8_t irq_set_timer;
    uint16_t irq_set_kbd;
    uint16_t irq_set_rtc;
    message msg;

    if (mouse_subscribe_int(&irq_set_mouse)) {
        printf("Error subscribing to mouse!\n");
        return 1;
    }
    if (kbd_subscribe_int(&irq_set_kbd))
        return 1;

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

    if (timer_set_frequency(0, 45)) {
        printf("Error setting the frequency!\n");
        return 1;
    }

    if (timer_subscribe_int(&irq_set_timer)) {
        printf("Error subscribing to timer!\n");
        return 1;
    }
    if (set_update()) {
        printf("Error activating interrupt mode!\n");
        return 1;
    }
    if (subscribe_rtc(&irq_set_rtc)) {
        printf("Error subscribing to rtc!\n");
        return 1;
    }
    sp_enable_int(0x3f8, (BIT(0))); // Enable receiver available interrupt
    uint32_t irq_set_serie;

    if (sp_subscribe_int(0x3f8, &irq_set_serie)) {
        printf("Error: It wasn't possible to subscribe the interruption\n");
        return -1;
    }

    init_game();

    char c = 1;
    while (running) {
        if ((receiver = driver_receive(ANY, &msg, &ipc_status)) != 0) {
            printf("error driver_receive");
            return 1;
        }
        if (is_ipc_notify(ipc_status)) {
            switch (_ENDPOINT_P(msg.m_source)) {
                case HARDWARE:
                    if (msg.m_notify.interrupts & irq_set_kbd) {
                        kbd_ih();
                        if (kbd_value == 0x81) {
                            game_reset();
                            state = Menu;
                        }
                        if (state == Game) {
                            game_keyboard_handler();
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_mouse) {
                        mouse_ih();
                        sync_bytes();
                        if (byte_index == 3) {
                            byte_index = 0;
                            create_packet();
                        }
                        if (state == Game) {
                            game_mouse_handler();
                        }
                        else if (state == Menu) {
                            menu_mouse_handler();
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_timer) {

                        if (state == Game) {
                            game_main_loop();
                        }
                        else if (state == Menu) {
                            menu_main_loop();
                        }
                        else if (state == HowTo) {
                            info_main_loop();
                        }
                    }
                    if (msg.m_notify.interrupts & irq_set_rtc) {
                        update();
                    }
                    if ((msg.m_notify.interrupts & irq_set_serie) && c != 0) {
                        receive(&c);
                        if (c == 'M' && host == 0) {
                            if (can_enter_multiplayer()) {
                                game_activate_multiplayer();
                            }
                        }
                        if (c == 'S' && host == 1) {
                            game_activate_multiplayer();
                        }
                        if (c == 'L') {
                            printf("You lost\n");
                            if (can_lose()) {
                                game_lose();
                            }
                        }
                        clear_receive_queue();
                    }
                    break;
                default:
                    break;
            }
        }
    }
    if (kbd_unsubscribe_int()) {
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
    if (unsubscribe_rtc()) {
        printf("Error unsubscribing to rtc!\n");
        return 1;
    }
    if (clean_update()) {
        printf("Error deactivating interrupt mode!\n");
        return 1;
    }
    sp_unsubscribe();
    return vg_exit_to_text();
}
