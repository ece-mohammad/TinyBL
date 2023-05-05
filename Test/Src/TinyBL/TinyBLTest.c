/******************************************************************************
 * @file      TinyBLTest.c
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
#include <stdbool.h>
#include <string.h>

#include "unity.h"
#include "unity_fixture.h"

#include "config.h"

#include "Flash.h"
#include "TinyMessage.h"
#include "TinyBLTest.h"

/* ------------------------------------------------------------------------- */
/* Test Definitions & Macros */
/* ------------------------------------------------------------------------- */
#define MAX_MESSAGE_SIZE                260

#define PROGRAM_DATA_ADDRESS            (FLASH_MEMORY_START_ADDRESS + 0)
/* 
#define FLASH_MEMORY_SIZE               (120 * 1024)
#define FLASH_MEMORY_START_ADDRESS      0x08002000
#define FLASH_MEMORY_END_ADDRESS        (FLASH_MEMORY_START_ADDRESS + FLASH_MEMORY_SIZE - 1)
 */

/* ------------------------------------------------------------------------- */
/* Test Private Variables */
/* ------------------------------------------------------------------------- */
static uint8_t  DataBuffer [MAX_MESSAGE_SIZE] = {0};

/* ------------------------------------------------------------------------- */
/* Messages used in test */
/* ------------------------------------------------------------------------- */

static const uint8_t TestMessage [] = {0xEF, 0xBE, 0xAD, 0xDE};
static const uint8_t ProgramData [] = {0x4A, 0x65, 0x6C, 0x6C, 0x6F, 0x76, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x0D, 0x0A, 0x00};   /* "Jellov world!\r\n"; */

/* program request */
static const TinyMessage_Request_Program_t ProgramRequest = {
    .address = PROGRAM_DATA_ADDRESS,
    .size = 100 << 10,
    .checksum = 0xDEADBEEF
};

/* program data request */
static const TinyMessage_Request_Program_Data_t ProgramDataRequest = {
    .address = PROGRAM_DATA_ADDRESS,
    .size = sizeof(ProgramData),
    .data = (uint8_t *)ProgramData
};

#if 0 
static const uint8_t ProgramDataRequest [] = {
    TINY_MESSAGE_TYPE_PROGRAM_DATA_REQUEST,   /* type */
    0x10, 0x00, 0x00, 0x00,                   /* program data message id */
    0x10, 0x00, 0x00, 0x00,                   /* program data size */
    0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x0A, 0x0B, 0x0C, 0x0C, 0x0D, 0x0E, 0x0F  /* program data */
};
#endif /* 0 */

/* ------------------------------------------------------------------------- */

static const TinyMessage_Request_Program_t ProgramMessage_AddressBeforeMemoryStart = {
    .address = FLASH_MEMORY_START_ADDRESS - 1,
    .size = 100 << 10,
    .checksum = 0xDEADBEEF
};

static const TinyMessage_Request_Program_t ProgramMessage_AddressAfterMemoryEnd = {
    .address = FLASH_MEMORY_END_ADDRESS + 1,
    .size = 100 << 10,
    .checksum = 0xDEADBEEF
};

static const TinyMessage_Request_Program_t ProgramMessage_ZeroSize = {
    .address = 0x08002000,
    .size = 0x00,
    .checksum = 0xDEADBEEF
};

static const TinyMessage_Request_Program_t ProgramMessage_InvalidSize = {
    .address = 0x08002000,
    .size = FLASH_MEMORY_SIZE + 1,
    .checksum = 0xDEADBEEF
};


/* ------------------------------------------------------------------------- */
/* Helper functions prototypes  */
/* ------------------------------------------------------------------------- */

/**
 * @brief initialize tinybl 
 */
static void InitTinyBL(void);

/**
 * @brief 
 */
static void InitBoootloader(void);

/**
 * @brief 
 */
static void DeInitBoootloader(void);

/**
 * @brief
 */
static void ConnectAndProgramRequest(const TinyMessage_Request_Program_t * const psProgramRequest);
/**
 * @brief
 */
static void OnProgramData(const TinyMessage_Request_Program_Data_t * const psProgramData);

/* ------------------------------------------------------------------------- */
/* Test group TinyBLInit */
/* ------------------------------------------------------------------------- */
TEST_GROUP(TinyBL_Bootloader_Init);


TEST_SETUP(TinyBL_Bootloader_Init)
{
}

TEST_TEAR_DOWN(TinyBL_Bootloader_Init)
{
}

/* ------------------------------------------------------------------------- */
/* Test group TinyBL */
/* ------------------------------------------------------------------------- */
TEST_GROUP(TinyBL_Bootloader);


