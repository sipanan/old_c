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
#include "app.h"

#ifndef ULN_H
#include "ULN.H"
#endif

extern uint8_t ULN_OUT;

void ULN_LED_TAST(bool Set)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_0,Set);
}
void ULN_HV_AC_ON(bool Set)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_F,PORTS_BIT_POS_0,Set);
}
void ULN_PWRDRV(bool Set)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_C,PORTS_BIT_POS_1,Set);
}
void ULN_OPEN(void)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_1,1);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_2,1);
}
void ULN_CLOSE(void)
{
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_1,0);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_2,0);
}
void ULN_Init(void)
{
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_0);
        // Slot Open/Close
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_1);
    // Slot Open/Close
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_2);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_0);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_F, PORTS_BIT_POS_1);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_1);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_0,0);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_1,0);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_2,0);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_3,0);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_4,0);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_F,PORTS_BIT_POS_0,0);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_F,PORTS_BIT_POS_1,0);
    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_C,PORTS_BIT_POS_1,0);
    
}
/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* *****************************************************************************
 End of File
 */
