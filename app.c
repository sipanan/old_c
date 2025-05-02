
// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "peripheral/ports/plib_ports.h"
#include "app.h"
#include "ads1210.h"
#include "ULN.h"
#include "DA.h"
#include "HV_Manegement.h"
#include "Move.h"
#include "RingBuffer.h"
// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
*/
#define SERVER_PORT 9760
#define AC true
#define DC false
#define SYS_CONSOLE_OFF

void HYT_StartMR();
void HYT_Getdata();
void I2C_Master_Init();
bool SafetySwitch(void);
bool StartTaste(void);
bool Switch_3(void);
bool ADS1210_Initialized = false;
SLOT_STATE SLOT=Undef;
LED_Struc LED;
APP_DATA appData;                                                                                                                                                                                                                                                                                                                                                                                                                                                                 
Q528_DATA Q628Data;
uint8_t Safety;
MOVE_DATA MoveData;
BSP_LED_STATE LEDstate = BSP_LED_STATE_ON;
static SYS_TMR_HANDLE blinkLedTMRHandle,AdCallbackHandle;
const uint16_t WaitAd = 5;
//---------------------------------------------------------
const uint32_t SerienNummer = 6280001;
// der Netbiosname muss in system_config/default/sytem_config.h eingetragen werden
// #define TCPIP_NETWORK_DEFAULT_HOST_NAME				"Q6280002"
const uint8_t MajorVersion = 1;
const uint8_t MinorVersion = 0;
//---------------------------------------------------------
uint8_t ADS1210_INCR;
uint8_t ADS1210_CRB0,ADS1210_CRB1,ADS1210_CRB2,ADS1210_CRB3;
uint8_t ADS1210_DOR0,ADS1210_DOR1,ADS1210_DOR2;
uint8_t ADS1210_OCR0,ADS1210_OCR1,ADS1210_OCR2;
uint8_t ADS1210_FCR0,ADS1210_FCR1,ADS1210_FCR2;
uint8_t ULN_OUT;
TCP_Package TCP_Data;
TCP_CMD_Struc TCP_CMD;
int rData;
FIFO FiFoBuffer;
FIFO *QFiFo = &FiFoBuffer;
char Strx[80] = "QUMA";
char *ptrStrx = Strx;
uint8_t InBuf[128] = "QUMAT628";
uint8_t *ptrInBuf = InBuf;
uint8_t I2CRcvByte,HYT_Status,Motor_Status;
int Feuchte,Temperatur;
bool NewData,Getdata = false,HV_AC_Flag = false;
bool SimulateStart = false;
bool TimerRun = false;
bool MoveUp = false;
unsigned char Blink;
uint16_t T=0,T_HV,T_HYT,T_LED,T_App_Wait;
uint8_t Status1;
bitsStatus2 Status2;
int HV_AC = 0, HV_AC_Cnt = 0;
int HV_DC = 0; // Werte in Volt
int WaitACDC;
uint8_t ProfileAC,ProfileDC;
uint32_t RunTime,EndTime,StartMeasureIndex;
uint8_t AD_Cnt;
uint32_t AD_Cntr = 0;
IPV4_ADDR TestAdr;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
// 10 ms
// *****************************************************************************
void AdCallback(void)
{
    if(isFull(QFiFo))
    {
        rData = dequeue(QFiFo);
    }
    ADS1210_ReadDOR();
    asm("nop");
    rData = ADS1210_DOR0 + (ADS1210_DOR1 << 8) + (ADS1210_DOR2 << 16);
    if (ADS1210_DOR2 & 0x00000080) rData = rData | 0xff000000;
    
    enqueue(QFiFo,rData);
    AD_Cntr++;
    AD_Cnt++;
    if ((AD_Cnt & 0x03) == 0x03)
    {
        if (HV_AC_Flag )
        {
            HV_AC_Cnt = 1024;
            if (HV_AC_Cnt>1023) 
                {ULN_HV_AC_ON(true);}
            else
                {ULN_HV_AC_ON(false);}
            HV_AC_Cnt &= 0x3FF;
        }
        else
            {ULN_HV_AC_ON(false);}
    }
}
// *****************************************************************************
// Heartbeat 100 ms
// *****************************************************************************
void Heartbeat(void)
{   
    T++;
    if(T>=T_LED) {
        // Starterkit LED3 
        PLIB_PORTS_PinToggle(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_2);
        T_LED=T+4;
    }
    if(T>=T_HYT) {
        if(Getdata) {
            HYT_Getdata();
            Getdata = false;
        }
        else {
            HYT_StartMR();
            Getdata = true;
        }
        T_HYT=T+2;
     }

        Motor_GetStatus();

   switch (LED.LedStat)
   {
       case LED_BLINK1:
        {
            if((T & 0x000F)==0x0008)
            PLIB_PORTS_PinToggle(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_0);    
            break;
        }   
        case LED_BLINK3:
        {
            if((T & 0x0001)==0x0001)
            PLIB_PORTS_PinToggle(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_0);    
            break;
        }
       case LED_BLINK2:
        {
            if((T & 0x0003)==0x0002)
            PLIB_PORTS_PinToggle(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_0);    
            break;
        }
       case LED_ON:
        {
            PLIB_PORTS_PinSet(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_0);
            break;
        }       
        default:
           break;
   }
   Move_Task(); 
} 
/* TODO:  Add any necessary callback funtions.
*/


// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
void DBGClear(void)
{
     PLIB_PORTS_PinClear(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_1);
}
void DBGToggle(void)
{
     PLIB_PORTS_PinToggle(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_1);
}

bool StartPort(void)
{
    return(PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_5));
}

bool StartTaste(void)
{
    uint8_t i;
    if (SimulateStart)
        { SimulateStart = false; return true; }
    else
    {   
        for(i=0;i<Debounce;i++) 
        {
            if(StartPort()==1) { break; } 
        }
        return (i>(Debounce-1));
    }
}

bool SlotOpen(void)
{
    ULN_OPEN();
    SLOT=Open;
    Status2.Open=true;
    Status2.Close=false;
}

bool SlotClose(void)
{
    ULN_CLOSE();
    SLOT=Close;
    Status2.Open=false;
    Status2.Close=true;
}


void SampleUp(void)
{
    Motor_Home();
}
bool SampleIsActiv(void)
{
    Motor_GetStatus();
    Status2.Unten = (Motor_Status & SampleActiv)==SampleActiv;
    Status2.Oben=~Status2.Unten;
    return Status2.Unten==1;
}

void SampleDown(void)
{
    Motor_Aktiv();
}
bool SampleIsPassiv(void)
{
    Motor_GetStatus();
    Status2.Oben = (Motor_Status & SamplePassiv)==SamplePassiv;
    Status2.Unten=~Status2.Oben;
    return Status2.Oben==1;
}

bool SafetyPort(void)
{
    return(PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_E,PORTS_BIT_POS_6));
}

bool SafetySwitch(void)
{
    uint8_t i;
    for(i=0;i<Debounce;i++) 
    { if(SafetyPort()==0) { break; } }
    return (i>(Debounce-1));
}

bool Switch3Port(void)
{
    return(PLIB_PORTS_PinGet(PORTS_ID_0,PORT_CHANNEL_D,PORTS_BIT_POS_13));
}

bool Switch_3(void)
{
    uint8_t i;
    for(i=0;i<Debounce;i++) 
    { if(Switch3Port()==1) { break; } }
    return (i>(Debounce-1));
}

