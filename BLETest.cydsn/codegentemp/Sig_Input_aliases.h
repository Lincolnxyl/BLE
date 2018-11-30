/*******************************************************************************
* File Name: Sig_Input.h  
* Version 2.20
*
* Description:
*  This file contains the Alias definitions for Per-Pin APIs in cypins.h. 
*  Information on using these APIs can be found in the System Reference Guide.
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Sig_Input_ALIASES_H) /* Pins Sig_Input_ALIASES_H */
#define CY_PINS_Sig_Input_ALIASES_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"


/***************************************
*              Constants        
***************************************/
#define Sig_Input_0			(Sig_Input__0__PC)
#define Sig_Input_0_PS		(Sig_Input__0__PS)
#define Sig_Input_0_PC		(Sig_Input__0__PC)
#define Sig_Input_0_DR		(Sig_Input__0__DR)
#define Sig_Input_0_SHIFT	(Sig_Input__0__SHIFT)
#define Sig_Input_0_INTR	((uint16)((uint16)0x0003u << (Sig_Input__0__SHIFT*2u)))

#define Sig_Input_INTR_ALL	 ((uint16)(Sig_Input_0_INTR))


#endif /* End Pins Sig_Input_ALIASES_H */


/* [] END OF FILE */
