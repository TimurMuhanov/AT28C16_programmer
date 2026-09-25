This project for programming parallel EEPROM AT28C16 by Arduino MEGA 2560

For program

1) set right pins in arduino sketch

2) just do it
``` bash
python3 AT28C16_programmer.py /dev/ttyUSB0 relative/path/to/file.bin
```

now speed of full program (2k) is 86 sec (or 41ms per byte) : 24 byte/sec