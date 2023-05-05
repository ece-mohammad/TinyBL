/******************************************************************************
 * @file      CRC32.h
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
 *            furnished to do so, subject to the following conditions:
 *            The above copyright notice and this permission notice shall be
 *            included in all copies or substantial portions of the Software.
 *            THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *            EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 *            MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *            NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *            HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *            WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *            OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 *            DEALINGS IN THE SOFTWARE.
 *            
 *            
 ******************************************************************************/

#ifndef _CRC32_H_
#define _CRC32_H_

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
 * @brief 
 * 
 */
typedef enum CRC32_Error_t {
    CRC32_ERROR_NONE,            /**< No error */
    CRC32_ERROR_NULLPTR,         /**< An unexpected null pointer */
    CRC32_ERROR_INVALID_PARAM,   /**< An invalid parameter value */
} CRC32_Error_t;

/* ------------------------------------------------------------------------- */
/* Public API Declarations */
/* ------------------------------------------------------------------------- */

/**
 * @brief Calculate CRC32 over given data buffer
 *
 * @param [in]  pu8DataBuffer pointer to data buffer bytes
 * @param [in]  u32Len Data buffer length
 * @param [out] pu32Result Pointer to a variable where the current CRC32 result
 * will be stored
 * @param [in]  pu32Prev Previous CRC32 value to use to update CRC32
 * value (when, for example, calculating CRC32 for partial data or a byte-steam)
 *
 * @return #CRC32_Error_t
 *
 */
CRC32_Error_t CRC32_tCalculateCrc(const uint8_t * const pu8DataBuffer, uint32_t u32Len, uint32_t * pu32Result, uint32_t u32Prev);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _CRC32_H_ */

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */


