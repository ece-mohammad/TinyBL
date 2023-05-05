/******************************************************************************
 * @file      AllTests.c
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

static void RunAllTests(void)
{
    /* Flash */
    RUN_TEST_GROUP(FlashInit);
    RUN_TEST_GROUP(FlashDeInit);
    RUN_TEST_GROUP(Flash);

    /* Fletcher16 */
    RUN_TEST_GROUP(Fletcher16);

    /* CRC32 */
    RUN_TEST_GROUP(CRC32);

    /* TinySerialize */
    RUN_TEST_GROUP(TinySerialize);
    
    /* TinyMessage */
    RUN_TEST_GROUP(TinyMessage);

    /* TinyBL bootloader */
    RUN_TEST_GROUP(TinyBL_Bootloader_Init);
    RUN_TEST_GROUP(TinyBL_Bootloader);
}

int main(int argc, const char * argv [])
{
    return UnityMain(argc, argv, RunAllTests);
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */
