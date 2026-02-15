#include "srv_lcd_stdio.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>


LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

void srvLCDSetup() {    
    static FILE lcd_out;
    fdev_setup_stream(&lcd_out, srvLCDPutChar, NULL, _FDEV_SETUP_WRITE);
    stdout = &lcd_out;
}

int srvLCDPutChar(char c, FILE *stream)
{
    if (c == '\n')
    {
        // Move to second line like a terminal
        lcd.setCursor(0, 1);
    }
    else if (c == '\r')
    {
        // Ignore carriage return
    }
    else
    {
        lcd.write(c);
    }

    return 0;
}

void srvLCDInit() {
    lcd.begin(LCD_COLS, LCD_ROWS);
    lcd.clear();
}

void srvLCDClear() {
    lcd.clear();
}

void srvLCDCursor(uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
}
