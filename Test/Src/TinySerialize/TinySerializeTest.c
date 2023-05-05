/******************************************************************************
 * @file      TinySerializeTest.c
 * @brief     
 * @author    Author name <email@org.com>
 * @date      2023/04/11
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

#include "TinySerializeTest.h"


static uint8_t * Test_au8Message = NULL;
static uint32_t  Test_u32Var = 0;

static const uint8_t Test_au8SerializedMessage [] = {
        0xEF,
        0xBE,
        0xAD,
        0xDE
};

static const uint32_t Test_u32DeserializedMessage = 0xDEADBEEF;


TEST_GROUP(TinySerialize);


TEST_SETUP(TinySerialize)
{
    Test_au8Message = malloc(4);
    Test_u32Var = 0;
}


TEST_TEAR_DOWN(TinySerialize)
{
    if(Test_au8Message)
    {
        free(Test_au8Message);
        Test_au8Message = NULL;
    }
}


TEST(TinySerialize, SerializeUint32)
{
    TinySerialize_vidSerialUint32(&Test_u32DeserializedMessage, Test_au8Message);
    TEST_ASSERT_EQUAL_UINT8_ARRAY(Test_au8SerializedMessage, Test_au8Message, 4);
}

TEST(TinySerialize, DeSerializedUint32)
{
    TinySerialize_vidDeserializedUint32(&Test_u32Var, Test_au8SerializedMessage);
    TEST_ASSERT_EQUAL_UINT32(Test_u32DeserializedMessage, Test_u32Var);
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

