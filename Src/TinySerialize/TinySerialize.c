/******************************************************************************
 * @file      TinySerialize.c
 * @brief     
 * @author    Author name <email@org.com>
 * @date      2023/04/11
 * @copyright Copyright 2023, Author name <email@org.com>
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
#include <string.h>

#include "TinySerialize.h"


/* ------------------------------------------------------------------------- */
/* Private Definitions & Macros */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Types */
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
/* Public API Implementations */
/* ------------------------------------------------------------------------- */

void TinySerialize_vidSerialUint32(const uint32_t * const pu32Var, uint8_t * const pu8DeserializedMessage)
{
    uint32_t Local_u32Index = 0;
    uint8_t * Local_pu8VarByte = NULL;
    
    for(Local_u32Index = 0, Local_pu8VarByte = (uint8_t *)pu32Var; Local_u32Index < sizeof(uint32_t); Local_u32Index++, Local_pu8VarByte++)
    {
        pu8DeserializedMessage[Local_u32Index] = (*Local_pu8VarByte);
    }
}

/* ------------------------------------------------------------------------- */

void TinySerialize_vidDeserializedUint32(uint32_t * const pu32Var, const uint8_t * const pu8SerializedMessage)
{
    uint32_t Local_u32Index = 0;
    uint8_t * Local_pu8VarByte = NULL;
    
    for(Local_u32Index = 0, Local_pu8VarByte = (uint8_t *)pu32Var; Local_u32Index < sizeof(uint32_t); Local_u32Index++, Local_pu8VarByte++)
    {
        (*Local_pu8VarByte) = pu8SerializedMessage[Local_u32Index];
    }

    memcpy(pu32Var, pu8SerializedMessage, 4);
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

