#include "lw1_1.h"
#include "srv_serial_stdio.h"
#include "dd_led.h"
#include <Arduino.h>

Led led_1_1(53);

void lab_1_1_setup() {
    srvSerialSetup();
    printf("Serial communication initialized!\n");
    printf("Available commands:\n  'ledon' - turn the LED on\n  'ledoff' - turn the LED off\n");
}

void lab_1_1_loop() {
    char c[11] = "";
    if (scanf("%10s", c) > 0) {
        printf("Received command: %s\n", c);
        if (strcmp(c, "ledon") == 0) {
            if (led_1_1.isOn()) {
                printf("Led is already on\n");
            } else {
                led_1_1.on();
                printf("Led was Turned On\n");
            }
        } else if (strcmp(c, "ledoff") == 0) {
            if (!led_1_1.isOn()) {
                printf("Led is already off\n");
            } else {
                led_1_1.off();
                printf("Led was Turned Off\n");
            }
        } else {
            printf("Invalid command\n");
        }
    }
}