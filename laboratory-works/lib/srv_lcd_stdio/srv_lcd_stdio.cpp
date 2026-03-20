#include "srv_lcd_stdio.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include <stdio.h>


LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
static bool lcdSetupDone = false;
static bool lcdInitDone = false;

void srvLCDSetup() {    
    if (lcdSetupDone) {
        return;
    }

    static FILE lcd_out;
    fdev_setup_stream(&lcd_out, srvLCDPutChar, NULL, _FDEV_SETUP_WRITE);
    stdout = &lcd_out;
    lcdSetupDone = true;
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
    if (lcdInitDone) {
        return;
    }

    // Force the LCD interface lines to a known idle state before initialization.
    pinMode(LCD_RS_PIN, OUTPUT);
    pinMode(LCD_ENABLE_PIN, OUTPUT);
    pinMode(LCD_D4_PIN, OUTPUT);
    pinMode(LCD_D5_PIN, OUTPUT);
    pinMode(LCD_D6_PIN, OUTPUT);
    pinMode(LCD_D7_PIN, OUTPUT);

    digitalWrite(LCD_RS_PIN, LOW);
    digitalWrite(LCD_ENABLE_PIN, LOW);
    digitalWrite(LCD_D4_PIN, LOW);
    digitalWrite(LCD_D5_PIN, LOW);
    digitalWrite(LCD_D6_PIN, LOW);
    digitalWrite(LCD_D7_PIN, LOW);

    // HD44780 controllers need a clean power-up delay before function-set commands.
    delay(60);
    lcd.begin(LCD_COLS, LCD_ROWS);
    delay(10);
    lcd.clear();
    delay(5);
    lcd.home();

    lcdInitDone = true;
}

void srvLCDClear() {
    lcd.clear();
}

void srvLCDCursor(uint8_t col, uint8_t row) {
    lcd.setCursor(col, row);
}
