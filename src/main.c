///*******************************************************************************
//  Main Source File
//
//  Company:
//    Microchip Technology Inc.
//
//  File Name:
//    main.c
//
//  Summary:
//    This file contains the "main" function for a project.
//
//  Description:
//    This file contains the "main" function for a project.  The
//    "main" function calls the "SYS_Initialize" function to initialize the state
//    machines of all modules in the system
// *******************************************************************************/
//
//// DOM-IGNORE-BEGIN
///*******************************************************************************
//* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
//*
//* Subject to your compliance with these terms, you may use Microchip software
//* and any derivatives exclusively with Microchip products. It is your
//* responsibility to comply with third party license terms applicable to your
//* use of third party software (including open source software) that may
//* accompany Microchip software.
//*
//* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
//* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
//* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
//* PARTICULAR PURPOSE.
//*
//* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
//* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
//* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
//* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
//* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
//* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
//* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
//*******************************************************************************/
//// DOM-IGNORE-END
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Included Files
//// *****************************************************************************
//// *****************************************************************************
//
//#include <stddef.h>                     // Defines NULL
//#include <stdbool.h>                    // Defines true
//#include <stdlib.h>                     // Defines EXIT_FAILURE
//#include "definitions.h"                // SYS function prototypes
//
//#define ADC_VREF                (3.3f)
//#define ADC_MAX_COUNT           (4095)
//#define TX_BUFFER_SIZE  2  // Size adjusted to 2 bytes
//#define RX_BUFFER_SIZE  2  // Size adjusted to 2 bytes
//
//uint8_t txBuffer[TX_BUFFER_SIZE] = {0x08, 0xFF};  // Data to transmit (2 bytes)
//uint8_t rxBuffer[RX_BUFFER_SIZE];  // Buffer to receive data (2 bytes)
//uint16_t adc_count;
//float input_voltage;
//uint8_t adcValue[2];  
//float analogVoltage;
//uint16_t adcResult;
//
//// *****************************************************************************
//// *****************************************************************************
//// Section: Main Entry Point
//// *****************************************************************************
//// *****************************************************************************
//
//
//#define DAC_CS_PIN LATCbits.LATC2  // Define the pin used for Chip Select (CS) for the DAC
//
//void DelayMicroseconds(uint32_t microseconds) {
//    volatile uint32_t count = microseconds * 10;  // Adjust multiplier based on system clock speed
//    while (count--) {
//        // Busy wait loop for delay
//    }
//}
//
//
//uint16_t Calculate_DAC_Value(float voltage) {
//    // Ensure the voltage is within the DAC range
//    if (voltage > 2.5) voltage = 2.5;
//    if (voltage < 0) voltage = 0;
//    // Calculate the 12-bit DAC value corresponding to the desired voltage
//    return (uint16_t)((voltage / 2.5) * (4096 - 1));
//}
//
//void Update_DAC(uint32_t data)
//{
//    // Bring CS low to start the SPI communication
//   // DelayMicroseconds(5);
//   // DAC_CS_PIN = 0;
//    
//    // SPI2_Write is used for transmitting the 24-bit data to the DAC
//    // The DAC expects a 24-bit sequence, so break the data into 3 bytes
//    uint8_t transmitData[3];
//    transmitData[0] = (data >> 16) & 0xFF;  // Upper 8 bits
//    transmitData[1] = (data >> 8) & 0xFF;   // Middle 8 bits
//    transmitData[2] = data & 0xFF;          // Lower 8 bits
//    
//    // Transmit the 24-bit command using SPI2
//    SPI2_Write(transmitData, 3);
//   
//    
//    // Bring CS high to end the SPI communication
//    DelayMicroseconds(500);
//    //DAC_CS_PIN = 1;
//}
//
//bool dmaTXComplete = false;
//bool dmaRXComplete = false;
//
//void DMA_CallbackHandler_TX(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
//{
//    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
//    {
//        dmaTXComplete = true;
//        printf("\rTX DMA transfer complete\n");
//    }
//}
//
//void DMA_CallbackHandler_RX(DMAC_TRANSFER_EVENT event, uintptr_t contextHandle)
//{
//    if (event == DMAC_TRANSFER_EVENT_COMPLETE)
//    {
//        dmaRXComplete = true;
//        printf("\rRX DMA transfer complete\n");
//    }
//}
//
//void SPI1_DMA_ChainedSetup()
//{
//    // Register the callback functions for both TX and RX DMA channels
//    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, DMA_CallbackHandler_TX, 0); // TX Channel
//    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_1, DMA_CallbackHandler_RX, 0); // RX Channel
//
//    // Configure the DMA channels in chained mode.
//    // When TX DMA is done, it should automatically trigger the RX DMA.
//
//    // Clear the DMA complete flags before starting transfer
//    dmaTXComplete = false;
//    dmaRXComplete = false;
//    printf("\rtxBuffer[0] = 0x%02X\n", txBuffer[0]);
//    printf("\rtxBuffer[1] = 0x%02X\n", txBuffer[1]);
//    
//    
//    DMAC_ChannelTransfer(DMAC_CHANNEL_0, txBuffer, 2, (const void *)&SPI1BUF, 1, 2);  // TX
//    DMAC_ChannelTransfer(DMAC_CHANNEL_1, (const void *)&SPI1BUF, 1, rxBuffer, 2, 2);  // RX
//    printf("\rDMA transfer complete\n");
//    // Wait for both TX and RX to complete using chaining
//    while (!dmaTXComplete || !dmaRXComplete);
//    printf("\rDMA transfer complete\n");
//    printf("\rSPI1BUF (before TX): 0x%02X\n", SPI1BUF);
//    // Check received data
//    printf("\rrxBuffer[0] = 0x%02X\n", rxBuffer[0]);
//}
//
//SPI1_DMA_Test()
//{
//    // Register the callback functions for both TX and RX DMA channels
//    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_0, DMA_CallbackHandler_TX, 0); // TX Channel
//    DMAC_ChannelCallbackRegister(DMAC_CHANNEL_1, DMA_CallbackHandler_RX, 0); // RX Channel
//
//    // Clear DMA complete flags
//    dmaTXComplete = false;
//    dmaRXComplete = false;
//
//    
//    printf("\rSPI1BUF (before TX): 0x%02X\n", SPI1BUF);
//
//    // Start DMA transfer for TX and RX
//    printf("\rtxBuffer[0] = 0x%02X\n", txBuffer[0]);
//    printf("\rtxBuffer[1] = 0x%02X\n", txBuffer[1]);
//   // DMAC_ChainTransferSetup(DMAC_CHANNEL_0, txBuffer, TX_BUFFER_SIZE, (const void *)&SPI1BUF, 1, TX_BUFFER_SIZE);
//   // DMAC_ChainTransferSetup(DMAC_CHANNEL_1, (const void *)&SPI1BUF, 1, rxBuffer, RX_BUFFER_SIZE, RX_BUFFER_SIZE); 
//                
//    printf("\rStarting DMA transfer\n");
//    DMAC_ChannelTransfer(DMAC_CHANNEL_0, txBuffer[0], 1, (const void *)&SPI1BUF, 1, TX_BUFFER_SIZE);  // TX
//    // DelayMicroseconds(100000);
//    //printf("\rSPI1BUF (after TX): 0x%02X\n", SPI1BUF);
//
//    DMAC_ChannelTransfer(DMAC_CHANNEL_1, (const void *)&SPI1BUF, 1, rxBuffer[0], 1, 1);  // RX
//    printf("\rDMA transfer started\n");
//    // Wait for the DMA transfer to complete
//    while (!dmaTXComplete || !dmaRXComplete);
//    printf("\rDMA transfer complete\n");
//
//
//    // Check received data
//    //DelayMicroseconds(100000);
//    printf("\rrxBuffer[0] = 0x%02X, rxBuffer[1] = 0x%02X\n", rxBuffer[0], rxBuffer[1]);
//}
//
//bool SPI1_WriteReadBlockingWithTimeout(uint8_t *txData, uint8_t *rxData, size_t length)
//{
//    LATDbits.LATD9 = 0;  // Pull SS low before SPI transaction
//    printf("\rSS pulled low\n");
//    for (size_t i = 0; i < length; i++) {
//        printf("\rWriting byte %d: 0x%02X\n", i, txData[i]);
//        SPI1BUF = txData[i];  // Write data to SPI buffer
//        
//        // Add timeout to prevent hanging
//        
//        uint32_t timeout = 1000000;  // Adjust the value as needed
//        while (!SPI1STATbits.SPIRBF && timeout--) {
//            if (timeout == 0) {
//                printf("\rSPI Timeout on byte %d\n", i);
//                LATDbits.LATD9 = 1;  // Pull SS high in case of error
//                return false;  // Timeout error
//            }
//        }
//        
//        rxData[i] = SPI1BUF;  // Read received data
//    }
//
//    LATDbits.LATD9 = 1;  // Pull SS high after SPI transaction
//    printf("\rSS pulled high\n");
//    return true;  // Success
//}
//int main ( void )
//{
//   
//    /* Initialize all modules */
//    SYS_Initialize ( NULL );
//
//    printf("\n\r---------------------------------------------------------");
//    printf("\n\r                    ADC Polled Mode Demo                 ");
//    printf("\n\r---------------------------------------------------------\n\r"); 
//    
//    TMR3_Start();
//     LATJbits.LATJ7 = 1;
//    
//    TRISJbits.TRISJ7 = 0;  // Set RJ7 as output (clear bit 7)
//    
//   // uint16_t adcValue;
//    //    uint16_t txData =0x0800; //selecting in2
//     //   LATCbits.LATC2 = 1;  // Bring CS low before SPI transfer
//      //  SPI3_WriteRead(&txData, 2, &adcValue, 2);
//       // LATBbits.LATB15 = 0;  // Bring CS high after SPI transfer
//     //   adcValue = adcValue >> 4;
//        // DelayMicroseconds(10000);
//        uint32_t command = 0x200001;  // Power up DAC A (command to set DAC A in normal operation)
//        SPI2_Write((uint8_t *)&command, 3);  // Send the 24-bit command over SPI
//        Update_DAC(command);
//        DelayMicroseconds(10000);
//        uint32_t command2 = 0x380001;// Turn on internal reference for DAC 
//        Update_DAC(command2);
// 
//    while ( true )
//    {
//        /* Maintain state machines of all polled MPLAB Harmony modules. */
//        SYS_Tasks ( );
//            DelayMicroseconds(1000000);
//        //SPI1_DMA_Test();  // Call the test function for DMA-based SPISPI1_DMA_ChainedSetup
//        //SPI1_DMA_ChainedSetup();
//        //SPI1_Blocking_Test();
//        uint16_t txData =0x08; //selecting in2
//        //Cbits.LATC2 = 1;  // Bring CS low before SPI transfer
////        uint8_t txBuffer[1] = {0x08};  // Send 1 byte
////        uint8_t rxBuffer[1];  // Receive 1 byte
////       // {0x08, 0xFF}
//        //LATDbits.LATD9=0;
////         printf("\rtxBuffer = 0x%02X\n", txBuffer[0]);
////        DMAC_ChannelTransfer(DMAC_CHANNEL_0, txBuffer, 1, (const void *)&SPI1BUF, 1, 1);  // TX 1 byte
//        //DMAC_ChannelTransfer(DMAC_CHANNEL_1, (const void *)&SPI1BUF, 1, rxBuffer, 1, 1);  // RX 1 byte
////         printf("\rrxBuffer =0x%02X\n", rxBuffer[0]);
//        //SPI1_Blocking_Test();
//         //LATDbits.LATD9=1;
//        //DelayMicroseconds(1000000);
//          
//      // __builtin_disable_interrupts();  // Disable global interrupts
//
//      // LATDbits.LATD9=1;
//        
//        SPI1_WriteRead(&txData, 1, adcValue, 2);
//       // while(SPI1_IsBusy()){}
//        
//      // LATDbits.LATD9 = 0;
//      
//       // SPI1_WriteRead(txBuffer, 2, rxBuffer, 2);
////       // SPI1_Write(txBuffer, 1);
////        //DelayMicroseconds(3);
////        //SPI1_Read(rxBuffer, 2);
////
////       // Wait for the SPI transfer to complete
////       while (SPI1_IsBusy()) {
////           // Do nothing, just wait
////       }
////
////       // Pull CS high to end SPI transaction
//       // LATDbits.LATD9 = 0;
//        // SPI transmission code
//     //    __builtin_enable_interrupts();   // Enable global interrupts
//        // Blocking call for sending 2 bytes
//        // Process the received data
//        printf("Received: 0x%02X 0x%02X\n", rxBuffer[0], rxBuffer[1]);
//    
//       
//        DelayMicroseconds(1000000);
//        //adcResult = (adcValue >> 4);
//        printf("\rRaw ADC Bytes: 0x%02X 0x%02X\n", adcValue[0], adcValue[1]);
//       // adcResult = ((adcValue[0] << 8) | adcValue[1]) >> 4;  // Shift to remove unused bits
//        //adcResult = (adcValue[0] << 8) | adcValue[1];  // No shift needed here
//         adcResult = (rxBuffer[0] << 8) | rxBuffer[1];  // No shift needed here
//
//        // Print the ADC result in both hex and decimal for clarity
//      //  printf("\rADC Result: 0x%03X (Decimal: %d)\n", adcResult, adcResult);
//
//        // Assuming VREF is 5.0V
//        //float ADC_VREF = 5.0;  // Make sure this matches your actual reference voltage
//
//        // Calculate the analog voltage based on the ADC result
//        analogVoltage = ((float)adcResult / 4096.0) * 5;
//        printf("\n\rAnalog Voltage: %.3f V\n", analogVoltage);
////          uint32_t dacValue = 0x300456;  // Example 24-bit value
////           Update_DAC(dacValue);
////         DelayMicroseconds(10);
//
//        DelayMicroseconds(100000);
//    
//       // uint32_t dacValue = 0x183330;  // Example 24-bit value for 0.5V 
//
//        // Print the DAC value for debugging
//        uint32_t dacValue;    // Final 24-bit DAC command
//        uint8_t commandBits;  // Command bits (3 bits)
//        uint8_t addressBits;  // Address bits (4 bits)
//        uint16_t dataBits;    // 12-bit DAC data (for the desired voltage)
//        float desired_voltage = 0.75;
//        //
//        dataBits = Calculate_DAC_Value(desired_voltage);
//
//    // Set the command and address bits (assuming channel 0 and "Write and Update")
//        commandBits = 0x3;  // '011' in binary, meaning Write and Update
//        addressBits = 0x0;  // '0000' in binary for DAC channel 0
//
//        // Combine the command, address, and data bits into a 24-bit value
//        dacValue = (commandBits << 19) | (addressBits << 16) | (dataBits << 4);  // Aligning everything correctly
//        Update_DAC(dacValue);
//        printf("DAC Value: 0x%06X\n", dacValue);
//        if (PORTJbits.RJ4 == 0) {  // Check if SW1 is pressed (active low)
//            printf("\n\rSW1 is pressed\n");
//            // LATJbits.LATJ7 = 0;  // Set RJ7 high (turn on LED1)
//            
//        } else {
//            printf("\n\rSW1 is not pressed\n");
//             //LATJbits.LATJ7 = 1;  // Set RJ7 high (turn on LED1)
//        }
//        
//        
//        /* Wait till ADC conversion result is available */
////        while(!ADCHS_ChannelResultIsReady(ADCHS_CH2))
////        {
////
////        };
//       // while (ADCCON2bits.REFFLT != 0);  // Ensure no reference voltage fault
//
//        /* Read the ADC result */
//       // int16_t adc_count = (int16_t)ADCHS_ChannelResultGet(ADCHS_CH2);
//
//        //adc_count = ADCHS_ChannelResultGet(ADCHS_CH2);
//        //input_voltage = (float)adc_count * ADC_VREF / ADC_MAX_COUNT; //for single ended
//        // input_voltage = (float)adc_count * ADC_VREF / ADC_MAX_COUNT * 2;
//
//
//        //printf("ADC Count = 0x%03x, ADC Input Voltage = %d.%02d V \r", adc_count, (int)input_voltage, (int)((input_voltage - (int)input_voltage)*100.0));        
//    }
//
//    /* Execution should not come here during normal operation */
//
//    return ( EXIT_FAILURE );
//}
//
//
///*******************************************************************************
// End of File
//*/
//
