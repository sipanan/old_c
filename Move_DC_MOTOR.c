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
#include "ULN.h"
#include "DA.h"
#include "HV_Manegement.h"

#ifndef MOVE_H
#include "Move.h"
#endif

#define DRIVE_OK
// #define NO_MOVE
#define MOVE

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
void MoveGetState(void)
{
    if ((PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_G,PORTS_BIT_POS_14)==0)
       && (PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_5)==1))
       MoveData.MoveState = 0;
    if ((PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_G,PORTS_BIT_POS_14)==0)
       && (PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_5)==0))
       MoveData.MoveState = 1;
    if ((PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_G,PORTS_BIT_POS_14)==1)
       && (PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_5)==0))
       MoveData.MoveState = 2;
}

bool Move_Is_Ready(void)
{
    uint8_t i;
    for(i=0;i<100;i++)
    {
        MoveGetState();
        if(MoveData.MoveState!=1)
        {
            return false;
            break;
        }
    }
    return true;
}
void Move_Get_Wait(void)
{
    switch (MoveData.Profile)
    {
        case 0: { MoveData.Wait=2; } break;
        case 1: { MoveData.Wait=2; } break;
        case 2: { MoveData.Wait=4; } break;
        case 3: { MoveData.Wait=4; } break;
        case 4: { MoveData.Wait=7; } break;
        case 5: { MoveData.Wait=7; } break;
        case 6: { MoveData.Wait=11; } break;
        case 7: { MoveData.Wait=11; } break;
        case 8: { MoveData.Wait=21; } break;
        case 9: { MoveData.Wait=21; } break;
        case 10: { MoveData.Wait=41; } break;
        case 11: { MoveData.Wait=41; } break;
        case 12: { MoveData.Wait=81; } break;
        case 13: { MoveData.Wait=81; } break;
    default:
        { MoveData.Wait=50; } break;
        break;
    }
#ifdef DRIVE_OK
    MoveData.Wait=31;
#endif    
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void Move_Init(void)
{
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_0);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_5);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_8);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_9);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_10);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_B, PORTS_BIT_POS_11);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_12);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_13);
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_G, PORTS_BIT_POS_14);
    PLIB_PORTS_PinDirectionOutputSet(PORTS_ID_0, PORT_CHANNEL_C, PORTS_BIT_POS_1);
    ULN_PWRDRV(OFF);
    MoveData.State = MOVE_STARTUP;
    MoveData.NextState = MOVE_IDLE;
    MoveData.Profile = 0;
    MoveData.Wait = 0;
}

bool Move_To(uint8_t Zielnummer)
{
    if(Zielnummer & 0x01)
        {
        PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_9,1);
        PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_10,0);
        PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_8,1);
        }
        else
        {
        PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_9,0);
        PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_10,1);
        PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_8,1);
        }
    Move_Get_Wait();
    asm ("nop");
    return true;
}
void Move_Task(void)
{
    switch(MoveData.State)
    {   case MOVE_STARTUP:
            break;
        case MOVE_TO:
        {
            MoveData.State = MOVE_MOVE;
            Move_To(MoveData.Profile);
        }
        break;
        case MOVE_HOME:
        {    
            MoveData.State = MOVE_MOVE;
            MoveData.Profile=1;
            Move_To(MoveData.Profile);
        }
        break;
        case MOVE_MOVE:
        {
            MoveGetState();
            if (MoveData.MoveState==1)
            {
                if (MoveData.Wait>0)
                {
                    MoveData.Wait--;
                }
                else
                {
                    MoveData.State = MOVE_END;
                    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_9,0);
                    PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_10,0);
                    DA_On(false);
                    HV_MANAGER_DC(false);
                }
                break;
            }
            MoveData.State = MOVE_END;
            PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_9,0);
            PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_10,0);
            DA_On(false);
            HV_MANAGER_DC(false);
        }
        break;
        case MOVE_END:
        {
            PLIB_PORTS_PinWrite(PORTS_ID_0,PORT_CHANNEL_B,PORTS_BIT_POS_8,0);
            MoveData.State=MOVE_IDLE;
        }
        break;
        case MOVE_IDLE:
        {
#ifdef NO_MOVE
            {    
            MoveData.NextState=MOVE_IDLE;
            MoveData.State=MOVE_IDLE;
            break;
            }
#endif        
            if (MoveData.NextState==MOVE_IDLE)
            {
                if(MoveData.Wait>0)
                {
                    MoveData.Wait--;
                    ULN_PWRDRV(ON);
                }
                else
                {
                    ULN_PWRDRV(OFF);
                }
                break;
            }
            if (MoveData.NextState==MOVE_POWERUP)
            {
                MoveData.Wait = 10;
                ULN_PWRDRV(ON);
                MoveData.State = MOVE_WAIT_ON;
                break;
            }
            MoveData.Wait = 5;
            ULN_PWRDRV(ON);
            MoveData.State = MOVE_WAIT_ON;
        }
        break;
        case MOVE_WAIT_ON:
        {
            if(MoveData.Wait>0)
            {
                MoveData.Wait--;
            }
            else
            {
                if (MoveData.NextState==MOVE_POWERUP)
                {
                    MoveData.State=MOVE_HOME;
                    MoveData.NextState = MOVE_IDLE;
                }
                else
                {
                    MoveData.State=MoveData.NextState;
                    MoveData.NextState = MOVE_IDLE;    
                }
            }
        }
        break;
        case MOVE_ERROR:
            DBGToggle();
            asm ("nop");
        break;
        default:
        break;
    }
}
// *****************************************************************************

/* *****************************************************************************
 End of File
 */
