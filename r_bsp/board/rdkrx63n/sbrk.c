/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name	   : sbrk.c
* Version      : 1.0 
* Device(s)    : RX
* Description  : Configures the MCU heap memory.  The size of the heap is defined by the macro HEAPSIZE below.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 26.10.2011 1.00     First Release
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Provides standard definitions used in this file */
#include <stddef.h>
/* Defines standard input/output functions used in this file */
#include <stdio.h>
/* Defines standard variable types used in this file */
#include <stdint.h>

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Size of area managed by sbrk */
#define HEAPSIZE 0x400

/***********************************************************************************************************************
Function Prototypes
***********************************************************************************************************************/
/* Memory allocation function prototype declaration */
int8_t  *sbrk(size_t size);

/***********************************************************************************************************************
Global Variables
***********************************************************************************************************************/
//const size_t _sbrk_size=      /* Specifies the minimum unit of */
/* the defined heap area */
extern int8_t *_s1ptr;

union HEAP_TYPE
{
    int32_t  dummy;             /* Dummy for 4-byte boundary */
    int8_t heap[HEAPSIZE];      /* Declaration of the area managed by sbrk*/
};
/* Declare memory heap area */
static union HEAP_TYPE heap_area;
/* End address allocated by sbrk    */
static int8_t *brk=(int8_t *)&heap_area;

/***********************************************************************************************************************
* Function name: sbrk
* Description  : This function configures MCU memory area allocation.
* Arguments    : size - 
*                    assigned area size
* Return value : Start address of allocated area (pass)
*                -1 (failure)
***********************************************************************************************************************/
int8_t  *sbrk(size_t size)                      
{
    int8_t  *p;

    if (brk+size > heap_area.heap+HEAPSIZE)
    {
        /* Empty area size  */
        p = (int8_t *)-1;
    }
    else
    {
        /* Area assignment */
        p = brk;  

        /* End address update */                           
        brk += size;                           
    }

    /* Return result */
    return p;
}
