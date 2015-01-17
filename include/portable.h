/********************************************************************************
*
*  FILE NAME    : portable.h
*
*  DESCRIPTION: The header file required for portability			
*                
*
*    DATE    NAME    REFERENCE          REASON
*
*  3-OCT-10  mayank                Initial creation
*
*  
*  Copyright 2010, Aricent Technologies (Holdings) Ltd
*
*******************************************************************************/

#ifndef _PORTABLE
#define _PORTABLE

typedef unsigned char UCHAR;
typedef signed char SCHAR;

#ifdef OS_64

typedef unsigned short U32_INT;
typedef signed short S32_INT;

#else

typedef unsigned int U32_INT;
typedef signed int S32_INT;

#endif

#endif