void StartSampling(void){
    Status2.Sampling=1;
    setEmpty(QFiFo);
    AD_Cntr = 0;
    StartMeasureIndex = 0; 
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */
void APP_Initialize ( void )
{
//    SYS_CONSOLE_MESSAGE(" APP: TCP/IP stack initialization !\r\n");
    /* Place the App state machine in its initial state. */
    // Starttaste
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_5);
    // Sicherheitsschalter
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_E, PORTS_BIT_POS_6);
    // SW3 auf PIC_32_Board
    PLIB_PORTS_PinDirectionInputSet(PORTS_ID_0, PORT_CHANNEL_D, PORTS_BIT_POS_13);
    WaitACDC=5;
    I2C_Master_Init();
    ADS1210_Init();
    ULN_Init();
    DA_Init();
    Move_Init();
    Q628Data.state = Q628_POWER_UP;
    appData.state = APP_TMR_WAIT_INIT;
    setEmpty(QFiFo);
    HV_AC_Flag = false;
    HV_AC=1024;
    //ULN_HV_AC_ON(true);
    //ULN_HV_AC_ON(false);
#if BoardRevision <=4 
    ULN_S4_AC_ON(true);
    ULN_S4_AC_ON(false);
#endif
   // ULN_S2_DC_ON(true);
   //DA_Set(65535);
   //DA_On(true);
   
   //DA_Set(3200);
   // DA_On(true);
    asm("nop");
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks ( void )
{
    SYS_STATUS          tcpipStat;
    const char          *netName, *netBiosName;
    static IPV4_ADDR    dwLastIP[2] = { {-1}, {-1} };
    IPV4_ADDR           ipAddr;
    int                 i,nNets;
    TCPIP_NET_HANDLE    netH;
    uint32_t            trmSize,rcvSize,AdValue;
    uint32_t            InCount = 0;
        
    //SYS_CMD_READY_TO_READ();

    switch(appData.state)
    {
        case APP_TMR_WAIT_INIT:
            netH = TCPIP_STACK_IndexToNet(0);
            if (SYS_STATUS_READY == SYS_TMR_Status(sysObj.sysTmr))
                {
                /* SYS TMR is in running mode */
                appData.state = APP_CONFIG_PERIODIC;
                }
            else
                {
                appData.state = APP_TMR_WAIT_INIT;
                }
            break;
        case APP_CONFIG_PERIODIC:
            T=0;
            blinkLedTMRHandle = SYS_TMR_CallbackPeriodic(100,0,(SYS_TMR_CALLBACK)&Heartbeat);
            AdCallbackHandle = SYS_TMR_CallbackPeriodic(10,0,(SYS_TMR_CALLBACK)&AdCallback);
            TimerRun = true;
            T_App_Wait = T + 25;
            appData.state=APP_TCPIP_WAIT_INIT;
            break;
        case APP_TCPIP_WAIT_INIT:
            tcpipStat = TCPIP_STACK_Status(sysObj.tcpip);
            if(tcpipStat < 0)
            {   // some error occurred
#ifdef SYS_CONSOLE
                SYS_CONSOLE_MESSAGE(" APP: TCP/IP stack initialization failed!\r\n");
#endif                
                appData.state = APP_TCPIP_ERROR;
            }
            else if(tcpipStat == SYS_STATUS_READY)
            {
                // now that the stack is ready we can check the
                // available interfacesdbsd
                nNets = TCPIP_STACK_NumberOfNetworksGet();
                for(i = 0; i < nNets; i++)
                {

                    netH = TCPIP_STACK_IndexToNet(i);
                    netName = TCPIP_STACK_NetNameGet(netH);
                    netBiosName = TCPIP_STACK_NetBIOSName(netH);
                    dwLastIP[i].Val = 0;

#if defined(TCPIP_STACK_USE_NBNS)
#ifdef SYS_CONSOLE
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS enabled\r\n", netName, netBiosName);
#endif
#else
                    SYS_CONSOLE_PRINT("    Interface %s on host %s - NBNS disabled\r\n", netName, netBiosName);
#endif  // defined(TCPIP_STACK_USE_NBNS)

                }
                appData.state = APP_TCPIP_WAIT_FOR_IP;
            }    
            break;

        case APP_TCPIP_WAIT_FOR_IP:
        {
            // if the IP address of an interface has changed
            // display the new value on the system console
            nNets = TCPIP_STACK_NumberOfNetworksGet();
            for (i = 0; i < nNets; i++)
            {
                netH = TCPIP_STACK_IndexToNet(i);
                TestAdr.Val = TCPIP_STACK_NetAddress(netH);
                ipAddr.Val = TestAdr.Val;
                if(dwLastIP[i].Val != ipAddr.Val)
                {
                    dwLastIP[i].Val = ipAddr.Val;
#ifdef SYS_CONSOLE
                    SYS_CONSOLE_MESSAGE(TCPIP_STACK_NetNameGet(netH));
                    SYS_CONSOLE_MESSAGE(" IP Address: ");
                    SYS_CONSOLE_PRINT("%d.%d.%d.%d \r\n", ipAddr.v[0], ipAddr.v[1], ipAddr.v[2], ipAddr.v[3]);
#endif
// }                    
                }
                if (ipAddr.v[0] != 0 && ipAddr.v[0] != 169) // Wait for a Valid IP
                    {
                    if (TCPIP_DHCP_IsServerDetected(netH))
                        {
                        appData.state = APP_TCPIP_OPENING_SERVER;
                        }
                    }
// }                
            }
        }
            break;
        case APP_TCPIP_OPENING_SERVER:
        {
#ifdef SYS_CONSOLE
            SYS_CONSOLE_PRINT("Waiting for Client Connection on port: %d\r\n", SERVER_PORT);
#endif
            appData.socket = TCPIP_TCP_ServerOpen(IP_ADDRESS_TYPE_IPV4, SERVER_PORT, 0);
            if (appData.socket == INVALID_SOCKET)
            {
#ifdef SYS_CONSOLE
                SYS_CONSOLE_MESSAGE("Couldn't open server socket\r\n");
#endif 
                break;
            }
            appData.state = APP_TCPIP_WAIT_FOR_CONNECTION;
        }
        break;

        case APP_TCPIP_WAIT_FOR_CONNECTION:
        {
            if (TCPIP_TCP_IsConnected(appData.socket))
            {
                // We got a connection
                appData.state = APP_TCPIP_SERVING_CONNECTION;
#ifdef SYS_CONSOLE
                SYS_CONSOLE_MESSAGE("Received a connection\r\n");
#endif
            }
            else
            {
                break;
            }
        }
        break;

        case APP_TCPIP_SERVING_CONNECTION:
        {
            if (!TCPIP_TCP_IsConnected(appData.socket))
            {
                appData.state = APP_TCPIP_CLOSING_CONNECTION;
#ifdef SYS_CONSOLE
                SYS_CONSOLE_MESSAGE("Connection was closed\r\n");
#endif
                break;
            }
            
            // Transfer the data out of the TCP RX FIFO and into our local processing buffer.
            InCount=sizeof(TCP_CMD);
            rcvSize = TCPIP_TCP_ArrayGet(appData.socket, (uint8_t*) &TCP_CMD, InCount);
            if (rcvSize>0) {
            switch (TCP_CMD.Cmd)
            {
            case 0x47 : {   // Get machine data
                TCP_Data.Size = 256;
                TCP_Data.RunTime = RunTime;
                TCP_Data.Feuchte = Feuchte;
                TCP_Data.Temperatur = Temperatur;
                TCP_Data.HV_AC = HV_AC;
                TCP_Data.HV_DC = HV_DC;
                TCP_Data.Profile1 = ProfileAC;
                TCP_Data.Profile2 = ProfileDC;
                TCP_Data.WaitAC_DC = WaitACDC;
                TCP_Data.Serialnumber = SerienNummer;
                TCP_Data.Status1 = Status1;
                TCP_Data.Status2 = Status2.status;
                TCP_Data.StartMeasureIndex = StartMeasureIndex;
                TCP_Data.DataCount = 0;
                TCP_Data.VerMinor = MinorVersion;
                TCP_Data.VerMajor = MajorVersion;
                TCP_Data.MoveNext = MoveData.NextState;
                TCP_Data.MoveWait = (uint16_t)T_HV;
                TCP_Data.Q528_State = Q628Data.state;
                for(i=0;i<50;i++)
                {
                    if(isEmpty(QFiFo))
                        break;
                    else        
                    {
                        AdValue = dequeue(QFiFo);
                        TCP_Data.Data[TCP_Data.DataCount] = AdValue;
                        TCP_Data.DataCount++;
                        if (Status2.Sampling)
                        {
                            RunTime++;
                            if (RunTime>EndTime) 
                            {
                                Status2.Sampling=0;
                                break;
                            }
                        }
                    }
                }
                TCP_Data.RunTime = RunTime;
                trmSize = sizeof(TCP_Data);
                TCP_Data.Size = trmSize;
                TCPIP_TCP_ArrayPut(appData.socket, (uint8_t*) &TCP_Data, trmSize);
                }
                break;
            case 0x52 : {   // Reset
                HV_AC_DC_Set(OFF);
                Status2.status &= 0x21;
                Status1 = 0;
                DBGClear();
                Q628Data.state = Q628_STARTUP;
                }
                break;
            case 0x53 : {   // Start enable
                ProfileAC = TCP_CMD.Profile1;
                ProfileDC = TCP_CMD.Profile2;
                HV_AC = 1024; //TCP_CMD.HV_AC;
                HV_DC = TCP_CMD.HV_DC;
                WaitACDC = TCP_CMD.WaitAC_DC;
                EndTime = TCP_CMD.RunTime*100;
                RunTime = 0;
                Status1|=0x01;
                }
                break;
            case 0x54 : {   // Simulate Start
                SimulateStart = true;
                }
                break;
            case 0x44 : {   // Down cmd
                if((Q628Data.state==Q628_IDLE) && (MoveData.State==MOVE_IDLE) && (MoveData.NextState==MOVE_IDLE)){                    
                    Q628Data.state=Q628_SERVICE1;
                    }
                }
                break;
            case 0x55 : {   // Up cmd
                if((Q628Data.state==Q628_IDLE) && (MoveData.State==MOVE_IDLE) && (MoveData.NextState==MOVE_IDLE)){
                    MoveUp=true;
                    Q628Data.state=Q628_SERVICE3;
                    }
                }
                  break;
            default: break;
                }
            }
        }
        break;
        case APP_TCPIP_CLOSING_CONNECTION:
        {
  			// Close the socket connection.
            TCPIP_TCP_Abort(appData.socket,true);
            appData.state = APP_TCPIP_WAIT_INIT;
        }
        break;
        default:
            break;
    } // Switch APP_Data.state
    Status2.SafetySwitchx = SafetySwitch();
    switch (Q628Data.state)
    {   case Q628_POWER_UP :
            {
            HV_AC_DC_Set(false);    
            if(TimerRun) {
                if( T_App_Wait<T ) 
                {
                    Q628Data.state = Q628_STARTUP;
                }
              }
            }
            break;
        case Q628_STARTUP :
            {
                LED.LedStat = LED_BLINK1;
                MoveData.State = MOVE_IDLE;
                MoveData.NextState = MOVE_POWERUP;
                SlotOpen();
                Q628Data.state = Q628_PRE_IDLE;
                T_App_Wait=T+25;
            }
            break;
        case Q628_PRE_IDLE :
            if((MoveData.State==MOVE_IDLE) && (MoveData.NextState==MOVE_IDLE) && (T_App_Wait<T) )
            {
                SampleIsActiv();
                SampleIsPassiv();
                SlotClose();
                T=1;                // �berlauf verhindern
                Q628Data.state = Q628_IDLE;
            }
            break;
        case Q628_IDLE:
            if((Status1 & 0x01) && (MoveData.State==MOVE_IDLE)  && (MoveData.NextState==MOVE_IDLE))
            {   
                LED.LedStat = LED_ON;
                Q628Data.state = Q628_WAIT_START_SW;
            }
            if((LED.LedStat==LED_BLINK1) && (StartTaste()) && (Status2.Unten==1)) {
                MoveUp=true;
                Q628Data.state = Q628_SERVICE3;
            }
            if((Switch_3()) && (MoveData.State==MOVE_IDLE) && (MoveData.NextState==MOVE_IDLE))
            {
                if(SampleIsPassiv()) Q628Data.state = Q628_SERVICE1;
                if(SampleIsActiv()) Q628Data.state = Q628_SERVICE3;
            }
            break;
        case Q628_SERVICE1:
                {
                MoveData.Profile=5; // 2 Sekunden Aktiv
                MoveData.NextState=MOVE_TO_AKTIV;
                Move_Get_Wait();
                T_HV = T + MoveData.Wait;
                Q628Data.state = Q628_SERVICE2;    
                }
            break;
        case Q628_SERVICE2:
            if ((MoveData.State==MOVE_IDLE)&(MoveData.NextState==MOVE_IDLE)){
                Status2.Oben = 0;
                Status2.Unten = 1;
                Q628Data.state = Q628_IDLE;
            }
            break;
        case Q628_SERVICE3:    
               {
                SlotOpen();   
                MoveData.Profile=5; // 2 Sekunden
                MoveData.NextState=MOVE_TO_PASSIV;
                Move_Get_Wait();
                T_HV = T + MoveData.Wait;
                Q628Data.state = Q628_SERVICE4;
                }
            break;
         case Q628_SERVICE4:
            {
            if ((MoveData.State==MOVE_IDLE)&&(MoveData.NextState==MOVE_IDLE)){
                Status2.Unten = 0;
                Status2.Oben = 1;
                if (MoveUp) { SlotClose(); MoveUp=false;}
                Q628Data.state=Q628_IDLE;
                }
            }
            break;

         case Q628_WAIT_START_SW: {
             if(StartTaste()) {
               HV_AC_Flag = true;
               HV_AC_DC_Set(ON);
               SlotOpen();
//               StartSampling();
               T_HV = T + 80;
               Q628Data.state = Q628_WAIT_HV;
               }
            }
            break;
/*            case Q628_WAIT_START_SW: {
             if(StartTaste()) {
                MoveData.Profile=3;
                MoveData.NextState=MOVE_TO_AKTIV;
                Move_Task();
                Status2.Unten=1;
                Status2.Oben=0;
                T_HV=T+25;
                Q628Data.state = Q628_WAIT_UNTEN;
               }
            }
            break;*/
            case Q628_WAIT_UNTEN: {
                if((MoveData.State==MOVE_IDLE) && (MoveData.NextState==MOVE_IDLE)) {
                    T_HV = T + 80;
                    Q628Data.state=Q628_WAIT_HV;
                }
            }
            break;
        case Q628_WAIT_HV : {
             if(T_HV<=T)  {
                StartSampling();
                MoveData.Profile=5;
                MoveData.NextState=MOVE_TO_AKTIV;
                Move_Task();
                Status2.Unten=1;
                Status2.Oben=0;
//                T_HV=T+25;
                T_HV=T+80;
                Q628Data.state = Q628_START;
                }
            }
            break;
        case Q628_START: {
            if((SLOT==Open) && (Status1==0x01) && (MoveData.State==MOVE_IDLE) && (MoveData.NextState==MOVE_IDLE))
                {
                T_App_Wait=T+5;
                Q628Data.state=Q628_PRE_ACTIV;
                }
            else if(T_HV <= T)
                {
                Q628Data.state = Q628_STARTUP;
                }
            }
            break;
        case Q628_PRE_ACTIV:
            if(T_App_Wait <= T) 
            {
                HV_AC_DC_Set(OFF);
                LED.LedStat = LED_BLINK3;
                Status2.RunHVOff=1;
                Q628Data.state=Q628_ACTIV;
            }
            break;
        case Q628_ACTIV: {
            if (!(Status2.Sampling)) {
                Status2.Sampling=0;
                Status1 = 0;
                LED.LedStat = LED_BLINK1;
                Q628Data.state=Q628_IDLE;
                }
            }
            break;    
        default:
            break;
    }
    
}


/*******************************************************************************
 End of File
 */

