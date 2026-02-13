#include "srv_serial_stdio.h"
#include <Arduino.h>
#include <stdio.h>

int srvSerialGetChar(FILE* stream) {
    while (!Serial.available()) {
        ;
    }

    char c = Serial.read();

    if (c == '\r') {              // handle Enter properly
        Serial.write('\r');
        Serial.write('\n');
        return '\n';              // scanf expects '\n'
    } else {
        Serial.write(c);          // echo typed character
        return c;
    }
}


int srvSerialPutChar(char c, FILE* stream) {
    if (c == '\n') {
        Serial.write('\r');
    }
    Serial.write(c);
    return 0;
}

void srvSerialSetup() {
    Serial.begin(9600);

    static FILE serial_out;
    fdev_setup_stream(&serial_out, srvSerialPutChar, NULL, _FDEV_SETUP_WRITE);
    stdout = &serial_out;
    
    static FILE serial_in;
    fdev_setup_stream(&serial_in, NULL, srvSerialGetChar, _FDEV_SETUP_READ);
    stdin = &serial_in;
}
