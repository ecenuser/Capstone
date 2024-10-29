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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <stdio.h>

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

#define ADC_VREF                (3.3f)
#define ADC_MAX_COUNT           (4095)
#define TX_BUFFER_SIZE  2  // Size adjusted to 2 bytes
#define RX_BUFFER_SIZE  2  // Size adjusted to 2 bytes

uint8_t adcValue[2];  
float analogVoltage;
uint16_t adcResult;
uint8_t txBuffer[TX_BUFFER_SIZE] = {0x08, 0xFF};  // Data to transmit (2 bytes)
uint8_t rxBuffer[RX_BUFFER_SIZE];  // Buffer to receive data (2 bytes)


//#define CNV_CS_PIN LATDbits.LATD4   // Shared CNV/CS pin
#define SDO2_PIN PORTFbits.RF5       // SDO pin (Busy indicator for first ADC) the oen closest/connected to the MCU 
// IN this case the closest is SPI 2 
#define NUM_ADCS 2                  // Number of ADCs in chain

uint16_t adcData[NUM_ADCS] = {0};  // ADC data buffers
#define SPI2_DUMMY_BYTE 0x00        // Dummy byte for SPI clocking

//uint16_t adcData[NUM_ADCS] = {0};  // Buffer to store ADC data



//uint16_t adcData[NUM_ADCS] = {0};  // Buffer to store ADC data

void DelayMicroseconds(uint32_t microseconds) {
    volatile uint32_t count = microseconds * 10;  // Adjust multiplier based on system clock speed
    while (count--) {
        // Busy wait loop for delay
    }
}
// Function to start ADC conversion by toggling CNV/CS pin
void StartConversion(void) {
    LATDbits.LATD4 = 1;    // Rising edge to start conversion
    //__delay_us(1);      // Ensure minimum high pulse width (datasheet requirement)
    DelayMicroseconds(2000);
    LATDbits.LATD4 = 0;    // Keep low to enable SPI communication
}

// Function to wait for the low-to-high transition on the SDO2 pin
void WaitForDataReady(void) {
    // Wait until SDO2 goes high (indicating data is ready)
    while (SDO2_PIN == 0) {
        // Keep polling until the busy indicator (SDO2) goes high
    }
}

// Function to read ADC data from the chain using SPI2
void ReadADCChain(void) {
    uint8_t spiTxBuffer[2 * NUM_ADCS] = {SPI2_DUMMY_BYTE};  // Dummy bytes to clock out data
    uint8_t spiRxBuffer[2 * NUM_ADCS] = {0};  // Buffer to receive data

    // Perform SPI2 write-read operation for all ADCs
     DelayMicroseconds(10000);
    bool transferSuccess = SPI1_WriteRead(spiTxBuffer, sizeof(spiTxBuffer), spiRxBuffer, sizeof(spiRxBuffer));

    if (transferSuccess) {
        // Combine the received bytes into 16-bit ADC values
        for (int i = 0; i < NUM_ADCS; i++) {
            adcData[i] = (spiRxBuffer[i * 2] << 8) | spiRxBuffer[i * 2 + 1];
        }
    } else {
        // Handle SPI transfer failure (optional)
        printf("SPI transfer failed!\n");
    }
}

void StartConversionAndRead(void) {
    uint8_t dummyByte = 0x00;  // Dummy byte to generate SPI clock pulses
    uint8_t spiRxBuffer[2 * NUM_ADCS] = {0};  // Buffer to store received data

    // Ensure CNV is low initially
    LATCbits.LATC2 = 0;

    // Start SPI transfer to align SCKx high with CNV rising edge
    SPI2_WriteRead(&dummyByte, 1, NULL, 0);  // Send dummy byte to generate clock

    // Set CNV high while SCKx is high to start conversion
    LATCbits.LATC2 = 1;  // CNV goes high, starting conversion

    // Poll the busy indicator (SDO2) to ensure data is ready
//    while (SDO2_PIN == 0) {
//        // Wait until SDO2 goes high (data ready)
//    }

    // Now read the ADC data from the chain using SPI
    bool transferSuccess = SPI2_WriteRead(&dummyByte, 2 * NUM_ADCS, spiRxBuffer, sizeof(spiRxBuffer));

    if (transferSuccess) {
        // Combine the bytes into 16-bit ADC values
        for (int i = 0; i < NUM_ADCS; i++) {
            adcData[i] = (spiRxBuffer[i * 2] << 8) | spiRxBuffer[i * 2 + 1];
        }

        // Print the results
        printf("\rADC1: %u, ADC2: %u\n", adcData[0], adcData[1]);

        // Convert to analog voltage and print
        float analogVoltage = ((float)adcData[0] / 4096.0) * 3.3;
        printf("\n\rAnalog Voltage: %.3f V\n", analogVoltage);
    } else {
        printf("SPI transfer failed!\n");
    }

    // Set CNV low after the read is complete to reset for the next cycle
    LATCbits.LATC2 = 0;
}


int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    printf("\rTesting1\n");
    fflush(stdout);
    while ( true )
    {
       
        /* Maintain state machines of all polled MPLAB Harmony modules. */
       //SYS_Tasks ( );
        SYS_Tasks ( );
        printf("\n\rINside\n");
        fflush(stdout);
       // uint16_t txData =0x08; //selecting in2
        //DelayMicroseconds(10000);
        //SPI1_WriteRead(&txData, 1, adcValue, 2);
        //SPI1_WriteRead(txBuffer, 2, rxBuffer, 2);
      //  printf("Received: 0x%02X 0x%02X\n", rxBuffer[0], rxBuffer[1]);
    
        //ReadADCChain();
        //DelayMicroseconds(1000000);
        //adcResult = (adcValue >> 4);
        //printf("\rReceived: 0x%02X 0x%02X\n", rxBuffer[0], rxBuffer[1]);
       // printf("\rRaw ADC Bytes: 0x%02X 0x%02X\n", adcValue[0], adcValue[1]);
       // adcResult = ((adcValue[0] << 8) | adcValue[1]) >> 4;  // Shift to remove unused bits
        //adcResult = (adcValue[0] << 8) | adcValue[1];  // No shift needed here
        //adcResult = (rxBuffer[0] << 8) | rxBuffer[1];  // No shift needed here
        
//        StartConversion();
//
//        // 2. Wait for the data to be ready (SDO2 goes high)
//        WaitForDataReady();
//        //printf("\r Value of ")
//        // 3. Read the ADC data using SPI2
//        ReadADCChain();
//
//        // 4. Release the CNV/CS pin after reading the data
//        LATDbits.LATD4 = 1;  // Set high to complete the SPI session
//
//        // 5. Display or process the ADC data
//        printf("\rADC1: %u, ADC2: %u\n", adcData[0], adcData[1]);
//          //adcResult = (adcValue[0] << 8) | adcValue[1];  // No shift needed here
//         adcResult = (adcData[0] << 8) | adcData[1];  // No shift needed here
//
//        // Print the ADC result in both hex and decimal for clarity
//      //  printf("\rADC Result: 0x%03X (Decimal: %d)\n", adcResult, adcResult);
//
//   
//         analogVoltage = ((float)adcResult / 4096.0) * 5;
//         printf("\n\rAnalog Voltage: %.3f V\n", analogVoltage);
         StartConversionAndRead();
//          uint32_t dacValue = 0x300456;  // Example 24-bit value

    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

