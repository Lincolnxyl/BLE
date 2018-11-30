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

short DataCollectControl = 0u;
short DataMeasureControl = DISABLED;

int main (void)
{
    /* Initialize Interrupts */
    CyGlobalIntEnable;
    
#if (DEBUG_UART_ENABLED == ENABLED)
    UART_DEB_Start();
#endif
    
    /* Print Start Message*/
    DBG_PRINTF("\r\n\r\nProgram Started! \r\n");

    /* Start CYBLE component and register generic event handler */
    CyBle_Start(AppCallBack);
    /* Register service specific callback functions */
    CyBle_SensorRegisterAttrCallback(SensorCallBack);
    
    ADC_Start();
    WDT_Start();
    
    int testcount = 0;
    while(1)
    {
        /* Place your application code here */
        CyBle_ProcessEvents();
        
        /* To achieve low power in the device */
        LowPowerImplementation();
        
        /***********************************************************************
        * Wait for connection established with Central device
        ***********************************************************************/
        if (CyBle_GetState() == CYBLE_STATE_CONNECTED){
            
            /*
            testcount++;
            if (testcount >= 20){
                DBG_PRINTF("Enter");
                testcount = 0;
            }*/
            //DBG_PRINTF("DataCollectControl = %d, DataMeasureControl = %d\r\n", DataCollectControl,
            //            DataMeasureControl);
            if ((DataCollectControl != 0) && (DataMeasureControl == ENABLED)){
                DBG_PRINTF("\r\n=========================================\r\n");
                DBG_PRINTF("Simulation: Measuring Analog Signal\r\n");
                MeasureAnalog();
                CyBle_ProcessEvents();
                DataCollectControl = 0;
            }
        }
    }
}











