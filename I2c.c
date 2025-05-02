/* ************************************************************************** */
/** Descriptive File Name

  @Company
    QUMA Elektronik & Analytik GmbH

  @File Name
    I2C.c

  @Summary
    Hilfsroutinen fur den Feuchtesensor

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <peripheral/i2c/plib_i2c.h>
/* TODO:  Include other files here if needed. */

void HYT_StartMR();
void HYT_Getdata();
uint8_t Motor_GetStatus();
void Motor_Aktiv();
void Motor_Passiv();
void Motor_Home();
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

const uint8_t HYT_Adr = 0x28;
const uint8_t Motor_Adr = 0x36,MotorHomeCmd = 0x0E;
const uint8_t MotorStatusCmd = 0x40 ,MotorAktivCmd = 0x0C,MotorPassivCmd = 0x0d;
//extern uint8_t ProfileAC,ProfileDC;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

#define I2C_ID I2C_ID_2
#define MY_I2C_BAUD_RATE 80000
#define MY_PERIPHEAL_CLOCK_FREQ 80000000
#define I2C_RD 0x01
#define ACK true
#define NACK false

void I2C_Master_Init()
{
    // Configure General I2C Options
PLIB_I2C_SlaveClockStretchingEnable(I2C_ID);
PLIB_I2C_GeneralCallDisable(I2C_ID);
PLIB_I2C_SMBDisable(I2C_ID);
PLIB_I2C_HighFrequencyDisable(I2C_ID);
PLIB_I2C_ReservedAddressProtectEnable(I2C_ID);
PLIB_I2C_StopInIdleEnable(I2C_ID);

// Set Desired baud rate
PLIB_I2C_BaudRateSet ( I2C_ID, MY_PERIPHEAL_CLOCK_FREQ, MY_I2C_BAUD_RATE);
PLIB_I2C_Enable(I2C_ID);
}

bool StartTransfer(bool restart)
{
//    uint16_t i;
    if ( PLIB_I2C_ArbitrationLossHasOccurred ( I2C_ID ) )
    {
        // Handle bus collision
        asm("nop");
        PLIB_I2C_ArbitrationLossClear ( I2C_ID );
    }
    PLIB_I2C_TransmitterOverflowClear(I2C_ID);
    PLIB_I2C_ReceiverOverflowClear(I2C_ID);
    PLIB_I2C_StartClear(I2C_ID);
    if (restart)
    {   
        PLIB_I2C_MasterStartRepeat(I2C_ID);
    }
    else
    {
        while(!PLIB_I2C_BusIsIdle(I2C_ID));
        {
            PLIB_I2C_MasterStart (I2C_ID);    
        }
    }
//    for(i=0;i<500;i++){}
    while(!PLIB_I2C_StartWasDetected(I2C_ID));
    return true;
}

void StopTransfer( void )
{
    while (!PLIB_I2C_StopWasDetected(I2C_ID)) PLIB_I2C_MasterStop(I2C_ID);
}

bool TransmitOneByte( uint8_t data)
{
uint16_t i;
    for(i=0;i<500;i++){}
    while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
    PLIB_I2C_TransmitterByteSend(I2C_ID,data);
    while(!(PLIB_I2C_TransmitterByteHasCompleted(I2C_ID)));
    if(PLIB_I2C_TransmitterByteWasAcknowledged(I2C_ID))
    {
        asm("nop");
        return true;
    }
    else
    {
        asm("nop");
        return false;
    }
 }

uint8_t ReceiveOneByte(bool Acknowledge)
{
uint8_t Rcv = 0;
uint16_t i;
    for(i=0;i<500;i++){}
    PLIB_I2C_MasterReceiverClock1Byte(I2C_ID);
    while(!PLIB_I2C_ReceivedByteIsAvailable(I2C_ID));
    Rcv = PLIB_I2C_ReceivedByteGet(I2C_ID);
    PLIB_I2C_ReceivedByteAcknowledge(I2C_ID,Acknowledge);
    PLIB_I2C_ReceiverByteAcknowledgeHasCompleted(I2C_ID);
    return Rcv;
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

extern uint8_t HYT_Status;
extern uint8_t Motor_Status;
int Feuchte,Temperatur;

uint8_t Motor_GetStatus()
{
uint8_t Result;
uint8_t data;
    if (StartTransfer(false)==true)
    {
        TransmitOneByte(Motor_Adr);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        data = MotorStatusCmd;
        Result = TransmitOneByte(data);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        StartTransfer(true);
        TransmitOneByte(Motor_Adr | I2C_RD);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        Motor_Status=ReceiveOneByte(NACK);
        StopTransfer();
    }
}

void Motor_Home()
{
bool Result;
uint8_t data;
    Result = false;
    if (StartTransfer(false)==true)
    {
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        data = Motor_Adr;
        Result = TransmitOneByte(data);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        data = MotorHomeCmd;
        Result = TransmitOneByte(data);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        StopTransfer();
    }
}
void Motor_Aktiv(void)
{
    uint8_t data;
    if (StartTransfer(false)==true)
    {
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        data = Motor_Adr;
        TransmitOneByte(data);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        data = MotorAktivCmd;
        TransmitOneByte(data);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        StopTransfer();
    }    
}
void Motor_Passiv(void)
{
uint8_t data;
    if (StartTransfer(false)==true)
    {
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        data = Motor_Adr;
        TransmitOneByte(data);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        data = MotorHomeCmd;
        TransmitOneByte(data);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        StopTransfer();
    }    
}
void HYT_StartMR()
{
bool Result;
uint8_t data;
    Result = false;
    if (StartTransfer(false)==true)
    {
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        data = HYT_Adr << 1;
        Result = TransmitOneByte(data);
        if(Result)
        {
            asm("nop");
            StopTransfer();
        }
        else
        {
            asm("nop");
            StopTransfer();
        }
    }
}

void HYT_Getdata()
{   
uint8_t RcvByte;
    if (StartTransfer(false)==true)
    {
        TransmitOneByte(0x51);
        while(PLIB_I2C_TransmitterIsBusy(I2C_ID)){};
        RcvByte=ReceiveOneByte(ACK);
        HYT_Status=RcvByte;
        if ((HYT_Status & 0x40) == 0x00)
        {
            Feuchte = (RcvByte & 0x3f) << 8;
            RcvByte = ReceiveOneByte(ACK);
            Feuchte = Feuchte + RcvByte;
            RcvByte = ReceiveOneByte(ACK);
            Temperatur = (RcvByte << 8);
            RcvByte = ReceiveOneByte(NACK);
            Temperatur = Temperatur + RcvByte;
            Temperatur = (Temperatur >> 2);
            Feuchte = (628 * Feuchte) >> 10;
            Temperatur = ((4125 * Temperatur) >> 12) - 4000;
        }
        
        StopTransfer();
    }
}

/* *****************************************************************************
 End of File
 */
