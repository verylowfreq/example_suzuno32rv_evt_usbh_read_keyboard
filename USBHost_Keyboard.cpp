#include "USBHost_Keyboard.h"

#include "ch32v20x_usbfs_host.h"
#include "app_km.h"
#include "usb_host_config.h"

volatile int is_usbkeyboard_connected;
volatile uint8_t global_hid_keyboard_report[8];


void USBHostKeyboard::init() {
    is_usbkeyboard_connected = false;
    memset((void*)global_hid_keyboard_report, 0x00, 8);

    TIM3_Init( 9, SystemCoreClock / 10000 - 1 );
    USBFS_RCC_Init( );
    USBFS_Host_Init( ENABLE );
    memset( &RootHubDev.bStatus, 0, sizeof( ROOT_HUB_DEVICE ) );
    memset( &HostCtl[ DEF_USBFS_PORT_INDEX * DEF_ONE_USB_SUP_DEV_TOTAL ].InterfaceNum, 0, DEF_ONE_USB_SUP_DEV_TOTAL * sizeof( HOST_CTL ) );

}

void USBHostKeyboard::update() {
    USBH_MainDeal();
}

bool USBHostKeyboard::connected() {
    return is_usbkeyboard_connected;
}

int USBHostKeyboard::get_key() {
    if (!this->connected()) {
        return -1;
    }
    for (int i = 0; i < 6; i++) {
        uint8_t ch = global_hid_keyboard_report[2 + i];
        if (ch != 0x00) {
            return ch;
        }
    }
    return -1;
}


void USBHostKeyboard::get_keyboard_report(uint8_t buf[8]) {
    if (!this->connected()) {
        memset(buf, 0x00, 8);
    } else {
        memcpy(buf, (void*)global_hid_keyboard_report, 8);
    }
}
