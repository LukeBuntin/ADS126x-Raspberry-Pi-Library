#include <stdio.h>
#include "ads1262_library.h"
#include <wiringPi.h>
#include <unistd.h> // for usleep

int main(void)
{
    ads1262_Init();
    ads1262_Start_Data_Conv_Command();
    usleep(10000);
    ads1262_Start_Read_Data_Continuous();
    usleep(10000);

    while (1)
    {
        if (digitalRead(GPIO_DOUT_DRDY) == LOW)
        {
            char* data = ads1262_Read_Data();
            printf("Data: %02x %02x %02x %02x %02x %02x\n", data[0], data[1], data[2], data[3], data[4], data[5]);
            usleep(10000); // delay for computational efficiency
        }
    }

    return 0;
}