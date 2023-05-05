/******************************************************************************
 * @file      CRC32Test.c
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




#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "unity.h"
#include "unity_fixture.h"

#include "CRC32Test.h"

/* -------------------------------------------------------------------------- */
/* Definitions & Macros */
/* -------------------------------------------------------------------------- */

#define RUN_OF_ZEROS            256
#define RUN_OF_ZEROS_CRC        0x1AA169B0

#define RUN_OF_0xFF             256
#define RUN_OF_0xFF_CRC         0x8415157F


/* -------------------------------------------------------------------------- */
/* Type definitions */
/* -------------------------------------------------------------------------- */

typedef struct TestVector_t {
    uint8_t * data;
    uint32_t  len;
    uint32_t  checksum;
} TestVector_t;

/* -------------------------------------------------------------------------- */
/* Private Variables */
/* -------------------------------------------------------------------------- */

static const TestVector_t Test_asVectors [] = {
    {
        .data = (uint8_t *)"",
        .len = 1,
        .checksum = 0xB1F7404B
    },
    {
        .data = (uint8_t *)"abc",
        .len = 3,
        .checksum = 0x648CBB73
    },
    {
        .data = (uint8_t *)"abcdefgh",
        .len = 8,
        .checksum = 0x5024EC61
    },
    {
        .data = (uint8_t *)"123456789",
        .len = 9,
        .checksum = 0xFC891918
    },
};

static uint8_t * Test_pu8Data;


/* -------------------------------------------------------------------------- */
/* Test group - CRC32 */
/* -------------------------------------------------------------------------- */

TEST_GROUP(CRC32);


TEST_SETUP(CRC32)
{
}


TEST_TEAR_DOWN(CRC32)
{
    if(Test_pu8Data)
    {
        free(Test_pu8Data);
        Test_pu8Data = NULL;
    }
}

/* -------------------------------------------------------------------------- */
/* Private Functions */
/* -------------------------------------------------------------------------- */

static uint8_t * prepare_data(uint32_t u32Size, uint8_t u8FillValue)
{
    Test_pu8Data = calloc(u32Size, sizeof(uint8_t));

    if(Test_pu8Data)
    {
        memset(Test_pu8Data, u8FillValue, u32Size);
    }

    return Test_pu8Data;
}

static void CheckTestVector(const TestVector_t * const psTestVector)
{
    CRC32_Error_t Local_tError = CRC32_ERROR_NONE;
    uint32_t Local_u32CRC = 0;

    Local_tError = CRC32_tCalculateCrc(
        psTestVector->data, 
        psTestVector->len, 
        &Local_u32CRC, 
        0
    );

    TEST_ASSERT_EQUAL(CRC32_ERROR_NONE, Local_tError);
    TEST_ASSERT_EQUAL_UINT32(psTestVector->checksum, Local_u32CRC);
}

/* -------------------------------------------------------------------------- */
/* Private Variables */
/* -------------------------------------------------------------------------- */
TEST(CRC32, RunOfZeros)
{
    const TestVector_t Local_sTestVector = {
        .data = prepare_data(RUN_OF_ZEROS, 0x00),
        .len = RUN_OF_ZEROS,
        .checksum = RUN_OF_ZEROS_CRC
    };

    CheckTestVector(&Local_sTestVector);
}

TEST(CRC32, RunOf0xFF)
{
    const TestVector_t Local_sTestVector = {
        .data = prepare_data(RUN_OF_0xFF, 0xFF),
        .len = RUN_OF_0xFF,
        .checksum = RUN_OF_0xFF_CRC
    };

    CheckTestVector(&Local_sTestVector);
}


TEST(CRC32, TesVectors)
{
    TestVector_t * Local_psTestVector = NULL;
    uint32_t Local_u32TestCount = sizeof(Test_asVectors) / sizeof(Test_asVectors[0]);

    for(uint32_t Local_u32VectorIndex = 0; Local_u32VectorIndex < Local_u32TestCount; Local_u32VectorIndex++)
    {
        Local_psTestVector = ((TestVector_t *)Test_asVectors + Local_u32VectorIndex);
        CheckTestVector(Local_psTestVector);
    }
}


