/******************************************************************************
 * @file      TinyMessage.c
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


#include "TinySerialize.h"
#include "TinyMessage.h"


/* ------------------------------------------------------------------------- */
/* Private Definitions & Macros */
/* ------------------------------------------------------------------------- */


/******************************************************************************
 * @brief message indices
 *****************************************************************************/

/* program request */
#define TINY_MESSAGE_PROGRAM_ADDRESS            1
#define TINY_MESSAGE_PROGRAM_ADDRESS_BYTES      4
#define TINY_MESSAGE_PROGRAM_SIZE               5
#define TINY_MESSAGE_PROGRAM_SIZE_BYTES         4
#define TINY_MESSAGE_PROGRAM_CHECKSUM           9
#define TINY_MESSAGE_PROGRAM_CHECKSUM_BYTES     4
#define TINY_MESSAGE_PROGRAM_BYTES              (TINY_MESSAGE_PROGRAM_ADDRESS_BYTES + TINY_MESSAGE_PROGRAM_SIZE_BYTES + TINY_MESSAGE_PROGRAM_CHECKSUM_BYTES + 1)

/* program data request */
#define TINY_MESSAGE_PROGRAM_DATA_OFFSET        1
#define TINY_MESSAGE_PROGRAM_DATA_OFFSET_BYTES  4
#define TINY_MESSAGE_PROGRAM_DATA_SIZE          5
#define TINY_MESSAGE_PROGRAM_DATA_SIZE_BYTES    4
#define TINY_MESSAGE_PROGRAM_DATA               9
#define TINY_MESSAGE_PROGRAM_DATA_BYTES         (TINY_MESSAGE_PROGRAM_DATA_OFFSET_BYTES + TINY_MESSAGE_PROGRAM_DATA_SIZE_BYTES + 1)

/* ------------------------------------------------------------------------- */
/* Private Types */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Functions Declarations */
/* ------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 * @param [in]  pu8SerializedProgramRequest 
 * @param [out] psRequestData 
 * 
 */
static void Tiny_vidDeserializeProgramRequest(const uint8_t * const pu8SerializedProgramRequest, TinyMessage_Request_Program_t * const psRequestData);

/**
 * @brief 
 * 
 * @param [in]  pu8SerializedProgramDataRequest 
 * @param [out] psProgramDataRequest 
 */
static void Tiny_vidDeserializeProgramDataRequest(const uint8_t * const pu8SerializedProgramDataRequest, TinyMessage_Request_Program_Data_t * const psProgramDataRequest);

/* ------------------------------------------------------------------------- */
/* Private Variables */
/* ------------------------------------------------------------------------- */


/* ------------------------------------------------------------------------- */
/* Private Functions Definitions */
/* ------------------------------------------------------------------------- */

static void Tiny_vidDeserializeProgramRequest(const uint8_t * const pu8SerializedProgramRequest, TinyMessage_Request_Program_t * const psRequestData)
{
    /* address */
    TinySerialize_vidDeserializedUint32(&psRequestData->address, pu8SerializedProgramRequest + TINY_MESSAGE_PROGRAM_ADDRESS);
    
    /* size */
    TinySerialize_vidDeserializedUint32(&psRequestData->size, pu8SerializedProgramRequest + TINY_MESSAGE_PROGRAM_SIZE);
    
    /* checksum */
    TinySerialize_vidDeserializedUint32(&psRequestData->checksum, pu8SerializedProgramRequest + TINY_MESSAGE_PROGRAM_CHECKSUM);
}

/* ------------------------------------------------------------------------- */

