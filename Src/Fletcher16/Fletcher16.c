/******************************************************************************
 * @file      Fletcher16.c
 * @brief
 * @author    Mohammad Mohsen <kuro.ece@gmail.com>
 * @date      2023/02/13
 * @copyright Copyright 2023, Mohammad Mohsen <kuro.ece@gmail.com>
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

#include "Fletcher16.h"

/* ------------------------------------------------------------------------- */
/* Private Definitions & Macros */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Private Types */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Private Functions Declarations */
/* ------------------------------------------------------------------------- */

/**
 * @brief Generate check bytes for given fletcher-16 checksum.
 *        Check bytes are 16-bit unsigned integer, when appended to
 *        a message (with LSB first), the fletcher 16 checksum of the 
 *        appended message is zero
 * 
 * @param [in] u16Checksum 
 * 
 * @return uint16_t 
 */
static uint16_t Fletcher16_u16CheckBytes(uint16_t u16Checksum);

/* ------------------------------------------------------------------------- */
/* Private Variables */
/* ------------------------------------------------------------------------- */

/* ------------------------------------------------------------------------- */
/* Private Functions Definitions */
/* ------------------------------------------------------------------------- */

static uint16_t Fletcher16_u16CheckBytes(uint16_t u16Checksum)
{
    uint16_t Local_u16ChecksumLSB = ((u16Checksum >> 0) & 0xFF);
    uint16_t Local_u16ChecksumMSB = ((u16Checksum >> 8) & 0xFF);
    uint16_t Local_u16CheckBytesLSB = 0;
    uint16_t Local_u16CheckBytesMSB = 0;

    Local_u16CheckBytesLSB = 0xFF - ((Local_u16ChecksumLSB + Local_u16ChecksumMSB) % 0xFF);
    Local_u16CheckBytesMSB = 0xFF - ((Local_u16ChecksumLSB + Local_u16CheckBytesLSB) % 0xFF);
    
    return ((Local_u16CheckBytesMSB << 8) | Local_u16CheckBytesLSB);
}

/* ------------------------------------------------------------------------- */
/* Public API Implementations */
/* ------------------------------------------------------------------------- */

Fletcher16_t Fletcher16_tCalculate(const uint8_t * const pu8Data, uint32_t u32Len, uint16_t u16Initial)
{
    uint16_t Local_u16Mod256Sum = ((u16Initial >> 0) & 0xFF);
    uint16_t Local_u16SumOfSums = ((u16Initial >> 8) & 0xFF);
    uint16_t Local_u16Checksum = 0;
    uint32_t Local_u32Index;
    
    for (Local_u32Index = 0; Local_u32Index < u32Len; ++Local_u32Index)
    {
        Local_u16Mod256Sum = (Local_u16Mod256Sum + pu8Data[Local_u32Index]) % 0xFF;
        Local_u16SumOfSums = (Local_u16SumOfSums + Local_u16Mod256Sum) % 0xFF;
    }

    Local_u16Checksum = (Local_u16SumOfSums << 8) | Local_u16Mod256Sum;

    return (Fletcher16_t){
        .checksum = Local_u16Checksum, 
        .check_bytes=Fletcher16_u16CheckBytes(Local_u16Checksum)
    };
}

/* ------------------------------------------------------------------------- */

uint8_t Fletcher16_u8Verify(const uint8_t * const pu8Data, uint32_t u32Len)
{
    return (Fletcher16_tCalculate(pu8Data, u32Len, 0x00).checksum == 0x00);
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */
