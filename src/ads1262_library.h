#ifndef ADS1262_LIBRARY_H
#define ADS1262_LIBRARY_H

#include <stdint.h>

// Register Read Commands
#define CONFIG_SPI_MASTER_DUMMY   0xFF
#define RREG    0x20
#define WREG    0x40
#define START   0x08
#define STOP    0x0A
#define RDATAC  0x10
#define SDATAC  0x11
#define RDATA   0x12

// Pin declaration (WiringPi pin numbers)
// https://github.com/WiringPi/WiringPi
// Temporary pin assignments for all but GPIO_DOUT_DRDY
#define GPIO_DRDY 5   // GPIO 24
#define GPIO_RESET 4  // GPIO 23
#define GPIO_START 6  // GPIO 25
#define ADS1220_CS_PIN 3   // GPIO 22
#define GPIO_DOUT_DRDY 13 // GPIO 9

// Register addresses
#define POWER    0x01
#define INTERFACE 0x02
#define MODE0    0x03
#define MODE1    0x04
#define MODE2    0x05
#define INPMUX   0x06
#define OFCAL0   0x07
#define OFCAL1   0x08
#define OFCAL2   0x09
#define FSCAL0   0x0A
#define FSCAL1   0x0B
#define FSCAL2   0x0C
#define IDACMUX  0x0D
#define IDACMAG  0x0E
#define REFMUX   0x0F
#define TDACP    0x10
#define TDACN    0x11
#define GPIOCON  0x12
#define GPIODIR  0x13
#define GPIODAT  0x14
#define ADC2CFG  0x15
#define ADC2MUX  0x16
#define ADC2OFC0 0x17
#define ADC2OFC1 0x18
#define ADC2FSC0 0x19
#define ADC2FSC1 0x1A

void ads1262_Init(void);
void ads1262_Reset(void);
void ads1262_Reg_Write(uint8_t READ_WRITE_ADDRESS, uint8_t DATA);
void ads1262_SPI_Command_Data(uint8_t data_in);
void ads1262_Disable_Start(void);
void ads1262_Enable_Start(void);
void ads1262_Hard_Stop(void);
void ads1262_Start_Data_Conv_Command(void);
void ads1262_Soft_Stop(void);
void ads1262_Start_Read_Data_Continuous(void);
void ads1262_Stop_Read_Data_Continuous(void);
char* ads1262_Read_Data(void);

#endif
