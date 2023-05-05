/******************************************************************************
 * @file    main.c
 * @author  Mohammad Mohsen <kuro.ece@gmail.com>
 * @brief 
 * @version 0.1
 * @date    2023-01-31
 * 
 * @copyright Copyright (c) 2023
 * 
 *****************************************************************************/

#include <stddef.h>
#include <stdint.h>

#include "main.h"

#include "TinyBL.h"


/* ------------------------------------------------------------------------- */
/* Private Defintions & Macros */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Datatypes */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Functions Declarations */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Variables */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Functions Definitions */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Main */
/* ------------------------------------------------------------------------- */


int main(void)
{
    /* Initialize system clock */
    /* Initialize NVIC */
    /* Initialize used components (clocks, GPIO, UART) */
    /* System_vidInitialize(); */
    
    /* dma initialization */
    /* uart initialization */
    /* timer initialization */
    /* interrupts initialization */
    /* Platform_vidInitialize() */

    /* bootloader transport initialization */
    /* bootloader protocol initialization */
    /* bootloader initialization */
    /* TinyBL_vidInitialize(); */

    while(1)
    {
        /* get state */

        /* switch(state) */

        /* case idle */
            /* tinybl :: connect */
        
        /* case connected */
            /* receive data */
            /* if program data */
                /* if valid ---> confirm data */
                /* else ---> report error */
            /* if disconnect */
        
        /* case connection timeout */
            /* disconnect */
        
        /* default */
            /*  */
    }
}
