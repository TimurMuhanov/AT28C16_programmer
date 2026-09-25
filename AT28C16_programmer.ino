/*
 * Commands formats:
 *    ".WAAADD+" ---- write to address AAA (in hex) data (DD) with checksum ('+' - it is low nibble of sum: 0+every char)
 *        answer: ".WAAADD-" - of OK, ".WxxxER-" - if ERROR where xxx = number of error, ('-' - difference: 0-every char)
 *    ".RAAA00+" ---- read byte from AAA (in hex), 00 it is "00"
 *        answer: ".RAAADD-" or ".RxxxER-"
 *    ".TPPP0x+" ---- test pin, x=0/1, PPP - number of pin for test (default all = 1)
 *        answer: ".TPPP0x-" or ".TxxxER-"
 */

#define DIP48_1  24
#define DIP48_2  25
#define DIP48_3  26
#define DIP48_4  27
#define DIP48_5  28
#define DIP48_6  29
#define DIP48_7  30
#define DIP48_8  31
#define DIP48_9  32
#define DIP48_10 33
#define DIP48_11 54
#define DIP48_12 55
#define DIP48_13 56
#define DIP48_14 57
#define DIP48_15 58
#define DIP48_16 59
#define DIP48_17 60
#define DIP48_18 61
#define DIP48_19 62
#define DIP48_20 63
#define DIP48_21 64
#define DIP48_22 65
#define DIP48_23 66
#define DIP48_24 67
#define DIP48_25 23
#define DIP48_26 22
#define DIP48_27 21
#define DIP48_28 20
#define DIP48_29 19
#define DIP48_30 18
#define DIP48_31 17
#define DIP48_32 16
#define DIP48_33 15
#define DIP48_34 14
#define DIP48_35 34
#define DIP48_36 35
#define DIP48_37 2
#define DIP48_38 3
#define DIP48_39 4
#define DIP48_40 5
#define DIP48_41 6
#define DIP48_42 7
#define DIP48_43 8
#define DIP48_44 9
#define DIP48_45 10
#define DIP48_46 11
#define DIP48_47 12
#define DIP48_48 13

const uint8_t pin_dip48[48] = {
    DIP48_1,  DIP48_2,  DIP48_3,  DIP48_4,  DIP48_5,  DIP48_6,  DIP48_7,  DIP48_8, 
    DIP48_9,  DIP48_10, DIP48_11, DIP48_12, DIP48_13, DIP48_14, DIP48_15, DIP48_16, 
    DIP48_17, DIP48_18, DIP48_19, DIP48_20, DIP48_21, DIP48_22, DIP48_23, DIP48_24, 
    DIP48_25, DIP48_26, DIP48_27, DIP48_28, DIP48_29, DIP48_30, DIP48_31, DIP48_32, 
    DIP48_33, DIP48_34, DIP48_35, DIP48_36, DIP48_37, DIP48_38, DIP48_39, DIP48_40, 
    DIP48_41, DIP48_42, DIP48_43, DIP48_44, DIP48_45, DIP48_46, DIP48_47, DIP48_48
};

#define AT28C16_A7 DIP48_1
#define AT28C16_A6 DIP48_2
#define AT28C16_A5 DIP48_3
#define AT28C16_A4 DIP48_4
#define AT28C16_A3 DIP48_5
#define AT28C16_A2 DIP48_6
#define AT28C16_A1 DIP48_7
#define AT28C16_A0 DIP48_8
#define AT28C16_D0 DIP48_9
#define AT28C16_D1 DIP48_10
#define AT28C16_D2 DIP48_11
#define AT28C16_G  DIP48_12
#define AT28C16_D3 DIP48_37
#define AT28C16_D4 DIP48_38
#define AT28C16_D5 DIP48_39
#define AT28C16_D6 DIP48_40
#define AT28C16_D7 DIP48_41
#define AT28C16_CE DIP48_42
#define AT28C16_A10 DIP48_43
#define AT28C16_OE DIP48_44
#define AT28C16_WE DIP48_45
#define AT28C16_A9 DIP48_46
#define AT28C16_A8 DIP48_47
#define AT28C16_V  DIP48_48

const uint8_t at28c16_addr_pins[11] = {
    AT28C16_A0, AT28C16_A1, AT28C16_A2, AT28C16_A3,
    AT28C16_A4, AT28C16_A5, AT28C16_A6, AT28C16_A7,
    AT28C16_A8, AT28C16_A9, AT28C16_A10
};

const uint8_t at28c16_data_pins[8] = {
    AT28C16_D0, AT28C16_D1, AT28C16_D2, AT28C16_D3,
    AT28C16_D4, AT28C16_D5, AT28C16_D6, AT28C16_D7
};

#define SERIAL_BUF_SIZE 16
char rx_buf[SERIAL_BUF_SIZE], tx_buf[SERIAL_BUF_SIZE];
char rx_buf_pointer, tx_buf_pointer;
char flag_rx_start_received;
#define SERIAL_RX_START_CHAR '.'
#define SERIAL_RX_COMMAND_SIZE 8

#define ERROR_CHECK_SUM 1

