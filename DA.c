/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include "peripheral/ports/plib_ports.h"

#ifndef DA_H
#include "DA.H"
#endif

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

uint16_t WAIT=3;
uint16_t DA_OFS_Out = 0;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void DA_Init(void)
{
// DA Wandler Ports definieren
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_1);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_2);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_3);
    DA_CS(0);
    DA_Set(0x0);
    DA_On(false);
}
void DA_CS(bool Set)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_3,!Set);
}

void DA_CLK(bool Set)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_2,Set);
}

void DA_DAT(bool Set)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_1,Set);
}
void DA_Set(uint16_t Value)
{
    DA_OFS_Out = Value;
}

void DA_On(bool On)
{
    uint8_t i,k;
    uint16_t Out,j;
    
    Out = 0;
    if(On)
    {
        Out = DA_OFS_Out;
    }
    DA_CLK(0);
    for(i=0;i<WAIT;i++){}
    DA_CS(true);
    for(i=0;i<WAIT;i++){}
    for (j=0x8000;j>0;j/=2)
        {
        DA_DAT(j & Out);
        for(i=0;i<WAIT;i++){}
        DA_CLK(1);
            for(i=0;i<WAIT;i++){}
        DA_CLK(0);
        for(i=0;i<WAIT;i++){}
        }
    DA_CS(false);

}


/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/* *****************************************************************************
 End of File
 */
