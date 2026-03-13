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
File        : GUI_SPY_Private.h
Purpose     : Private header file for Spy
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_SPY_PRIVATE_H
#define GUI_SPY_PRIVATE_H

#include "GUI_Private.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#ifndef   GUI_SPY_COM_BUFFER_SIZE
  #define GUI_SPY_COM_BUFFER_SIZE 1000
#endif

#define JOB_REQUEST_STATUS  1
#define JOB_REQUEST_VERSION 2
#define JOB_REQUEST_WINDOWS 3
#define JOB_REQUEST_INPUT   4
#define JOB_REQUEST_BMP     5
#define JOB_QUIT            6

#define INPUT_PID    1
#define INPUT_KEY    2
#define INPUT_MTOUCH 3

#define TRY(x) x; if (r) return 1

/*********************************************************************
*
*       Types
*
**********************************************************************
*/
typedef struct SPY_CONTEXT SPY_CONTEXT;

struct SPY_CONTEXT{
  GUI_tSend pfSend;
  GUI_tRecv pfRecv;
  void * pConnectInfo;
  U8 acBuffer[GUI_SPY_COM_BUFFER_SIZE];
  int NumBytesInBuffer;
  int Result;
  int (* pfFlush)   (SPY_CONTEXT * pContext);
  int (* pfStoreU8) (SPY_CONTEXT * pContext,  U8 Byte);
  int (* pfStoreU16)(SPY_CONTEXT * pContext, U16 Data);
  int (* pfStoreU32)(SPY_CONTEXT * pContext, U32 Data);
  int (* pfRead)    (SPY_CONTEXT * pContext, U8 * buf, int len);
};

typedef struct {
  U16 Type;
  U32 Time;
  union {
    GUI_KEY_STATE    StateKEY;
    GUI_PID_STATE    StatePID;
    GUI_MTOUCH_STATE StateMTOUCH;
  } u;
} SPY_INPUT;



#endif // GUI_SPY_PRIVATE_H

/*************************** End of file ****************************/
