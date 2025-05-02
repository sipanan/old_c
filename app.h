#ifndef _APP_H
#define _APP_H


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_config.h"
#include "system_definitions.h"
#include "tcpip/tcpip.h"

#define ON 1
#define OFF 0
#define BoardRevision 5
#define Debounce 10

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
// Application States
// *****************************************************************************
typedef enum
{
    APP_TMR_WAIT_INIT,
    APP_CONFIG_PERIODIC,        
    /* In this state, the application waits for the initialization of the TCP/IP stack
     * to complete. */
    APP_TCPIP_WAIT_INIT,
    /* In this state, the application waits for a IP Address */
    APP_TCPIP_WAIT_FOR_IP,
    APP_TCPIP_OPENING_SERVER,
    APP_TCPIP_WAIT_FOR_CONNECTION,
    APP_TCPIP_SERVING_CONNECTION,
    APP_TCPIP_CLOSING_CONNECTION,
    APP_TCPIP_ERROR,
} APP_STATES;

typedef enum
{   Q628_POWER_UP,
    Q628_STARTUP,
    Q628_PRE_IDLE,
    Q628_IDLE,
    Q628_WAIT_HV,
    Q628_WAIT_HV_2,
    Q628_START,
    Q628_WAIT_UNTEN,
    Q628_WAIT_START_SW,
    Q628_PRE_ACTIV,
    Q628_ACTIV,
    Q628_PRE_RUNNING,
    Q628_RUNNING,
    Q628_RUNNING_HV_OFF,
    Q628_SERVICE1,
    Q628_SERVICE2,
    Q628_SERVICE3,
    Q628_SERVICE4,
} Q528_STATES;

enum {
    SamplePassiv = 0x40,
    SampleActiv = 0x10,
};

typedef union
{   
    struct
    {
    uint8_t SafetySwitchx:1;
    uint8_t Sampling:1;
    uint8_t Oben:1;
    uint8_t Unten:1;
    uint8_t Open:1;
    uint8_t :1;
    uint8_t Close:1;
    uint8_t RunHVOff:1;
    };
    struct
    {
    uint8_t status;
    };
} bitsStatus2;


typedef enum
{
    LED_OFF,
    LED_ON,
    LED_BLINK1,
    LED_BLINK2,
    LED_BLINK3,
} LED_STATES;

typedef enum
{
    Undef,
    Open,
    Close,
} SLOT_STATE;

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
    /* The application's current state */
    APP_STATES state;
    TCP_SOCKET socket;

} APP_DATA;

typedef struct
{
    Q528_STATES state;
} Q528_DATA;

typedef struct
{
    LED_STATES LedStat;
} LED_Struc;

typedef struct
{
    uint32_t    Size;       //0
    uint32_t    RunTime;    //4
    int32_t     Temperatur; //8
    int32_t     Feuchte;    //12
    int32_t     HV_AC;      //16
    int32_t     HV_DC;      //20
    uint32_t    Profile1;   //24
    uint32_t    Profile2;   //28
    uint32_t    WaitAC_DC;  //32
    uint32_t    Serialnumber; //36
    uint8_t     Status1;    //40
    uint8_t     Status2;    //41
    uint8_t     DataCount;  //42 Anzahl der Datenwerte
    uint8_t     Dummy1;     //43
    uint32_t    StartMeasureIndex;  //44
    uint8_t     VerMinor;  //48 
    uint8_t     VerMajor;  //49 
    uint8_t     MoveNext;  //50 
    uint16_t    MoveWait;  //51 
    uint8_t     Q528_State;  //53 
    uint8_t     NotUsed5;  //54 
    int32_t     Data[50];   //55
} TCP_Package;
typedef struct
{
    uint32_t    Size;
    uint32_t    Cmd;
    uint32_t    RunTime;    //4
    uint32_t    HV_AC;      //16
    uint32_t    HV_DC;      //20
    uint32_t    Profile1;   //24
    uint32_t    Profile2;   //28
    uint32_t    WaitAC_DC;  //32
} TCP_CMD_Struc;
void APP_Initialize ( void );
void APP_Tasks ( void );
void DBGToggle(void);
bool SlotOpened(void);
bool SlotClosed(void);
void Motor_GetStatus();
void Motor_Aktiv();
void Motor_Passiv();
void Motor_Home();
bool SampleIsPassiv(void);

#endif /* _APP_H */
/*******************************************************************************
 End of File
 */

