/******************************************************************************
 * @file      TinyBL.c
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
#include <stdlib.h>
#include <string.h>

/* ----------------------------------------------------------------------------
#include "UART.h"
#include "Timer.h"

#include "Transport.h"
#include "Protocol.h"
---------------------------------------------------------------------------- */

#include "config.h"

#include "Flash.h"
#include "TinyMessage.h"
#include "CRC32.h"
#include "TinyBL.h"


/* ------------------------------------------------------------------------- */
/* Private Definitions & Macros */
/* ------------------------------------------------------------------------- */
#ifndef MIN
#define MIN(a, b)       (((a) < (b)) ? (a) : (b))
#endif /* MIN */

/* program response */


/* program data request */

/* program data response */

/* status response */

/* ------------------------------------------------------------------------- */
/* Private Types */
/* ------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 */
typedef enum TinyBL_Event_Type_t {
    TINYBL_EVENT_NONE,
    TINYBL_EVENT_CONNECTION_STATUS_UPDATE,
    TINYBL_EVENT_WRITE_STATUS_UPDATE,
    TINYBL_EVENT_STATUS_REQUEST,
    TINYBL_EVENT_PROGRAM_REQUEST,
    TINYBL_EVENT_PROGRAM_DATA_WRITE_REQUEST,
} TinyBL_Event_Type_t;

/**
 * @brief 
 * 
 */
typedef struct TinyBL_Write_Status_t {
    uint8_t * data;
    uint32_t  size;
    uint32_t  address;
    Flash_Write_Status_t status;
} TinyBL_Write_Status_t;

/**
 * @brief 
 * 
 */
typedef struct TinyBL_Event_Data_t {
    union {
        TinyBL_Connection_Status_t connection_status;
        TinyMessage_Request_Program_t program_request;
        TinyMessage_Request_Program_Data_t program_data;
        TinyBL_Write_Status_t write_status;
    };
} TinyBL_Event_Data_t;

/**
 * @brief 
 * 
 */
typedef struct TinyBL_Event_t {
    TinyBL_Event_Type_t type;
    TinyBL_Event_Data_t data;
} TinyBL_Event_t;

/* ------------------------------------------------------------------------- */
/* Private Functions Declarations */
/* ------------------------------------------------------------------------- */

/**
 * @brief 
 * 
 * @param [in] psEvent 
 */
static void TinyBL_vidAddEvent(const TinyBL_Event_t * const psEvent);

/**
 * @brief 
 * 
 * @param [in] psEvent 
 */
static void TinyBL_vidUpdate(const TinyBL_Event_t * const psEvent);

/* ------------------------------------------------------------------------- */
/* Private Variables */
/* ------------------------------------------------------------------------- */

static TinyBL_State_t TinyBL_tCurrentState;
static TinyBL_StatusFlags_t TinyBL_tStatusFlags;

static uint8_t * TinyBL_au8MessageBuffer;
static uint32_t  TinyBL_u32BufferSize;
static uint32_t  TinyBL_u32MessageSize;

static TinyBL_Event_t TinyBL_sLastEvent = {0};

static uint32_t TinyBL_u32CurrentProgramAddress = 0;
static uint32_t TinyBL_u32RemainingProgramBytes = 0;
static uint32_t TinyBL_u32ProgramChecksum = 0;
static uint32_t TinyBL_u32CurrentProgramChecksum = 0;


/* ------------------------------------------------------------------------- */
/* Private Functions Definitions */
/* ------------------------------------------------------------------------- */

static void TinyBL_vidAddEvent(const TinyBL_Event_t * const psEvent)
{
    TinyBL_vidUpdate(psEvent);
}

/* ------------------------------------------------------------------------- */

static void TinyBL_vidUpdate(const TinyBL_Event_t * const psEvent)
{
    switch (psEvent->type)
    {
        case TINYBL_EVENT_CONNECTION_STATUS_UPDATE:
        {
            /* TODO handle connection timeout regardless of current state */
            if((psEvent->data.connection_status == TINYBL_CONNECTION_SUCCESS) && (TinyBL_tCurrentState == TINYBL_STATE_WAIT_CONNECTION))
            {
                TinyBL_tCurrentState = TINYBL_STATE_CONNECTED;
            }
            else
            {
                TinyBL_tCurrentState = TINYBL_STATE_EXIT;
            }
        }
        break;

        case TINYBL_EVENT_PROGRAM_REQUEST:
        {
            if(TinyBL_tCurrentState == TINYBL_STATE_CONNECTED)
            {
                if(Flash_u8IsValidProgramAddressAndSize(psEvent->data.program_request.address, psEvent->data.program_request.size))
                {
                    TinyBL_u32CurrentProgramAddress = psEvent->data.program_request.address;
                    TinyBL_u32RemainingProgramBytes = psEvent->data.program_request.size;
                    TinyBL_u32ProgramChecksum = psEvent->data.program_request.checksum;

                    TinyBL_tCurrentState = TINYBL_STATE_RECEIVE_PROGRAM;
                }
                else
                {
                    TinyBL_tCurrentState = TINYBL_STATE_EXIT;
                }
            }
        }
        break;

        case TINYBL_EVENT_PROGRAM_DATA_WRITE_REQUEST:
        {
            if(TinyBL_tCurrentState == TINYBL_STATE_RECEIVE_PROGRAM)
            {
                Flash_Error_t Local_tError = FLASH_ERROR_NONE;

                Local_tError = Flash_tWrite(
                    psEvent->data.program_data.address,
                    psEvent->data.program_data.data,
                    psEvent->data.program_data.size
                );

                if(Local_tError == FLASH_ERROR_NONE)
                {
                    TinyBL_tCurrentState = TINYBL_STATE_WRITE_PROGRAM_DATA;
                }
                else
                {
                    TinyBL_tCurrentState = TINYBL_STATE_EXIT;
                }
            }
        }
        break;
        
        case TINYBL_EVENT_WRITE_STATUS_UPDATE:
        {
            if(/* (TinyBL_tCurrentState == TINYBL_STATE_WRITE_PROGRAM_DATA) && */ (psEvent->data.write_status.status == FLASH_WRITE_STATUS_OK))
            {
                /* TODO update checksum */
                CRC32_tCalculateCrc(
                    psEvent->data.write_status.data,
                    psEvent->data.write_status.size,
                    &TinyBL_u32CurrentProgramChecksum,
                    TinyBL_u32CurrentProgramChecksum
                );

                if(TinyBL_u32RemainingProgramBytes >= psEvent->data.write_status.size)
                {
                    TinyBL_u32RemainingProgramBytes -= psEvent->data.write_status.size;
                }
                else
                {
                    /* TODO error (bad program size) */
                }

                if(TinyBL_u32RemainingProgramBytes == 0)
                {
                    TinyBL_tCurrentState = TINYBL_STATE_EXIT;
                }
            }
            else
            {
                /* TODO report write error */
            }
        }
        break;

        case TINYBL_EVENT_STATUS_REQUEST:
        case TINYBL_EVENT_NONE:
        default:
        {
        }
        break;
    }

#if 0
    switch(TinyBL_tCurrentState)
    {
        case TINYBL_STATE_WAIT_CONNECTION:
        {
            if((psEvent->type == TINYBL_EVENT_CONNECTION_STATUS_UPDATE) && \
                (psEvent->data.connection_status == TINYBL_CONNECTION_SUCCESS))
            {
                TinyBL_tCurrentState = TINYBL_STATE_CONNECTED;
            }
        }
        break;

        case TINYBL_STATE_CONNECTED:
        {
            /* program event */
            if(psEvent->type == TINYBL_EVENT_PROGRAM_REQUEST)
            {
                if(Flash_u8IsValidProgramAddressAndSize(psEvent->data.program_request.address, psEvent->data.program_request.size))
                {
                    TinyBL_tCurrentState = TINYBL_STATE_RECEIVE_PROGRAM;
                }
                else
                {
                    /* send error */
                    /* send exit */
                    TinyBL_tCurrentState = TINYBL_STATE_EXIT;
                }
            }
        }
        break;

        case TINYBL_STATE_RECEIVE_PROGRAM:
        case TINYBL_STATE_WRITE_PROGRAM_DATA:
        case TINYBL_STATE_CLOSE:
        case TINYBL_STATE_INIT:
        default:
        {
        }
        break;
    }

    /* connection timeout */
    if((psEvent->type == TINYBL_EVENT_CONNECTION_STATUS_UPDATE) && \
        (psEvent->data.connection_status == TINYBL_CONNECTION_TIMEOUT))
    {
        TinyBL_tCurrentState = TINYBL_STATE_EXIT;
    }
#endif /* 0 */
}

/* ------------------------------------------------------------------------- */
/* Public API Implementations */
/* ------------------------------------------------------------------------- */

void TinyBL_vidInitialize(uint8_t * pu8DataBuffer, uint32_t u32BufferSize)
{
    TinyBL_au8MessageBuffer = pu8DataBuffer;
    TinyBL_u32BufferSize = u32BufferSize;
    TinyBL_u32MessageSize = 0;
    
    TinyBL_tCurrentState = TINYBL_STATE_WAIT_CONNECTION;
    TinyBL_tStatusFlags = TINYBL_STATUS_FLAG_READY;

    TinyBL_u32CurrentProgramAddress = 0;
    TinyBL_u32RemainingProgramBytes = 0;
    TinyBL_u32CurrentProgramChecksum = 0;
    TinyBL_u32ProgramChecksum = 0;

    memset(&TinyBL_sLastEvent, 0x00, sizeof(TinyBL_sLastEvent));
}

