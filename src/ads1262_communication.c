#include <stdio.h>
#include "ads1262_library.h"
#include <wiringPi.h>
#include <unistd.h> // for usleep

int main(void)
{
    // Initialize the ADS1262
    ads1262_Init();
    ads1262_Reset();
    usleep(10000);

    // Start data conversion
    ads1262_Start_Data_Conv_Command();
    usleep(10000);

    // Start continuous data reading
    ads1262_Start_Read_Data_Continuous();
    usleep(10000);

    while (1)
    {
        // Check if data is ready
        if (digitalRead(GPIO_DOUT_DRDY) == LOW)
        {
            // Read the data
            char* data = ads1262_Read_Data();
            
            // Ensure data is not NULL before printing
            if (data != NULL)
            {
                printf("Data: %02x %02x %02x %02x %02x %02x\n", data[0], data[1], data[2], data[3], data[4], data[5]);
            }
            
            // Delay for computational efficiency
            usleep(1000);
        }
    }

    return 0;
}
