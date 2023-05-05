/******************************************************************************
 * @file      Flash.c
 * @brief     
 * @author    Author name <email@org.com>
 * @date      2023/03/30
 * @copyright Copyright 2023, Author name <email@org.com>
 *            Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *            The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *            
 *            
 ******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "Flash.h"


/* ------------------------------------------------------------------------- */
/* Private Definitions & Macros */
/* ------------------------------------------------------------------------- */

#ifndef IN_RANGE
#define IN_RANGE(v, s, e)   (((v) <= (s)) && ((v) <= (e)))
#endif /* IN_RANGE */

/* ------------------------------------------------------------------------- */
/* Private Types */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Functions Declarations */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Variables */
/* ------------------------------------------------------------------------- */

static uint8_t * Flash_pu8Memory                    = NULL;
static uint32_t Flash_u32StartAddress               = 0;
static uint32_t Flash_u32PageSize                   = 0;
static uint32_t Flash_u32NumOfPages                 = 0;
static Flash_pfWriteCallback_t Flash_pfWriteCallback = NULL;

/* ------------------------------------------------------------------------- */
/* Private Functions Definitions */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Public API Implementations */
/* ------------------------------------------------------------------------- */

Flash_Error_t Flash_tInitialize(uint32_t u32StartAddress, uint32_t u32PageSize, uint32_t u32NumOfPages)
{
    if(u32PageSize == 0 || u32NumOfPages == 0)
    {
        return FLASH_ERROR_INVALID_PARAM;
    }

    Flash_pu8Memory = calloc(sizeof(*Flash_pu8Memory), u32PageSize * u32NumOfPages);

    if(Flash_pu8Memory)
    {
        Flash_u32StartAddress = u32StartAddress;
        Flash_u32PageSize     = u32PageSize;
        Flash_u32NumOfPages   = u32NumOfPages;
        return FLASH_ERROR_NONE;
    }

    return FLASH_ERROR_MEMORY;
}

/* ------------------------------------------------------------------------- */

Flash_Error_t Flash_tSetWriteCallback(Flash_pfWriteCallback_t pfWriteCallback)
{
    if(pfWriteCallback == NULL)
    {
        return FLASH_ERROR_NULLPTR;
    }

    Flash_pfWriteCallback = pfWriteCallback;

    return FLASH_ERROR_NONE;
}

/* ------------------------------------------------------------------------- */

void Flash_vidDeInitialize(void)
{
    free(Flash_pu8Memory);

    Flash_u32StartAddress = 0;
    Flash_u32PageSize     = 0;
    Flash_u32NumOfPages   = 0;
    Flash_pfWriteCallback = NULL;
    Flash_pu8Memory       = NULL;
}

/* ------------------------------------------------------------------------- */

Flash_Error_t Flash_tRead(uint32_t u32Address, uint8_t * pu8Buffer, uint32_t u32Size)
{
    if(Flash_pu8Memory == NULL)
    {
        return FLASH_ERROR_MEMORY;
    }

    if(pu8Buffer == NULL)
    {
        return FLASH_ERROR_NULLPTR;
    }

    if(u32Size == 0)
    {
        return FLASH_ERROR_INVALID_PARAM;
    }

    if((u32Address < Flash_u32StartAddress) ||
        (u32Address >= (Flash_u32StartAddress + (Flash_u32NumOfPages * Flash_u32PageSize))))
    {
        return FLASH_ERROR_INVALID_ADDRESS;
    }

    if((u32Address + u32Size) > (Flash_u32StartAddress + (Flash_u32NumOfPages * Flash_u32PageSize)))
    {
        return FLASH_ERROR_SIZE;
    }

    memcpy(pu8Buffer, Flash_pu8Memory + (u32Address - Flash_u32StartAddress), u32Size);

    return FLASH_ERROR_NONE;
}

/* ------------------------------------------------------------------------- */

Flash_Error_t Flash_tWrite(uint32_t u32Address, const uint8_t * const pu8Data, uint32_t u32Size)
{
    if(Flash_pu8Memory == NULL)
    {
        return FLASH_ERROR_MEMORY;
    }

    if(pu8Data == NULL)
    {
        return FLASH_ERROR_NULLPTR;
    }

    if(u32Size == 0)
    {
        return FLASH_ERROR_INVALID_PARAM;
    }

    if((u32Address < Flash_u32StartAddress) ||
        (u32Address >= (Flash_u32StartAddress + (Flash_u32NumOfPages * Flash_u32PageSize))))
    {
        return FLASH_ERROR_INVALID_ADDRESS;
    }

    if((u32Address + u32Size) > (Flash_u32StartAddress + (Flash_u32NumOfPages * Flash_u32PageSize)))
    {
        return FLASH_ERROR_SIZE;
    }
    
    memcpy(Flash_pu8Memory + (u32Address - Flash_u32StartAddress), pu8Data, u32Size);

    Flash_vidOnWriteComplete(FLASH_WRITE_STATUS_OK, u32Address, pu8Data, u32Size);

    return FLASH_ERROR_NONE;
}

/* ------------------------------------------------------------------------- */

Flash_Error_t Flash_tErasePage(uint32_t u32Page)
{
    if(Flash_pu8Memory == NULL)
    {
        return FLASH_ERROR_MEMORY;
    }

    if(!IN_RANGE(u32Page, 0, Flash_u32NumOfPages - 1))
    {
        return FLASH_ERROR_INVALID_PARAM;
    }

    memset(Flash_pu8Memory + (u32Page * Flash_u32PageSize), 0x00, Flash_u32PageSize);

    return FLASH_ERROR_NONE;
}

/* ------------------------------------------------------------------------- */

void Flash_vidMassErase(void)
{
    if(Flash_pu8Memory == NULL)
    {
        return;
    }

    memset(Flash_pu8Memory, 0x00, Flash_u32PageSize * Flash_u32NumOfPages);
}

/* ------------------------------------------------------------------------- */

void Flash_vidOnWriteComplete(Flash_Write_Status_t tWriteStatus, uint32_t u32Address, const uint8_t * const pu8Data, uint32_t u32Size)
{
    if(Flash_pfWriteCallback)
    {
        Flash_pfWriteCallback(tWriteStatus, u32Address, pu8Data, u32Size);
    }
}

/* ------------------------------------------------------------------------- */

uint32_t Flash_u32GetStartAddress(void)
{
    return Flash_u32StartAddress;
}

/* ------------------------------------------------------------------------- */

uint32_t Flash_u32GetMemorySize(void)
{
    return (Flash_u32NumOfPages * Flash_u32PageSize);
}

/* ------------------------------------------------------------------------- */

uint8_t Flash_u8IsValidProgramAddressAndSize(uint32_t u32Address, uint32_t u32Size)
{
    if((u32Address < Flash_u32StartAddress) ||
        (u32Address >= (Flash_u32StartAddress + (Flash_u32NumOfPages * Flash_u32PageSize))))
    {
        return 0;
    }

    if((u32Address + u32Size) > (Flash_u32StartAddress + (Flash_u32NumOfPages * Flash_u32PageSize)))
    {
        return 0;
    }

    if(u32Size == 0)
    {
        return 0;
    }

    return 1;
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

