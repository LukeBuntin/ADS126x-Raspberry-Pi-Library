#include "ads1262_library.h"
#include <wiringPi.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <unistd.h> // for usleep
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define SPI_DEVICE "/dev/spidev0.0"
#define SPI_MODE SPI_MODE_1
#define SPI_SPEED 1000000

int spi_fd;

void ads1262_Init(void)
{
    wiringPiSetup();
    pinMode(GPIO_DRDY, INPUT);
    pinMode(GPIO_DOUT_DRDY, INPUT);
    pinMode(GPIO_RESET, OUTPUT);
    pinMode(GPIO_START, OUTPUT);
    pinMode(ADS1220_CS_PIN, OUTPUT);

    spi_fd = open(SPI_DEVICE, O_RDWR);
    if (spi_fd < 0) {
        perror("Failed to open SPI device");
        exit(EXIT_FAILURE);
    }

    uint8_t mode = SPI_MODE;
    uint32_t speed = SPI_SPEED;
    if (ioctl(spi_fd, SPI_IOC_WR_MODE, &mode) < 0) {
        perror("Failed to set SPI mode");
        exit(EXIT_FAILURE);
    }
    if (ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) < 0) {
        perror("Failed to set SPI speed");
        exit(EXIT_FAILURE);
    }

    usleep(350000); // Delay to power internal voltage reference
  
    ads1262_Reg_Write(POWER, 0x11);        // Default (Will need to change to external voltage for higher input analog voltage)
    usleep(10000);
    ads1262_Reg_Write(INTERFACE, 0x05);    // Default
    usleep(10000);
    ads1262_Reg_Write(MODE0, 0x00);        // Default
    usleep(10000);
    ads1262_Reg_Write(MODE1, 0x80);        // Default
    usleep(10000);
    ads1262_Reg_Write(MODE2, 0x04);        // 20 sps (max in fir mode)
    usleep(10000);
    ads1262_Reg_Write(INPMUX, 0x0F);       // Positive AIN0 and Negative Float (Connect to the universal ground)
    usleep(10000);  
    ads1262_Reg_Write(OFCAL0, 0x00);       // Default
    usleep(10000);  
    ads1262_Reg_Write(OFCAL1, 0x00);       // Default
    usleep(10000);  
    ads1262_Reg_Write(OFCAL2, 0x00);       // Default
    usleep(10000);  
    ads1262_Reg_Write(FSCAL0, 0x00);       // Default
    usleep(10000);  
    ads1262_Reg_Write(FSCAL1, 0x00);       // Default
    usleep(10000);  
    ads1262_Reg_Write(FSCAL2, 0x40);       // Default
    usleep(10000);  
    ads1262_Reg_Write(IDACMUX, 0xBB);      // Default
    usleep(10000);  
    ads1262_Reg_Write(IDACMAG, 0x00);      // Default
    usleep(10000);  
    ads1262_Reg_Write(REFMUX, 0x02);       // Negative side connected to AIN3 (Connect to ground)
    usleep(10000);    
    ads1262_Reg_Write(TDACP, 0x00);        // Default
    usleep(10000);    
    ads1262_Reg_Write(TDACN, 0x00);        // Default
    usleep(10000);    
    ads1262_Reg_Write(GPIOCON, 0x00);      // Default
    usleep(10000);    
    ads1262_Reg_Write(GPIODIR, 0x00);      // Default
    usleep(10000);    
    ads1262_Reg_Write(GPIODAT, 0x00);      // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2CFG, 0x00);      // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2MUX, 0x01);      // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2OFC0, 0x00);     // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2OFC1, 0x00);     // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2FSC0, 0x00);     // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2FSC1, 0x40);     // Default
    usleep(10000);
}

void ads1262_Reset(void)
{
    digitalWrite(GPIO_RESET, HIGH);
    usleep(100000);  // Wait 100 mSec
    digitalWrite(GPIO_RESET, LOW);
    usleep(100000);
    digitalWrite(GPIO_RESET, HIGH);
    usleep(100000);
}

void ads1262_Disable_Start(void)
{
    digitalWrite(GPIO_START, LOW);
    usleep(20000);
}

void ads1262_Enable_Start(void)
{
    digitalWrite(GPIO_START, HIGH);
    usleep(20000);
}

void ads1262_Hard_Stop(void)
{
    digitalWrite(GPIO_START, LOW);
    usleep(100000);
}

void ads1262_Start_Data_Conv_Command(void)
{
    ads1262_SPI_Command_Data(START);  // Send 0x08 to the ADS1262
}

void ads1262_Soft_Stop(void)
{
    ads1262_SPI_Command_Data(STOP);  // Send 0x0A to the ADS1262
}

void ads1262_Start_Read_Data_Continuous(void)
{
    ads1262_SPI_Command_Data(RDATAC);  // Send 0x10 to the ADS1262
}

void ads1262_Stop_Read_Data_Continuous(void)
{
    ads1262_SPI_Command_Data(SDATAC);  // Send 0x11 to the ADS1262
}

void ads1262_SPI_Command_Data(uint8_t data_in)
{
    digitalWrite(ADS1220_CS_PIN, LOW);
    usleep(2000);

    struct spi_ioc_transfer spi = {
        .tx_buf = (unsigned long)&data_in,
        .rx_buf = (unsigned long)&data_in,
        .len = 1,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8,
    };

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
        perror("Failed to send SPI command");
        digitalWrite(ADS1220_CS_PIN, HIGH);
        return;  // Indicate failure to send command
    }

    usleep(2000);
    digitalWrite(ADS1220_CS_PIN, HIGH);
}

void ads1262_Reg_Write(uint8_t READ_WRITE_ADDRESS, uint8_t DATA)
{
    uint8_t dataToSend[2];
    dataToSend[0] = READ_WRITE_ADDRESS | WREG;
    dataToSend[1] = DATA;

    digitalWrite(ADS1220_CS_PIN, LOW);
    usleep(2000);

    struct spi_ioc_transfer spi = {
        .tx_buf = (unsigned long)dataToSend,
        .rx_buf = 0,
        .len = 2,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8,
    };

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
        perror("Failed to write to SPI device");
        exit(EXIT_FAILURE);
    }

    usleep(2000);
    digitalWrite(ADS1220_CS_PIN, HIGH);
}


char* ads1262_Read_Data(void)
{
    static char SPI_Dummy_Buff[6];

    digitalWrite(ADS1220_CS_PIN, LOW);

    struct spi_ioc_transfer spi = {
        .tx_buf = (unsigned long)SPI_Dummy_Buff,
        .rx_buf = (unsigned long)SPI_Dummy_Buff,
        .len = 6,
        .speed_hz = SPI_SPEED,
        .bits_per_word = 8,
    };

    if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &spi) < 0) {
        perror("Failed to read from SPI device");
        digitalWrite(ADS1220_CS_PIN, HIGH);
        return NULL;  // Indicate failure to read data
    }

    digitalWrite(ADS1220_CS_PIN, HIGH);

    return SPI_Dummy_Buff;
}
