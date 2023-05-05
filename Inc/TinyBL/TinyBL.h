/******************************************************************************
 * @file      TinyBL.h
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

#ifndef _TinyBL_H_
#define _TinyBL_H_


#ifdef __cplusplus
extern "C" 
{
#endif /* __cplusplus */


/* ------------------------------------------------------------------------- */
/* Public Definitions & Macros */
/* ------------------------------------------------------------------------- */

/**
 * @brief status flags bits
 */
#define TINYBL_STATUS_FLAG_BIT_READY            ((TinyBL_StatusFlags_t)0)
#define TINYBL_STATUS_FLAG_BIT_PROCESS_ERROR    ((TinyBL_StatusFlags_t)1)
#define TINYBL_STATUS_FLAG_BIT_ADDRESS_ERROR    ((TinyBL_StatusFlags_t)2)
#define TINYBL_STATUS_FLAG_BIT_SIZE_ERROR       ((TinyBL_StatusFlags_t)3)
#define TINYBL_STATUS_FLAG_BIT_CRC_ERROR        ((TinyBL_StatusFlags_t)4)
#define TINYBL_STATUS_FLAG_BIT_WRITE_ERROR      ((TinyBL_StatusFlags_t)5)
#define TINYBL_STATUS_FLAG_BIT_MEMORY_ERROR     ((TinyBL_StatusFlags_t)6)

/**
 * @brief status flags
 */
#define TINYBL_STATUS_FLAG_READY                ((TinyBL_StatusFlags_t)1 << TINYBL_STATUS_FLAG_BIT_READY)
#define TINYBL_STATUS_FLAG_PROCESS_ERROR        ((TinyBL_StatusFlags_t)1 << TINYBL_STATUS_FLAG_BIT_PROCESS_ERROR)
#define TINYBL_STATUS_FLAG_ADDRESS_ERROR        ((TinyBL_StatusFlags_t)1 << TINYBL_STATUS_FLAG_BIT_ADDRESS_ERROR)
#define TINYBL_STATUS_FLAG_SIZE_ERROR           ((TinyBL_StatusFlags_t)1 << TINYBL_STATUS_FLAG_BIT_SIZE_ERROR)
#define TINYBL_STATUS_FLAG_CRC_ERROR            ((TinyBL_StatusFlags_t)1 << TINYBL_STATUS_FLAG_BIT_CRC_ERROR)
#define TINYBL_STATUS_FLAG_WRITE_ERROR          ((TinyBL_StatusFlags_t)1 << TINYBL_STATUS_FLAG_BIT_WRITE_ERROR)
#define TINYBL_STATUS_FLAG_MEMORY_ERROR         ((TinyBL_StatusFlags_t)1 << TINYBL_STATUS_FLAG_BIT_MEMORY_ERROR)

/**
 * @brief status flags masks
 */
#define TINYBL_STATUS_FLAG_MASK_READY           (~TINYBL_STATUS_FLAG_READY)
#define TINYBL_STATUS_FLAG_MASK_PROCESS_ERROR   (~TINYBL_STATUS_FLAG_PROCESS_ERROR)
#define TINYBL_STATUS_FLAG_MASK_ADDRESS_ERROR   (~TINYBL_STATUS_FLAG_ADDRESS_ERROR)
#define TINYBL_STATUS_FLAG_MASK_SIZE_ERROR      (~TINYBL_STATUS_FLAG_SIZE_ERROR)
#define TINYBL_STATUS_FLAG_MASK_CRC_ERROR       (~TINYBL_STATUS_FLAG_CRC_ERROR)
#define TINYBL_STATUS_FLAG_MASK_WRITE_ERROR     (~TINYBL_STATUS_FLAG_WRITE_ERROR)
#define TINYBL_STATUS_FLAG_MASK_MEMORY_ERROR    (~TINYBL_STATUS_FLAG_MEMORY_ERROR)


/* ------------------------------------------------------------------------- */
/* Public Types */
/* ------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 */
typedef uint8_t TinyBL_StatusFlags_t;

/**
 * @brief 
 * 
 */
typedef enum TinyBL_State_t {
    TINYBL_STATE_INIT              ,    /**< Initialize client/bootloader */
    TINYBL_STATE_CONNECT           ,    /**< client tries to connect to bootloader */
    TINYBL_STATE_WAIT_CONNECTION   ,    /**< bootloader/client waits for connection */
    TINYBL_STATE_CONNECTED         ,    /**< bootloader/client is connected (and idle) */
    TINYBL_STATE_RECEIVE_PROGRAM   ,    /**< bootloader is receiving program data */
    TINYBL_STATE_WRITE_PROGRAM_DATA,    /**< bootloader is writing program data */
    TINYBL_STATE_CLOSE             ,    /**< bootloader/client is closing connection */
    TINYBL_STATE_EXIT              ,    /**< bootloader/client exits the bootloader/client */
    TINYBL_STATE_REPORT_ERROR      ,    /**< bootloader/client error reporting */
} TinyBL_State_t;

/**
 * @brief 
 * 
 */
typedef enum TinyBL_Connection_Status_t {
    TINYBL_CONNECTION_SUCCESS,
    TINYBL_CONNECTION_TIMEOUT,
} TinyBL_Connection_Status_t;

/* ------------------------------------------------------------------------- */
/* Public API Declarations */
/* ------------------------------------------------------------------------- */

/******************************************************************************
 * @brief Initialization & de-initialization
 *****************************************************************************/

/**
 * @brief 
 * 
 * @param [in] pu8DataBuffer 
 * @param [in] u32BufferSize 
 */
void TinyBL_vidInitialize(uint8_t * pu8DataBuffer, uint32_t u32BufferSize);

/**
 * @brief 
 * 
 */
void TinyBL_vidDeInitialize(void);

/******************************************************************************
 * @brief Getters
 *****************************************************************************/

/**
 * @brief 
 * 
 * @return TinyBL_State_t 
 */
TinyBL_State_t TinyBL_tGetState(void);

/**
 * @brief 
 * 
 * @return TinyBL_StatusFlags_t 
 */
TinyBL_StatusFlags_t TinyBL_tGetStatusFlags(void);

/**
 * @brief 
 * 
 * @param [in] pu8Message 
 * @param [in] pu32Len 
 */
void TinyBL_vidGetLastMessage(uint8_t * pu8Message, uint32_t * pu32Len);

/******************************************************************************
 * @brief Callbacks
 *****************************************************************************/

/**
 * @brief 
 * 
 * @param [in] tConnectionStatus 
 */
void TinyBL_vidOnConnect(TinyBL_Connection_Status_t tConnectionStatus);

/**
 * @brief 
 * 
 * @param [in] pu8Message 
 * @param [in] u32Size 
 */
void TinyBL_vidOnMessage(const uint8_t * const pu8Message, uint32_t u32Size);

/**
 * @brief 
 * 
 */
void TinyBL_vidOnDisconnect(void);

/* ------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 * @param [in] tWriteStatus 
 * @param [in] u32Adress 
 * @param [in] pu8Data 
 * @param [in] u32Size 
 */
void TinyBL_vidOnWrite(Flash_Write_Status_t tWriteStatus, uint32_t u32Adress, const uint8_t * const pu8Data, uint32_t u32Size);

/******************************************************************************
 * @brief Functional APIs
 *****************************************************************************/

/**
 * @brief 
 * 
 * @param [in] pu8Message 
 * @param [in] u32Size 
 */
void TinyBL_vidSendMessage(const uint8_t * const pu8Message, uint32_t u32Size);


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _TinyBL_H_ */

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */


