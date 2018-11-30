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
#include "common.h"
#include "stdlib.h"

uint8  measure = ENABLED;
uint16 AnalogMeasure = 0u;
uint8  analog_data[SENSOR_ANALOG_DATA_SIZE] = {0u, 0u};
static CYBLE_GATT_DB_ATTR_HANDLE_T cyBle_htssReqHandle;
CYBLE_CALLBACK_T CyBle_MeasureCallBack;

/*******************************************************************************
* Function Name: HtsCallBack()
********************************************************************************
*
* Summary:
*   This is an event callback function to receive service specific events from 
*   Health Thermometer Service.
*
* Parameters:
*  event - the event code
*  *eventParam - the event parameters
*
* Return:
*  None.
*
*******************************************************************************/
void SensorCallBack(uint32 event, void *eventParam)
{
    uint8 locCharIndex;
    locCharIndex = ((CYBLE_SENSOR_CHAR_VALUE_T *)eventParam)->charIndex;
    DBG_PRINTF("HTS event: %lx, ", event);

    switch(event)
    {
        case CYBLE_EVT_HTSS_NOTIFICATION_ENABLED:
            break;
        case CYBLE_EVT_HTSS_NOTIFICATION_DISABLED:
            break;
        case CYBLE_EVT_HTSS_INDICATION_ENABLED:
            DBG_PRINTF("CYBLE_EVT_HTSS_INDICATION_ENABLED: char: %x\r\n", locCharIndex);
            if(locCharIndex == CYBLE_SENSOR_ANALOG)
            {
                DataMeasureControl = ENABLED;
                measure = ENABLED;
                analog_data[0] = 0u;
                analog_data[1] = 0u;
            }
            break;
        case CYBLE_EVT_HTSS_INDICATION_DISABLED:
            DBG_PRINTF("CYBLE_EVT_HTSS_INDICATION_DISABLED: char: %x\r\n", locCharIndex);
            if(locCharIndex == CYBLE_SENSOR_ANALOG)
            {
                DataMeasureControl = DISABLED;
            }
            break;
        case CYBLE_EVT_HTSS_INDICATION_CONFIRMED:
            DBG_PRINTF("CYBLE_EVT_HTSS_INDICATION_CONFIRMED\r\n");
            break;
        case CYBLE_EVT_HTSS_CHAR_WRITE:
            DBG_PRINTF("CYBLE_EVT_HTSS_CHAR_WRITE: %x ", locCharIndex);
            ShowValue(((CYBLE_SENSOR_CHAR_VALUE_T *)eventParam)->value);
            break;
        case CYBLE_EVT_HTSC_NOTIFICATION:
            break;
        case CYBLE_EVT_HTSC_INDICATION:
            break;
        case CYBLE_EVT_HTSC_READ_CHAR_RESPONSE:
            break;
        case CYBLE_EVT_HTSC_WRITE_CHAR_RESPONSE:
            break;
        case CYBLE_EVT_HTSC_READ_DESCR_RESPONSE:
            break;
        case CYBLE_EVT_HTSC_WRITE_DESCR_RESPONSE:
            break;
		default:
            DBG_PRINTF("Not supported event\r\n");
			break;
    }
}


/*******************************************************************************
* Function Name: MeasureAnalog()
********************************************************************************
*
* Summary:
*   This function measures the analog signal and send it to the client
*
*******************************************************************************/
void MeasureAnalog(void)
{
    /* Variable Definition */
    CYBLE_API_RESULT_T apiResult;
    
    /* Start the ADC and measure analog siganl */
    uint16 signal = 0x00;
    
    /* Perform a measurement */
    CyDelay(1u);
    ADC_StartConvert();
    ADC_IsEndConversion(ADC_WAIT_FOR_RESULT);
    signal = ADC_GetResult16(ADC_SENSOR_CHANNEL);
    
    /* Copy Analog data to array */
    Set16ByPtr(analog_data, (uint16)signal);
    
    /* Send Analog Data to Client */
    apiResult = CyBle_SensorSendIndication(cyBle_connHandle, CYBLE_SENSOR_ANALOG,
        sizeof(analog_data), analog_data);
    
    if(apiResult != CYBLE_ERROR_OK)
    {
        DBG_PRINTF("CyBle_HtssSendIndication API Error: %x \r\n", apiResult);
        DataMeasureControl = DISABLED;
    }
    else
    {
        DBG_PRINTF("Measure Signal: %d (0x%x) \r\n", signal, signal);
    }
}


/******************************************************************************
* Function Name: CyBle_AnalogSendNotification
***************************************************************************//**
* 
*  Sends notification of the collected analog signal
* 
*  On enabling notification successfully for a service characteristic, if the GATT
*  server has an updated value to be notified to the GATT Client, it sends out a
*  'Handle Value Notification' which results in CYBLE_EVT_HTSC_NOTIFICATION event
*  at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be 
*               sent to the client's device.
* 
* \return
*  Return value is of type CYBLE_API_RESULT_T.
*   * CYBLE_ERROR_OK - The request handled successfully
*   * CYBLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CYBLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CYBLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CYBLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed. 
*   * CYBLE_ERROR_NTF_DISABLED - Notification is not enabled by the client.
*
******************************************************************************/
CYBLE_API_RESULT_T CyBle_SensorSendNotification(CYBLE_CONN_HANDLE_T connHandle,
     CYBLE_SENSOR_CHAR_INEDX_T charIndex, uint8 attrSize, uint8 *attrValue)
{

    CYBLE_API_RESULT_T apiResult;
    DBG_PRINTF("CyBle_HtssSendNotification()\r\n");
    /* Send Notification if it is enabled and connected */
    if(CYBLE_STATE_CONNECTED != CyBle_GetState())
    {
        apiResult = CYBLE_ERROR_INVALID_STATE;
    }
    else if(!CYBLE_IS_NOTIFICATION_ENABLED(cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[charIndex].customServCharDesc[CYBLE_ANALOG_CCCD]))
    {
        apiResult = CYBLE_ERROR_NTF_DISABLED;
    }
    else
    {
        CYBLE_GATTS_HANDLE_VALUE_NTF_T ntfReqParam;
        DBG_PRINTF("Enter Setting\r\n");
        /* Fill all fields of write request structure ... */
        ntfReqParam.attrHandle = cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[charIndex].customServCharHandle;
        ntfReqParam.value.val = attrValue;
        ntfReqParam.value.len = attrSize;
        
        DBG_PRINTF("Ready to Send\r\n");
        /* Send notification to client using previously filled structure */
        apiResult = CyBle_GattsNotification(connHandle, &ntfReqParam);
        
    }
    return (apiResult);
}

