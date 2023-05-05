/******************************************************************************
 * @file      CRC32.c
 * @brief     
 * @author    Mohammad Mohsen <kuro.ece@gmail.com>
 * @date      2023/02/13
 * @copyright Copyright 2023, Mohammad Mohsen <kuro.ece@gmail.com>
 *            Permission is hereby granted, free of charge, to any person
 *            obtaining a copy of this software and associated documentation
 *            files (the "Software"), to deal in the Software without
 *            restriction, including without limitation the rights to use, copy,
 *            modify, merge, publish, distribute, sublicense, and/or sell copies
 *            of the Software, and to permit persons to whom the Software is
 *            furnished to do so, subject to the following conditions: The above
 *            copyright notice and this permission notice shall be included in
 *            all copies or substantial portions of the Software. THE SOFTWARE
 *            IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *            IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *            MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *            OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *            DEALINGS IN THE SOFTWARE.
 *            
 *            
 ******************************************************************************/

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "CRC32.h"

#include <stdio.h>

/* -------------------------------------------------------------------------- */
/* Module Preprocessor Constants */
/* -------------------------------------------------------------------------- */

/**
 * @brief CRC32 options
 */

/**
 * @brief CRC32 constants
 * polynomial:
 *  x^{{32}}+x^{{26}}+x^{{23}}+x^{{22}}+x^{{16}}+x^{{12}}+x^{{11}}+x^{{10}}+x^{8}+x^{7}+x^{5}+x^{4}+x^{2}+x+1
 */
#define CRC32_POLYNOMIAL        0x04C11DB7
#define CRC32_INITIAL_VALUE     0xFFFFFFFFUL
#define CRC32_OUTPUT_XOR        0xFFFFFFFFUL

/**
 * @brief CRC32 lookup table size
 */
#define CRC_LUT_SIZE            256

/* -------------------------------------------------------------------------- */

#define LAST_BIT_MASK           (1ul << 31)
#define FIRST_BIT_MASK          (1)

/* -------------------------------------------------------------------------- */

/**
 * @brief CRC32 flags
 */
#define CRC32_BYTE_TABLE_FLAG    (1 << 0)
#define CRC32_CRC_TABLE_FLAG     (1 << 1)

/* -------------------------------------------------------------------------- */
/* Module Preprocessor Macros */
/* -------------------------------------------------------------------------- */


/* -------------------------------------------------------------------------- */
/* Module Typedefs */
/* -------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 */
typedef enum CRC32_ReflectReverse_t {
    CRC32_NO_REFLECT_REVERSE,  /**<  CRC32 do not reflect data bits of input/output or reverse bits of data/polynomial or CRC  */
    CRC32_REFLECT_INPUT     ,  /**<  reflect input data bits  */
    CRC32_REVERSE_OUTPUT    ,  /**<  reverse output crc bits  */
    CRC32_REFLECT_REVERSE   ,  /**<  reflect input data bits and reverse output crc  */
} CRC32_ReflectReverse_t;

/**
 * @brief 
 */
typedef struct CRC32_Descriptor_t {
    char                   * name           ;  /**< CRC32 name  */
    uint32_t                 polynomial     ;  /**< CRC32 polynomial  */
    uint32_t                 initial        ;  /**< CRC32 inital value  */
    uint32_t                 input_xor      ;  /**< CRC32 value XORed to input */
    uint32_t                 output_xor     ;  /**< CRC32 value XORed to output */
    CRC32_ReflectReverse_t   reflect_reverse;  /**< CRC32 reflect input & output or  */
} CRC32_Descriptor_t;


/* -------------------------------------------------------------------------- */
/* Module Variable Definitions */
/* -------------------------------------------------------------------------- */

static uint32_t CRC32_au32CrcTable [CRC_LUT_SIZE] = {0};
static uint8_t CRC32_u8HasTable = 0;


/* -------------------------------------------------------------------------- */
/* Private Function Prototypes */
/* -------------------------------------------------------------------------- */

/**
 * @brief Generate lookup table used for CRC32 computation
 * 
 * @note sets #CRC32_CRC_TABLE_FLAG flag in #CRC32_u8HasTable
 */
