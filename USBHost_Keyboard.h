#pragma once

#include <stddef.h>
#include <stdint.h>

class USBHostKeyboard {
public:
    bool is_keyboard_connected;

    void init();
    void update();
    bool connected();
    int get_key();
    void get_keyboard_report(uint8_t buf[8]);
};

extern "C" {
  extern volatile uint8_t global_hid_keyboard_report[8];
  extern volatile int is_usbkeyboard_connected;
}
