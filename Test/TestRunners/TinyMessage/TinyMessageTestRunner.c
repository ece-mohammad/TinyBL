/******************************************************************************
 * @file      TinyMessageTestRunner.c
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


TEST_GROUP_RUNNER(TinyMessage)
{
    /* program request */
    RUN_TEST_CASE(TinyMessage, SerializeProgramRequest);
    RUN_TEST_CASE(TinyMessage, ParseProgramRequest);

    /* program data request */
    RUN_TEST_CASE(TinyMessage, SerializeProgramDataRequest);
    RUN_TEST_CASE(TinyMessage, ParseProgramDataRequest);
}


/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

