#include <stdio.h>
#include <string.h>
#include "lw1_2.h"
#include "srv_lcd_stdio.h"
#include "srv_keypad_stdio.h"
#include "dd_led.h"

Led greenLed(53);
Led redLed(51);

void led_access_ok_on() {
    greenLed.on();
    redLed.off();
}

void led_access_denied_on() {
    redLed.on();
    greenLed.off();
}

void lab_1_2_setup() {
    srvLCDSetup();
    srvLCDInit();
    srvKeypadSetup();
}

void readCode(char *buffer, int maxLen)
{
    int i = 0;

    while (i < maxLen - 1)
    {
        char key;
        scanf("%c", &key);

        if (key == '#')
            break;

        if (key)
        {
            buffer[i++] = key;
            printf("*");
        }
    }

    buffer[i] = '\0';
}

void lab_1_2_loop(void)
{
    char code[10];

    while (1)
    {
        srvLCDClear();
        greenLed.off();
        redLed.off();
        printf("Enter Code (#):\n");

        readCode(code, sizeof(code));

        srvLCDClear();

        if (strcmp(code, PASSWORD) == 0)
        {
            printf("ACCESS OK\n");
            led_access_ok_on();
        }
        else
        {
            printf("ACCESS DENIED\n");
            led_access_denied_on();
        }

        // letting the user see the result for a while before clearing the screen and turning off the LEDs
        for (volatile long i = 0; i < 2000000; ++i);
    }
}
