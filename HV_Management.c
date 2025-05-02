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
#include <stdio.h>
#include <stdbool.h>
#include "system/tmr/sys_tmr.h"
#include "peripheral/ports/plib_ports.h"
#include "ULN.h"
#include "DA.h"
#include "app.h"

extern int HV_AC,HV_DC; // Werte in Volt
extern bool HV_AC_Flag;
SYS_TMR_HANDLE HVTmr;

void HV_AC_DC_Set(bool Set)
{
    uint16_t Out;
    int32_t X;
  
    if (Set)
    {
        X = HV_DC * 65535; 
        Out = X / 10000;
        DA_Set(Out);
        HV_AC=1023;
        HV_AC_Flag = true;
        ULN_HV_AC_ON(true);
        DA_On(1);
    }
    else
    {
        HV_AC=0;
        DA_Set(0);
        ULN_HV_AC_ON(false);
        HV_AC_Flag = false;
        DA_On(0);
    }
}

/* *****************************************************************************
 End of File
 */
