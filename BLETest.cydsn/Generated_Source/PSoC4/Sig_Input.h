/*******************************************************************************
* File Name: Sig_Input.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Sig_Input_H) /* Pins Sig_Input_H */
#define CY_PINS_Sig_Input_H

#include "cytypes.h"
#include "cyfitter.h"
#include "Sig_Input_aliases.h"


/***************************************
*     Data Struct Definitions
***************************************/

/**
* \addtogroup group_structures
* @{
*/
    
/* Structure for sleep mode support */
typedef struct
{
    uint32 pcState; /**< State of the port control register */
    uint32 sioState; /**< State of the SIO configuration */
    uint32 usbState; /**< State of the USBIO regulator */
} Sig_Input_BACKUP_STRUCT;

/** @} structures */


/***************************************
*        Function Prototypes             
***************************************/
/**
* \addtogroup group_general
* @{
*/
uint8   Sig_Input_Read(void);
void    Sig_Input_Write(uint8 value);
uint8   Sig_Input_ReadDataReg(void);
#if defined(Sig_Input__PC) || (CY_PSOC4_4200L) 
    void    Sig_Input_SetDriveMode(uint8 mode);
#endif
void    Sig_Input_SetInterruptMode(uint16 position, uint16 mode);
uint8   Sig_Input_ClearInterrupt(void);
/** @} general */

/**
* \addtogroup group_power
* @{
*/
void Sig_Input_Sleep(void); 
void Sig_Input_Wakeup(void);
/** @} power */


/***************************************
*           API Constants        
***************************************/
#if defined(Sig_Input__PC) || (CY_PSOC4_4200L) 
    /* Drive Modes */
    #define Sig_Input_DRIVE_MODE_BITS        (3)
    #define Sig_Input_DRIVE_MODE_IND_MASK    (0xFFFFFFFFu >> (32 - Sig_Input_DRIVE_MODE_BITS))

    /**
    * \addtogroup group_constants
    * @{
    */
        /** \addtogroup driveMode Drive mode constants
         * \brief Constants to be passed as "mode" parameter in the Sig_Input_SetDriveMode() function.
         *  @{
         */
        #define Sig_Input_DM_ALG_HIZ         (0x00u) /**< \brief High Impedance Analog   */
        #define Sig_Input_DM_DIG_HIZ         (0x01u) /**< \brief High Impedance Digital  */
        #define Sig_Input_DM_RES_UP          (0x02u) /**< \brief Resistive Pull Up       */
        #define Sig_Input_DM_RES_DWN         (0x03u) /**< \brief Resistive Pull Down     */
        #define Sig_Input_DM_OD_LO           (0x04u) /**< \brief Open Drain, Drives Low  */
        #define Sig_Input_DM_OD_HI           (0x05u) /**< \brief Open Drain, Drives High */
        #define Sig_Input_DM_STRONG          (0x06u) /**< \brief Strong Drive            */
        #define Sig_Input_DM_RES_UPDWN       (0x07u) /**< \brief Resistive Pull Up/Down  */
        /** @} driveMode */
    /** @} group_constants */
#endif

/* Digital Port Constants */
#define Sig_Input_MASK               Sig_Input__MASK
#define Sig_Input_SHIFT              Sig_Input__SHIFT
#define Sig_Input_WIDTH              1u

