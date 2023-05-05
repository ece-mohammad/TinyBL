/******************************************************************************
 * @file      TinyMessageTest.c
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

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "unity_fixture.h"

#include "TinyMessageTest.h"

/*****************************************************************************/
/* Private variables */
/*****************************************************************************/

/******************************************************************************
 * @brief Program Request
 *****************************************************************************/
#define PROGRAM_ADDRESS     0x08020000
#define PROGRAM_SIZE        1024
#define PROGRAM_CHECKSUM    0xDEADBEEF
#define BYTE_STREAM_SIZE    256 /* must be large enough to fit largest request */

static const TinyMessage_Request_Program_t ProgramRequest = {
    .address = PROGRAM_ADDRESS,
    .size = PROGRAM_SIZE,
    .checksum = PROGRAM_CHECKSUM,
};

static const uint8_t SerializedProgramRequest [] = {
    TINY_MESSAGE_TYPE_PROGRAM_REQUEST,
    0x00, 0x00, 0x02, 0x08, /* address */
    0x00, 0x04, 0x00, 0x00, /* size */
    0xEF, 0xBE, 0xAD, 0xDE, /* checksum */
};

#define SERIALIZED_PROGRAM_REQUEST_SIZE         sizeof(SerializedProgramRequest)

/******************************************************************************
 * @brief Program DataRequest
 *****************************************************************************/

#define PROGRAM_DATA_OFFSET     0
#define PROGRAM_DATA_SIZE       0
#define PROGRAM_DATA_CHECKSUM   0
/* must be large enough to fit largest request */

static const uint8_t ProgramData [] = {0x4A, 0x65, 0x6C, 0x6C, 0x6F, 0x76, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x0D, 0x0A, 0x00};   /* "Jellov world!\r\n"; */

static const TinyMessage_Request_Program_Data_t ProgramDataRequest = {
    .address = 0x00000000,
    .size   = 0x00000010,
    .data   = ProgramData,
};

static const uint8_t SerializedProgramDataRequest [] = {
    TINY_MESSAGE_TYPE_PROGRAM_DATA_REQUEST,
    0x00, 0x00, 0x00, 0x00, /* address */
    0x10, 0x00, 0x00, 0x00, /* size */
    0x4A, 0x65, 0x6C, 0x6C, 0x6F, 0x76, 0x20, 0x77, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x0D, 0x0A, 0x00 /* data */
};

#define PROGRAM_DATA_SIZE                       sizeof(ProgramData)
#define SERIALIZED_PROGRAM_DATA_REQUEST_SIZE    sizeof(SerializedProgramDataRequest)


static uint8_t * ByteStream = NULL;

/******************************************************************************
 * @brief Test Group - TinyMessage
 *****************************************************************************/

TEST_GROUP(TinyMessage);

TEST_SETUP(TinyMessage)
{
    ByteStream = calloc(sizeof(uint8_t), BYTE_STREAM_SIZE);
}

TEST_TEAR_DOWN(TinyMessage)
{
    if(ByteStream)
    {
        free(ByteStream);
        ByteStream = NULL;
    }
}

/******************************************************************************
 * @brief Test Cases - TinyMessage
 *****************************************************************************/

TEST(TinyMessage, SerializeProgramRequest)
{
    uint32_t Local_u32Bytes = 0;

    Local_u32Bytes = TinyMessage_u32SerializeProgramRequest(ByteStream, &ProgramRequest);

    TEST_ASSERT_EQUAL_UINT32(SERIALIZED_PROGRAM_REQUEST_SIZE, Local_u32Bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(SerializedProgramRequest, ByteStream, SERIALIZED_PROGRAM_REQUEST_SIZE);
}

TEST(TinyMessage, ParseProgramRequest)
{
    TinyMessage_Request_Program_t Local_sProgramRequest = {0};

    TinyMessage_u8ParseProgramRequest(SerializedProgramRequest, SERIALIZED_PROGRAM_REQUEST_SIZE, &Local_sProgramRequest);
    
    TEST_ASSERT_EQUAL_UINT32(ProgramRequest.address, Local_sProgramRequest.address);
    TEST_ASSERT_EQUAL_UINT32(ProgramRequest.size, Local_sProgramRequest.size);
    TEST_ASSERT_EQUAL_UINT32(ProgramRequest.checksum, Local_sProgramRequest.checksum);
}

TEST(TinyMessage, SerializeProgramDataRequest)
{
    uint32_t Local_u32Bytes = 0;

    Local_u32Bytes = TinyMessage_u32SerializeProgramDataRequest(ByteStream, &ProgramDataRequest);

    TEST_ASSERT_EQUAL_UINT32(SERIALIZED_PROGRAM_DATA_REQUEST_SIZE, Local_u32Bytes);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(SerializedProgramDataRequest, ByteStream, SERIALIZED_PROGRAM_DATA_REQUEST_SIZE);
}

TEST(TinyMessage, ParseProgramDataRequest)
{
    TinyMessage_Request_Program_Data_t Local_sProgramdataRequest = {0};

    TinyMessage_u8ParseProgramDataRequest(SerializedProgramDataRequest, SERIALIZED_PROGRAM_DATA_REQUEST_SIZE, &Local_sProgramdataRequest);
    
    TEST_ASSERT_EQUAL_UINT32(ProgramDataRequest.address, Local_sProgramdataRequest.address);
    TEST_ASSERT_EQUAL_UINT32(ProgramDataRequest.size, Local_sProgramdataRequest.size);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(ProgramDataRequest.data, Local_sProgramdataRequest.data, PROGRAM_DATA_SIZE);
}
/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