TEST_SETUP(TinyBL_Bootloader)
{
    InitBoootloader();

    memset(DataBuffer, 0x00, sizeof(DataBuffer));
}

TEST_TEAR_DOWN(TinyBL_Bootloader)
{
    DeInitBoootloader();
}

/* ------------------------------------------------------------------------- */
/* Helper functions */
/* ------------------------------------------------------------------------- */

/******************************************************************************
 * @brief Assertions
 *****************************************************************************/
static void AssertState(const TinyBL_State_t tState)
{
    TEST_ASSERT_EQUAL_UINT8(tState, TinyBL_tGetState());
}

static void AssertStatusFlagIsSet(const TinyBL_StatusFlags_t tStatusFlagMask)
{
    TEST_ASSERT_EQUAL(tStatusFlagMask, tStatusFlagMask & TinyBL_tGetStatusFlags());
}

static void AssertStatusFlagIsReset(const TinyBL_StatusFlags_t tStatusFlagMask)
{
    TEST_ASSERT_EQUAL(0x00, tStatusFlagMask & TinyBL_tGetStatusFlags());
}

static void AssertStatusFlagsEqual(const TinyBL_StatusFlags_t tStatusFlags)
{
    TEST_ASSERT_EQUAL_UINT8(tStatusFlags, TinyBL_tGetStatusFlags());
}

static void AssertLastReceivedMessage(const uint8_t * const pu8ExpectedMessage, const uint32_t u32ExpectedLen)
{
    uint8_t Local_au8Message [MAX_MESSAGE_SIZE] = {0};
    uint32_t Local_u32MessageLen = 0;

    TinyBL_vidGetLastMessage(Local_au8Message, &Local_u32MessageLen);
    TEST_ASSERT_EQUAL_UINT32(u32ExpectedLen, Local_u32MessageLen);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(pu8ExpectedMessage, Local_au8Message, u32ExpectedLen);
}

/******************************************************************************
 * @brief helper functions
 *****************************************************************************/

static void InitTinyBL(void)
{
    TinyBL_vidInitialize(DataBuffer, MAX_MESSAGE_SIZE);
}

static void InitBoootloader(void)
{
    InitTinyBL();
    Flash_tInitialize(FLASH_MEMORY_START_ADDRESS, FLASH_MEMORY_PAGE_SIZE, FLASH_MEMORY_PAGES);
    Flash_tSetWriteCallback(TinyBL_vidOnWrite);
}

static void DeInitBoootloader(void)
{
    TinyBL_vidDeInitialize();
    Flash_vidDeInitialize();
}

static void ConnectAndProgramRequest(const TinyMessage_Request_Program_t * const psProgramRequest)
{
    uint32_t Local_u32MessageLen = 0;

    TinyBL_vidOnConnect(TINYBL_CONNECTION_SUCCESS);

    Local_u32MessageLen = TinyMessage_u32SerializeProgramRequest(DataBuffer, psProgramRequest);
    TinyBL_vidOnMessage(DataBuffer, Local_u32MessageLen);
}

static void OnProgramData(const TinyMessage_Request_Program_Data_t * const psProgramData)
{
    uint32_t  Local_u32MessageLen = 0;

    Local_u32MessageLen = TinyMessage_u32SerializeProgramDataRequest(DataBuffer, psProgramData);
    TinyBL_vidOnMessage(DataBuffer, Local_u32MessageLen);
}

/* ------------------------------------------------------------------------- */
/* Test cases for TinyBLInit test group */
/* ------------------------------------------------------------------------- */

/**
 * @brief state
 */
TEST(TinyBL_Bootloader_Init, StateIsInitBeforeInitialization)
{
    AssertState(TINYBL_STATE_INIT);
}

TEST(TinyBL_Bootloader_Init, InitChangesStateToListen)
{
    InitTinyBL();
    AssertState(TINYBL_STATE_WAIT_CONNECTION);
}

TEST(TinyBL_Bootloader_Init, DeInitializeChangesStateToInit)
{
    InitTinyBL();
    TinyBL_vidDeInitialize();
    AssertState(TINYBL_STATE_INIT);
}

/**
 * @brief status flags
 */
TEST(TinyBL_Bootloader_Init, InitSetsStatusFlagReady)
{
    InitTinyBL();
    AssertStatusFlagIsSet(TINYBL_STATUS_FLAG_READY);
}

TEST(TinyBL_Bootloader_Init, DeInitializeClearsAllStatusFlags)
{
    InitTinyBL();
    TinyBL_vidDeInitialize();
    AssertStatusFlagsEqual(0x00);
}

