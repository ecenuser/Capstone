/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
*******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes

#define ADC_VREF                (3.3f)
#define ADC_MAX_COUNT           (4095)

uint16_t adc_count;
float input_voltage;
uint8_t adcValue[2];  
float analogVoltage;
uint16_t adcResult;

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************


#define DAC_CS_PIN LATCbits.LATC2  // Define the pin used for Chip Select (CS) for the DAC

void DelayMicroseconds(uint32_t microseconds) {
    volatile uint32_t count = microseconds * 10;  // Adjust multiplier based on system clock speed
    while (count--) {
        // Busy wait loop for delay
    }
}


uint16_t Calculate_DAC_Value(float voltage) {
    // Ensure the voltage is within the DAC range
    if (voltage > 2.5) voltage = 2.5;
    if (voltage < 0) voltage = 0;
    // Calculate the 12-bit DAC value corresponding to the desired voltage
    return (uint16_t)((voltage / 2.5) * (4096 - 1));
}

void Update_DAC(uint32_t data)
{
    // Bring CS low to start the SPI communication
   // DelayMicroseconds(5);
   // DAC_CS_PIN = 0;
    
    // SPI2_Write is used for transmitting the 24-bit data to the DAC
    // The DAC expects a 24-bit sequence, so break the data into 3 bytes
    uint8_t transmitData[3];
    transmitData[0] = (data >> 16) & 0xFF;  // Upper 8 bits
    transmitData[1] = (data >> 8) & 0xFF;   // Middle 8 bits
    transmitData[2] = data & 0xFF;          // Lower 8 bits
    
    // Transmit the 24-bit command using SPI2
    SPI2_Write(transmitData, 3);
   
    
    // Bring CS high to end the SPI communication
    DelayMicroseconds(500);
    //DAC_CS_PIN = 1;
}

int main ( void )
{
   
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    printf("\n\r---------------------------------------------------------");
    printf("\n\r                    ADC Polled Mode Demo                 ");
    printf("\n\r---------------------------------------------------------\n\r"); 
    
    TMR3_Start();
     LATJbits.LATJ7 = 1;
    
    TRISJbits.TRISJ7 = 0;  // Set RJ7 as output (clear bit 7)
    
   // uint16_t adcValue;
    //    uint16_t txData =0x0800; //selecting in2
     //   LATCbits.LATC2 = 1;  // Bring CS low before SPI transfer
      //  SPI3_WriteRead(&txData, 2, &adcValue, 2);
       // LATBbits.LATB15 = 0;  // Bring CS high after SPI transfer
     //   adcValue = adcValue >> 4;
        // DelayMicroseconds(10000);
        uint32_t command = 0x200001;  // Power up DAC A (command to set DAC A in normal operation)
        SPI2_Write((uint8_t *)&command, 3);  // Send the 24-bit command over SPI
        Update_DAC(command);
        DelayMicroseconds(10000);
        uint32_t command2 = 0x380001;// Turn on internal reference 
        Update_DAC(command2);

    while ( true )
    {
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );
        
        uint16_t txData =0x08; //selecting in2
        //Cbits.LATC2 = 1;  // Bring CS low before SPI transfer
        DelayMicroseconds(1000000);
        SPI1_WriteRead(&txData, 1, adcValue, 2);
        DelayMicroseconds(500);
        //adcResult = (adcValue >> 4);
        printf("\rRaw ADC Bytes: 0x%02X 0x%02X\n", adcValue[0], adcValue[1]);
       // adcResult = ((adcValue[0] << 8) | adcValue[1]) >> 4;  // Shift to remove unused bits
        adcResult = (adcValue[0] << 8) | adcValue[1];  // No shift needed here
 

        // Print the ADC result in both hex and decimal for clarity
      //  printf("\rADC Result: 0x%03X (Decimal: %d)\n", adcResult, adcResult);

        // Assuming VREF is 5.0V
        //float ADC_VREF = 5.0;  // Make sure this matches your actual reference voltage

        // Calculate the analog voltage based on the ADC result
        analogVoltage = ((float)adcResult / 4096.0) * 5;
        printf("\n\rAnalog Voltage: %.3f V\n", analogVoltage);
//          uint32_t dacValue = 0x300456;  // Example 24-bit value
//           Update_DAC(dacValue);
//         DelayMicroseconds(10);

        DelayMicroseconds(10000);
    
       // uint32_t dacValue = 0x183330;  // Example 24-bit value for 0.5V 
//     
//        Update_DAC(dacValue);
        
        //uint32_t commandBits = 0x3 << 20;  // Write and Update command (you can adjust this based on your requirements)
        //uint32_t addressBits = 0x0 << 16;  // Assuming channel 0 (adjust if necessary)

        // Combine command, address, and data bits
        //uint32_t dacValue = commandBits | addressBits | (adcResult << 4);  // Shift the 12-bit ADC result to align with D11-D0 of the DAC data
        //uint32_t dacValue = (0x3 << 20) | (0x0 << 16) | (0x99A << 4);

        // Send the 24-bit value to the DAC
        //uint32_t dacValue= 0x1899A0;
       // uint32_t dacValue = (0x3 << 20) | (0x0 << 16) | (0x4CD << 4);
       // Update_DAC(dacValue);
       // DelayMicroseconds(100000);

        // Print the DAC value for debugging
        uint32_t dacValue;    // Final 24-bit DAC command
        uint8_t commandBits;  // Command bits (3 bits)
        uint8_t addressBits;  // Address bits (4 bits)
        uint16_t dataBits;    // 12-bit DAC data (for the desired voltage)
            float desired_voltage = 0.75;
        //
        dataBits = Calculate_DAC_Value(analogVoltage);

    // Set the command and address bits (assuming channel 0 and "Write and Update")
        commandBits = 0x3;  // '011' in binary, meaning Write and Update
        addressBits = 0x0;  // '0000' in binary for DAC channel 0

        // Combine the command, address, and data bits into a 24-bit value
        dacValue = (commandBits << 19) | (addressBits << 16) | (dataBits << 4);  // Aligning everything correctly
        Update_DAC(dacValue);
        printf("DAC Value: 0x%06X\n", dacValue);
        if (PORTJbits.RJ4 == 0) {  // Check if SW1 is pressed (active low)
            printf("\n\rSW1 is pressed\n");
             LATJbits.LATJ7 = 0;  // Set RJ7 high (turn on LED1)
            
        } else {
            printf("\n\rSW1 is not pressed\n");
             LATJbits.LATJ7 = 1;  // Set RJ7 high (turn on LED1)
        }
        
        
        /* Wait till ADC conversion result is available */
//        while(!ADCHS_ChannelResultIsReady(ADCHS_CH2))
//        {
//
//        };
       // while (ADCCON2bits.REFFLT != 0);  // Ensure no reference voltage fault

        /* Read the ADC result */
       // int16_t adc_count = (int16_t)ADCHS_ChannelResultGet(ADCHS_CH2);

        //adc_count = ADCHS_ChannelResultGet(ADCHS_CH2);
        //input_voltage = (float)adc_count * ADC_VREF / ADC_MAX_COUNT; //for single ended
        // input_voltage = (float)adc_count * ADC_VREF / ADC_MAX_COUNT * 2;


        //printf("ADC Count = 0x%03x, ADC Input Voltage = %d.%02d V \r", adc_count, (int)input_voltage, (int)((input_voltage - (int)input_voltage)*100.0));        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