static void Tiny_vidDeserializeProgramDataRequest(const uint8_t * const pu8SerializedProgramDataRequest, TinyMessage_Request_Program_Data_t * const psProgramDataRequest)
{
    /* address */
    TinySerialize_vidDeserializedUint32(&psProgramDataRequest->address, pu8SerializedProgramDataRequest + TINY_MESSAGE_PROGRAM_DATA_OFFSET);

    /* size */
    TinySerialize_vidDeserializedUint32(&psProgramDataRequest->size, pu8SerializedProgramDataRequest + TINY_MESSAGE_PROGRAM_DATA_SIZE);
    
    /* data */
    /* memcpy(&psProgramDataRequest->data, pu8SerializedProgramDataRequest + TINY_MESSAGE_PROGRAM_DATA, psProgramDataRequest->size); */
    psProgramDataRequest->data = (uint8_t *)(pu8SerializedProgramDataRequest + TINY_MESSAGE_PROGRAM_DATA);
}

/* ------------------------------------------------------------------------- */
/* Public API Implementations */
/* ------------------------------------------------------------------------- */


/******************************************************************************
 * @brief Message (request/response) Serialization
 *****************************************************************************/

uint32_t TinyMessage_u32SerializeProgramRequest(uint8_t * pu8SerializedProgramRequest, const TinyMessage_Request_Program_t * const psRequestData)
{
    pu8SerializedProgramRequest[TINY_MESSAGE_REQUEST_TYPE] = TINY_MESSAGE_TYPE_PROGRAM_REQUEST;

    /* address */
    TinySerialize_vidSerialUint32(&psRequestData->address, pu8SerializedProgramRequest + TINY_MESSAGE_PROGRAM_ADDRESS);
    
    /* size */
    TinySerialize_vidSerialUint32(&psRequestData->size, pu8SerializedProgramRequest + TINY_MESSAGE_PROGRAM_SIZE);
    
    /* checksum */
    TinySerialize_vidSerialUint32(&psRequestData->checksum, pu8SerializedProgramRequest + TINY_MESSAGE_PROGRAM_CHECKSUM);

    return TINY_MESSAGE_PROGRAM_BYTES;
}

/* ------------------------------------------------------------------------- */

uint32_t TinyMessage_u32SerializeProgramDataRequest(uint8_t * pu8SerializedProgramDataRequest, const TinyMessage_Request_Program_Data_t * const psRequestData)
{
    pu8SerializedProgramDataRequest[TINY_MESSAGE_REQUEST_TYPE] = TINY_MESSAGE_TYPE_PROGRAM_DATA_REQUEST;

    /* address */
    TinySerialize_vidSerialUint32(&psRequestData->address, pu8SerializedProgramDataRequest + TINY_MESSAGE_PROGRAM_DATA_OFFSET);

    /* size */
    TinySerialize_vidSerialUint32(&psRequestData->size, pu8SerializedProgramDataRequest + TINY_MESSAGE_PROGRAM_DATA_SIZE);

    /* data */
    memcpy(pu8SerializedProgramDataRequest + TINY_MESSAGE_PROGRAM_DATA_BYTES, psRequestData->data, psRequestData->size);

    return TINY_MESSAGE_PROGRAM_DATA_BYTES + psRequestData->size;
}

/******************************************************************************
 * @brief Message (request/response) De-serialization (parsing)
 *****************************************************************************/

uint8_t TinyMessage_u8ParseProgramRequest(const uint8_t * const pu8Message, uint32_t u32Size, TinyMessage_Request_Program_t * const psProgramRequest)
{
    if((u32Size < TINY_MESSAGE_PROGRAM_BYTES) || (pu8Message == NULL) || (psProgramRequest == NULL))
    {
        return 0;
    }

    Tiny_vidDeserializeProgramRequest(pu8Message, psProgramRequest);

    return 1;
}

/* ------------------------------------------------------------------------- */

uint8_t TinyMessage_u8ParseProgramDataRequest(const uint8_t * const pu8Message, uint32_t u32Size, TinyMessage_Request_Program_Data_t * const psProgramDataRequest)
{
    if(u32Size < TINY_MESSAGE_PROGRAM_DATA_BYTES)
    {
        return 0;
    }

    Tiny_vidDeserializeProgramDataRequest(pu8Message, psProgramDataRequest);
    
    return 0;
}


/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

