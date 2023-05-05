/******************************************************************************
 * @file      Fletcher16.h
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

#ifndef _Fletcher16_H_
#define _Fletcher16_H_

#ifdef __cplusplus
extern "C" 
{
#endif /* __cplusplus */


/* ------------------------------------------------------------------------- */
/* Public Definitions & Macros */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Public Types */
/* ------------------------------------------------------------------------- */

/**
 * @brief Fletcher16 result type
 */
typedef struct Fletcher16_t {
    uint16_t checksum;
    uint16_t check_bytes;
}Fletcher16_t;


/* ------------------------------------------------------------------------- */
/* Public API Declarations */
/* ------------------------------------------------------------------------- */

/**
 * @brief Calculate Fletcher16 checksum for 
 * 
 * @param [in] pu8Data 
 * @param [in] u32Len 
 * @param [in] tInitial 
 * 
 * @return Fletcher16_t 
 */
Fletcher16_t Fletcher16_tCalculate(const uint8_t * const pu8Data, uint32_t u32Len, uint16_t u16Initial);

uint8_t Fletcher16_u8Verify(const uint8_t * const pu8Data, uint32_t u32Len);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _Fletcher16_H_ */

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */


