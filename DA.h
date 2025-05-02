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

#ifndef DA_h   /* Guard against multiple inclusion */
#define DA_h

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#define DA_ON 1
#define DA_OFF 0
    
extern void DA_Init(void);
extern void DA_Set(uint16_t Value);
extern void DA_On(bool On);
void DA_CS(bool Set);
void DA_CLK(bool Set);
void DA_DAT(bool Set);    

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************



    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
