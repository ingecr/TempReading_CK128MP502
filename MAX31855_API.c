/*
 * File:   MAX31855_API.c
 * Author: hectorcruz
 *
 * Created on August 5, 2019, 11:35 AM
 */


#include "xc.h"
#include "MAX31855_API.h"

#define MAX31855_BYTES 4

void get_MAX31855_temperatures(uint16_t* thermocoupleTemp, uint16_t* internalTemp){

    uint16_t MAX31855_dataBuffer[MAX31855_BYTES/2];
    uint16_t dummyData = 0x0295;
    SS1_ON();
    int i;
    for(i = 0; i < MAX31855_BYTES/2; i++){
        MAX31855_dataBuffer[i] = SPI1_Exchange16bit(dummyData);
        //MAX31855_dataBuffer[i] = ((MAX31855_dataBuffer[i]<<8)&0xff00)|((MAX31855_dataBuffer[i]>8)&0x00ff);
    }
    SS1_OFF();
    *((uint16_t*)thermocoupleTemp)   = MAX31855_dataBuffer[0] >> 5;
    *((uint16_t*)internalTemp  )     = MAX31855_dataBuffer[1] >> 9;

}