static void CRC32_vidPopulateCrcTable(void);

/* -------------------------------------------------------------------------- */
/* Private Function Definitions */
/* -------------------------------------------------------------------------- */


static void CRC32_vidPopulateCrcTable(void)
{
    const uint32_t Local_u32Polynomial = CRC32_POLYNOMIAL;
    uint32_t Local_u32Remainder = 0;
    uint16_t Local_u16ByteValue = 0;

    if((CRC32_u8HasTable & CRC32_CRC_TABLE_FLAG) == 0)
    {
        for(Local_u16ByteValue = 0; Local_u16ByteValue < 256; Local_u16ByteValue++)
        {
            Local_u32Remainder = (((uint8_t)Local_u16ByteValue & 0xFF) << 24);

            for(uint8_t Local_u8Bit  = 0; Local_u8Bit  < 8; Local_u8Bit ++ )
            {
                if( Local_u32Remainder & LAST_BIT_MASK)
                {
                    Local_u32Remainder = (Local_u32Remainder << 1) ^ Local_u32Polynomial;
                }
                else
                {
                    Local_u32Remainder = (Local_u32Remainder << 1);
                }
            }

            CRC32_au32CrcTable[(uint8_t)Local_u16ByteValue] = Local_u32Remainder;
        }

        CRC32_u8HasTable |= CRC32_CRC_TABLE_FLAG;
    }
}

/* -------------------------------------------------------------------------- */
/* Public Function Definitions */
/* -------------------------------------------------------------------------- */

CRC32_Error_t CRC32_tCalculateCrc(const uint8_t * const pu8DataBuffer, uint32_t u32Len, uint32_t * pu32Result, uint32_t u32Prev)
{
    const uint8_t * Local_pu8Buffer = (const uint8_t*) pu8DataBuffer;
    uint32_t Local_u32Crc = 0;

    if((Local_pu8Buffer == NULL) || (pu32Result == NULL))
    {
        return CRC32_ERROR_NULLPTR;
    }

    if(u32Len == 0)
    {
        return CRC32_ERROR_INVALID_PARAM;
    }

    if((CRC32_u8HasTable & CRC32_CRC_TABLE_FLAG) == 0)
    {
        CRC32_vidPopulateCrcTable();
        CRC32_u8HasTable |= CRC32_CRC_TABLE_FLAG;
    }

    if(u32Prev)
    {
        Local_u32Crc = u32Prev ^ CRC32_OUTPUT_XOR;
    }
    else
    {
        Local_u32Crc = CRC32_INITIAL_VALUE;
    }

    while(u32Len--)
    {
        Local_u32Crc = CRC32_au32CrcTable[(uint8_t)((Local_u32Crc >> 24) ^ (*Local_pu8Buffer))] ^ (Local_u32Crc << 8);
        Local_pu8Buffer++;
    }

    Local_u32Crc ^= CRC32_OUTPUT_XOR;

    (*pu32Result) = Local_u32Crc;

    return CRC32_ERROR_NONE;
}

/* ------------------------------------------------------------------------- */

#if 0 //* DEBUGGING PURPOSE *//

static void CRC32_vidPrintTable(void)
{
    uint32_t Local_u32Cols = 8;
    uint32_t Local_u32Rows = sizeof(CRC32_au32CrcTable)/(sizeof(CRC32_au32CrcTable[0])*Local_u32Cols);
    uint32_t Local_u32Index = 0;

    printf("Lookup table:\n");
    for(uint32_t Local_u32CurrentRow = 0; Local_u32CurrentRow < Local_u32Rows; Local_u32CurrentRow++)
    {
        for(uint32_t Local_u32CurrentCol = 0; Local_u32CurrentCol < Local_u32Cols; Local_u32CurrentCol++)
        {
            Local_u32Index = Local_u32CurrentCol + (Local_u32CurrentRow * Local_u32Cols);
            printf("0x%08X, ", CRC32_au32CrcTable[Local_u32Index]);
        }
        printf("\n");
    }

    printf("---------------------------------------\n");
}

#endif /* 0 */

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

