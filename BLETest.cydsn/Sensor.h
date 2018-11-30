/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/

#ifndef _SENSOR_H_
    #define _SENSOR_H_
#endif
    
#include <project.h>

/***************************************
* Data Struct Definition
* The following Macro definitions can also be found in BLE_custom.h,
* I rewrite them to make it eaiser to understand. 
* (refer to BLE_Temperature_Measurement01 Project)
***************************************/

/* Custom service index */
typedef enum{
    CYBLE_SERVICE_SENSOR = 0u,        /* Index of Sensor service in the cyBle_customs array */
    CYBLE_SERVICE_COUNT          /* Total count of custom services */
} CYBLE_CUSTOM_SERVICE_INDEX_T;

/** Sensor Characteristic indexes */
typedef enum{
    CYBLE_SENSOR_ANALOG = 0u,        /* Index of Analog characteristic */
    CYBLE_SENSOR_CHAR_COUNT     /*  Total count of characteristics */
} CYBLE_SENSOR_CHAR_INEDX_T;

/* Analog Descriptors indexes */
typedef enum{
    //CYBLE_ANALOG_SCCD,          /* Index of Server Characteristic Configuration descriptor */
    CYBLE_ANALOG_CCCD,          /* Index of Client Characteristic Configuration descriptor */
    CYBLE_ANALOG_SESCR_COUNT    /* Total count of descriptors */
} CYBLE_SENSOR_DESCR_INDEX_T;

/** HTS Characteristic value parameter structure */
typedef struct
{
    CYBLE_CONN_HANDLE_T connHandle;        /* Peer device handle */
    CYBLE_SENSOR_CHAR_INEDX_T charIndex;   /* Index of service characteristic */
    CYBLE_GATT_VALUE_T *value;             /* Characteristic value */
} CYBLE_SENSOR_CHAR_VALUE_T;


/***************************************
*          Constants
***************************************/
#define SENSOR_ANALOG_DATA_SIZE         (2u)    /* Size of the data from ADC */

/***************************************
*       Function Prototypes
***************************************/
void SensorCallBack(uint32 event, void *eventParam);
void MeasureAnalog(void);
CYBLE_API_RESULT_T CyBle_SensorSendNotification(CYBLE_CONN_HANDLE_T connHandle,
     CYBLE_SENSOR_CHAR_INEDX_T charIndex, uint8 attrSize, uint8 *attrValue);
CYBLE_API_RESULT_T CyBle_SensorSendIndication(CYBLE_CONN_HANDLE_T connHandle,
     CYBLE_SENSOR_CHAR_INEDX_T charIndex, uint8 attrSize, uint8 *attrValue);
void CyBle_SensorRegisterAttrCallback(CYBLE_CALLBACK_T callbackFunc);
CYBLE_API_RESULT_T CyBle_SensorSetCharacteristicValue(CYBLE_SENSOR_CHAR_INEDX_T charIndex,
    uint8 attrSize, uint8 *attrValue);
CYBLE_API_RESULT_T CyBle_SensorGetCharacteristicValue(CYBLE_SENSOR_CHAR_INEDX_T charIndex,
    uint8 attrSize, uint8 *attrValue);
CYBLE_API_RESULT_T CyBle_SensorSetCharacteristicDescriptor(CYBLE_SENSOR_CHAR_INEDX_T charIndex,
   CYBLE_SENSOR_DESCR_INDEX_T descrIndex, uint8 attrSize, uint8 *attrValue);
CYBLE_API_RESULT_T CyBle_SensorGetCharacteristicDescriptor(CYBLE_SENSOR_CHAR_INEDX_T charIndex,
    CYBLE_SENSOR_DESCR_INDEX_T descrIndex, uint8 attrSize, uint8 *attrValue);
CYBLE_GATT_ERR_CODE_T CyBle_SensorWriteEventHandler(CYBLE_GATTS_WRITE_REQ_PARAM_T *eventParam);
void CyBle_SensorConfirmationEventHandler(const CYBLE_CONN_HANDLE_T *eventParam);
void SensorCallBack(uint32 event, void *eventParam);


/***************************************
* External data references
***************************************/


/* [] END OF FILE */