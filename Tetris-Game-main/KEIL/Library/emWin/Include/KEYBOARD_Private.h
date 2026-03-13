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
File        : KEYBOARD_Private.h
Purpose     : Internal header file
--------------------END-OF-HEADER-------------------------------------
*/

#ifndef KEYBOARD_PRIVATE_H
#define KEYBOARD_PRIVATE_H

#include "WM_Intern.h"
#include "KEYBOARD.h"

#if (GUI_WINSUPPORT)

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define FLAG_FIXED        (1 << 0)
#define FLAG_CODES        (1 << 1)
#define FLAG_SHIFT        (1 << 2)
#define FLAG_EXTRA        (1 << 3)
#define FLAG_KEYBACKSPACE (1 << 4)
#define FLAG_KEYENTER     (1 << 5)
#define FLAG_KEYSHIFT     (1 << 6)
#define FLAG_KEYSWITCH    (1 << 7)
#define FLAG_KEYSPACE     (1 << 8)

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct {
  GUI_RECT       Rect;
  U16            Code;
  U16            Long;
  U16            Flags;
  KEYBOARD_CODES cLong;
} KEYBOARD_KEY;

typedef struct {
  GUI_HMEM     hText;  // Handle to text
  const void * pBm;    // Pointer to GUI_BITMAP or pointer to streamed bitmap
  U32          Size;   // In case of a streamed bitmap 'Size' contains its size
} KEYBOARD_BITMAP_KEY;

/*********************************************************************
*
*       Object definition
*
**********************************************************************
*/
typedef struct {
  GUI_COLOR aColor[7];       // Colors to be used
  unsigned  aPeriod[2];      // Array for periods [0:long press, 1:repeat mode]
  unsigned  FrameRadius;     // Radius to be used for all keys
  unsigned  FrameSize;       // Frame size in pixels to be used for all keys
  unsigned  SpaceX, SpaceY;  // Space in /1000 between the keys
  unsigned  CursorSensy;     // Sensitivity of cursor control window
} KEYBOARD_PROPS;

typedef struct {
  WIDGET           Widget;
  KEYBOARD_PROPS   Props;
  const GUI_FONT * apFont[2];  // 0: Key codes, 1: Longpress codes
  //
  // Common
  //
  U8           State;
  U8           IsPressed;
  U8           StateLocked;
  WM_HTIMER    hTimer;
  KEYBOARD_KEY kPressed;
  int          PressedIndex;
  //
  // Code keys
  //
  unsigned NumKeys;
  GUI_HMEM hKeys;
  //
  // Special keys
  //
  KEYBOARD_BITMAP_KEY aKeyShift[4];
  KEYBOARD_BITMAP_KEY aKeySwitch[2];
  KEYBOARD_BITMAP_KEY KeyBackspace;
  KEYBOARD_BITMAP_KEY KeyEnter;
  KEYBOARD_BITMAP_KEY KeySpace;
  //
  // Timer callback
  //
  void (* pfOnTimer)(KEYBOARD_Handle hObj);
  //
  // Size of longpress key
  //
  int xSizeKey, ySizeKey;
} KEYBOARD_OBJ;

/*********************************************************************
*
*       Macros for internal use
*
**********************************************************************
*/
#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  #define KEYBOARD_INIT_ID(p) (p->Widget.DebugId = WIDGET_TYPE_KEYBOARD)
#else
  #define KEYBOARD_INIT_ID(p)
#endif

#if GUI_DEBUG_LEVEL >= GUI_DEBUG_LEVEL_CHECK_ALL
  KEYBOARD_OBJ * KEYBOARD_LockH(KEYBOARD_Handle h);
  #define KEYBOARD_LOCK_H(h) KEYBOARD_LockH(h)
#else
  #define KEYBOARD_LOCK_H(h) (KEYBOARD_OBJ *)WM_LOCK_H(h)
#endif

/*********************************************************************
*
*       Private functions
*
**********************************************************************
*/
int KEYBOARD__SetStreamedLayoutEx(KEYBOARD_Handle hObj, const void * pVoid, U32 Size, void (* pFunc)(KEYDEF_KEYBOARD * pKeyboard));

/*********************************************************************
*
*       Public data (internal defaults)
*
**********************************************************************
*/
extern KEYBOARD_PROPS KEYBOARD__DefaultProps;

#endif  // GUI_WINSUPPORT
#endif  // KEYBOARD_PRIVATE_H

/*************************** End of file ****************************/
