/*
 * File:   EMC1414_API.c
 * Author: hectorcruz
 *
 * Created on July 31, 2019, 5:00 PM
 */


#include "xc.h"
#include "EMC1414_API.h"

void   EMC1414_Init(void){
    MCP1414_setConversRate(MCP1414_Data_Conv_Rate);
    MCP1414_setTemeratureLimit(MCP1414_Data_Temp_Limit_Int_H, MCP1414_Data_Temp_Limit_1_H,
                               MCP1414_Data_Temp_Limit_2_H , MCP1414_Data_Temp_Limit_3_H, 
                               MCP1414_Data_Temp_Limit_Int_L, MCP1414_Data_Temp_Limit_1_L,
                               MCP1414_Data_Temp_Limit_2_L , MCP1414_Data_Temp_Limit_3_L);
    MCP1414_setThermLimit(MCP1414_Data_Therm_Limit_Int , MCP1414_Data_Therm_Limit_Ext_1 , 
                          MCP1414_Data_Therm_Limit_Ext_2 , MCP1414_Data_Therm_Limit_Ext_3 ,
                          MCP1414_Data_Therm_Hysteresis);
    MCP1414_setChannelMask(MCP1414_Data_Channel_Mask);
    MCP1414_setConsecAlert(MCP1414_Data_Consec_Alert);
    MCP1414_setDiode1BetaConfig(MCP1414_Data_Beta_Config_1);
    MCP1414_setDiode2BetaConfig(MCP1414_Data_Beta_Config_2);
    MCP1414_setExtIdealityFactor(MCP1414_Data_Ext_Ideality_Factor_1 , MCP1414_Data_Ext_Ideality_Factor_2,
                                 MCP1414_Data_Ext_Ideality_Factor_3);
    MCP1414_setFilterRate(MCP1414_Data_Filter_Config);
    MCP1414_writeConfig(MCP1414_Data_Config);
}