void setup() {
    // initialize gpio
    for (char i=0; i<48; i++) {
        pinMode(pin_dip48[i],OUTPUT);
        digitalWrite(pin_dip48[i],HIGH);
    }
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

char doCommand(char cmd, uint16_t addr, uint8_t *data) {
    if (addr >= 0x800) {
        return 0x20;
    }
    uint8_t tmp_data = *data;
    uint16_t tmp_addr = addr;
    if ((cmd == 'W') || (cmd == 'R')) {
        for (char i=0; i<11; i++) {
            if (tmp_addr & 1) { digitalWrite(at28c16_addr_pins[i],HIGH); }
            else { digitalWrite(at28c16_addr_pins[i],LOW); }
            tmp_addr >>= 1;
        }
    }
    switch (cmd) {
        case 'W':
            for (char i=0; i<8; i++) {
                if (tmp_data & 1) { digitalWrite(at28c16_data_pins[i],HIGH); }
                else { digitalWrite(at28c16_data_pins[i],LOW); }
                tmp_data >>= 1;
            }
            digitalWrite(AT28C16_CE,LOW);
            delay(1);
            digitalWrite(AT28C16_WE,LOW);
            delay(1);
            digitalWrite(AT28C16_WE,HIGH);
            digitalWrite(AT28C16_CE,HIGH);
            for (char i=0; i<8; i++) { digitalWrite(at28c16_data_pins[i],HIGH); }
            break;
        case 'R':
            for (char i=0; i<8; i++) { pinMode(at28c16_data_pins[i],INPUT); }
            digitalWrite(AT28C16_CE,LOW);
            digitalWrite(AT28C16_OE,LOW);
            delay(1);
            tmp_data = 0;
            for (char i=0; i<8; i++) {
                tmp_data >>= 1;
                if (digitalRead(at28c16_data_pins[i]) == HIGH) {
                    tmp_data |= 0x80;
                }
            }
            digitalWrite(AT28C16_OE,HIGH);
            digitalWrite(AT28C16_CE,HIGH);
            for (char i=0; i<8; i++) { pinMode(at28c16_data_pins[i],OUTPUT); }
            *data = tmp_data;
            break;
        case 'T':
            if ((addr > 48) || (addr < 1)) {
                return 0x22;
            }
            if ((*data) > 1) { return 0x23; }
            if (*data) { digitalWrite(pin_dip48[addr-1],HIGH); }
            else { digitalWrite(pin_dip48[addr-1],LOW); }
            break;
        default:
            return 0x21;
    }
    if ((cmd == 'W') || (cmd == 'R')) {
        for (char i=0; i<11; i++) {
            digitalWrite(at28c16_addr_pins[i],HIGH);
        }
    }
    return 0;
}

char numFromHex(char *buf, char sz, uint16_t *res) {
//    Serial.print("numFromHex ");
    uint16_t tmp_res = 0;
    char error = 0, tmp;
    for (char i=0;i<sz;i++) {
        tmp = buf[i];
        if ((tmp >= 'A') && (tmp <= 'F')) {
            tmp -= ('A'-10);
        } else {
            if ((tmp >= '0') && (tmp <= '9')) {
                tmp -= '0';
            } else {
//                Serial.print(" error in symbol ");
//                Serial.println(buf[i]);
                error = i + 0x10;
                break;
            }
        }
        tmp_res <<= 4;
        tmp_res |= (uint16_t)tmp;
    }
    *res = tmp_res;
    return error;
}

void numToHex(char *buf, char sz, uint16_t num) {
    char new_sz = sz;
    char tmp;
    uint16_t num_tmp = num;
    if (sz>4) { new_sz = 4; }
    for (char i=0; i<new_sz; i++) {
        tmp = (char)(num_tmp & 0xf);
        if (tmp>9) { tmp += ('A'-10); }
        else { tmp += '0'; }
        buf[new_sz-1-i] = tmp;
        num_tmp >>= 4;
    }
}

void decodeCommand(void) {
    // check checksum
    char tmp=0, error=0;
    for (char i=0; i<SERIAL_RX_COMMAND_SIZE-1; i++) {
        tmp += rx_buf[i];
    }
    tmp &= 0xf;
    if (tmp > 9) { tmp += 'A'-10; }
    else { tmp += '0'; }
    if (tmp != rx_buf[SERIAL_RX_COMMAND_SIZE-1]) {
        error = 1;
    }
//    Serial.write("Error ");
//    Serial.println((int)error);
//    Serial.println((int)tmp);
    // get address
    uint16_t addr=0;
    if (!error) {
        error = numFromHex(rx_buf+2,3,&addr);
    }
//    Serial.write("Error ");
//    Serial.println((int)error);
    // get data (if need)
    uint8_t data=0;
    if (!error) {
        uint16_t tmp16;
        error = numFromHex(rx_buf+5,2,&tmp16);
        if (!error) {
            data = (uint8_t)tmp16;
        }
    }
//    Serial.print("addr=");
//    Serial.print((int)addr);
//    Serial.print(" data=");
//    Serial.println((int)data);
    // do it command
    if (!error) {
        error = doCommand(rx_buf[1],addr,&data);
    }
//    Serial.write("Error ");
//    Serial.println((int)error);
    // clear flag received
    flag_rx_start_received = 0;
    // answer
    for (char i=0; i<8; i++) { tx_buf[i] = rx_buf[i]; }
    if (error) {
        numToHex(tx_buf+2,3,error);
        tx_buf[5] = 'E';
        tx_buf[6] = 'R';
    } else {
        if (tx_buf[1] == 'R') {
            numToHex(tx_buf+5,2,data);
        }
    }
//    Serial.write("Error ");
//    Serial.println((int)error);
    tmp = 0;
    for (char i=0; i<7; i++) { tmp -= tx_buf[i]; }
    tmp &= 0xf;
    numToHex(tx_buf+7,1,tmp);
    Serial.write(tx_buf,8);
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
