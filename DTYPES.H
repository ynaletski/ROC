/****************************************************************************/
/*  dtypes.h       Fisher primitive data type definitions                   */
/*  Copyright (c)  1995  Fisher Controls International Incorporated         */
/*                                                                          */
/*  CONFIDENTIAL                                                            */
/*  This program is the property of Fisher Controls. Its configuration and  */
/*  algorithms are confidential and may not be used, reproduced, or         */
/*  revealed to others except in accord with contract or other written      */
/*  consent of Fisher Controls. Any permitted  reproduction, in whole or in */
/*  part, must bear this legend.                                            */
/****************************************************************************/
/*  This file contains the definitions for Fisher primitive data types      */
/****************************************************************************/
/*
 *  10/24/95  GSW  Created new - dtypes.h
 */
/****************************************************************************/

/*lint -e767 */

#ifndef DTYPES_H
#define DTYPES_H

#define UINT8  unsigned char
#define UINT16 unsigned short int
#define UINT32 unsigned long int
#define INT8   char
#define INT16  short int
#define INT32  long int 
#define FLOAT  float
#define DOUBLE double 

#ifndef GLOBALS_DEFINED
#define EXTERN extern
#else
#define EXTERN
#endif

#endif
/*lint +e767 */
