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

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "mcc.h"
#include "i2c2_driver.h"

#ifndef FCY
#define FCY _XTAL_FREQ/2
#endif

#include <libpic30.h>
#include <p33CK128MP502.h>

void (*i2c2_driver_busCollisionISR)(void);
void (*i2c2_driver_Masteri2cISR)(void);
void (*i2c2_driver_Slavei2cISR)(void);

inline void i2c2_driver_close(void)
{
    I2C2CONLbits.I2CEN = 0;
}

/* Interrupt Control */
inline void i2c2_enableIRQ(void)
{
    IEC2bits.MI2C2IE = 1;
    IEC2bits.SI2C2IE = 1;
}

inline bool i2c2_IRQisEnabled(void)
{
    return IEC2bits.MI2C2IE || IEC2bits.SI2C2IE;
}

inline void i2c2_disableIRQ(void)
{
    IEC2bits.MI2C2IE = 0;
    IEC2bits.SI2C2IE = 0;
}

inline void i2c2_clearIRQ(void)
{
    IFS2bits.MI2C2IF = 0;
    IFS2bits.SI2C2IF = 0;
}

inline void i2c2_setIRQ(void)
{
    IFS2bits.MI2C2IF = 1;
    IFS2bits.SI2C2IF = 1;
}

inline void i2c2_waitForEvent(uint16_t *timeout)
{
    //uint16_t to = (timeout!=NULL)?*timeout:100;
    //to <<= 8;
    if((IFS2bits.MI2C2IF == 0) && (IFS2bits.SI2C2IF == 0))
    {
        while(1)// to--)
        {
            if(IFS2bits.MI2C2IF || IFS2bits.SI2C2IF) break;
            __delay_us(100);
        }
    }
}

bool i2c2_driver_driver_open(void)
{
    if(!I2C2CONLbits.I2CEN)
    {
        // initialize the hardware
        // STAT Setting 
        I2C2STAT = 0x0;
     
        // Baud Rate Generator Value: I2CBRG 100000;   
        I2C2BRG = 0x12;
        
        // CON Setting
        I2C2CONL = 0b0000001100000000;          /*0x8000- 0b 1000 0011 0000 0000*/
        I2C2CONLbits.SCLREL =   1;     
        I2C2CONLbits.I2CEN  =   1;
        IEC1bits.MI2C1IE    =   0;
        I2C2STATbits.S      =   0;
       
        return true;
    }
    else
        return false;
}
bool i2c2_driver_initSlaveHardware(void)
{
    if(!I2C2CONLbits.I2CEN)
    {
/* NOTE on AHEN:
 * If multiple slaves are to be emulated, then AHEN must be set.  It must be set
 * because the driver needs to selectively ACK/NACK the address depending on its
 * ability to handle the address.
*/

/* NOTE on DHEN:
 * DHEN must be set so that the data is not automatically NACK'ed if it is not read
 * from the SSPBUF.  This driver will ALWAYS read the SSPBUF so that it can pass
 * the value to the appropriate slave handler.  Because the data is ALWAYS read
 * the data will always be ACK'd if DHEN is cleared.  If the slave does not want
 * the data byte from the master then it will return false and a NACK will be returned.
 */

/* NOTE on SEN:
 * SEN will be set enabling clock stretching.  This is because we don't know how
 * long the user will take to process data bytes in their callbacks.  If they are fast,
 * we may not need to stretch the clock.  If they are slow, we need to stretch the clock.
 * If we ALWAYS stretch the clock, we will release the clock when the ISR is complete.
 */

/* NOTE on PCIE:
 * PCIE will be set to enable interrupts on STOP.  This will allow us know when
 * the master is finished
 */
        
/* NOTE on SCIE:
 * SCIE will be set to enable interrupts on START.  This will allow us to detect
 * both a START and a RESTART event and prepare to restart communications.
 */
        I2C2CONHbits.AHEN = 1;
        I2C2CONHbits.DHEN = 1;
        I2C2CONLbits.STREN = 1;
        
        I2C2CONLbits.I2CEN = 1;
        return true;
    }
    return false;
}

inline void i2c2_driver_resetBus(void)
{
    
}

