/******************************************************************************
 * @file      FlashTest.c
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

#include "unity.h"
#include "unity_fixture.h"

#include "config.h"
#include "FlashTest.h"


/* ------------------------------------------------------------------------- */
/* Test Definitions & Macros */
/* ------------------------------------------------------------------------- */
#define FLASH_START_ADDRESS         0x08002000
#define FLASH_PAGE_SIZE_BYTES       1024
#define FLASH_NUM_OF_PAGES          120
#define FLASH_SIZE                  (FLASH_PAGE_SIZE_BYTES * FLASH_NUM_OF_PAGES)

/* program addresses */
#define ADDRESS_BEFORE_FLASH_START  (FLASH_START_ADDRESS - 1)
#define FLASH_LAST_ADDRESS          (FLASH_START_ADDRESS + FLASH_SIZE - 1)
#define ADDRESS_AFTER_FLASH_END     (FLASH_LAST_ADDRESS + 1)

/* program sizes */
#define ZERO_PROGRAM_SIZE           0
#define PROGRAM_SIZE_GT_FLASH_SIZE  (FLASH_SIZE + 1)

/* flash pages */
#define FLASH_PAGE_NUMBER           0
#define FLASH_PAGE_ADDRESS          FLASH_START_ADDRESS
#define INVALID_FLASH_PAGE_NUMBER   FLASH_NUM_OF_PAGES


#define INVALID_PAGE_SIZE           0
#define INVALID_NUM_OF_PAGES        0

/* ------------------------------------------------------------------------- */
/* Test Private Variables */
/* ------------------------------------------------------------------------- */

static const uint8_t Test_au8Data [FLASH_SIZE] = \
    "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Cras nibh nunc, "\
    "laoreet et malesuada id, vehicula ut ipsum. In faucibus dolor sapien, " \
    "id dapibus felis molestie vitae. Morbi leo magna, pretium eget velit a, " \
    "sollicitudin auctor neque. Aenean id ex ipsum. Donec nec nulla nulla. " \
    "Integer est mi, faucibus dictum fermentum et, accumsan consequat est. " \
    "Phasellus tempor augue sed nunc fermentum euismod. " \
    "Integer nec ligula nec sapien ultricies venenatis vel ac est. " \
    "Quisque vel nunc sit amet turpis aliquam elementum quis et justo. " \
    "Fusce gravida aliquam urna, at condimentum lorem. " \
    "Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. " \
    "Aliquam sit amet quam elit. " \
    "Nulla ac interdum nunc. " \
    "Cras id ipsum sit amet eros aliquet hendrerit. " \
    "Etiam semper erat quis elit faucibus tincidunt. " \
    "Vestibulum pellentesque nunc ac est cursus, a laoreet ligula luctus. " \
    "Fusce lobortis eros dolor, venenatis sagittis turpis congue eu.";

static const uint32_t Test_u32Address = FLASH_START_ADDRESS;

static uint8_t * TEST_pu8ReadBuffer = NULL;

static Flash_Write_Status_t Test_tWriteStatus;
static uint32_t  TEST_u32LastWriteAddress = 0;
static uint32_t  TEST_u32LastWriteSize = 0;
static uint8_t * TEST_pu8LastWriteData = 0;

/* ------------------------------------------------------------------------- */
/* Helper functions prototypes  */
/* ------------------------------------------------------------------------- */

/**
 * @brief Flash write callback, called after flash write is completed
 * 
 * @param [in] tWriteStatus flash write status #Flash_Write_Status_t
 * @param [in] u32Adress flash address of last write
 * @param [in] pu8Data pointer to the last written data 
 * @param [in] u32Size size of the last written data
 */
static void WriteCallback(Flash_Write_Status_t tWriteStatus, uint32_t u32Adress, const uint8_t * const pu8Data, uint32_t u32Size);

/**
 * @brief write data to flash and check that write was successful
 * 
 * @param [in] u32Address address to write data to
 * @param [in] pu8Data data to write
 * @param [in] u32Len size of data (number of bytes)
 */
static void Given(uint32_t u32Address, const uint8_t * const pu8Data, uint32_t u32Len);

/**
 * @brief verifies that data is located in the flash, by reading the flash at specific address and 
 * comparing the read data with the given data
 * 
 * @param [in] u32Address flash address where the data is located
 * @param [in] pu8Data data to check
 * @param [in] u32Len size of the data
 */
static void VerifyFlashContents(uint32_t u32Address, const uint8_t * const pu8Data, uint32_t u32Len);

/* ------------------------------------------------------------------------- */
/* Helper functions Definitions */
/* ------------------------------------------------------------------------- */