/******************************************************************************
* Function Name: CyBle_SensorSendIndication
***************************************************************************//**
* 
*  Sends indication with a characteristic value of the Health Thermometer 
*  Service, which is a value specified by charIndex, to the Client device.
*
*  On enabling indication successfully, if the GATT server has an updated value to be 
*  indicated to the GATT Client, it sends out a 'Handle Value Indication' which
*  results in CYBLE_EVT_HTSC_INDICATION or CYBLE_EVT_GATTC_HANDLE_VALUE_IND (if 
*  service specific callback function is not registered) event at the GATT Client's end.
*
*  \param connHandle: The connection handle.
*  \param charIndex:  The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               sent to the Client device.
* 
* \return
*  Return value is of type CYBLE_API_RESULT_T.
*   * CYBLE_ERROR_OK - The request handled successfully
*   * CYBLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*   * CYBLE_ERROR_INVALID_OPERATION - This operation is not permitted
*   * CYBLE_ERROR_INVALID_STATE - Connection with the client is not established
*   * CYBLE_ERROR_MEMORY_ALLOCATION_FAILED - Memory allocation failed
*   * CYBLE_ERROR_IND_DISABLED - Indication is not enabled by the client
*
* \events
*  In case of successful execution (return value = CYBLE_ERROR_OK)
*  the next events can appear: \n
*   If the HTS service-specific callback is registered 
*      (with CyBle_HtsRegisterAttrCallback):
*  * CYBLE_EVT_HTSS_INDICATION_CONFIRMED - in case if the indication is
*                                successfully delivered to the peer device.
*  .
*   Otherwise (if the HTS service-specific callback is not registered):
*  * CYBLE_EVT_GATTS_HANDLE_VALUE_CNF - in case if the indication is
*                                successfully delivered to the peer device.
*
******************************************************************************/
CYBLE_API_RESULT_T CyBle_SensorSendIndication(CYBLE_CONN_HANDLE_T connHandle,
     CYBLE_SENSOR_CHAR_INEDX_T charIndex, uint8 attrSize, uint8 *attrValue)
{
    CYBLE_API_RESULT_T apiResult;
    DBG_PRINTF("CyBle_HtssSendIndication()\r\n");
    /* Send Notification if it is enabled and connected */
    if(CYBLE_STATE_CONNECTED != CyBle_GetState())
    {
        apiResult = CYBLE_ERROR_INVALID_STATE;
    }
    else if(!CYBLE_IS_INDICATION_ENABLED(cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[charIndex].customServCharDesc[CYBLE_ANALOG_CCCD]))
    {
        apiResult = CYBLE_ERROR_IND_DISABLED;
    }
    else
    {
        CYBLE_GATTS_HANDLE_VALUE_NTF_T ntfReqParam;
        
        /* Fill all fields of write request structure ... */
        ntfReqParam.attrHandle = cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[charIndex].customServCharHandle;
        ntfReqParam.value.val = attrValue;
        ntfReqParam.value.len = attrSize;
        
        /* Send indication to client using previously filled structure */
        apiResult = CyBle_GattsIndication(connHandle, &ntfReqParam);
        /* Save handle to support service specific value confirmation response from client */
        if(apiResult == CYBLE_ERROR_OK)
        {
            cyBle_htssReqHandle = ntfReqParam.attrHandle;
        }
    }
    return (apiResult);
}

/******************************************************************************
* Function Name: CyBle_HtsRegisterAttrCallback
***************************************************************************//**
* 
*  Registers a callback function for service specific attribute operations.
*  Service specific write requests from peer device will not be handled with
*  unregistered callback function.
* 
*  \param callbackFunc: An application layer event callback function to receive 
*                     events from the BLE Component. The definition of 
*                     CYBLE_CALLBACK_T for HTS Service is:\n
*                     typedef void (* CYBLE_CALLBACK_T) (uint32 eventCode, 
*                                                        void *eventParam)
*                     * eventCode indicates the event that triggered this 
*                       callback (e.g. CYBLE_EVT_HTSS_NOTIFICATION_ENABLED).
*                     * eventParam contains the parameters corresponding to the 
*                       current event. (e.g. pointer to CYBLE_HTS_CHAR_VALUE_T
*                       structure that contains details of the characteristic 
*                       for which notification enabled event was triggered).
* 
******************************************************************************/
void CyBle_SensorRegisterAttrCallback(CYBLE_CALLBACK_T callbackFunc)
{
    CyBle_MeasureCallBack = callbackFunc;
}

/******************************************************************************
* Function Name: CyBle_HtssSetCharacteristicValue
***************************************************************************//**
* 
*  Sets the characteristic value of the service in the local database.
* 
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size (in Bytes) of the characteristic value attribute.
*  \param attrValue: The pointer to the characteristic value data that should be
*               stored in the GATT database.
* 
* \return
*  Return value is of type CYBLE_API_RESULT_T.
*  * CYBLE_ERROR_OK - The request handled successfully
*  * CYBLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
CYBLE_API_RESULT_T CyBle_SensorSetCharacteristicValue(CYBLE_SENSOR_CHAR_INEDX_T charIndex,
    uint8 attrSize, uint8 *attrValue)
{
    DBG_PRINTF("CyBle_HtssSetCharacteristicValue()\r\n");
    CYBLE_API_RESULT_T apiResult = CYBLE_ERROR_OK;
    CYBLE_GATT_HANDLE_VALUE_PAIR_T locHandleValuePair;

    if(charIndex >= CYBLE_SENSOR_CHAR_COUNT)
    {
        apiResult = CYBLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Store data in database */
        locHandleValuePair.attrHandle = cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[charIndex].customServCharHandle;
        locHandleValuePair.value.len = attrSize;
        locHandleValuePair.value.val = attrValue;
        if(CYBLE_GATT_ERR_NONE !=
            CyBle_GattsWriteAttributeValue(&locHandleValuePair, 0u, NULL, CYBLE_GATT_DB_LOCALLY_INITIATED))
        {
            apiResult = CYBLE_ERROR_INVALID_PARAMETER;
        }
    }
    return (apiResult);
}