inline void i2c2_driver_start(uint8_t slave_address)
{
    while(1){
        I2C2CONLbits.SEN = 1;
        while(I2C2CONLbits.SEN);
        i2c2_clearIRQ();
        
        if(!I2C2STATbits.S){
            if(I2C2STATbits.IWCOL || I2C2STATbits.I2COV){
               I2C2STATbits.IWCOL = 0;
               I2C2STATbits.I2COV = 0;
               continue;
            }
        }
        if(!i2c2_driver_TXData(slave_address)){
            i2c2_driver_stop();
            continue;
        }
        break;
    }
}

inline char i2c2_driver_restart(uint8_t slave_ReadAddress)
{
    I2C2CONLbits.RSEN = 1;              
    while(I2C2CONLbits.SEN);
    i2c2_clearIRQ();
    if(!i2c2_driver_TXData(slave_ReadAddress)){
        return 0;
    }else{
        return 1;
    }
}

inline void i2c2_driver_stop(void)
{
    I2C2CONLbits.PEN = 1;
    while(I2C2CONLbits.PEN);
    i2c2_clearIRQ();
    
    
}

inline char i2c2_driver_isNACK(void)
{
    return (I2C2STATbits.ACKSTAT);
}

inline void i2c2_driver_startRX(void)
{
    I2C2CONLbits.RCEN = 1;
}

inline char i2c2_driver_getRXData(char flag)
{
    i2c2_driver_startRX();
    while(!I2C2STATbits.RBF);
    char buffer = I2C2RCV;
    if (flag == 0){
        i2c2_driver_sendACK();
    }else{
        i2c2_driver_sendNACK();
    }
    return buffer;
    
}

inline char i2c2_driver_TXData(uint8_t data)
{
 
    I2C2TRN = data;
    while(I2C2STATbits.TBF);     /*Wait untill the Tx buffer is Empty*/
    if(!I2C2STATbits.ACKSTAT){
        return 1;
    }else{
        return 0;
    }
}

inline void i2c2_driver_sendACK(void)
{
    I2C2CONLbits.ACKDT = 0;
    I2C2CONLbits.ACKEN = 1; // start the ACK/NACK
}

inline void i2c2_driver_sendNACK(void)
{
    I2C2CONLbits.ACKDT = 1;
    I2C2CONLbits.ACKEN = 1; // start the ACK/NACK
}

inline void i2c2_driver_releaseClock(void)
{
    I2C2CONLbits.SCLREL = 1;
}

inline bool i2c2_driver_isBuferFull(void)
{
    return I2C2STATbits.RBF || I2C2STATbits.TBF;
}

inline bool i2c2_driver_isStart(void)
{
    return I2C2STATbits.S;
}

inline bool i2c2_driver_isAddress(void)
{
    return !I2C2STATbits.D_A;
}

inline bool i2c2_driver_isStop(void)
{
    return I2C2STATbits.P;
}

inline bool i2c2_driver_isData(void)
{
    return I2C2STATbits.D_A;
}

inline bool i2c2_driver_isRead(void)
{
    return I2C2STATbits.R_W;
}

inline void i2c2_driver_clearBusCollision(void)
{
    I2C2STATbits.BCL = 0; // clear the bus collision.
}

inline void i2c2_driver_enableStartIRQ(void)
{
    I2C2CONHbits.SCIE = 1;
}

inline void i2c2_driver_disableStartIRQ(void)
{
    I2C2CONHbits.SCIE = 0;
}

inline void i2c2_driver_enableStopIRQ(void)
{
    I2C2CONHbits.PCIE = 1;
}

inline void i2c2_driver_disableStopIRQ(void)
{
    I2C2CONHbits.PCIE = 0;
}

inline void i2c2_driver_setBusCollisionISR(void *f){
    i2c2_driver_busCollisionISR = f;
}

inline void i2c2_driver_setMasterI2cISR(void *f){
    i2c2_driver_Masteri2cISR = f;
}

inline void i2c2_driver_setSlaveI2cISR(void *f){
    i2c2_driver_Slavei2cISR = f;
}

void __attribute__ ( ( interrupt, no_auto_psv ) ) _MI2C2Interrupt ( void )
{
    (*i2c2_driver_Masteri2cISR)();
}
void __attribute__ ( ( interrupt, no_auto_psv ) ) _SI2C2Interrupt ( void )
{
    (*i2c2_driver_Slavei2cISR)();
}