static void WriteCallback(Flash_Write_Status_t tWriteStatus, uint32_t u32Adress, const uint8_t * const pu8Data, uint32_t u32Size)
{
    Test_tWriteStatus = tWriteStatus;
    TEST_u32LastWriteAddress = u32Adress;
    TEST_u32LastWriteSize = u32Size;
    TEST_pu8LastWriteData = (uint8_t *)pu8Data;

}

static void Given(uint32_t u32Address, const uint8_t * const pu8Data, uint32_t u32Len)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NONE, 
        Flash_tWrite(u32Address, pu8Data, u32Len)
    );

    TEST_ASSERT_EQUAL(FLASH_WRITE_STATUS_OK, Test_tWriteStatus);
    TEST_ASSERT_EQUAL(u32Address, TEST_u32LastWriteAddress);
    TEST_ASSERT_EQUAL(u32Len, TEST_u32LastWriteSize);
    TEST_ASSERT_EQUAL(pu8Data, TEST_pu8LastWriteData);
}

static void VerifyFlashContents(uint32_t u32Address, const uint8_t * const pu8DataAtAddress, uint32_t u32Len)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NONE, 
        Flash_tRead(u32Address, TEST_pu8ReadBuffer, u32Len)
    );

    TEST_ASSERT_EQUAL_UINT8_ARRAY(pu8DataAtAddress, TEST_pu8ReadBuffer, u32Len);
}

/* ------------------------------------------------------------------------- */
/* Test Group - Flash Init */
/* Tests flash API calls before flash is initialized */
/* ------------------------------------------------------------------------- */
TEST_GROUP(FlashInit);


TEST_SETUP(FlashInit)
{
}

TEST_TEAR_DOWN(FlashInit)
{
    Flash_vidDeInitialize();
}

/* ------------------------------------------------------------------------- */
/* Test Group - Flash DeInit */
/* Tests flash API calls after flash is deinitialized */
/* ------------------------------------------------------------------------- */
TEST_GROUP(FlashDeInit);


TEST_SETUP(FlashDeInit)
{
    Flash_vidDeInitialize();
}

TEST_TEAR_DOWN(FlashDeInit)
{
}

/* ------------------------------------------------------------------------- */
/* Test Group - Flash */
/* Test behavior of flash APIs */
/* ------------------------------------------------------------------------- */
TEST_GROUP(Flash);


TEST_SETUP(Flash)
{
    Flash_tInitialize(FLASH_START_ADDRESS, FLASH_PAGE_SIZE_BYTES, FLASH_NUM_OF_PAGES);
    Flash_tSetWriteCallback(WriteCallback);

    Test_tWriteStatus = FLASH_WRITE_STATUS_NONE;
    TEST_u32LastWriteAddress = 0;
    TEST_u32LastWriteSize = 0;
    TEST_pu8LastWriteData = NULL;

    TEST_pu8ReadBuffer = calloc(sizeof(uint8_t), FLASH_SIZE);
}

TEST_TEAR_DOWN(Flash)
{
    Flash_vidDeInitialize();

    free(TEST_pu8ReadBuffer);
    TEST_pu8ReadBuffer = NULL;
}

/* ------------------------------------------------------------------------- */
/* Test Cases - Flash Init */
/* ------------------------------------------------------------------------- */

/**
 * @brief Flash_tInitialize()
 */
TEST(FlashInit, InitWithValidParamsReturnNoError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NONE, 
        Flash_tInitialize(FLASH_START_ADDRESS, FLASH_PAGE_SIZE_BYTES, FLASH_NUM_OF_PAGES)
    );
}

TEST(FlashInit, InitWithZeroPageSizeReturnInvalidParamError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_PARAM, 
        Flash_tInitialize(FLASH_START_ADDRESS, INVALID_PAGE_SIZE, FLASH_NUM_OF_PAGES)
    );
}

TEST(FlashInit, InitWithZeroNumOfPagesReturnInvalidParamError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_PARAM, 
        Flash_tInitialize(FLASH_START_ADDRESS, FLASH_PAGE_SIZE_BYTES, INVALID_NUM_OF_PAGES)
    );
}

/* ------------------------------------------------------------------------- */

