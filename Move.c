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


#ifndef MOVE_H
#include "Move.h"
#endif

#define NO_DRIVE_OK

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */
extern uint8_t Motor_Status;

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */
void MoveGetState(void)
{
    Motor_GetStatus();
    if (Motor_Status==0xff) Motor_Status=0x80;
    MoveData.MoveState = Motor_Status;
}

bool Move_Is_Ready(void)
{
    uint8_t i;
    for(i=0;i<100;i++)
    {
        MoveGetState();
        if((MoveData.MoveState!=3) | (MoveData.MoveState!=2))
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
        case 1: { MoveData.Wait=8; } break; // 0.3s vor
        
        case 2: { MoveData.Wait=11; } break;// 0.6s vor
        
        case 3: { MoveData.Wait=15; } break;// 1.0s vor
        
        case 4: { MoveData.Wait=25; } break;// 2.0s vor
        
        case 5: { MoveData.Wait=45; } break;// 4.0s vor
        
        case 6: { MoveData.Wait=85; } break;// 8.0s vor
        
    default:
        { MoveData.Wait=50; } break;
        break;
    }
#ifdef DRIVE_OK
    MoveData.Wait=1;
#endif    
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

void Move_Init(void)
{
// Epot Ports definieren
    MoveData.State = MOVE_STARTUP;
    MoveData.NextState = MOVE_IDLE;
    MoveData.Profile = 0;
    MoveData.Wait = 0;
    MoveData.ErrCnt = 0;
}

bool Move_To_Aktiv(void)
{
    Motor_Aktiv(); // Send I2C command
    Move_Get_Wait();
    asm ("nop");
    return true;
}
bool Move_To_Passiv(void)
{
    Motor_Passiv(); // Send I2C command
    Move_Get_Wait();
    asm ("nop");
    return true;
}
void Move_Task(void)
{
    MoveData.State &= 0x0f;
    switch(MoveData.State)
    {   case MOVE_STARTUP:
            break;
        case MOVE_TO_AKTIV:
        {
            MoveData.State = MOVE_MOVE;
            Move_To_Aktiv();
        }
        break;
        case MOVE_TO_PASSIV:
        {
            MoveData.State = MOVE_MOVE;
            Move_To_Passiv();
        }
        break;
        case MOVE_HOME:
        {    
            MoveData.State = MOVE_HOME_WAIT;
            Motor_Home();
        }
        break;
        case MOVE_HOME_WAIT:
        {
            if( SampleIsPassiv())
            {
                MoveData.State = MOVE_IDLE;
                MoveData.NextState = MOVE_IDLE;
            }
        }
        break;
        case MOVE_MOVE:
        {
            MoveGetState();
            if(MoveData.MoveState & 0x04)
            {
                MoveData.State = MOVE_ERROR;
                Motor_Home();
                break;
            }
            if (((MoveData.MoveState & 0xd0)==0x10) | ((MoveData.MoveState & 0xD0)==0x40))
            {
                MoveData.State = MOVE_IDLE;
                MoveData.NextState = MOVE_IDLE;
                break;
            }
        }
        break;
        case MOVE_IDLE:
        {
            if (MoveData.NextState==MOVE_IDLE)
            {
                if(MoveData.Wait>0)
                {
                    MoveData.Wait--;
                }
                break;
            }
            if (MoveData.NextState==MOVE_POWERUP)
            {
                MoveData.Wait = 10;
                MoveData.State = MOVE_HOME;
                break;
            }
            if (MoveData.NextState==MOVE_TO_AKTIV)
            {
                MoveData.Wait = 10;
                MoveData.State = MOVE_TO_AKTIV;
                break;
            }
            if (MoveData.NextState==MOVE_TO_PASSIV)
            {
                MoveData.Wait = 10;
                MoveData.State = MOVE_TO_PASSIV;
                break;
            }
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
                    MoveData.State = MOVE_HOME;
                    MoveData.NextState = MOVE_IDLE;
                }
                else
                {
                    MoveData.State = MoveData.NextState;
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
