/*******************************************************************************
* File Name: common.h
*
* Version 1.0
*
* Description:
*  Contains the function prototypes and constants available to the example
*  project.
*
********************************************************************************
* Copyright 2016, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include <project.h>
#include <stdio.h>
#include "Sensor.h"
#include "utils.h"
#include "ISR.h"

#define ENABLED                     (1u)
#define DISABLED                    (0u)

/***************************************
* Conditional Compilation Parameters
***************************************/
#define DEBUG_UART_ENABLED          DISABLED


/***************************************
*           API Constants
***************************************/
#define LED_ON                      (0u)
#define LED_OFF                     (1u)
#define LED_TIMEOUT                 (1000u/300u)    /* Сounts depend on advertising interval parameter */

#define ADC_SENSOR_CHANNEL          (0u)

#define WDT_COUNTER                 (CY_SYS_WDT_COUNTER1)
#define WDT_COUNTER_MASK            (CY_SYS_WDT_COUNTER1_MASK)
#define WDT_INTERRUPT_SOURCE        (CY_SYS_WDT_COUNTER1_INT) 
#define WDT_COUNTER_ENABLE          (1u)
#define WDT_1SEC                    (32767u)

/***************************************
*           System Parameters
***************************************/
#define HFCLK_FREEQUENCY_KHZ        48000

/***************************************
*           Function Parameters
***************************************/
#define TIMER_PERIOD_MS             500
#define TIMER_FREQUENCY_KHZ         10
#define TIMER_COMPARE               50


/***************************************
*        External Function Prototypes
***************************************/
void ShowValue(CYBLE_GATT_VALUE_T *value);
void Set16ByPtr(uint8 ptr[], uint32 value);


/***************************************
*        Macros
***************************************/
#if (DEBUG_UART_ENABLED == ENABLED)
    #define DBG_PRINTF(...)          (printf(__VA_ARGS__))
#else
    #define DBG_PRINTF(...)
#endif /* (DEBUG_UART_ENABLED == ENABLED) */


/***************************************
* External data references
***************************************/
extern CYBLE_CONN_HANDLE_T connHandle;
extern volatile uint32 mainTimer;

/***************************************
* Global Variables
***************************************/
extern short DataCollectControl;
extern short DataMeasureControl;

/* [] END OF FILE */
