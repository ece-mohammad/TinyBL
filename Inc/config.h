/******************************************************************************
 * @file      config.h
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

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* ------------------------------------------------------------------------- */
/* Flash memory configurations */
/* ------------------------------------------------------------------------- */

#define FLASH_MEMORY_START_ADDRESS      0x08020000
#define FLASH_MEMORY_PAGES              120
#define FLASH_MEMORY_PAGE_SIZE          1024
#define FLASH_MEMORY_SIZE               (FLASH_MEMORY_PAGES * FLASH_MEMORY_PAGE_SIZE)
#define FLASH_MEMORY_END_ADDRESS        (FLASH_MEMORY_START_ADDRESS + FLASH_MEMORY_SIZE - 1)


#endif /* _CONFIG_H_ */

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

