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
File        : QRCODE.h
Purpose     : QRCODE public header file (API)
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef QRCODE_H
#define QRCODE_H

#include "WM.h"
#include "DIALOG_Type.h"      /* Req. for Create indirect data structure */
#include "WIDGET.h"

#if GUI_WINSUPPORT

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
/*********************************************************************
*
*       QRCODE WiFi encryption types
*
*  Description
*    These macros are to be used for the \a{Encryption} parameter of
*    QRCODE_SetWiFiText().
*/
#define QRCODE_WIFI_WPA    0    // If the WiFi password is WPA encrypted.
#define QRCODE_WIFI_WEP    1    // If the WiFi password is WEP encrypted.

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef WM_HMEM QRCODE_Handle;

/*********************************************************************
*
*       Create function(s)

  Note: the parameters to a create function may vary.
         Some widgets may have multiple create functions
*/
QRCODE_Handle QRCODE_CreateUser    (int x0, int y0, int xSize, int ySize, WM_HWIN hParent, U32 WinFlags, int ExFlags, int Id, const char * pText, int PixelSize, int EccLevel, int Version, int NumExtraBytes);
QRCODE_Handle QRCODE_CreateIndirect(const GUI_WIDGET_CREATE_INFO * pCreateInfo, WM_HWIN hWinParent, int x0, int y0, WM_CALLBACK * cb);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void QRCODE_Callback(WM_MESSAGE *pMsg);

/*********************************************************************
*
*       Member functions
*
**********************************************************************
*/
void QRCODE_SetEccLevel  (QRCODE_Handle hObj, int EccLevel);
void QRCODE_SetNumModules(QRCODE_Handle hObj, int NumModules);
void QRCODE_SetPixelSize (QRCODE_Handle hObj, int PixelSize);
void QRCODE_SetText      (QRCODE_Handle hObj, const char * pText);
void QRCODE_SetVersion   (QRCODE_Handle hObj, int Version);
void QRCODE_SetWiFiText  (QRCODE_Handle hObj, const char * pSSID, U8 Encryption, const char * pPassword, U8 Hidden);

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // QRCODE_H

/*************************** End of file ****************************/

