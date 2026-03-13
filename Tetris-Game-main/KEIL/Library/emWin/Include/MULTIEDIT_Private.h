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
File        : MULTIEDIT_Private.h
Purpose     : MULTIEDIT include
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef MULTIEDIT_PRIVATE_H
#define MULTIEDIT_PRIVATE_H

#include "GUI_Private.h"
#include "WM_Intern.h"

#if GUI_WINSUPPORT

#include <stddef.h>

#include "MULTIEDIT.h"

#if defined(__cplusplus)
  extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/

#define NUM_DISP_MODES 2

/*********************************************************************
*
*       Invalid flags
*
*  Used for partial invalidation. Stored in pObj->InvalidFlags.
*/
#define INVALID_NUMCHARS   (1 << 0)
#define INVALID_NUMLINES   (1 << 1)
#define INVALID_TEXTSIZE   (1 << 2)
#define INVALID_CURSORXY   (1 << 3)
#define INVALID_CURSORLINE (1 << 4)
#define INVALID_LINEPOSB   (1 << 5)
#define INVALID_SCROLL     (1 << 6)

//
// MULTIEDIT properties
//
typedef struct {
  GUI_COLOR        aBkColor    [NUM_DISP_MODES];
  GUI_COLOR        aColor      [NUM_DISP_MODES];
  GUI_COLOR        aCursorColor[2];
  U16              Align;
  const GUI_FONT * pFont;
  U8               HBorder;
} MULTIEDIT_PROPS;

//
// MULTIEDIT object
//
typedef struct {
  WIDGET           Widget;
  MULTIEDIT_PROPS  Props;
  WIDGET_COPY      Copy;
  WM_HMEM          hText;
  U32              MaxNumChars;         // Maximum number of characters including the prompt
  U32              NumChars;            // Number of characters (text and prompt) in object
  U32              NumCharsPrompt;      // Number of prompt characters
  U32              NumLines;            // Number of text lines needed to show all data
  U16              TextSizeX;           // Size in X of text depending of wrapping mode
  U32              BufferSize;
  U32              CursorLine;          // Number of current cursor line
  U32              CursorPosChar;       // Character offset number of cursor
  U32              CursorPosByte;       // Byte offset number of cursor
  I32              CursorPosX;          // Cursor position in X
  I32              CursorPosY;          // Cursor position in Y
  U32              CacheLinePosByte;
  U32              CacheLineNumber;
  U32              CacheFirstVisibleLine;
  U32              CacheFirstVisibleByte;
  WM_SCROLL_STATE  ScrollStateV;
  WM_SCROLL_STATE  ScrollStateH;
  U16              Flags;
  WM_HTIMER        hTimer;
  int              TimerPeriod;
  GUI_WRAPMODE     WrapMode;
  int              MotionPosY;
  int              MotionPosOld;
  WM_HMEM          hContext;             // Motion context.
  U8               CursorVis;            // Indicates whether cursor is visible or not
  U8               InvertCursor;
  U8               InvalidFlags;         // Flags to save validation status
  U8               EditMode;
  U8               MotionActive;
  U8               Radius;               // Currently only used by AppWizard
  U8               Frame;
  U8               Up;
#if GUI_SUPPORT_MEMDEV
  GUI_MEMDEV_Handle ahMemCursor[2];
#endif
} MULTIEDIT_OBJ;

/*********************************************************************
*
*       Private (module internal) data
*
**********************************************************************
*/
extern MULTIEDIT_PROPS MULTIEDIT_DefaultProps;

#if defined(__cplusplus)
  }
#endif

#endif  // GUI_WINSUPPORT
#endif  // MULTIEDIT_PRIVATE_H

/*************************** End of file ****************************/
