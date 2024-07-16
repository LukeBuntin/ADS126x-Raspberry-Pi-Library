#include "ads1262_library.h"
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h> // for usleep

void ads1262_Init(void)
{
    wiringPiSetup();
    pinMode(GPIO_DRDY, INPUT);
    pinMode(GPIO_RESET, OUTPUT);
    pinMode(GPIO_START, OUTPUT);
    pinMode(ADS1220_CS_PIN, OUTPUT);
    wiringPiSPISetup(0, 1000000); // SPI channel 0, 1MHz

    ads1262_Reset();
    usleep(100000);
  
    ads1262_Hard_Stop();
    usleep(350000);
  
    ads1262_Reg_Write(POWER, 0x11); 		// Default (Will need to change to external voltage for higher input analog voltage)
    usleep(10000);
    ads1262_Reg_Write(INTERFACE, 0x05);	// Default
    usleep(10000);
    ads1262_Reg_Write(MODE0, 0x00);		// Default
    usleep(10000);
    ads1262_Reg_Write(MODE1, 0x80);	    // Default
    usleep(10000);
    ads1262_Reg_Write(MODE2, 0x0F);	    // 38400 sps
    usleep(10000);
    ads1262_Reg_Write(INPMUX, 0x0F);	    // Positive AIN0 and Negative Float (Connect to the universal ground)
    usleep(10000);  
    ads1262_Reg_Write(OFCAL0, 0x00);	    // Default
    usleep(10000);  
    ads1262_Reg_Write(OFCAL1, 0x00);	    // Default
    usleep(10000);  
    ads1262_Reg_Write(OFCAL2, 0x00);	    // Default
    usleep(10000);  
    ads1262_Reg_Write(FSCAL0, 0x00);	    //  Default
    usleep(10000);  
    ads1262_Reg_Write(FSCAL1, 0x00);	    // Default
    usleep(10000);  
    ads1262_Reg_Write(FSCAL2, 0x40);	    // Default
    usleep(10000);  
    ads1262_Reg_Write(IDACMUX, 0xBB);	    // Default
    usleep(10000);  
    ads1262_Reg_Write(IDACMAG, 0x00);	    // Default
    usleep(10000);  
    ads1262_Reg_Write(REFMUX, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(TDACP, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(TDACN, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(GPIOCON, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(GPIODIR, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(GPIODAT, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2CFG, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2MUX, 0x01);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2OFC0, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2OFC1, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2FSC0, 0x00);	    // Default
    usleep(10000);    
    ads1262_Reg_Write(ADC2FSC1, 0x40);	    // Default
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
    digitalWrite(ADS1220_CS_PIN, HIGH);
    usleep(2000);
    digitalWrite(ADS1220_CS_PIN, LOW);
    usleep(2000);

    wiringPiSPIDataRW(0, &data_in, 1);

    usleep(2000);
    digitalWrite(ADS1220_CS_PIN, HIGH);
}

void ads1262_Reg_Write(uint8_t READ_WRITE_ADDRESS, uint8_t DATA)
{
    uint8_t dataToSend[3];
    dataToSend[0] = READ_WRITE_ADDRESS | WREG;
    dataToSend[1] = 0x00;  // number of registers to write - 1
    dataToSend[2] = DATA;

    digitalWrite(ADS1220_CS_PIN, LOW);
    usleep(2000);
    digitalWrite(ADS1220_CS_PIN, HIGH);
    usleep(2000);
    digitalWrite(ADS1220_CS_PIN, LOW);
    usleep(2000);

    wiringPiSPIDataRW(0, dataToSend, 3);

    usleep(2000);
    digitalWrite(ADS1220_CS_PIN, HIGH);
}

char* ads1262_Read_Data(void)
{
    static char SPI_Dummy_Buff[6];

    digitalWrite(ADS1220_CS_PIN, LOW);

    for (int i = 0; i < 6; ++i)
    {
        uint8_t data = CONFIG_SPI_MASTER_DUMMY;
        wiringPiSPIDataRW(0, &data, 1);
        SPI_Dummy_Buff[i] = data;
    }

    digitalWrite(ADS1220_CS_PIN, HIGH);

    return SPI_Dummy_Buff;
}