/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Sig_Input_SetInterruptMode() function.
     *  @{
     */
        #define Sig_Input_INTR_NONE      ((uint16)(0x0000u)) /**< \brief Disabled             */
        #define Sig_Input_INTR_RISING    ((uint16)(0x5555u)) /**< \brief Rising edge trigger  */
        #define Sig_Input_INTR_FALLING   ((uint16)(0xaaaau)) /**< \brief Falling edge trigger */
        #define Sig_Input_INTR_BOTH      ((uint16)(0xffffu)) /**< \brief Both edge trigger    */
    /** @} intrMode */
/** @} group_constants */

/* SIO LPM definition */
#if defined(Sig_Input__SIO)
    #define Sig_Input_SIO_LPM_MASK       (0x03u)
#endif

/* USBIO definitions */
#if !defined(Sig_Input__PC) && (CY_PSOC4_4200L)
    #define Sig_Input_USBIO_ENABLE               ((uint32)0x80000000u)
    #define Sig_Input_USBIO_DISABLE              ((uint32)(~Sig_Input_USBIO_ENABLE))
    #define Sig_Input_USBIO_SUSPEND_SHIFT        CYFLD_USBDEVv2_USB_SUSPEND__OFFSET
    #define Sig_Input_USBIO_SUSPEND_DEL_SHIFT    CYFLD_USBDEVv2_USB_SUSPEND_DEL__OFFSET
    #define Sig_Input_USBIO_ENTER_SLEEP          ((uint32)((1u << Sig_Input_USBIO_SUSPEND_SHIFT) \
                                                        | (1u << Sig_Input_USBIO_SUSPEND_DEL_SHIFT)))
    #define Sig_Input_USBIO_EXIT_SLEEP_PH1       ((uint32)~((uint32)(1u << Sig_Input_USBIO_SUSPEND_SHIFT)))
    #define Sig_Input_USBIO_EXIT_SLEEP_PH2       ((uint32)~((uint32)(1u << Sig_Input_USBIO_SUSPEND_DEL_SHIFT)))
    #define Sig_Input_USBIO_CR1_OFF              ((uint32)0xfffffffeu)
#endif


/***************************************
*             Registers        
***************************************/
/* Main Port Registers */
#if defined(Sig_Input__PC)
    /* Port Configuration */
    #define Sig_Input_PC                 (* (reg32 *) Sig_Input__PC)
#endif
/* Pin State */
#define Sig_Input_PS                     (* (reg32 *) Sig_Input__PS)
/* Data Register */
#define Sig_Input_DR                     (* (reg32 *) Sig_Input__DR)
/* Input Buffer Disable Override */
#define Sig_Input_INP_DIS                (* (reg32 *) Sig_Input__PC2)

/* Interrupt configuration Registers */
#define Sig_Input_INTCFG                 (* (reg32 *) Sig_Input__INTCFG)
#define Sig_Input_INTSTAT                (* (reg32 *) Sig_Input__INTSTAT)

/* "Interrupt cause" register for Combined Port Interrupt (AllPortInt) in GSRef component */
#if defined (CYREG_GPIO_INTR_CAUSE)
    #define Sig_Input_INTR_CAUSE         (* (reg32 *) CYREG_GPIO_INTR_CAUSE)
#endif

/* SIO register */
#if defined(Sig_Input__SIO)
    #define Sig_Input_SIO_REG            (* (reg32 *) Sig_Input__SIO)
#endif /* (Sig_Input__SIO_CFG) */

/* USBIO registers */
#if !defined(Sig_Input__PC) && (CY_PSOC4_4200L)
    #define Sig_Input_USB_POWER_REG       (* (reg32 *) CYREG_USBDEVv2_USB_POWER_CTRL)
    #define Sig_Input_CR1_REG             (* (reg32 *) CYREG_USBDEVv2_CR1)
    #define Sig_Input_USBIO_CTRL_REG      (* (reg32 *) CYREG_USBDEVv2_USB_USBIO_CTRL)
#endif    
    
    
/***************************************
* The following code is DEPRECATED and 
* must not be used in new designs.
***************************************/
/**
* \addtogroup group_deprecated
* @{
*/
#define Sig_Input_DRIVE_MODE_SHIFT       (0x00u)
#define Sig_Input_DRIVE_MODE_MASK        (0x07u << Sig_Input_DRIVE_MODE_SHIFT)
/** @} deprecated */

#endif /* End Pins Sig_Input_H */


/* [] END OF FILE */
