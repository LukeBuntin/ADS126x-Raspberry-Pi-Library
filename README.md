# ADS126x-Raspberry-Pi-Library
(Unfinished) Library for communicating with the ADS1262 and ADS1263 using a Raspberry Pi

# Connections from Raspberry Pi to ADS1262 ADC Chip

## Raspberry Pi to ADS1262 Pin Connections

| Raspberry Pi Pin (BCM) | Raspberry Pi Pin Name | ADS1262 Pin Name    | Function               |
|------------------------|-----------------------|---------------------|------------------------|
| GND                    | Ground                | CS                  | Chip Select            |
| FLOAT                  | N/A                   | RESET               | Reset                  |
| FLOAT                  | N/A                   | DRDY                | Data Ready             |
| FLOAT                  | N/A                   | START               | Start Conversion       |
| 10                     | MOSI (GPIO 10)        | DIN                 | SPI Data In            |
| 9                      | MISO (GPIO 9)         | DOUT                | SPI Data Out           |
| 11                     | SCLK (GPIO 11)        | SCLK                | SPI Clock              |
| GND                    | Ground                | DGND                | Digital Ground         |
| 3.3V                   | 3.3V                  | DVDD                | Digital Power Supply   |
| 3.3V                   | 3.3V                  | AVDD                | Analog Power Supply    |
| GND                    | Ground                | AVSS                | Analog Ground          |

## Description

1. **Chip Select (CS)**: This pin is connected to GPIO 22 on the Raspberry Pi. It is used to select the ADS1262 chip for SPI communication (Switch to a GPIO Pin if you are using multiple chips).
2. **Reset (RESET)**: This pin is connected to float. It is unused.
3. **Data Ready (DRDY)**: This pin is connected to float. We are using DOUT/DRDY.
4. **Start Conversion (START)**: This pin is connected to float. We are using the SPI command.
5. **SPI Data In (DIN)**: This pin is connected to the MOSI pin (GPIO 10) on the Raspberry Pi. It is used for sending data to the ADS1262.
6. **SPI Data Out (DOUT)**: This pin is connected to the MISO pin (GPIO 9) on the Raspberry Pi. It is used for receiving data from the ADS1262.
7. **SPI Clock (SCLK)**: This pin is connected to the SCLK pin (GPIO 11) on the Raspberry Pi. It is used for the SPI clock signal.
8. **Digital Ground (DGND)**: This pin is connected to a ground pin on the Raspberry Pi. It is used to provide a common digital ground between the Raspberry Pi and the ADS1262.
9. **Digital Power Supply (DVDD)**: This pin is connected to the 3.3V power supply pin on the Raspberry Pi. It provides digital power to the ADS1262.
10. **Analog Power Supply (AVDD)**: This pin is connected to the 3.3V power supply pin on the Raspberry Pi. It provides analog power to the ADS1262.
11. **Analog Ground (AVSS)**: This pin is connected to a ground pin on the Raspberry Pi. It is used to provide a common analog ground for the ADS1262.

## How to Use

1. **Connect the Raspberry Pi to the ADS1262**: Follow the connection table above to connect the appropriate pins from the Raspberry Pi to the ADS1262 ADC chip.
2. **Configure and Initialize the ADS1262**: Use the provided `ads1262_library.h` and `ads1262_library.c` files to initialize and configure the ADS1262.
3. **Read Data**: Continuously read data from the ADS1262 using the provided example in `ads1262_communication.c`.

This setup allows for accurate analog-to-digital conversion using the ADS1262 connected to a Raspberry Pi.

## Manual Compilation Instructions

To manually compile the code using `gcc`, follow these steps:

1. **Ensure WiringPi and SPI Libraries are Installed**:
   Make sure that WiringPi and SPI libraries are installed on your Raspberry Pi.

   ```bash
   sudo apt-get update
   sudo apt-get install wiringpi libwiringpi-dev
   gcc -o ads1262_example ads1262_communication.c ads1262_library.c -lwiringPi -lwiringPiDev
   sudo ./ads1262_example

