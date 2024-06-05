#include "mouse.h"

int mouse_hook_id = 12;
uint8_t current_byte = 0;
uint8_t byte_index = 0;
uint8_t mouse_bytes[3];
struct packet mouse_packet;

int(mouse_subscribe_int)(uint16_t *irq_set) {
    if (irq_set == NULL)
        return 1;
    *irq_set = BIT(mouse_hook_id);
    return sys_irqsetpolicy(12, IRQ_REENABLE | IRQ_EXCLUSIVE, &mouse_hook_id);
}

int(mouse_unsubscribe_int)() {
    return sys_irqrmpolicy(&mouse_hook_id);
}

void(mouse_ih)() {
    // Read the status from the KBC.
    uint8_t status;
    if (util_sys_inb(0x64, &status))
        return;

    // Check for error in the status.
    if (status & BIT(7)) {
        printf("Parity error!\n");
        return;
    }
    if (status & BIT(6)) {
        printf("Timeout error!\n");
        return;
    }

    // If the output buffer is full, data is available for reading.
    if ((status & BIT(0)) != 0) {
        if (status & BIT(5)) {
            util_sys_inb(0x60, &current_byte);
        }
    }
}

void(sync_bytes)() {
    if ((byte_index == 0 && (current_byte & BIT(3))) || (byte_index > 0)) {
        mouse_bytes[byte_index] = current_byte;
        byte_index++;
    }
}

void create_packet() {
    for (int i = 0; i < 3; i++) {
        mouse_packet.bytes[i] = mouse_bytes[i];
    }

    mouse_packet.rb = mouse_bytes[0] & BIT(1);
    mouse_packet.mb = mouse_bytes[0] & BIT(2);
    mouse_packet.lb = mouse_bytes[0] & BIT(0);
    mouse_packet.delta_x = (mouse_bytes[0] & BIT(4)) ? (0xFF00 | mouse_bytes[1]) : mouse_bytes[1];
    mouse_packet.delta_y = (mouse_bytes[0] & BIT(5)) ? (0xFF00 | mouse_bytes[2]) : mouse_bytes[2];
    mouse_packet.x_ov = mouse_bytes[0] & BIT(6);
    mouse_packet.y_ov = mouse_bytes[0] & BIT(7);
}

int(kbc_send_command)(uint8_t port, uint8_t command) {
    uint8_t status;
    if (util_sys_inb(0x64, &status)) {
        printf("Error reading KBC status");
        return 1;
    }
    if ((status & BIT(1)) == 0) {
        if (sys_outb(port, command)) {
            printf("Error sending command!");
            return 1;
        }
        return 0;
    }
    return 1;
}

int(mouse_send_command)(uint8_t command) {
    uint8_t attemps = 10;
    uint8_t response;

    // Keep trying until the response is different than ACK (0xFA) and attemps are positive.
    do {
        attemps--;
        // Request forwarding of command to the mouse.
        if (kbc_send_command(0x64, 0xD4))
            return 1;
        // Send the command to the mouse.
        if (kbc_send_command(0x60, command))
            return 1;

        // Wait for a bit.
        tickdelay(micros_to_ticks(10000));

        // Read acknowledgement from the mouse.
        if (util_sys_inb(0x60, &response))
            return 1;
        // If the response is ACK, return.
        if (response == 0xFA)
            return 0;

    } while (response != 0xFA && attemps > 0);

    return 1;
}