/* ------------------------------------------------------------------------- */
/* Test cases for TinyBL test group */
/* ------------------------------------------------------------------------- */

/**
 * @brief send message
 * 
 */
TEST(TinyBL_Bootloader, SendMessageWritesToDataBuffer)
{
    TinyBL_vidSendMessage(TestMessage, sizeof(TestMessage));
    TEST_ASSERT_EQUAL_UINT8_ARRAY(TestMessage, DataBuffer, sizeof(TestMessage));
}

/**
 * @brief on connect
 * 
 */
TEST(TinyBL_Bootloader, OnConnectSuccessChangesStateFromListenToConnected)
{
    AssertState(TINYBL_STATE_WAIT_CONNECTION);
    TinyBL_vidOnConnect(TINYBL_CONNECTION_SUCCESS);
    AssertState(TINYBL_STATE_CONNECTED);
}

TEST(TinyBL_Bootloader, OnConnectTimeoutChangesStateFromListenToExit)
{
    AssertState(TINYBL_STATE_WAIT_CONNECTION);
    TinyBL_vidOnConnect(TINYBL_CONNECTION_TIMEOUT);
    AssertState(TINYBL_STATE_EXIT);
}

/**
 * @brief onMessage callback
 */
TEST(TinyBL_Bootloader, OnMessageCallbackSetsLastMessage)
{
    TinyBL_vidOnMessage(TestMessage, sizeof(TestMessage));
    AssertLastReceivedMessage(TestMessage, sizeof(TestMessage));
}

/**
 * @brief onMessage :: Program Message
 */
TEST(TinyBL_Bootloader, ValidProgramMessageChangesStateToReceiveProgram)
{
    ConnectAndProgramRequest(&ProgramRequest);
    AssertState(TINYBL_STATE_RECEIVE_PROGRAM);
}

TEST(TinyBL_Bootloader, ProgramMessageAddressBeforeMemoryStartChangeStateToExit)
{
    ConnectAndProgramRequest(&ProgramMessage_AddressBeforeMemoryStart);
    AssertState(TINYBL_STATE_EXIT);
}

TEST(TinyBL_Bootloader, ProgramMessageAddressAfterMemoryEndChangeStateToExit)
{
    ConnectAndProgramRequest(&ProgramMessage_AddressAfterMemoryEnd);
    AssertState(TINYBL_STATE_EXIT);
}

TEST(TinyBL_Bootloader, ProgramMessageZeroSizeChangeStateToExit)
{
    ConnectAndProgramRequest(&ProgramMessage_ZeroSize);
    AssertState(TINYBL_STATE_EXIT);
}

TEST(TinyBL_Bootloader, ProgramMessageInvalidSizeChangeStateToExit)
{
    ConnectAndProgramRequest(&ProgramMessage_InvalidSize);
    AssertState(TINYBL_STATE_EXIT);
}

/**
 * @brief onMessage :: Program Data Message
 */
TEST(TinyBL_Bootloader, ProgramDataMessageChangesStateToWriteProgram)
{
    ConnectAndProgramRequest(&ProgramRequest);
    OnProgramData(&ProgramDataRequest);

    AssertState(TINYBL_STATE_WRITE_PROGRAM_DATA);
}

TEST(TinyBL_Bootloader, ProgramDataMessageWriteDataToMemory)
{
    uint8_t Local_au8ReadBuffer [MAX_MESSAGE_SIZE] = {0};

    ConnectAndProgramRequest(&ProgramRequest);
    OnProgramData(&ProgramDataRequest);

    Flash_tRead(
        ProgramDataRequest.address,
        Local_au8ReadBuffer,
        ProgramDataRequest.size
    );

    TEST_ASSERT_EQUAL_UINT8_ARRAY(ProgramDataRequest.data, Local_au8ReadBuffer, ProgramDataRequest.size);
}

IGNORE_TEST(TinyBL_Bootloader, FullProgramWriteChangesStateToExit)
{
    ConnectAndProgramRequest(&ProgramRequest);
    OnProgramData(&ProgramDataRequest);
    AssertState(TINYBL_STATE_CLOSE);
}

IGNORE_TEST(TinyBL_Bootloader, PartialProgramDataWriteSuccessChangesStateToReceiveProgram)
{
    TEST_FAIL_MESSAGE("Not implemented!");
}

/**
 * @brief onMessage :: Status Request Message
 */
IGNORE_TEST(TinyBL_Bootloader, OnStatusRequestSendsCurrentStatusFlags)
{
    TEST_FAIL_MESSAGE("Not implemented!");
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