void   MCP1414_writeByte (uint8_t wrAddr, uint8_t wrData){
    i2c2_driver_start(MCP1414_Addr_Write);	 /* Start with device write address */
	i2c2_driver_TXData(wrAddr);	 
	i2c2_driver_TXData(wrData);	    
	i2c2_driver_stop();
}
uint8_t     MCP1414_readByte (uint8_t rdAddr){
    uint8_t data;
    i2c2_driver_start(MCP1414_Addr_Write);								/* I2C start with device write address */
	i2c2_driver_TXData(rdAddr);							/* Write start location address from where to read */ 
	i2c2_driver_restart(MCP1414_Addr_Read);							/* I2C start with device read address */
    data = i2c2_driver_getRXData(1);
    i2c2_driver_stop();
    return data;
}
int  MCP1414_readInterTemp (void){
    //int data;
    uint8_t dataMSB;
    uint8_t dataLSB;
    dataMSB = MCP1414_readByte(MCP1414_Addr_Int_Temp_HB);
    dataLSB = MCP1414_readByte(MCP1414_Addr_Int_Temp_LB);
    //data = (dataMSB + (float)((dataLSB&(0b10000000))*(float)0.5) + (float)((dataLSB&(0b01000000))*(float)0.25) + (float)((dataLSB&(0b00100000))*(float)0.125) ) - 64;
    
    return (dataMSB - 64);
}
int  MCP1414_readExtern1Temp (void){
    //int data;
    uint8_t dataMSB;
    uint8_t dataLSB;
    dataMSB = MCP1414_readByte(MCP1414_Addr_Ext_Temp_1_HB);
    dataLSB = MCP1414_readByte(MCP1414_Addr_Ext_Temp_1_LB);
    //data = (data + (float)((datatemp & 0b10000000)*(float)0.5) + (float)((datatemp&0b01000000)*(float)0.25) + (float)((datatemp&0b00100000)*(float)0.125)) - 64;
    return (dataMSB - 64);
}
int  MCP1414_readExtern2Temp (void){
    //int data;
    uint8_t dataMSB;
    uint8_t dataLSB;
    dataMSB = MCP1414_readByte(MCP1414_Addr_Ext_Temp_2_HB);
    dataLSB = MCP1414_readByte(MCP1414_Addr_Ext_Temp_2_LB);
    //data = (data + (float)((datatemp&(0b10000000))*(float)0.5) + (float)((datatemp&(0b01000000))*(float)0.25) + (float)((datatemp&(0b00100000))*(float)0.125)) - 64;
    return (dataMSB - 64);
}
int  MCP1414_readExtern3Temp (void){
    //int data;
    uint8_t dataMSB;
    uint8_t dataLSB;
    dataMSB = MCP1414_readByte(MCP1414_Addr_Ext_Temp_3_HB);
    dataLSB = MCP1414_readByte(MCP1414_Addr_Ext_Temp_3_LB);
    //data = (data + (float)((datatemp&(0b10000000))*(float)0.5) + (float)((datatemp&(0b01000000))*(float)0.25) + (float)((datatemp&(0b00100000))*(float)0.125)) - 64;
    return (dataMSB - 64);
}
uint8_t     MCP1414_readStatReg          (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Status);
    return data;
} 
void   MCP1414_writeConfig (uint8_t wrData){
    MCP1414_writeByte(MCP1414_Addr_Configuration,wrData);
}
void   MCP1414_setConversRate (uint8_t ConversRate){
    MCP1414_writeByte(MCP1414_Addr_Configuration , ConversRate);
}
void   MCP1414_setTemeratureLimit (uint8_t LimitInt_H ,uint8_t Limit1_H, uint8_t Limit2_H, uint8_t Limit3_H , uint8_t LimitInt_L ,uint8_t Limit1_L, uint8_t Limit2_L, uint8_t Limit3_L){
    MCP1414_writeByte(MCP1414_Addr_Int_Temp_H_Limit      , LimitInt_H);
    MCP1414_writeByte(MCP1414_Addr_Int_Temp_L_Limit      , LimitInt_L);
    MCP1414_writeByte(MCP1414_Addr_Ext_Temp_1_H_Limit_HB , Limit1_H);
    MCP1414_writeByte(MCP1414_Addr_Ext_Temp_1_L_Limit_HB , Limit1_L);
    MCP1414_writeByte(MCP1414_Addr_Ext_Temp_2_H_Limit_HB , Limit2_H);
    MCP1414_writeByte(MCP1414_Addr_Ext_Temp_2_L_Limit_HB , Limit2_L);
    MCP1414_writeByte(MCP1414_Addr_Ext_Temp_3_H_Limit_HB , Limit3_H);
    MCP1414_writeByte(MCP1414_Addr_Ext_Temp_3_L_Limit_HB , Limit3_L);
}
void   MCP1414_setThermLimit(uint8_t IntLimit, uint8_t ExtLimit1, uint8_t ExtLimit2, uint8_t ExtLimit3, uint8_t Hysteresis){
    MCP1414_writeByte(MCP1414_Addr_Therm_Hysteresis  , Hysteresis);
    MCP1414_writeByte(MCP1414_Addr_Int_Therm_limit   , IntLimit);
    MCP1414_writeByte(MCP1414_Addr_Ext_Therm_limit_1 , ExtLimit1);
    MCP1414_writeByte(MCP1414_Addr_Ext_Therm_limit_2 , ExtLimit2);
    MCP1414_writeByte(MCP1414_Addr_Ext_Therm_limit_3 , ExtLimit3);
}
void   MCP1414_setChannelMask (uint8_t ChannelMask){
    MCP1414_writeByte(MCP1414_Addr_Channel_Mask , ChannelMask);
}
void   MCP1414_setConsecAlert (uint8_t ConsecAlert){
    MCP1414_writeByte(MCP1414_Addr_Consec_Alert , ConsecAlert);
}
void   MCP1414_setDiode1BetaConfig (uint8_t enable){
    MCP1414_writeByte(MCP1414_Addr_Ext_Beta_Config_1 , enable);
} 
void   MCP1414_setDiode2BetaConfig (uint8_t enable){
    MCP1414_writeByte(MCP1414_Addr_Ext_Beta_Config_2 , enable);
}
void   MCP1414_setExtIdealityFactor (uint8_t Factor1 , uint8_t Factor2 , uint8_t Factor3 ){
    MCP1414_writeByte(MCP1414_Addr_Ext_Ideality_Factor_1 , Factor1);
    MCP1414_writeByte(MCP1414_Addr_Ext_Ideality_Factor_2 , Factor2);
    MCP1414_writeByte(MCP1414_Addr_Ext_Ideality_Factor_3 , Factor3);
}
void   MCP1414_setFilterRate (uint8_t filterDecodeRate){
    MCP1414_writeByte(MCP1414_Addr_Filter_Control , filterDecodeRate);
}

uint8_t     MCP1414_readProductID        (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Product_ID);
    return data;
}
uint8_t     MCP1414_readManufacturerID   (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Microchip_ID);
    return data;
} 
uint8_t     MCP1414_readRevisionRegister (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Revision);
    return data;
}
uint8_t     MCP1414_getInterruptState    (void){
    return 0;
}
uint8_t     MCP1414_readThermLimitStat   (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Limit_Status_Therm);
    return data;
}
uint8_t     MCP1414_readExterDiodeFault  (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Ext_Fault);
    return data;
}
uint8_t     MCP1414_readLowLimitStat     (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Limit_Status_L);
    return data;
}
uint8_t     MCP1414_readHighLimitStat    (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Limit_Status_H);
    return data;
}
uint8_t     MCP1414_readConfig           (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Configuration);
    return data;
}
uint8_t     MCP1414_readFilterRate       (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Filter_Control);
    return data;
}
uint8_t     MCP1414_readDiode1BetaConfig (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Ext_Beta_Config_1);
    return data;
} 
uint8_t     MCP1414_readDiode2BetaConfig (void){
    uint8_t data;
    data = MCP1414_readByte(MCP1414_Addr_Ext_Beta_Config_2);
    return data;
} 
