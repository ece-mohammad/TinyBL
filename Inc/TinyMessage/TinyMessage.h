/******************************************************************************
 * @file      TinyMessage.h
 * @brief
 * @author    Author name <email@org.com>
 * @date      2023/04/12
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

#ifndef _TinyMessage_H_
#define _TinyMessage_H_

#ifdef __cplusplus
extern "C" 
{
#endif /* __cplusplus */


/* ------------------------------------------------------------------------- */
/* Public Definitions & Macros */
/* ------------------------------------------------------------------------- */

#define TINY_MESSAGE_REQUEST_TYPE                     0

/* ------------------------------------------------------------------------- */
/* Public Types */
/* ------------------------------------------------------------------------- */

/**
 * @brief Message Types
 * 
 */
typedef enum TinyMessage_Message_Type_t {
    TINY_MESSAGE_TYPE_NONE,
    TINY_MESSAGE_TYPE_PROGRAM_REQUEST,
    TINY_MESSAGE_TYPE_PROGRAM_RESPONSE,
    TINY_MESSAGE_TYPE_PROGRAM_DATA_REQUEST,
    TINY_MESSAGE_TYPE_PROGRAM_DATA_RESPONSE,
    TINY_MESSAGE_TYPE_STATUS_REQUEST,
    TINY_MESSAGE_TYPE_STATUS_RESPONSE,
} TinyMessage_Message_Type_t;

/**
 * @brief 
 * 
 */
typedef struct TinyMessage_Request_Program_t {
    uint32_t address;
    uint32_t size;
    uint32_t checksum;
} TinyMessage_Request_Program_t;

/**
 * @brief 
 * 
 */
typedef struct TinyMessage_Request_Program_Data_t {
    uint32_t  address;
    uint32_t  size;
    uint8_t * data;
} TinyMessage_Request_Program_Data_t;


/* ------------------------------------------------------------------------- */
/* Public API Declarations */
/* ------------------------------------------------------------------------- */


/******************************************************************************
 * @brief Message (request/response) Serialization
 *****************************************************************************/

/**
 * @brief 
 * 
 * @param [out] pu8SerializedProgramRequest 
 * @param [in]  psRequestData 
 * 
 * @return uint32_t
 */
uint32_t TinyMessage_u32SerializeProgramRequest(uint8_t * pu8SerializedProgramRequest, const TinyMessage_Request_Program_t * const psRequestData);

/**
 * @brief 
 * 
 * @param [out] pu8SerializedProgramDataRequest 
 * @param [in]  psRequestData 
 * 
 * @return uint32_t 
 */
uint32_t TinyMessage_u32SerializeProgramDataRequest(uint8_t * pu8SerializedProgramDataRequest, const TinyMessage_Request_Program_Data_t * const psRequestData);


/******************************************************************************
 * @brief Message (request/response) De-serialization (parsing)
 *****************************************************************************/

/**
 * @brief 
 * 
 * @param [in]  pu8Message 
 * @param [in]  u32Size 
 * @param [out] psMessageData 
 * 
 * @return uint8_t 
 */
uint8_t TinyMessage_u8ParseProgramRequest(const uint8_t * const pu8Message, uint32_t u32Size, TinyMessage_Request_Program_t * const psProgramRequest);

/**
 * @brief 
 * 
 * @param [in]  pu8Message 
 * @param [in]  u32Size 
 * @param [out] psProgramDataRequest 
 * 
 * @return uint8_t 
 */
uint8_t TinyMessage_u8ParseProgramDataRequest(const uint8_t * const pu8Message, uint32_t u32Size, TinyMessage_Request_Program_Data_t * const psProgramDataRequest);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TinyMessage_H_ */

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */


