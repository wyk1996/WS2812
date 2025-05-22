/**********************************************************************
 $Id$		IndependentDataType.h		2023-7-31
*//**
* @file    IndependentDataType.h
* @brief   32 bit系统小端模式数据类型定义
*
*
* @version 
* @author  JYC
* @date    2023-7-31
*
* @history
*
*********************************************************************/

#ifndef _INDEPENDENT_DATA_TYPE_H_
#define _INDEPENDENT_DATA_TYPE_H_

/*******************************************************************************
*	Public macro definition:
*******************************************************************************
*/

/*
*********************************************************************************************************
*                                              DATA TYPES
*                                         (Compiler Specific)
*********************************************************************************************************
*/

typedef unsigned char	 BOOLEAN;
typedef unsigned char    INT8U;                    /* Unsigned  8 bit quantity                           */
typedef signed   char    INT8S;                    /* Signed    8 bit quantity                           */
typedef unsigned short   INT16U;                   /* Unsigned 16 bit quantity                           */
typedef signed   short   INT16S;                   /* Signed   16 bit quantity                           */
typedef unsigned int     INT32U;                   /* Unsigned 32 bit quantity                           */
typedef signed   int     INT32S;                   /* Signed   32 bit quantity                           */
typedef unsigned long long 	 INT64U;                   /* Unsigned   64 bit quantity                           */
typedef signed   long long	 INT64S;                   /* Signed   64 bit quantity                           */
typedef float            FP32;                     /* Single precision floating point                    */
typedef double           FP64;                     /* Double precision floating point                    */

#endif

