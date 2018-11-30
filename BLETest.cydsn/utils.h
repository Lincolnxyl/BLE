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

#ifndef _UTILS_H_
    #define _UTILS_H_
#endif
    
#include <project.h>

void LowPowerImplementation(void);
void AppCallBack(uint32 event, void* eventParam);
void TimerInit();
void WDT_Start(void);
void WDT_Stop(void);

/* [] END OF FILE */
