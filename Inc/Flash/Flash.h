/******************************************************************************
 * @file      Flash.h
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

#ifndef _Flash_H_
#define _Flash_H_

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
 * @brief Flash error code
 */
typedef enum Flash_Error_t {
    FLASH_ERROR_NONE,               /**< No error */
    FLASH_ERROR_NULLPTR,            /**< Unexpected null pointer */
    FLASH_ERROR_INVALID_PARAM,      /**< Invalid parameter value */
    FLASH_ERROR_INVALID_ADDRESS,    /**< Invalid address value */
    FLASH_ERROR_SIZE,               /**< Invalid data size */
    FLASH_ERROR_WRITE_ERROR,        /**< Write failed */
    FLASH_ERROR_MEMORY,             /**< Memory error */
} Flash_Error_t;

/**
 * @brief 
 */
typedef enum Flash_Write_Status_t {
    FLASH_WRITE_STATUS_NONE = -1,   /**< No status (default value) */
    FLASH_WRITE_STATUS_OK,          /**< Write was successful */
    FLASH_WRITE_STATUS_ERROR,       /**< Write failed */
} Flash_Write_Status_t;

/**
 * @brief Write callback, called after memory write is completed
 * 
 * @param [in] tWriteStatus write status #Flash_Write_Status_t
 * @param [in] u32Adress memory address of last write
 * @param [in] pu8Data pointer to the last data write
 * @param [in] u32Size size of the last data write
 */
typedef void (*Flash_pfWriteCallback_t)(Flash_Write_Status_t tWriteStatus, uint32_t u32Adress, const uint8_t * const pu8Data, uint32_t u32Size);

/* ------------------------------------------------------------------------- */
/* Public API Declarations */
/* ------------------------------------------------------------------------- */


/**
 * @brief Initialize flash memory
 * 
 * @param [in] u32StartAddress Flash memory's start address
 * @param [in] u32PageSize Flash page size
 * @param [in] u32NumOfPages Flash number of pages/blocks
 * 
 * @note u32StartAddress, u32PageSize and u32NumOfPages are used to validate addresss of read/write
 * calls. A call is valid if :
 * - flash start address <= start address < flash end address
 * - and flash start address <= (start address + size) <= flash end address
 * 
 * @return Flash_Error_t 
 */
Flash_Error_t Flash_tInitialize(uint32_t u32StartAddress, uint32_t u32PageSize, uint32_t u32NumOfPages);

/**
 * @brief Set's flash write callback
 * 
 * @param [in] pfWriteCallback pointer to the flash write callback function
 * 
 * @return Flash_Error_t 
 */
Flash_Error_t Flash_tSetWriteCallback(Flash_pfWriteCallback_t pfWriteCallback);

/**
 * @brief Deinitialize flash memory
 * 
 */
void Flash_vidDeInitialize(void);

/**
 * @brief read data from flash memory
 * 
 * @param [in]  u32Adress memory address of the data to be read
 * @param [out] pu8Buffer pointer to the buffer where read data will be written
 * @param [in]  u32Size size of data to read (number of bytes)
 * 
 * @return Flash_Error_t 
 */
Flash_Error_t Flash_tRead(uint32_t u32Adress, uint8_t * pu8Buffer, uint32_t u32Size);

/**
 * @brief write data to the flash memory
 * 
 * @param [in] u32Adress memory address where data will bwe written
 * @param [in] pu8Data pointer to the data to be written
 * @param [in] u32Size size of data to write
 * 
 * @return Flash_Error_t 
 */
Flash_Error_t Flash_tWrite(uint32_t u32Adress, const uint8_t * const pu8Data, uint32_t u32Size);

/**
 * @brief erase a page/block of flash memory
 * 
 * @param [in] u32Page page/block's number 0 <= u32Page < flash number of pages
 * 
 * @return Flash_Error_t 
 */
Flash_Error_t Flash_tErasePage(uint32_t u32Page);

/**
 * @brief Erase all flash contents
 * 
 */
void Flash_vidMassErase(void);

/**
 * @brief 
 * 
 * @param [in] tWriteStatus 
 * @param [in] u32Adress 
 * @param [in] pu8Data 
 * @param [in] u32Size 
 */
void Flash_vidOnWriteComplete(Flash_Write_Status_t tWriteStatus, uint32_t u32Adress, const uint8_t * const pu8Data, uint32_t u32Size);

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t Flash_u32GetStartAddress(void);

/**
 * @brief 
 * 
 * @return uint32_t 
 */
uint32_t Flash_u32GetMemorySize(void);

/**
 * @brief 
 * 
 * @param [in] u32Address 
 * @param [in] u32Size 
 * 
 * @return uint8_t 
 */
uint8_t Flash_u8IsValidProgramAddressAndSize(uint32_t u32Address, uint32_t u32Size);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _Flash_H_ */

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */


