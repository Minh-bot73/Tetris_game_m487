/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2023  SEGGER Microcontroller GmbH                *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V6.46 - Graphical user interface for embedded applications **
emWin is protected by international copyright laws. Knowledge of the
source code may not be used to write a similar product. 
This file may
only be used in accordance with the following terms:

The  software has  been licensed by SEGGER Software GmbH to Nuvoton Technology Corporation
at the address: No. 4, Creation Rd. III, Hsinchu Science Park, Taiwan
for the purposes  of  creating  libraries  for its 
Arm Cortex-A, Arm Cortex-M and  Arm9 32-bit microcontrollers, commercialized and distributed by Nuvoton Technology Corporation
under  the terms and conditions  of  an  End  User  
License  Agreement  supplied  with  the libraries.
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
Licensing information
Licensor:                 SEGGER Software GmbH
Licensed to:              Nuvoton Technology Corporation, No. 4, Creation Rd. III, Hsinchu Science Park, 30077 Hsinchu City, Taiwan
Licensed SEGGER software: emWin
License number:           GUI-00735
License model:            emWin License Agreement, signed February 27, 2018
Licensed platform:        Cortex-M, Cortex-A, and ARM9 32-bit series microcontroller designed and manufactured by Nuvoton Technology Corporation.
----------------------------------------------------------------------
Support and Update Agreement (SUA)
SUA period:               2018-03-26 - 2025-09-04
Contact to extend SUA:    sales@segger.com
----------------------------------------------------------------------
File        : GUIDRV_SH_MEM.h
Purpose     : Interface definition for GUIDRV_SH_MEM driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIDRV_SH_MEM_H
#define GUIDRV_SH_MEM_H

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_Win_API;

extern const GUI_DEVICE_API GUIDRV_SH_MEM_API;
extern const GUI_DEVICE_API GUIDRV_SH_MEM_API_OXY;
extern const GUI_DEVICE_API GUIDRV_SH_MEM_API_OSX;
extern const GUI_DEVICE_API GUIDRV_SH_MEM_API_OSY;

extern const GUI_DEVICE_API GUIDRV_SH_MEM_3_API;
extern const GUI_DEVICE_API GUIDRV_SH_MEM_3_API_OXY;
extern const GUI_DEVICE_API GUIDRV_SH_MEM_3_API_OSX;
extern const GUI_DEVICE_API GUIDRV_SH_MEM_3_API_OSY;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_SH_MEM            &GUIDRV_Win_API
  #define GUIDRV_SH_MEM_OXY        &GUIDRV_Win_API
  #define GUIDRV_SH_MEM_OSX        &GUIDRV_Win_API
  #define GUIDRV_SH_MEM_OSY        &GUIDRV_Win_API

  #define GUIDRV_SH_MEM_3          &GUIDRV_Win_API
  #define GUIDRV_SH_MEM_3_OXY      &GUIDRV_Win_API
  #define GUIDRV_SH_MEM_3_OSX      &GUIDRV_Win_API
  #define GUIDRV_SH_MEM_3_OSY      &GUIDRV_Win_API

#else

  #define GUIDRV_SH_MEM            &GUIDRV_SH_MEM_API
  #define GUIDRV_SH_MEM_OXY        &GUIDRV_SH_MEM_API_OXY
  #define GUIDRV_SH_MEM_OSX        &GUIDRV_SH_MEM_API_OSX
  #define GUIDRV_SH_MEM_OSY        &GUIDRV_SH_MEM_API_OSY

  #define GUIDRV_SH_MEM_3          &GUIDRV_SH_MEM_3_API
  #define GUIDRV_SH_MEM_3_OXY      &GUIDRV_SH_MEM_3_API_OXY
  #define GUIDRV_SH_MEM_3_OSX      &GUIDRV_SH_MEM_3_API_OSX
  #define GUIDRV_SH_MEM_3_OSY      &GUIDRV_SH_MEM_3_API_OSY

#endif

#define GUIDRV_SH_MEM_8BITMODE  0
#define GUIDRV_SH_MEM_9BITMODE  1
#define GUIDRV_SH_MEM_10BITMODE 2

typedef struct {
  unsigned Period;           // Period used for toggling VCOM
  unsigned ExtMode;          // Setting of EXTMODE configuration pin
  unsigned BitMode;          // 8- or 10-bit line addressing
  unsigned SendEnd;          // Send end command 0 - no, 1 - yes
  void (* pfToggleVCOM)(void);
} CONFIG_SH_MEM;

/*********************************************************************
*
*       Prototypes
*
**********************************************************************
*/
void GUIDRV_SH_MEM_SetBus8  (GUI_DEVICE * pDevice, GUI_PORT_API * pHW_API);
void GUIDRV_SH_MEM_Config   (GUI_DEVICE * pDevice, CONFIG_SH_MEM * pConfig);
void GUIDRV_SH_MEM_3_SetBus8(GUI_DEVICE * pDevice, GUI_PORT_API * pHW_API);
void GUIDRV_SH_MEM_3_Config (GUI_DEVICE * pDevice, CONFIG_SH_MEM * pConfig);

#endif

/*************************** End of file ****************************/
