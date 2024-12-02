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


#define SYS_FREQ 200000000  //200 MHz

#define rxBUFFER_SIZE           (1)
#define ADC_VREF                (3.3f)  //VDD
#define ADC_MAX_COUNT           (4095)
#define EXT_MAX_COUNT           (65535)
#define EXT_VREF                (2.5f)
#define SENSOR_INPUTV           (5)
#define SHUNT_RESISTANCE        (200)

//Global EQN constants and Table Macros
#define CONST_A                 (8269.83983488298f)
#define CONST_B                 (0.626052504536199f)
#define CONST_C                 (-0.0000453556859925276f)
#define CONST_D                 (5.04269850276224f)
#define CONST_E                 (-1570.54774682464f)

#define SIGNAL_S_RESOLUTION     (0.01f)
#define SIGNAL_S_MAX            (0.1f)
#define SIGNAL_S_MIN            (0.0f)
#define BRIDGE_R_RESOLUTION     (500)     //steps between each bridge resistance section of table
#define BRIDGE_R_MAX            (9000)
#define BRIDGE_R_MIN            (6000)
#define TABLE_ROW_R             (7)       //(int)((BRIDGE_R_MAX-BRIDGE_R_MIN)/BRIDGE_R_RESOLUTION + 1)  
#define TABLE_COL_S             (4)            //(int)((SIGNAL_S_MAX-SIGNAL_S_MIN)/SIGNAL_S_RESOLUTION + 1)
#define TABLE_COL_P             (3)
//store in 3D array with 3rd layer storing [0]=R [1]=S [2]=P


//ADC buffer

//NEW2
static uint16_t rxData[rxBUFFER_SIZE];
static uint16_t processData;
//static int count_divider = 2;   //slows down external signal sampling (sample every 2 samples)
//NEW2


//ADCHS
float BridgeR1;
float BridgeR2;
uint16_t shuntCount1;
uint16_t shuntCount2;

//general flags
volatile static bool timerDone2=false;

//ADC flags
volatile static bool receiveStatus1=false; 
//volatile static bool receiveStatus2=false; test

//DAC flags
//volatile static bool transferStatus3=false; test
// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************
//General

// Function to populate the 2D array with random float values

/* //use UART to send in csv data
void populateArray(float arr[ROWS][COLS], ) {
    // Loop through each row
    for (int i = 0; i < ROWS; i++) {
        // Loop through each column in the current row
        for (int j = 0; j < COLS; j++) {
            // Populate with random float values, or use any value you prefer
            arr[i][j] = (float);  // Example: filling with product of row and column indices
        }
    }
}
*/

//NEW1
//TABLE FXNS
float Get_Pressure(float R, float S)
{
    float P = CONST_A*S + CONST_B*R + CONST_C*R*R + CONST_D*S*R + CONST_E;
    return P;
}

/*
void Populate_Table(float ***table)   //Creates an array with voltages and pressure for each bridge resistance
{
    
    float R = 0;
    float S = 0;
    float P = 0;
    for(int i=0; i < TABLE_ROW_R; i++)
    {
        for(int j=0; j < TABLE_COL_S; j++)
        {
            for(int k=0; k < TABLE_COL_P; k++)
            {
                R = BRIDGE_R_MIN + i*BRIDGE_R_RESOLUTION;
                S = SIGNAL_S_MIN + j*SIGNAL_S_RESOLUTION;
                P = Get_Pressure(R, S);
                table[i][j][0] = R;
                table[i][j][1] = S;
                table[i][j][2] = P;
            }
        }
    }
}
*/
//NEW1
//NEW2 SPI
float Get_Signal_V(uint16_t ADC_count)  //16-bit ADC [-2.5,+2.5]
{
    int zero = 32768;   //1/2 16 bits
    float signal_resolution = 0.00007629;
    float V_signal = 101;
    if(ADC_count > zero)    //(0,+2.5]
    {
        V_signal = (ADC_count - zero)*signal_resolution;
    }
    else if(ADC_count < zero)   //[-2.5,0)
    {
        V_signal = 0-((zero - ADC_count)*signal_resolution);
    }
    else    //0
    {
        V_signal = 0;
    }
    
    return V_signal;
}

void delay_us(unsigned int us)
{
    // Convert microseconds us into how many clock ticks it will take
    us *= SYS_FREQ / 1000000 / 2; // Core Timer updates every 2 ticks

    _CP0_SET_COUNT(0); // Set Core Timer count to 0

    while (us > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}

void delay_ns(unsigned int ns)
{
    // Convert microseconds us into how many clock ticks it will take
    ns *= SYS_FREQ / 10000000 / 2; // Core Timer updates every 2 ticks

    _CP0_SET_COUNT(0); // Set Core Timer count to 0

    while (ns > _CP0_GET_COUNT()); // Wait until Core Timer count reaches the number we calculated earlier
}

void delay_timer() //not functional needs a while loop
{
    for(int i = 0; i<5; i++)
    {
        TMR2_Start();
        while (!timerDone2);
    }
    timerDone2 = false;
    //timerDone2 = true;
}

//ADC Internal

//ADC SPI
//single callback controls all ADC, should be in sync
//avoiding interrupts at the same instance
//pnly using 1 SPI for now
void SPI1_Callback(uintptr_t context)
{
    //Check what interrupt
    //Clear flags
    receiveStatus1 = true;
}

void TIMER2_Callback(uint32_t status, uintptr_t context)
{
    timerDone2 = true;
    printf("-\r\n");
    TMR2_Stop();
    TMR2 = 0;
    
}

float Get_Shunt_V(uint16_t count)
{   
    return (float)count * (ADC_VREF / ADC_MAX_COUNT); //since we are using single, subtract Vcm [2.1,2.6]->[1.1,1.6]   
}

float Get_Bridge_R(uint16_t count)  //Full ADC conversions for shunt (0-Vref))
{
    float voltage = Get_Shunt_V(count);
    float current = voltage / SHUNT_RESISTANCE;
    float pot_voltage = SENSOR_INPUTV - voltage;
    return pot_voltage / current;
}

void Ready_ADCHS()
{
    while(!ADCHS_ChannelResultIsReady(ADCHS_CH3) && !ADCHS_ChannelResultIsReady(ADCHS_CH2))
    {
    };
}

void Ready_SPI1()
{
    while (SPI1_IsBusy()) 
    {
    };
}
int main ( void )
{
    //NEW1
    /*
    Populate_Table(table);
    */
    
    /* Initialize all modules */
    SYS_Initialize ( NULL );
    SPI1_CallbackRegister(SPI1_Callback, 0);
    TMR2_CallbackRegister(TIMER2_Callback, (uintptr_t) NULL);
    TMR3_Start();   //ADCHS Clock begin
    
    printf("begin\r\n");
    TMR2_Start();   //Used as a delay
    while (!timerDone2);
    timerDone2 = false;
    printf("interesting\r\n");
    //NEW2
    /*
    SPI1_Read(&rxData, sizeof(rxData));
    Ready_SPI1();
    for (size_t i = 0; i < rxBUFFER_SIZE; i++) {
        processData = rxData[i];    
    }
    */
    //NEW2
    while ( true )
    {
        shuntCount1 = 0;
        BridgeR1 = 0;
        
        /* Maintain state machines of all polled MPLAB Harmony modules. */
        SYS_Tasks ( );

        //Sample ADC Internal in alignment with External
        receiveStatus1 = false;
        
        //delay_timer();
        TMR2_Start();
        while (!timerDone2);
        timerDone2 = false;
        
        TMR2_Start();
        while (!timerDone2);
        
        if(timerDone2)//flag
        {
            timerDone2 = false;
             //Test
            //ADC EXT process
            //Ready_SPI1();
            //SPI2_Read(&rxData, sizeof(rxData));
            
            //ADCHS Process
            Ready_ADCHS();
            shuntCount1 = ADCHS_ChannelResultGet(ADCHS_CH2);    //Get ADCHS shunt voltage counts 0-4095    //1.8 Vref+     Currently not differential
            printf("voltage is: %f\r\n", Get_Shunt_V(shuntCount1));
            printf("shuntCount is: %d\r\n", shuntCount1);
            BridgeR1 = Get_Bridge_R(shuntCount1);
            printf("The Resist is: %f\r\n", BridgeR1);
            //delay_timer();
           
        }
        
        //NEW2
        receiveStatus1 = true;
        if(receiveStatus1 == true)
        {
            receiveStatus1 = false;
            SPI1_Read(&rxData, sizeof(rxData));
            Ready_SPI1();
            UART3_Write("\r\n", 2);
            UART3_Write("rxData is: \r\n", sizeof("rxData is: \r\n"));
            printf("The processData is: %d\r\n", processData); 
            printf("The BuffData is: %d\r\n", rxData[0]);
            /*
            
            
            //Signal Process
            
            SPI1_Read(&rxData, sizeof(rxData));
            Ready_SPI1();
            processData = rxData[0];
            Get_Signal_V(processData);
            
            
            //Bridge R process
            */
            
            
        }
        
        //NEW2
        
        
        
        
        
        
    }

    /* Execution should not come here during normal operation */

    return ( EXIT_FAILURE );
}


/*******************************************************************************
 End of File
*/

