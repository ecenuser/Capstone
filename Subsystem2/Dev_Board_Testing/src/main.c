#include <stddef.h>                     // Defines NULL
#include <stdbool.h>                    // Defines true
#include <stdlib.h>                     // Defines EXIT_FAILURE
#include "definitions.h"                // SYS function prototypes
#include <string.h>

#define LED_On()                        LED1_Clear()    //shorts turn on these leds
#define LED_Off()                       LED1_Set()

/* Global variables */
#define BUFFER_SIZE 1
static uint8_t txData[]  = "SELF LOOPBACK DEMO FOR SPI!\r\n";
static uint16_t rxData[BUFFER_SIZE];
static uint16_t Received[]
uint16_t Data;
volatile static bool transferStatus=false;
volatile static bool delay_complete=false;
//static char errorMessage[] = "\r\n**** UART error has occurred ****\r\n";
//static char local[sizeof()];

/* This function will be called by SPI PLIB when transfer is completed */
void SPI0_Callback(uintptr_t context )
{
    transferStatus = true;
}

/* This function will be called by TMR when period matches*/
void TIMER3_InterruptSvcRoutine(uint32_t status, uintptr_t context)
{
    delay_complete = true;
    TMR3_Stop();
}


// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main ( void )
{
    /* Initialize all modules */
    SYS_Initialize ( NULL );

    /* Register callback function   */
    SPI2_CallbackRegister(SPI0_Callback, 0);
    TMR3_CallbackRegister(TIMER3_InterruptSvcRoutine, (uintptr_t) NULL);
    /* SPI Write Read */
    
    LED_Off();
    
    //dispaly smthn for test
    //receiver pre operation
    /*
    UART6_Write("\r\n", 2);
        for (size_t i = 0; i < sizeof(txData); i++)
    {
        char temp[10];
        sprintf(temp, "0x%02X ", txData[i]);
        UART6_Write(temp, strlen(temp));
    }
    UART6_Write("\r\n", 2);
    */
    UART6_Write("Begin Test\r\n", sizeof("Begin Test\r\n"));
    //Clock in SPI data from ADC
    SPI2_Read(&rxData, sizeof(rxData));
    while (SPI2_IsBusy()) {
    // Wait for SPI to finish
    }
    //Process data
    //Currently 16-bits with 2 leading 0 caps on either end
    for (size_t i = 0; i < BUFFER_SIZE; i++) {
        uint16_t rawData = rxData[i];
        uint16_t actualData = (rawData >> 2) & 0x0FFF; //masks 10 middle bits
        Data = actualData;
    }
    
    while(1)
    {
        SYS_Tasks();
        if(transferStatus == true)
        {
            transferStatus = false;

            /* Compare received data with the transmitted data */
            //if(memcmp(txData, rxData, sizeof(txData)) == 0)
            if(transferStatus)
            {
                /* Pass: Received data is same as transmitted data */
                LED_On();
                UART6_Write("Pass!\r\n", sizeof("Pass!\r\n"));  
                UART6_Write("rxData\r\n", sizeof("rxData\r\n")); 
                for (size_t i = 0; i < sizeof(txData); i++)
                {
                    char temp[10];
                    sprintf(temp, "0x%02X ", txData[i]);
                    UART6_Write(temp, strlen(temp));
                }
                UART6_Write("\r\n", 2);
                UART6_Write("txData\r\n", sizeof("txData\r\n"));
                    for (size_t i = 0; i < sizeof(rxData); i++)
                {
                    char temp[10];
                    sprintf(temp, "0x%02X ", rxData[i]);
                    UART6_Write(temp, strlen(temp));
                }
                UART6_Write("\r\n", 2);                
            }
            else
            {
				/* Fail: Received data is not same as transmitted data */
                LED_Off();
                UART6_Write("\r\n", 2);
                UART6_Write("rxData is: \r\n", sizeof("rxData is: \r\n"));
                printf("The Data is: %d\r\n", Data); 
                printf("The BuffData is: %d\r\n", rxData[0]);
                /*
                for (size_t i = 0; i < sizeof(rxData); i++)
                {
                    char temp[10];
                    sprintf(temp, "0x%02X ", rxData[i]);
                    UART6_Write(temp, strlen(temp));
                }
                */
                UART6_Write("\r\n", 2); 
                
                //looping code
                
                //delays
                /*            
                while (SPI2_IsBusy()) {
                    // Wait for SPI to finish
                }
                TMR3_Start(); 
                
                //Resample
                SPI2_Read(&rxData, sizeof(rxData));
                for (size_t i = 0; i < BUFFER_SIZE; i++) {
                    uint16_t rawData = rxData[i];
                    uint16_t actualData = (rawData >> 2) & 0x0FFF; //masks 10 middle bits
                    Data = actualData;
                }
                */
            }
        }
        
    }
}

/*******************************************************************************
 End of File
*/