/* ------------------------------------------------------------------------- */

void TinyBL_vidDeInitialize(void)
{
    memset(TinyBL_au8MessageBuffer, 0x00, TinyBL_u32BufferSize);
    TinyBL_u32MessageSize = 0;
    TinyBL_tCurrentState = TINYBL_STATE_INIT;
    TinyBL_tStatusFlags = 0x00;
}

/* ------------------------------------------------------------------------- */

TinyBL_State_t TinyBL_tGetState(void)
{
    return TinyBL_tCurrentState;
}

/* ------------------------------------------------------------------------- */

TinyBL_StatusFlags_t TinyBL_tGetStatusFlags(void)
{
    return TinyBL_tStatusFlags;
}

/* ------------------------------------------------------------------------- */

void TinyBL_vidGetLastMessage(uint8_t * pu8Message, uint32_t * pu32Len)
{
    memcpy(pu8Message, TinyBL_au8MessageBuffer, TinyBL_u32MessageSize);
    (*pu32Len) = TinyBL_u32MessageSize;
}

/* ------------------------------------------------------------------------- */

void TinyBL_vidOnConnect(TinyBL_Connection_Status_t tConnectionStatus)
{
    TinyBL_Event_t Local_sEvent = {
        .type = TINYBL_EVENT_CONNECTION_STATUS_UPDATE,
        .data.connection_status = tConnectionStatus
    };

    TinyBL_vidAddEvent(&Local_sEvent);
}

/* ------------------------------------------------------------------------- */

void TinyBL_vidOnMessage(const uint8_t * const pu8Message, uint32_t u32Size)
{
    TinyBL_Event_t Local_sEvent = {0};

    TinyBL_u32MessageSize = MIN(TinyBL_u32BufferSize, u32Size);
    memcpy(TinyBL_au8MessageBuffer, pu8Message, TinyBL_u32MessageSize);

    switch(pu8Message[TINY_MESSAGE_REQUEST_TYPE])
    {
        case TINY_MESSAGE_TYPE_PROGRAM_REQUEST:
        {
            if(TinyMessage_u8ParseProgramRequest(pu8Message, u32Size, &Local_sEvent.data.program_request))
            {
                Local_sEvent.type = TINYBL_EVENT_PROGRAM_REQUEST;
            }
        }
        break;

        case TINY_MESSAGE_TYPE_PROGRAM_DATA_REQUEST:
        {
            Local_sEvent.type = TINYBL_EVENT_PROGRAM_DATA_WRITE_REQUEST;
            if(TinyMessage_u8ParseProgramDataRequest(pu8Message, u32Size, &Local_sEvent.data.program_data))
            {
                Local_sEvent.type = TINYBL_EVENT_PROGRAM_DATA_WRITE_REQUEST;
            }
        }
        break;

        case TINY_MESSAGE_TYPE_PROGRAM_DATA_RESPONSE:
        case TINY_MESSAGE_TYPE_STATUS_REQUEST:
        case TINY_MESSAGE_TYPE_STATUS_RESPONSE:
        default:
        {
            /* do nothing */
        }
        break;
    }

    if(Local_sEvent.type != TINYBL_EVENT_NONE)
    {
        TinyBL_vidAddEvent(&Local_sEvent);
    }
}

/* ------------------------------------------------------------------------- */

void TinyBL_vidOnDisconnect(void)
{
}

/* ------------------------------------------------------------------------- */

void TinyBL_vidOnWrite(Flash_Write_Status_t tWriteStatus, uint32_t u32Adress, const uint8_t * const pu8Data, uint32_t u32Size)
{
    TinyBL_Event_t Local_sEvent = {
        .type = TINYBL_EVENT_WRITE_STATUS_UPDATE,
        .data.write_status.data = (uint8_t *)pu8Data,
        .data.write_status.address = u32Adress,
        .data.write_status.size = u32Size,
        .data.write_status.status = tWriteStatus,
    };
    
    TinyBL_vidAddEvent(&Local_sEvent);
}

/* ------------------------------------------------------------------------- */

void TinyBL_vidSendMessage(const uint8_t * const pu8Message, uint32_t u32Size)
{
    TinyBL_u32MessageSize = MIN(TinyBL_u32BufferSize, u32Size);
    memcpy(TinyBL_au8MessageBuffer, pu8Message, TinyBL_u32MessageSize);
}

/* ------------------------------------------------------------------------- */
/*  End of File  */
/* ------------------------------------------------------------------------- */