TEST(CRC32, Partial)
{
    const char * Local_pcLargeData = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer non convallis felis, id luctus ipsum. Nam pretium augue eu dapibus finibus. Sed mauris odio, aliquet at tincidunt sit amet, condimentum in nisl. Nullam ullamcorper, sapien at convallis venenatis, felis neque dignissim ligula, in scelerisque ligula mi nec felis. Suspendisse malesuada nibh sit amet massa pharetra tincidunt. Duis vel erat nec nibh aliquet consectetur. Cras non semper nulla. Maecenas rhoncus metus erat, sodales posuere ipsum mollis quis.\n"
        "Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae; Donec vitae fringilla mauris, in euismod lorem. Quisque justo enim, ultrices sed dictum ut, imperdiet non risus. Fusce lacus ante, semper sit amet feugiat vitae, volutpat eu ligula. Class aptent taciti sociosqu ad litora torquent per conubia nostra, per inceptos himenaeos. Proin at lectus id dolor suscipit vulputate. Curabitur condimentum est in maximus ornare. Integer ut nisl finibus, gravida dolor eget, venenatis velit. Ut ut nibh vitae orci sodales fermentum quis sit amet est.\n"
        "Pellentesque luctus, massa sed elementum pulvinar, tellus urna imperdiet ante, ut lobortis leo arcu ut velit. Phasellus bibendum risus nec urna bibendum vehicula. Maecenas nec mauris a turpis tristique mollis vel eget erat. Nunc et augue nibh. Donec sit amet elementum nibh. Nam venenatis orci risus, et condimentum nulla porta ut. Etiam at ultricies ipsum.\n"
        "Cras porttitor sodales tellus, quis vulputate velit condimentum congue. Morbi purus nulla, bibendum a consectetur id, hendrerit sed eros. Aenean rhoncus orci odio, pulvinar pretium odio maximus vel. In hac habitasse platea dictumst. Mauris placerat eget erat quis sollicitudin. Phasellus tempor, justo sit amet pulvinar auctor, quam eros ornare nisl, a consectetur lacus odio ut risus. Sed ut justo ac justo varius commodo. Pellentesque pharetra est vitae lorem dapibus egestas. In mi magna, posuere ac tortor tempus, porttitor dapibus risus. Ut porta auctor est, eu ornare metus vestibulum vitae. Mauris in elit a tortor tempor interdum. Proin a mollis nisl.\n"
        "Pellentesque habitant morbi tristique senectus et netus et malesuada fames ac turpis egestas. Praesent interdum mi ipsum, at imperdiet dolor congue non. Mauris dui justo, maximus quis semper eu, tincidunt at lacus. Quisque mauris nisi, dapibus quis est vel, dignissim posuere mauris. Aliquam nisl leo, rutrum at pharetra eget, rutrum id arcu. Proin eu condimentum quam. Nullam consequat purus ac eros convallis vehicula. Donec feugiat diam sit amet accumsan sodales. Fusce at magna augue. Phasellus vel metus posuere, ullamcorper nisl viverra, mollis libero.\n";
    const uint32_t Local_u32Len = strlen(Local_pcLargeData);
    const TestVector_t Local_sTestVector = {
        .data = (uint8_t *)Local_pcLargeData,
        .len = Local_u32Len,
        .checksum = 0x53FFC2A9
    };

    CRC32_Error_t Local_tError = CRC32_ERROR_NONE;
    uint32_t Local_u32Checksum = 0;

    Local_tError = CRC32_tCalculateCrc(
        Local_sTestVector.data,
        Local_u32Len / 2,
        &Local_u32Checksum,
        Local_u32Checksum
    );

    TEST_ASSERT_EQUAL(CRC32_ERROR_NONE, Local_tError);
    
    Local_tError = CRC32_tCalculateCrc(
        (Local_sTestVector.data + (Local_u32Len / 2)),
        Local_u32Len - (Local_u32Len / 2),
        &Local_u32Checksum,
        Local_u32Checksum
    );

    TEST_ASSERT_EQUAL(CRC32_ERROR_NONE, Local_tError);

    TEST_ASSERT_EQUAL_HEX32(
        Local_sTestVector.checksum,
        Local_u32Checksum
    );
}


/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */
