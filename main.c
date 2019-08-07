/**
  Generated main.c file from MPLAB Code Configurator

  @Company
    Microchip Technology Inc.

  @File Name
    main.c

  @Summary
    This is the generated main.c using PIC24 / dsPIC33 / PIC32MM MCUs.

  @Description
    This source file provides main entry point for system initialization and application code development.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - 1.125
        Device            :  dsPIC33CK128MP502
    The generated drivers are tested against the following:
        Compiler          :  XC16 v1.36B
        MPLAB 	          :  MPLAB X v5.20
*/

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

/**
  Section: Included Files
*/
#include "mcc_generated_files/system.h"
#include "libpic30.h"
#include "EMC1414_API.h"
#include "MAX31855_API.h"
#include <xc.h>



int Temp_Int    = 0;
int Temp_Ext_1  = 0;
int Temp_Ext_2  = 0;
int Temp_Ext_3  = 0;

uint16_t thermoTemp     = 0;
uint16_t internalTemp   = 0;

int main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    //EMC1414_Init();
    EXT_INT_Initialize();
    
    //I2C2CONLbits.I2CEN = 0;
    //LED_OP_SetLow();
    while (1)
    {
        
        /*Extraction of temperatures from MAX31855*/
        //get_MAX31855_temperatures(&thermoTemp,&internalTemp);
          
        /*Extraction of temperatures from EMC1414*/
        
        /*
        I2C2CONLbits.I2CEN = 1;
        Temp_Int = MCP1414_readInterTemp();
        Temp_Ext_1 = MCP1414_readExtern1Temp();
        Temp_Ext_2 = MCP1414_readExtern2Temp();
        Temp_Ext_3 = MCP1414_readExtern3Temp();
        I2C2CONLbits.I2CEN = 0;      
        */   
        
        
        LED_OP_Toggle();
        //TRIAC_TRIGGER_Toggle();
        //__delay_ms(500);
        //TRIAC_TRIGGER_SetLow();
        __delay_ms(50);
         
    }
    return 1; 
}
