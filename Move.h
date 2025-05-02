/* ************************************************************************** */
/** Descriptive File Name

  @Company
    QUMA Elektronik & Analytik GmbH

  @File Name
    DA.h

  @Summary
    Header file for DA converter

 */
/* ************************************************************************** */

#ifndef MOVE_H   /* Guard against multiple inclusion */
#define MOVE_H

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************
 typedef enum
{   
    MOVE_POWERUP, 
    MOVE_STARTUP, 
    MOVE_TO_AKTIV,
    MOVE_TO_PASSIV,
    MOVE_MOVE,
    MOVE_END,
    MOVE_IDLE,
    MOVE_HOME,
    MOVE_HOME_WAIT,        
    MOVE_WAIT_ON,
    MOVE_ERROR        
} MOVE_STATES;
typedef struct
{
    MOVE_STATES State;
    MOVE_STATES NextState;
    uint8_t Profile;
    uint8_t MoveState;
    uint8_t Wait;
    uint8_t ErrCnt;
} MOVE_DATA;

extern MOVE_DATA MoveData;

    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
void Move_Init(void);
bool Move_To(uint8_t ZielNummer);
void Move_Task(void);
void Move_Get_Wait(void);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
