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
File        : GUI_SVG_Global.h
Purpose     : Global public definitions used by SVG drivers
---------------------------END-OF-HEADER------------------------------
*/

#ifndef GUI_SVG_GLOBAL_H
#define GUI_SVG_GLOBAL_H

#if defined(__cplusplus)
extern "C" {     /* Make sure we have C-declarations in C++ programs */
#endif

/*********************************************************************
*
*       Standard library fixed size types
*
**********************************************************************
*/
#ifdef WIN32
  #if (_MSC_VER <= 1900)                      // Older compiler version do not know about the header
    typedef char                 int8_t;
    typedef unsigned char        uint8_t;
    typedef short                int16_t;
    typedef unsigned short       uint16_t;
    typedef int                  int32_t;
    typedef unsigned int         uint32_t;
    #if (_MSC_VER > 1200)                     // VC6 does not support typedef (unsigned) long long 
      typedef long long          int64_t;
      typedef unsigned long long uint64_t;
    #endif
  #else
    #include <stdint.h>
  #endif
#else
  #include <stdint.h>
#endif

/*********************************************************************
*
*       Macros
*
**********************************************************************
*/
//
// Redirect 3rd party typename to the same type defined as GUI_...
//
#define REDIRECT_TYPE(TYPE_NAME)    typedef GUI_##TYPE_NAME TYPE_NAME
//
// Used to define a given 3rd party type with private members
// with the same size of given type if source is not available.
//
#define DEFINE_TYPE_PRIVATE(TYPE_NAME, NUM_BYTES)   \
  typedef struct {                                  \
    U32 a[NUM_BYTES / 4];                           \
  } GUI_##TYPE_NAME;                                \
  REDIRECT_TYPE(TYPE_NAME)
//
// Used to define a given 3rd party type with public members,
// required when the named members are used in the source file.
//
#define DEFINE_TYPE_PUBLIC(TYPE_NAME, MEMBERS)   \
  typedef struct {                               \
    MEMBERS                                      \
  } GUI_##TYPE_NAME;                             \
  REDIRECT_TYPE(TYPE_NAME)

//
// Stringify defines
//
#define SVG_STRINGIFY(x)   #x
#define SVG_MAKE_STR(x)    SVG_STRINGIFY(x)

#if defined(__cplusplus)
}
#endif

#endif /* GUI_SVG_GLOBAL_H */

/*************************** End of file ****************************/