/******************************************************************************
* Function Name: CyBle_HtssGetCharacteristicValue
***************************************************************************//**
* 
*  Gets the characteristic value of the service, which is a value identified by
*  charIndex.
* 
*  \param charIndex: The index of the service characteristic.
*  \param attrSize: The size of the characteristic value attribute.
*  \param attrValue: The pointer to the location where characteristic value data
*               should be stored.
* 
* \return
*  Return value is of type CYBLE_API_RESULT_T.
*  * CYBLE_ERROR_OK - The request handled successfully
*  * CYBLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
CYBLE_API_RESULT_T CyBle_SensorGetCharacteristicValue(CYBLE_SENSOR_CHAR_INEDX_T charIndex,
    uint8 attrSize, uint8 *attrValue)
{
    DBG_PRINTF("CyBle_HtssGetCharacteristicValue()\r\n");
    CYBLE_API_RESULT_T apiResult = CYBLE_ERROR_OK;
    CYBLE_GATT_HANDLE_VALUE_PAIR_T locHandleValuePair;

    if(charIndex >=  CYBLE_SENSOR_CHAR_COUNT)
    {
        apiResult = CYBLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        /* Read characteristic value from database */
        locHandleValuePair.attrHandle = cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[charIndex].customServCharHandle;
        locHandleValuePair.value.len = attrSize;
        locHandleValuePair.value.val = attrValue;
        if(CYBLE_GATT_ERR_NONE !=
            CyBle_GattsReadAttributeValue(&locHandleValuePair, NULL, CYBLE_GATT_DB_LOCALLY_INITIATED))
        {
            apiResult = CYBLE_ERROR_INVALID_PARAMETER;
        }
    }
    return (apiResult);
}


/******************************************************************************
* Function Name: CyBle_HtssSetCharacteristicDescriptor
***************************************************************************//**
* 
*  Sets the characteristic descriptor of the specified characteristic.
* 
*  \param charIndex:  The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize:   The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the descriptor value data that should 
*              be stored in the GATT database. 
* 
* \return
*  Return value is of type CYBLE_API_RESULT_T.
*  * CYBLE_ERROR_OK - The request handled successfully
*  * CYBLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
CYBLE_API_RESULT_T CyBle_SensorSetCharacteristicDescriptor(CYBLE_SENSOR_CHAR_INEDX_T charIndex,
   CYBLE_SENSOR_DESCR_INDEX_T descrIndex, uint8 attrSize, uint8 *attrValue)
{
    DBG_PRINTF("CyBle_HtssSetCharacteristicDescriptor()\r\n");
    CYBLE_API_RESULT_T apiResult = CYBLE_ERROR_OK;
    CYBLE_GATT_HANDLE_VALUE_PAIR_T locHandleValuePair;

    if((charIndex >= CYBLE_SENSOR_CHAR_COUNT) || (descrIndex >= CYBLE_ANALOG_SESCR_COUNT))
    {
        apiResult = CYBLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        locHandleValuePair.attrHandle = cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[charIndex].customServCharHandle;
        if(locHandleValuePair.attrHandle != CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            locHandleValuePair.value.len = attrSize;
            locHandleValuePair.value.val = attrValue;
            if(CYBLE_GATT_ERR_NONE !=
                CyBle_GattsWriteAttributeValue(&locHandleValuePair, 0u, NULL, CYBLE_GATT_DB_LOCALLY_INITIATED))
            {
                apiResult = CYBLE_ERROR_INVALID_PARAMETER;
            }
        }
        else
        {
            apiResult = CYBLE_ERROR_INVALID_PARAMETER;
        }
    }
    return (apiResult);
}


/******************************************************************************
* Function Name: CyBle_HtssGetCharacteristicDescriptor
***************************************************************************//**
* 
*  Gets the characteristic descriptor of the specified characteristic.
* 
*  \param charIndex: The index of the service characteristic.
*  \param descrIndex: The index of the service characteristic descriptor.
*  \param attrSize: The size of the characteristic descriptor attribute.
*  \param attrValue: The pointer to the location where characteristic descriptor
*               value data should be stored.
* 
* \return
*  Return value is of type CYBLE_API_RESULT_T.
*  * CYBLE_ERROR_OK - The request handled successfully
*  * CYBLE_ERROR_INVALID_PARAMETER - Validation of the input parameter failed
*
******************************************************************************/
CYBLE_API_RESULT_T CyBle_SensorGetCharacteristicDescriptor(CYBLE_SENSOR_CHAR_INEDX_T charIndex,
    CYBLE_SENSOR_DESCR_INDEX_T descrIndex, uint8 attrSize, uint8 *attrValue)
{
    CYBLE_API_RESULT_T apiResult = CYBLE_ERROR_OK;
    CYBLE_GATT_HANDLE_VALUE_PAIR_T locHandleValuePair;
    DBG_PRINTF("CyBle_HtssGetCharacteristicDescriptor\r\n");
    if((charIndex >= CYBLE_SENSOR_CHAR_COUNT) || (descrIndex >= CYBLE_ANALOG_SESCR_COUNT))
    {
        apiResult = CYBLE_ERROR_INVALID_PARAMETER;
    }
    else
    {
        locHandleValuePair.attrHandle = cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[charIndex].customServCharHandle;
        if(locHandleValuePair.attrHandle != CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE)
        {
            locHandleValuePair.value.len = attrSize;
            locHandleValuePair.value.val = attrValue;
            if(CYBLE_GATT_ERR_NONE !=
                CyBle_GattsReadAttributeValue(&locHandleValuePair, NULL, CYBLE_GATT_DB_LOCALLY_INITIATED))
            {
                apiResult = CYBLE_ERROR_INVALID_PARAMETER;
            }
        }
        else
        {
            apiResult = CYBLE_ERROR_INVALID_PARAMETER;
        }
    }
    return (apiResult);
}


