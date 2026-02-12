# LED Driver (dd_led)

Provides a reusable object-oriented interface for controlling LED peripherals connected to any GPIO pin. The driver abstracts digital write operations behind a simple `Led` class with methods for turning the LED on/off, toggling state, and querying current state. Supports both active-high (standard) and active-low (e.g., ESP8266 built-in LED) configurations via constructor parameters.
