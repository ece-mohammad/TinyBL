/******************************************************************************
 * @file      Fletcher16Test.c
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
#include <string.h>

#include "unity.h"
#include "unity_fixture.h"

#include "Fletcher16Test.h"


typedef struct TestVector_t {
    char *   data;
    uint32_t len;
    uint16_t expected_checksum;
}TestVector_t;


static TestVector_t Test_asVectors [] = {
    {
        .data = "",
        .len = 1,
        .expected_checksum = 0x00
    },
    {
        .data = "abcde",
        .len = 5,
        .expected_checksum = 0xC8F0
    },
    {
        .data = "abcdef",
        .len = 6,
        .expected_checksum = 0x2057
    },
    {
        .data = "abcdefgh",
        .len = 8,
        .expected_checksum = 0x0627
    },
};


TEST_GROUP(Fletcher16);


TEST_SETUP(Fletcher16)
{
}

TEST_TEAR_DOWN(Fletcher16)
{
}

static void assertChecksum(uint8_t * pu8Data, uint32_t u32Len, uint16_t u16InitialChecksum, uint16_t u16ExpectedChecksum)
{
    Fletcher16_t Local_tCalculatedChecksum = Fletcher16_tCalculate(pu8Data, u32Len, u16InitialChecksum);
    TEST_ASSERT_EQUAL_HEX16(u16ExpectedChecksum, Local_tCalculatedChecksum.checksum);
}

/* ------------------------------------------------------------------------- */
/* Test group - Fletchr16 */
/* ------------------------------------------------------------------------- */

/******************************************************************************
 * @brief Fletcher16_tCalculate()
 *****************************************************************************/
TEST(Fletcher16, TestVectors)
{
    uint32_t Local_u32TestVectors = sizeof(Test_asVectors) / sizeof(Test_asVectors[0]);
    TestVector_t * Local_psTestVector = NULL;

    for(uint32_t Local_u32Index = 0; Local_u32Index < Local_u32TestVectors; Local_u32Index++)
    {
        Local_psTestVector = (Test_asVectors + Local_u32Index);
        assertChecksum(
            (uint8_t *)Local_psTestVector->data, 
            Local_psTestVector->len, 
            0,
            Local_psTestVector->expected_checksum
        );
    }
}

TEST(Fletcher16, StreamOfZeros)
{
    uint8_t Local_au8TestVector [8];

    memset(Local_au8TestVector, 0x00, sizeof(Local_au8TestVector));
    assertChecksum(
        (uint8_t *)Local_au8TestVector, 
        sizeof(Local_au8TestVector), 
        0,
        0
    );
}

TEST(Fletcher16, StreamOfOnes)
{
    uint8_t Local_au8TestVector [8];

    memset(Local_au8TestVector, 0x01, sizeof(Local_au8TestVector));
    assertChecksum(
        (uint8_t *)Local_au8TestVector, 
        sizeof(Local_au8TestVector), 
        0,
        0x2408
    );
}

TEST(Fletcher16, PartialDataStream)
{
    const char * Local_pcLargeData = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Nulla et lectus non ligula sollicitudin aliquet et vitae nulla. Mauris gravida mauris risus, eu porta odio gravida eget. Donec turpis odio, pulvinar eu velit at, tincidunt porta urna. Vivamus metus.";
    const uint32_t Local_u32Len = strlen(Local_pcLargeData);
    Fletcher16_t Local_tOneShotChecksum = {0};
    Fletcher16_t Local_tChecksum = {0};

    Local_tOneShotChecksum = Fletcher16_tCalculate(
        (uint8_t *)Local_pcLargeData,
        Local_u32Len,
        0
    );

    Local_tChecksum = Fletcher16_tCalculate(
        (uint8_t *)Local_pcLargeData,
        Local_u32Len / 2,
        0
    );

    Local_tChecksum = Fletcher16_tCalculate(
        (uint8_t *)(Local_pcLargeData + (Local_u32Len / 2)),
        (Local_u32Len - (Local_u32Len / 2)),
        Local_tChecksum.checksum
    );
    
    TEST_ASSERT_EQUAL_HEX16(Local_tOneShotChecksum.checksum, Local_tChecksum.checksum);
    TEST_ASSERT_EQUAL_HEX16(Local_tOneShotChecksum.check_bytes, Local_tChecksum.check_bytes);
}

/******************************************************************************
 * @brief Fletcher16_u8Verify()
 *****************************************************************************/
TEST(Fletcher16, VerifyData)
{
    uint8_t Local_au8TestVector [10] = "abcdefgh";
    Fletcher16_t Local_tChecksum = {0};

    Local_tChecksum = Fletcher16_tCalculate(
        Local_au8TestVector,
        8,
        0
    );

    Local_au8TestVector[8] = (Local_tChecksum.check_bytes >> 0) & 0xFF;
    Local_au8TestVector[9] = (Local_tChecksum.check_bytes >> 8) & 0xFF;

    TEST_ASSERT_EQUAL_UINT8(1, Fletcher16_u8Verify(Local_au8TestVector, 10));
}

TEST(Fletcher16, InvalidData)
{
    uint8_t Local_au8TestVector [10] = "abcdefgh";
    Fletcher16_t Local_tChecksum = {0};

    Local_tChecksum = Fletcher16_tCalculate(
        Local_au8TestVector,
        8,
        0
    );

    Local_au8TestVector[8] = (Local_tChecksum.check_bytes >> 0) & 0xFF;
    Local_au8TestVector[9] = (Local_tChecksum.check_bytes >> 8) & 0xFF;
    Local_au8TestVector[0] = 'b';
    Local_au8TestVector[1] = 'a';
    TEST_ASSERT_EQUAL_UINT8(0, Fletcher16_u8Verify(Local_au8TestVector, 10));
}


/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