/******************************************************************************
* Function Name: CyBle_HtssWriteEventHandler
***************************************************************************//**
* 
*  Handles Write Request Event for HTS service.
* 
*  \param void *eventParam: The pointer to the data structure specified by the event.
* 
******************************************************************************/
CYBLE_GATT_ERR_CODE_T CyBle_SensorWriteEventHandler(CYBLE_GATTS_WRITE_REQ_PARAM_T *eventParam)
{
    CYBLE_SENSOR_CHAR_INEDX_T locCharIndex;
    CYBLE_SENSOR_CHAR_VALUE_T locCharValue;
    CYBLE_GATT_ERR_CODE_T gattErr = CYBLE_GATT_ERR_NONE;
    uint8 locReqHandle = 0u;
    DBG_PRINTF("CyBle_HtssWriteEventHandler()\r\n");
    if(NULL != CyBle_MeasureCallBack)
    {
        for(locCharIndex = CYBLE_SENSOR_ANALOG; (locCharIndex < CYBLE_SENSOR_CHAR_COUNT) && (locReqHandle == 0u); 
            locCharIndex++)
        {
            
            if((eventParam->handleValPair.attrHandle == cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[locCharIndex].customServCharDesc[CYBLE_ANALOG_CCCD]) ||
               (eventParam->handleValPair.attrHandle == cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[locCharIndex].customServCharHandle))
            {
                locCharValue.connHandle = eventParam->connHandle;
                locCharValue.charIndex = locCharIndex;
                locCharValue.value = &eventParam->handleValPair.value;
                /* Characteristic value write request */
                
                if(eventParam->handleValPair.attrHandle == cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[locCharIndex].customServCharHandle)
                {
                    if(gattErr == CYBLE_GATT_ERR_NONE)
                    {   /* Store value to database */
                        gattErr = CyBle_GattsWriteAttributeValue(&eventParam->handleValPair, 0u, 
                                    &eventParam->connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                        if(gattErr == CYBLE_GATT_ERR_NONE)
                        {
                            CyBle_MeasureCallBack((uint32)CYBLE_EVT_HTSS_CHAR_WRITE, &locCharValue);
                        }
                    }    
                }
                else /* Client Characteristic Configuration descriptor write request */
                {
                    /* Store value to database */
                    gattErr = CyBle_GattsWriteAttributeValue(&eventParam->handleValPair, 0u, 
                                    &eventParam->connHandle, CYBLE_GATT_DB_PEER_INITIATED);
                    if(gattErr == CYBLE_GATT_ERR_NONE)
                    {
                        /* Check characteristic properties for Notification */
                        if(CYBLE_IS_NOTIFICATION_SUPPORTED(cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[locCharIndex].customServCharHandle))
                        {
                            if(CYBLE_IS_NOTIFICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                CyBle_MeasureCallBack((uint32)CYBLE_EVT_HTSS_NOTIFICATION_ENABLED, &locCharValue);
                            }
                            else
                            {
                                CyBle_MeasureCallBack((uint32)CYBLE_EVT_HTSS_NOTIFICATION_DISABLED, &locCharValue);
                            }
                        }
                        /* Check characteristic properties for Indication */
                        if(CYBLE_IS_INDICATION_SUPPORTED(cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[locCharIndex].customServCharHandle))
                        {
                            if(CYBLE_IS_INDICATION_ENABLED_IN_PTR(eventParam->handleValPair.value.val))
                            {
                                CyBle_MeasureCallBack((uint32)CYBLE_EVT_HTSS_INDICATION_ENABLED, &locCharValue);
                            }
                            else
                            {
                                CyBle_MeasureCallBack((uint32)CYBLE_EVT_HTSS_INDICATION_DISABLED, &locCharValue);
                            }
                        }
                    #if((CYBLE_GAP_ROLE_PERIPHERAL || CYBLE_GAP_ROLE_CENTRAL) && \
                        (CYBLE_BONDING_REQUIREMENT == CYBLE_BONDING_YES))
                        /* Set flag to store bonding data to flash */
                        if(cyBle_peerBonding == CYBLE_GAP_BONDING)
                        {
                            cyBle_pendingFlashWrite |= CYBLE_PENDING_CCCD_FLASH_WRITE_BIT;
                        }
                    #endif /* (CYBLE_BONDING_REQUIREMENT == CYBLE_BONDING_YES) */
                        
                    }
                }
                cyBle_eventHandlerFlag &= (uint8)~CYBLE_CALLBACK;
                locReqHandle = 1u;
            }
        }
    }
    return (gattErr);
}


