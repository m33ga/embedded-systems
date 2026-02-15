#include "lw1_1.h"
#include "srv_serial_stdio.h"
#include "dd_led.h"
#include <Arduino.h>

Led led_1_1(53);

void lab_1_1_setup() {
    srvSerialSetup();
    printf("Serial communication initialized!\n");
    printf("Available commands:\n  'led on' - turn the LED on\n  'led off' - turn the LED off\n");
}

void lab_1_1_loop() {
    char word1[5] = "";
    char word2[5] = "";

    if (scanf("%4s %4s", word1, word2) == 2) {
        printf("Received command: %s %s\n", word1, word2);

        if (strcmp(word1, "led") == 0 && strcmp(word2, "on") == 0) {
            if (led_1_1.isOn()) {
                printf("Led is already on\n");
            } else {
                led_1_1.on();
                printf("Led was Turned On\n");
            }
        } else if (strcmp(word1, "led") == 0 && strcmp(word2, "off") == 0) {
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