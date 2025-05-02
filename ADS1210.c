#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "peripheral/ports/plib_ports.h"

#ifndef ADS1210_H
#include "ADS1210.H"
#endif

extern uint8_t ADS1210_INCR;
extern uint8_t ADS1210_OCR0,ADS1210_OCR1,ADS1210_OCR2;
extern uint8_t ADS1210_FCR0,ADS1210_FCR1,ADS1210_FCR2;

static bool FetchAD = false;
extern const uint16_t WaitAd;

void Delay_N_US(uint16_t N)
{
uint16_t i,j;
for(j=0;j<N;j++) {
    for(i=0;i<7;i++){}
    }
}

void ADS1210_SDI(bool Set)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_G,PORTS_BIT_POS_7,Set);
}

void ADS1210_SCK(bool Set)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_G,PORTS_BIT_POS_6,Set);
}

bool ADS1210_SDO_IN(void)
{
    return(PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_C,PORTS_BIT_POS_4));
}

bool ADS1210_DRDY_IN(void)
{
    return(PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_7));
}

void ADS1210_Reset(void)
{
uint16_t i;
    ADS1210_SCK(1);
    Delay_N_US(28);
    ADS1210_SCK(0);
    Delay_N_US(1);
    ADS1210_SCK(1);
    Delay_N_US(56);
    ADS1210_SCK(0);
    Delay_N_US(1);
    ADS1210_SCK(1);
    Delay_N_US(112);
    ADS1210_SCK(0);
    Delay_N_US(1);
}


void ADS1210_Sendbyte(uint8_t *OUTByte)
{
    uint8_t i,j;
    for (j=0x80;j>0;j/=2)
        {
        ADS1210_SDI(j & *OUTByte);
        ADS1210_SCK(1);
        for(i=0;i<WaitAd;i++){}
        ADS1210_SCK(0);
        for(i=0;i<WaitAd;i++){}
        }
}
void ADS1210_Readbyte(uint8_t *INByte)
{
    uint8_t i , j;
    
    *INByte = 0;
    for (j=0x80;j>0;j/=2)
        {
        ADS1210_SCK(1);
        for(i=0;i<WaitAd;i++){}
        if(ADS1210_SDO_IN())
            {
            *INByte = (*INByte | j);
            }
        ADS1210_SCK(0);
        for(i=0;i<WaitAd;i++){}
        asm("nop");
        }
}

void ADS1210_ReadCR(void)
{   
    ADS1210_INCR = 0;
    ADS1210_INCRbits.ADR = 4;
    ADS1210_INCRbits.R_W = 1;
    ADS1210_INCRbits.MB = 3;
    while (ADS1210_DRDY_IN()==1) {};
    ADS1210_Sendbyte(&ADS1210_INCR);
    ADS1210_Readbyte(&ADS1210_CRB3);
    ADS1210_Readbyte(&ADS1210_CRB2);
    ADS1210_Readbyte(&ADS1210_CRB1);
    ADS1210_Readbyte(&ADS1210_CRB0);
}

void ADS1210_ReadOCR(void)
{   
    ADS1210_INCR = 0;
    ADS1210_INCRbits.ADR = 8;
    ADS1210_INCRbits.R_W = 1;
    ADS1210_INCRbits.MB = 2;
    while (ADS1210_DRDY_IN()==1) {};
    ADS1210_Sendbyte(&ADS1210_INCR);
    ADS1210_Readbyte(&ADS1210_OCR2);
    ADS1210_Readbyte(&ADS1210_OCR1);
    ADS1210_Readbyte(&ADS1210_OCR0);
 }

void ADS1210_ReadFCR(void)
{   
    ADS1210_INCR = 0;
    ADS1210_INCRbits.ADR = 12;
    ADS1210_INCRbits.R_W = 1;
    ADS1210_INCRbits.MB = 2;
    while (ADS1210_DRDY_IN()==1) {};
    ADS1210_Sendbyte(&ADS1210_INCR);
    ADS1210_Readbyte(&ADS1210_FCR2);
    ADS1210_Readbyte(&ADS1210_FCR1);
    ADS1210_Readbyte(&ADS1210_FCR0);
 }

void ADS1210_WriteCR(void)
{
    ADS1210_INCR = 0;
    ADS1210_INCRbits.ADR = 4;
    ADS1210_INCRbits.R_W = 0;
    ADS1210_INCRbits.MB = 3;
    while (ADS1210_DRDY_IN()) {};
    ADS1210_Sendbyte(&ADS1210_INCR);
    ADS1210_Sendbyte(&ADS1210_CRB3);
    ADS1210_Sendbyte(&ADS1210_CRB2);
    ADS1210_Sendbyte(&ADS1210_CRB1);
    ADS1210_Sendbyte(&ADS1210_CRB0);
}

void ADS1210_Init(void)
{
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_7);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_4);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_6);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_7);
    
    ADS1210_Reset();
    asm ("nop");
    ADS1210_CRB0 = 0x1a; // lower byte Datarate 0x138
    ADS1210_CRB1 = 0x06 | (0x03<< 5); // higher byte datarate + turbomode
    ADS1210_CRB2bits.CH = 0;
    ADS1210_CRB2bits.GAIN = 1;          // war 0
    ADS1210_CRB2bits.MODE = 1;
    ADS1210_CRB3bits.BIAS = 0;
    ADS1210_CRB3bits.REFO = 1;
    ADS1210_CRB3bits.DF = 0;
    ADS1210_CRB3bits.U_B = 0;
    ADS1210_CRB3bits.BD = 0;
    ADS1210_CRB3bits.MSB = 0;
    ADS1210_CRB3bits.SDL = 1;
    ADS1210_CRB3bits.DS_DR = 0;
    ADS1210_WriteCR();
}

bool ADS1210_ReadDOR(void)
{
    if (!FetchAD & ADS1210_Initialized)
    {
    FetchAD = true;
    ADS1210_INCR = 0;
    ADS1210_INCRbits.ADR = 5;
    ADS1210_INCRbits.R_W = 0;
    ADS1210_INCRbits.MB = 0;
    ADS1210_Sendbyte(&ADS1210_INCR);
    ADS1210_CRB2bits.CH = 0;
    ADS1210_CRB2bits.GAIN = 0;
    ADS1210_CRB2bits.MODE = 1;
    ADS1210_Sendbyte(&ADS1210_CRB2);
    return false;
    }
    else
    {
        FetchAD = true;
        ADS1210_INCR = 0;
        ADS1210_INCRbits.ADR = 0;
        ADS1210_INCRbits.R_W = 1;
        ADS1210_INCRbits.MB = 3;
        if (!ADS1210_DRDY_IN())
        {
            ADS1210_Sendbyte(&ADS1210_INCR);
            ADS1210_Readbyte(&ADS1210_DOR2);
            ADS1210_Readbyte(&ADS1210_DOR1);
            ADS1210_Readbyte(&ADS1210_DOR0);
            ADS1210_Readbyte(&ADS1210_CRB3);
            asm("nop");
            return true;
        }
        else
        {
            return false;
        }
    }
}
