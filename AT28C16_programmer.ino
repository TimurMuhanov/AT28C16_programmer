/*
 * Commands formats:
 *    ".WAAADDS" ---- write to address AAA (in hex) data (DD) with checksum (S)
 *        answer: ".WAAADDS" - of OK, ".WExxxxx" - if ERROR where xxxxx = number of error
 *    ".RAAA00S" ---- read byte from AAA (in hex), 00 it is "00", S - checksum
 *        answer: ".RAAADDS" - if OK DD-data, ".RExxxxx" - if ERROR where xxxxx = number of error
 */

#define SERIAL_BUF_SIZE 16
char rx_buf[SERIAL_BUF_SIZE], tx_buf[SERIAL_BUF_SIZE];
char rx_buf_pointer, tx_buf_pointer;
char flag_rx_start_received;
#define SERIAL_RX_START_CHAR '.'
#define SERIAL_RX_COMMAND_SIZE 8

void setup() {
    // initialize gpio
    // initialize variable
    for (char i=0; i<SERIAL_BUF_SIZE; i++) {
        rx_buf[i] = 0;
        tx_buf[i] = 0;
    }
    flag_rx_start_received = 0;
    rx_buf_pointer = 0;
    tx_buf_pointer = 0;
    // initialize serial
    Serial.begin(115200);
}

void doCommand(char cmd, uint16_t addr, uint8_t data) {
    // try to do it
    // prepare answer
    // answer
}

void decodeCommand(void) {
    // check checksum
    // check all hex symbols
    // check command
    // get address
    // get data (if need)
    // do it command
}

void loop() {
    // wait for new byte
    if (Serial.available()>0) {
        char c = Serial.read();
        // wait for start char of command
        if (!flag_rx_start_received) {
            rx_buf_pointer = 0;
            if (c == SERIAL_RX_START_CHAR) {
                rx_buf[rx_buf_pointer] = c;
                flag_rx_start_received = 1;
                rx_buf_pointer++;
            }
        } else {
            // receiving command
            rx_buf[rx_buf_pointer] = c;
            rx_buf_pointer++;
            if (rx_buf_pointer >= SERIAL_RX_COMMAND_SIZE) {
                // if command was received ---- decode it and do it
                decodeCommand();
            }
        }
    }
}