TEST(FlashInit, ReadBeforeInitReturnMemoryError)
{
    TEST_pu8ReadBuffer = calloc(sizeof(*TEST_pu8ReadBuffer), FLASH_PAGE_SIZE_BYTES);

    TEST_ASSERT_EQUAL(
        FLASH_ERROR_MEMORY, 
        Flash_tRead(FLASH_START_ADDRESS, TEST_pu8ReadBuffer, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(FlashInit, WriteBeforeInitReturnMemoryError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_MEMORY, 
        Flash_tWrite(FLASH_START_ADDRESS, Test_au8Data, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(FlashInit, PageEraseBeforeInitReturnMemoryError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_MEMORY, 
        Flash_tErasePage(FLASH_PAGE_NUMBER)
    );
}

TEST(FlashInit, MassEraseBeforeInitDoesNotCauseError)
{
    Flash_vidMassErase();
}

/******************************************************************************
 * @brief Flash_tSetCallback()
 *****************************************************************************/
TEST(FlashInit, SetWriteCallbackWithNullReturnsNullError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NULLPTR, 
        Flash_tSetWriteCallback(NULL)
    );
}

TEST(FlashInit, SetWriteCallbackReturnsNoError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NONE, 
        Flash_tSetWriteCallback(WriteCallback)
    );
}

/* ------------------------------------------------------------------------- */
/* Test Cases - Flash DeInit */
/* ------------------------------------------------------------------------- */

TEST(FlashDeInit, ReadAfterDeInitReturnMemoryError)
{
    TEST_pu8ReadBuffer = calloc(sizeof(*TEST_pu8ReadBuffer), FLASH_PAGE_SIZE_BYTES);

    TEST_ASSERT_EQUAL(
        FLASH_ERROR_MEMORY, 
        Flash_tRead(FLASH_START_ADDRESS, TEST_pu8ReadBuffer, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(FlashDeInit, WriteAfterDeInitReturnMemoryError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_MEMORY, 
        Flash_tWrite(FLASH_START_ADDRESS, Test_au8Data, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(FlashDeInit, PageEraseAfterDeInitReturnMemoryError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_MEMORY, 
        Flash_tErasePage(FLASH_PAGE_NUMBER)
    );
}

TEST(FlashDeInit, MassEraseAfterDeInitDoesNotCauseError)
{
    Flash_vidMassErase();
}

/* ------------------------------------------------------------------------- */
/* Test Cases - Flash */
/* ------------------------------------------------------------------------- */
TEST(Flash, ReadAndWriteToValidAddress)
{
    Given(Test_u32Address, Test_au8Data, FLASH_PAGE_SIZE_BYTES);
    VerifyFlashContents(Test_u32Address, Test_au8Data, FLASH_PAGE_SIZE_BYTES);
}

/******************************************************************************
 * @brief Flash_tWrite()
 *****************************************************************************/

TEST(Flash, WriteNullDataReturnNullError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NULLPTR,
        Flash_tWrite(FLASH_START_ADDRESS, NULL, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(Flash, WriteWithZeroSizeDataReturnInvalidParamError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_PARAM,
        Flash_tWrite(FLASH_START_ADDRESS, Test_au8Data, ZERO_PROGRAM_SIZE)
    );
}

/* ------------------------------------------------------------------------- */

TEST(Flash, WriteToAddressBeforeMemoryStartReturnInvalidAddressError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_ADDRESS,
        Flash_tWrite(ADDRESS_BEFORE_FLASH_START, Test_au8Data, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(Flash, WriteToAddressAfterMemoryEndReturnInvalidAddressError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_ADDRESS,
        Flash_tWrite(ADDRESS_AFTER_FLASH_END, Test_au8Data, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(Flash, WriteOutsideMemoryReturnInvalidAddressError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_SIZE,
        Flash_tWrite(FLASH_START_ADDRESS, Test_au8Data, PROGRAM_SIZE_GT_FLASH_SIZE)
    );
}

TEST(Flash, WriteLargeDataReturnNoError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NONE,
        Flash_tWrite(FLASH_START_ADDRESS, Test_au8Data, FLASH_SIZE)
    );
}

/******************************************************************************
 * @brief Flash_tRead()
 *****************************************************************************/

TEST(Flash, ReadWithNullBufferReturnNullError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NULLPTR,
        Flash_tRead(FLASH_START_ADDRESS, NULL, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(Flash, ReadWithZeroSizeBufferReturnInvalidParamError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_PARAM,
        Flash_tRead(FLASH_START_ADDRESS, TEST_pu8ReadBuffer, ZERO_PROGRAM_SIZE)
    );
}

/* ------------------------------------------------------------------------- */

TEST(Flash, ReadFromAddressBeforeFlashStartReturnInvalidAddressError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_ADDRESS,
        Flash_tRead(ADDRESS_BEFORE_FLASH_START, TEST_pu8ReadBuffer, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(Flash, ReadFromAddressAfterFlashEndReturnInvalidAddressError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_ADDRESS,
        Flash_tRead(ADDRESS_AFTER_FLASH_END, TEST_pu8ReadBuffer, FLASH_PAGE_SIZE_BYTES)
    );
}

TEST(Flash, ReadOutsideFlashEndReturnSizeError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_SIZE,
        Flash_tRead(FLASH_START_ADDRESS, TEST_pu8ReadBuffer, PROGRAM_SIZE_GT_FLASH_SIZE)
    );
}

TEST(Flash, ReadLargeDataReturnNoError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NONE,
        Flash_tRead(FLASH_START_ADDRESS, TEST_pu8ReadBuffer, FLASH_SIZE)
    );
}

/******************************************************************************
 * @brief Flash_tErasePage()
 *****************************************************************************/

TEST(Flash, ErasePageClearsPageContent)
{
    uint8_t Local_au8Zeros [FLASH_PAGE_SIZE_BYTES] = {0};

    Given(FLASH_START_ADDRESS, Test_au8Data, FLASH_SIZE);

    TEST_ASSERT_EQUAL(
        FLASH_ERROR_NONE,
        Flash_tErasePage(FLASH_PAGE_NUMBER)
    );

    VerifyFlashContents(FLASH_PAGE_ADDRESS, Local_au8Zeros, FLASH_PAGE_SIZE_BYTES);
}

TEST(Flash, ErasePageWithInvalidPageNumberReturnInvalidParamError)
{
    TEST_ASSERT_EQUAL(
        FLASH_ERROR_INVALID_PARAM,
        Flash_tErasePage(INVALID_FLASH_PAGE_NUMBER)
    );
}

/******************************************************************************
 * @brief Flash_vidMassErase()
 *****************************************************************************/

TEST(Flash, MassEraseClearsFlashContents)
{
    uint8_t Local_au8Zeros [FLASH_SIZE] = {0};

    Given(FLASH_START_ADDRESS, Test_au8Data, FLASH_SIZE);
    Flash_vidMassErase();
    
    VerifyFlashContents(FLASH_START_ADDRESS, Local_au8Zeros, FLASH_SIZE);
}

/******************************************************************************
 * @brief Flash_vidMassErase()
 *****************************************************************************/

TEST(Flash, StartAddress)
{
    TEST_ASSERT_EQUAL_UINT32(
        FLASH_START_ADDRESS,
        Flash_u32GetStartAddress()
    );
}

/******************************************************************************
 * @brief Flash_vidMassErase()
 *****************************************************************************/

TEST(Flash, MemorySize)
{
    TEST_ASSERT_EQUAL_UINT32(
        FLASH_SIZE,
        Flash_u32GetMemorySize()
    );
}

/******************************************************************************
 * @brief Flash_vidMassErase()
 *****************************************************************************/

TEST(Flash, ValidProgram)
{
    TEST_ASSERT_TRUE(
        Flash_u8IsValidProgramAddressAndSize(FLASH_START_ADDRESS, FLASH_SIZE)
    );

    TEST_ASSERT_TRUE(
        Flash_u8IsValidProgramAddressAndSize(FLASH_START_ADDRESS + 1, FLASH_SIZE - 1)
    );

    TEST_ASSERT_TRUE(
        Flash_u8IsValidProgramAddressAndSize(FLASH_LAST_ADDRESS, 1)
    );
}

TEST(Flash, ValidProgram_Address)
{
    /* start address != 0 */
    TEST_ASSERT_NOT_EQUAL(0, FLASH_START_ADDRESS);

    /* zero address */
    TEST_ASSERT_FALSE(
        Flash_u8IsValidProgramAddressAndSize(0, FLASH_SIZE)
    );

    /* address before flash start address */
    TEST_ASSERT_FALSE(
        Flash_u8IsValidProgramAddressAndSize(ADDRESS_BEFORE_FLASH_START, FLASH_SIZE)
    );

    /* address after flash end */
    TEST_ASSERT_FALSE(
        Flash_u8IsValidProgramAddressAndSize(ADDRESS_AFTER_FLASH_END, FLASH_SIZE)
    );
}

TEST(Flash, ValidProgram_Size)
{
    /* zero size program */
    TEST_ASSERT_FALSE(
        Flash_u8IsValidProgramAddressAndSize(FLASH_START_ADDRESS, ZERO_PROGRAM_SIZE)
    );

    /* program size > flash size */
    TEST_ASSERT_FALSE(
        Flash_u8IsValidProgramAddressAndSize(FLASH_START_ADDRESS, PROGRAM_SIZE_GT_FLASH_SIZE)
    );

    /* available flash memory < program size < flash size */
    TEST_ASSERT_FALSE(
        Flash_u8IsValidProgramAddressAndSize(FLASH_START_ADDRESS + 1, FLASH_SIZE)
    );
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

