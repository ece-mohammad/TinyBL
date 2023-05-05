/******************************************************************************
 * @file      TinyBLTestRunner.c
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

#include "TinyBLTest.h"


TEST_GROUP_RUNNER(TinyBL_Bootloader_Init)
{
    /* before init */
    RUN_TEST_CASE(TinyBL_Bootloader_Init, StateIsInitBeforeInitialization);
    
    /* init changes state & status flags */
    RUN_TEST_CASE(TinyBL_Bootloader_Init, InitChangesStateToListen);
    RUN_TEST_CASE(TinyBL_Bootloader_Init, InitSetsStatusFlagReady);

    /* de-init changes state & status flags */
    RUN_TEST_CASE(TinyBL_Bootloader_Init, DeInitializeChangesStateToInit);
    RUN_TEST_CASE(TinyBL_Bootloader_Init, DeInitializeClearsAllStatusFlags);
}

TEST_GROUP_RUNNER(TinyBL_Bootloader)
{
    /* send message */
    RUN_TEST_CASE(TinyBL_Bootloader, SendMessageWritesToDataBuffer);

    /* on connect callback */
    RUN_TEST_CASE(TinyBL_Bootloader, OnConnectSuccessChangesStateFromListenToConnected);
    RUN_TEST_CASE(TinyBL_Bootloader, OnConnectTimeoutChangesStateFromListenToExit);

    /* on message callback :: any message */
    RUN_TEST_CASE(TinyBL_Bootloader, OnMessageCallbackSetsLastMessage);

    /* on message callback :: program message */
    RUN_TEST_CASE(TinyBL_Bootloader, ValidProgramMessageChangesStateToReceiveProgram); 
    RUN_TEST_CASE(TinyBL_Bootloader, ProgramMessageAddressBeforeMemoryStartChangeStateToExit);
    RUN_TEST_CASE(TinyBL_Bootloader, ProgramMessageAddressAfterMemoryEndChangeStateToExit);
    RUN_TEST_CASE(TinyBL_Bootloader, ProgramMessageZeroSizeChangeStateToExit);
    RUN_TEST_CASE(TinyBL_Bootloader, ProgramMessageInvalidSizeChangeStateToExit);

    /* on message callback :: program data message */
    RUN_TEST_CASE(TinyBL_Bootloader, ProgramDataMessageChangesStateToWriteProgram);
    RUN_TEST_CASE(TinyBL_Bootloader, ProgramDataMessageWriteDataToMemory);
    RUN_TEST_CASE(TinyBL_Bootloader, FullProgramWriteChangesStateToExit);
    // RUN_TEST_CASE(TinyBL_Bootloader, PartialProgramDataWriteSuccessChangesStateToReceiveProgram);

    /* on message callback :: status request message */
    // RUN_TEST_CASE(TinyBL_Bootloader, OnStatusRequestSendsCurrentStatusFlags);
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */



