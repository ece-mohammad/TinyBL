/******************************************************************************
 * @file      FlashTestRunner.c
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

#include "FlashTest.h"



TEST_GROUP_RUNNER(FlashInit)
{
    /* Valid init */
    RUN_TEST_CASE(FlashInit, InitWithValidParamsReturnNoError);

    /* Init with invalid params */
    RUN_TEST_CASE(FlashInit, InitWithZeroPageSizeReturnInvalidParamError);
    RUN_TEST_CASE(FlashInit, InitWithZeroNumOfPagesReturnInvalidParamError);

    /* SetCallback */
    RUN_TEST_CASE(FlashInit, SetWriteCallbackWithNullReturnsNullError);
    RUN_TEST_CASE(FlashInit, SetWriteCallbackReturnsNoError);

    /* Erase, write erase before init */
    RUN_TEST_CASE(FlashInit, ReadBeforeInitReturnMemoryError);
    RUN_TEST_CASE(FlashInit, WriteBeforeInitReturnMemoryError);
    RUN_TEST_CASE(FlashInit, PageEraseBeforeInitReturnMemoryError);
    RUN_TEST_CASE(FlashInit, MassEraseBeforeInitDoesNotCauseError);
}

TEST_GROUP_RUNNER(FlashDeInit)
{
    /* Erad, write erase after Deinit */
    RUN_TEST_CASE(FlashDeInit ,ReadAfterDeInitReturnMemoryError);
    RUN_TEST_CASE(FlashDeInit ,WriteAfterDeInitReturnMemoryError);
    RUN_TEST_CASE(FlashDeInit ,PageEraseAfterDeInitReturnMemoryError);
    RUN_TEST_CASE(FlashDeInit ,MassEraseAfterDeInitDoesNotCauseError);
}

TEST_GROUP_RUNNER(Flash)
{
    RUN_TEST_CASE(Flash, ReadAndWriteToValidAddress);
    
    /* Flash_tWrite() */
    RUN_TEST_CASE(Flash, WriteNullDataReturnNullError);
    RUN_TEST_CASE(Flash, WriteWithZeroSizeDataReturnInvalidParamError);
    RUN_TEST_CASE(Flash, WriteToAddressBeforeMemoryStartReturnInvalidAddressError);
    RUN_TEST_CASE(Flash, WriteToAddressAfterMemoryEndReturnInvalidAddressError);
    RUN_TEST_CASE(Flash, WriteOutsideMemoryReturnInvalidAddressError);
    RUN_TEST_CASE(Flash, WriteLargeDataReturnNoError);

    /* Flash_tRead() */
    RUN_TEST_CASE(Flash, ReadWithNullBufferReturnNullError);
    RUN_TEST_CASE(Flash, ReadWithZeroSizeBufferReturnInvalidParamError);
    RUN_TEST_CASE(Flash, ReadFromAddressBeforeFlashStartReturnInvalidAddressError);
    RUN_TEST_CASE(Flash, ReadFromAddressAfterFlashEndReturnInvalidAddressError);
    RUN_TEST_CASE(Flash, ReadOutsideFlashEndReturnSizeError);
    RUN_TEST_CASE(Flash, ReadLargeDataReturnNoError);

    /* Flash_tErase() */
    RUN_TEST_CASE(Flash, ErasePageClearsPageContent);
    RUN_TEST_CASE(Flash, ErasePageWithInvalidPageNumberReturnInvalidParamError);
    
    /* Flash_vidMassErase() */
    RUN_TEST_CASE(Flash, MassEraseClearsFlashContents);

    /* Flash_u32GetStartAddress */
    RUN_TEST_CASE(Flash, StartAddress);
    
    /* Flash_u32GetMemorySize */
    RUN_TEST_CASE(Flash, MemorySize);

    /* Flash_u8IsValidProgramAddressAndSize */
    RUN_TEST_CASE(Flash, ValidProgram);
    RUN_TEST_CASE(Flash, ValidProgram_Address);
    RUN_TEST_CASE(Flash, ValidProgram_Size);

}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

