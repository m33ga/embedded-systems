# Serial STDIO Service (srv_serial_stdio)

Redirects standard C input/output streams (stdin and stdout) to hardware UART communication, enabling portable use of `printf()` and `scanf()` functions without exposing platform-specific serial API calls. The service initializes the serial port at 9600 baud and configures custom character read/write callbacks to bridge AVR LIBC's STDIO layer with Arduino's Serial hardware.
