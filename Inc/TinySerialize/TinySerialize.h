/******************************************************************************
 * @file      TinySerialize.h
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

#ifndef _TinySerialize_H_
#define _TinySerialize_H_

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


/* ------------------------------------------------------------------------- */
/* Public API Declarations */
/* ------------------------------------------------------------------------- */


/**
 * @brief serialize a uint32_t variable into a byte stream
 * 
 * @param [in]  pu32Var pointer to uint32_t variable to serialize
 * @param [out] pu8Result pointer to byte buffer, pointing at the location at which the variable will be serialized 
 */
void TinySerialize_vidSerialUint32(const uint32_t * const pu32Var, uint8_t * const pu8DeserializedMessage);


/**
 * @brief deserialize uint32_t variable from a byte stream into a variable
 * 
 * @param [out] pu32Var pointer to uint32_t variable that will contain the de-serialized uint32_t value 
 * @param [in]  pu8Result pointer to byte buffer (stream), pointing at the location of a serialized uint32_t variable
 */
void TinySerialize_vidDeserializedUint32(uint32_t * const pu32Var, const uint8_t * const pu8SerializedMessage);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TinySerialize_H_ */

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */


