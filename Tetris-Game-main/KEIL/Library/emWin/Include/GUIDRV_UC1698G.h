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
File        : GUIDRV_UC1698G.h
Purpose     : Interface definition for GUIDRV_UC1698G driver
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUIDRV_UC1698G_H
#define GUIDRV_UC1698G_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Configuration structure
*/
typedef struct {
  //
  // Driver specific configuration items
  //
  int FirstSEG;
  int FirstCOM;
  int NumDummyReads;
} CONFIG_UC1698G;

/*********************************************************************
*
*       Display drivers
*/
//
// Addresses
//
extern const GUI_DEVICE_API GUIDRV_UC1698G_1C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OY_1C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OX_1C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OXY_1C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OS_1C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSY_1C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSX_1C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSXY_1C0_API;

extern const GUI_DEVICE_API GUIDRV_UC1698G_1C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OY_1C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OX_1C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OXY_1C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OS_1C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSY_1C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSX_1C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSXY_1C1_API;

extern const GUI_DEVICE_API GUIDRV_UC1698G_5C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OY_5C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OX_5C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OXY_5C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OS_5C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSY_5C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSX_5C0_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSXY_5C0_API;

extern const GUI_DEVICE_API GUIDRV_UC1698G_5C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OY_5C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OX_5C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OXY_5C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OS_5C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSY_5C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSX_5C1_API;
extern const GUI_DEVICE_API GUIDRV_UC1698G_OSXY_5C1_API;

//
// Macros to be used in configuration files
//
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_UC1698G_1C0       &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OY_1C0    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OX_1C0    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OXY_1C0   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OS_1C0    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSY_1C0   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSX_1C0   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSXY_1C0  &GUIDRV_Win_API

  #define GUIDRV_UC1698G_1C1       &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OY_1C1    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OX_1C1    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OXY_1C1   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OS_1C1    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSY_1C1   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSX_1C1   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSXY_1C1  &GUIDRV_Win_API

  #define GUIDRV_UC1698G_5C0       &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OY_5C0    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OX_5C0    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OXY_5C0   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OS_5C0    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSY_5C0   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSX_5C0   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSXY_5C0  &GUIDRV_Win_API

  #define GUIDRV_UC1698G_5C1       &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OY_5C1    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OX_5C1    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OXY_5C1   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OS_5C1    &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSY_5C1   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSX_5C1   &GUIDRV_Win_API
  #define GUIDRV_UC1698G_OSXY_5C1  &GUIDRV_Win_API

#else

  #define GUIDRV_UC1698G_1C0       &GUIDRV_UC1698G_1C0_API
  #define GUIDRV_UC1698G_OY_1C0    &GUIDRV_UC1698G_OY_1C0_API
  #define GUIDRV_UC1698G_OX_1C0    &GUIDRV_UC1698G_OX_1C0_API
  #define GUIDRV_UC1698G_OXY_1C0   &GUIDRV_UC1698G_OXY_1C0_API
  #define GUIDRV_UC1698G_OS_1C0    &GUIDRV_UC1698G_OS_1C0_API
  #define GUIDRV_UC1698G_OSY_1C0   &GUIDRV_UC1698G_OSY_1C0_API
  #define GUIDRV_UC1698G_OSX_1C0   &GUIDRV_UC1698G_OSX_1C0_API
  #define GUIDRV_UC1698G_OSXY_1C0  &GUIDRV_UC1698G_OSXY_1C0_API

  #define GUIDRV_UC1698G_1C1       &GUIDRV_UC1698G_1C1_API
  #define GUIDRV_UC1698G_OY_1C1    &GUIDRV_UC1698G_OY_1C1_API
  #define GUIDRV_UC1698G_OX_1C1    &GUIDRV_UC1698G_OX_1C1_API
  #define GUIDRV_UC1698G_OXY_1C1   &GUIDRV_UC1698G_OXY_1C1_API
  #define GUIDRV_UC1698G_OS_1C1    &GUIDRV_UC1698G_OS_1C1_API
  #define GUIDRV_UC1698G_OSY_1C1   &GUIDRV_UC1698G_OSY_1C1_API
  #define GUIDRV_UC1698G_OSX_1C1   &GUIDRV_UC1698G_OSX_1C1_API
  #define GUIDRV_UC1698G_OSXY_1C1  &GUIDRV_UC1698G_OSXY_1C1_API

  #define GUIDRV_UC1698G_5C0       &GUIDRV_UC1698G_5C0_API
  #define GUIDRV_UC1698G_OY_5C0    &GUIDRV_UC1698G_OY_5C0_API
  #define GUIDRV_UC1698G_OX_5C0    &GUIDRV_UC1698G_OX_5C0_API
  #define GUIDRV_UC1698G_OXY_5C0   &GUIDRV_UC1698G_OXY_5C0_API
  #define GUIDRV_UC1698G_OS_5C0    &GUIDRV_UC1698G_OS_5C0_API
  #define GUIDRV_UC1698G_OSY_5C0   &GUIDRV_UC1698G_OSY_5C0_API
  #define GUIDRV_UC1698G_OSX_5C0   &GUIDRV_UC1698G_OSX_5C0_API
  #define GUIDRV_UC1698G_OSXY_5C0  &GUIDRV_UC1698G_OSXY_5C0_API

  #define GUIDRV_UC1698G_5C1       &GUIDRV_UC1698G_5C1_API
  #define GUIDRV_UC1698G_OY_5C1    &GUIDRV_UC1698G_OY_5C1_API
  #define GUIDRV_UC1698G_OX_5C1    &GUIDRV_UC1698G_OX_5C1_API
  #define GUIDRV_UC1698G_OXY_5C1   &GUIDRV_UC1698G_OXY_5C1_API
  #define GUIDRV_UC1698G_OS_5C1    &GUIDRV_UC1698G_OS_5C1_API
  #define GUIDRV_UC1698G_OSY_5C1   &GUIDRV_UC1698G_OSY_5C1_API
  #define GUIDRV_UC1698G_OSX_5C1   &GUIDRV_UC1698G_OSX_5C1_API
  #define GUIDRV_UC1698G_OSXY_5C1  &GUIDRV_UC1698G_OSXY_5C1_API

#endif

/*********************************************************************
*
*       Public routines
*/
#if defined(WIN32) && !defined(LCD_SIMCONTROLLER)

  #define GUIDRV_UC1698G_Config(pDevice, pConfig)
  #define GUIDRV_UC1698G_SetBus8(pDevice, pHW_API)
  #define GUIDRV_UC1698G_SetBus16(pDevice, pHW_API)

#else

  void GUIDRV_UC1698G_Config  (GUI_DEVICE * pDevice, CONFIG_UC1698G * pConfig);
  void GUIDRV_UC1698G_SetBus8 (GUI_DEVICE * pDevice, GUI_PORT_API   * pHW_API);
  void GUIDRV_UC1698G_SetBus16(GUI_DEVICE * pDevice, GUI_PORT_API   * pHW_API);

#endif

#if defined(__cplusplus)
}
#endif

#endif

/*************************** End of file ****************************/


