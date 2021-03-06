/*******************************************************************************
* File Name: Sig_Input.c  
* Version 2.20
*
* Description:
*  This file contains APIs to set up the Pins component for low power modes.
*
* Note:
*
********************************************************************************
* Copyright 2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "cytypes.h"
#include "Sig_Input.h"

static Sig_Input_BACKUP_STRUCT  Sig_Input_backup = {0u, 0u, 0u};


/*******************************************************************************
* Function Name: Sig_Input_Sleep
****************************************************************************//**
*
* \brief Stores the pin configuration and prepares the pin for entering chip 
*  deep-sleep/hibernate modes. This function applies only to SIO and USBIO pins.
*  It should not be called for GPIO or GPIO_OVT pins.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None 
*  
* \sideeffect
*  For SIO pins, this function configures the pin input threshold to CMOS and
*  drive level to Vddio. This is needed for SIO pins when in device 
*  deep-sleep/hibernate modes.
*
* \funcusage
*  \snippet Sig_Input_SUT.c usage_Sig_Input_Sleep_Wakeup
*******************************************************************************/
void Sig_Input_Sleep(void)
{
    #if defined(Sig_Input__PC)
        Sig_Input_backup.pcState = Sig_Input_PC;
    #else
        #if (CY_PSOC4_4200L)
            /* Save the regulator state and put the PHY into suspend mode */
            Sig_Input_backup.usbState = Sig_Input_CR1_REG;
            Sig_Input_USB_POWER_REG |= Sig_Input_USBIO_ENTER_SLEEP;
            Sig_Input_CR1_REG &= Sig_Input_USBIO_CR1_OFF;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Sig_Input__SIO)
        Sig_Input_backup.sioState = Sig_Input_SIO_REG;
        /* SIO requires unregulated output buffer and single ended input buffer */
        Sig_Input_SIO_REG &= (uint32)(~Sig_Input_SIO_LPM_MASK);
    #endif  
}


/*******************************************************************************
* Function Name: Sig_Input_Wakeup
****************************************************************************//**
*
* \brief Restores the pin configuration that was saved during Pin_Sleep(). This 
* function applies only to SIO and USBIO pins. It should not be called for
* GPIO or GPIO_OVT pins.
*
* For USBIO pins, the wakeup is only triggered for falling edge interrupts.
*
* <b>Note</b> This function is available in PSoC 4 only.
*
* \return 
*  None
*  
* \funcusage
*  Refer to Sig_Input_Sleep() for an example usage.
*******************************************************************************/
void Sig_Input_Wakeup(void)
{
    #if defined(Sig_Input__PC)
        Sig_Input_PC = Sig_Input_backup.pcState;
    #else
        #if (CY_PSOC4_4200L)
            /* Restore the regulator state and come out of suspend mode */
            Sig_Input_USB_POWER_REG &= Sig_Input_USBIO_EXIT_SLEEP_PH1;
            Sig_Input_CR1_REG = Sig_Input_backup.usbState;
            Sig_Input_USB_POWER_REG &= Sig_Input_USBIO_EXIT_SLEEP_PH2;
        #endif
    #endif
    #if defined(CYIPBLOCK_m0s8ioss_VERSION) && defined(Sig_Input__SIO)
        Sig_Input_SIO_REG = Sig_Input_backup.sioState;
    #endif
}


/* [] END OF FILE */
