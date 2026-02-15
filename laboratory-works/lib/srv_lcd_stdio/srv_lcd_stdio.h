#ifndef SRV_LCD_STDIO_H
#define SRV_LCD_STDIO_H

#include <stdio.h>

#define LCD_ROWS 2
#define LCD_COLS 16
#define LCD_RS_PIN 50
#define LCD_ENABLE_PIN 52
#define LCD_D4_PIN 48
#define LCD_D5_PIN 46
#define LCD_D6_PIN 44
#define LCD_D7_PIN 42

void srvLCDSetup();
int srvLCDPutChar(char c, FILE* stream);
void srvLCDInit();
void srvLCDClear();
void srvLCDCursor(uint8_t col, uint8_t row);

#endif