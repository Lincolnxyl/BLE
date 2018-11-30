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

CY_ISR(WDT_ISR)
{
    DataCollectControl++;
    CySysWdtClearInterrupt(WDT_INTERRUPT_SOURCE);
}

CY_ISR(WakeUp_ISR)
{
}

/* [] END OF FILE */