/******************************************************************************
* Function Name: CyBle_HtssConfirmationEventHandler
***************************************************************************//**
* 
*  Handles a Value Confirmation request event from the BLE stack.
* 
*  *eventParam - The pointer to a structure of type CYBLE_CONN_HANDLE_T.
* 
******************************************************************************/
void CyBle_SensorConfirmationEventHandler(const CYBLE_CONN_HANDLE_T *eventParam)
{
    CYBLE_SENSOR_CHAR_INEDX_T locCharIndex;
    CYBLE_SENSOR_CHAR_VALUE_T locCharValue;
    uint8 locReqHandle = 0u;
    DBG_PRINTF("CyBle_HtssConfirmationEventHandler()\r\n");
    if((NULL != CyBle_MeasureCallBack) && (CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE != cyBle_htssReqHandle))
    {
        for(locCharIndex = CYBLE_SENSOR_ANALOG; (locCharIndex < CYBLE_SENSOR_CHAR_COUNT) && (locReqHandle == 0u); 
            locCharIndex++)
        {
            if(cyBle_htssReqHandle == cyBle_customs[CYBLE_SERVICE_SENSOR].customServInfo[locCharIndex].customServCharHandle)
            {
                locCharValue.connHandle = *eventParam;
                locCharValue.charIndex = locCharIndex;
                locCharValue.value = NULL;
                cyBle_eventHandlerFlag &= (uint8)~CYBLE_CALLBACK;
                cyBle_htssReqHandle = CYBLE_GATT_INVALID_ATTR_HANDLE_VALUE;
                CyBle_MeasureCallBack((uint32)CYBLE_EVT_HTSS_INDICATION_CONFIRMED, &locCharValue);
                locReqHandle = 1u;
            }
        }
    }
}

/* [] END OF FILE */
