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
File        : CHOOSECOLOR.h
Purpose     : Message box interface
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef CHOOSECOLOR_H
#define CHOOSECOLOR_H

#include "WM.h"

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
#define CHOOSECOLOR_CF_MOVEABLE FRAMEWIN_CF_MOVEABLE

/*********************************************************************
*
*       CHOOSECOLOR color indexes
*
*  Description
*    Color indexes used by the CHOOSECOLOR dialog.
*/
#define CHOOSECOLOR_CI_FRAME 0    // Color to be used to draw the frame surrounding each color. Default is GUI_GRAY.
#define CHOOSECOLOR_CI_FOCUS 1    // Color to be used to draw the focus rectangle. Default is GUI_BLACK.

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
/*********************************************************************
*
*       CHOOSECOLOR_PROPS
*/
typedef struct {
  unsigned  aBorder[2];
  unsigned  aSpace[2];
  unsigned  aButtonSize[2];
  GUI_COLOR aColor[2];
} CHOOSECOLOR_PROPS;

/*********************************************************************
*
*       CHOOSECOLOR_CONTEXT
*/
typedef struct {
  U32               LastColor;
  const GUI_COLOR * pColor;
  unsigned          NumColors;
  unsigned          NumColorsPerLine;
  int               SelOld;
  int               Sel;
  WM_HWIN           hParent;
  CHOOSECOLOR_PROPS Props;
} CHOOSECOLOR_CONTEXT;

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
WM_HWIN CHOOSECOLOR_Create(WM_HWIN           hParent,
                           int               xPos,
                           int               yPos,
                           int               xSize,
                           int               ySize,
                           const GUI_COLOR * pColor,
                           unsigned          NumColors,
                           unsigned          NumColorsPerLine,
                           int               Sel,
                           const char      * sCaption,
                           int               Flags);

int  CHOOSECOLOR_GetSel(WM_HWIN hObj);
void CHOOSECOLOR_SetSel(WM_HWIN hObj, int Sel);

void CHOOSECOLOR_SetDefaultColor     (unsigned Index, GUI_COLOR Color);
void CHOOSECOLOR_SetDefaultSpace     (unsigned Index, unsigned Space);
void CHOOSECOLOR_SetDefaultBorder    (unsigned Index, unsigned Border);
void CHOOSECOLOR_SetDefaultButtonSize(unsigned Index, unsigned ButtonSize);

/*********************************************************************
*
*       The callback ...
*
* Do not call it directly ! It is only to be used from within an
* overwritten callback.
*/
void CHOOSECOLOR_Callback(WM_MESSAGE * pMsg);

#if defined(__cplusplus)
  }
#endif

#endif /* GUI_WINSUPPORT */

#endif /* CHOOSECOLOR_H */

/*************************** End of file ****************************/
