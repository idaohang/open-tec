/**
 * @addtogroup platform_src_sirf_pal
 * @{
 */

/*
 *                   SiRF Technology, Inc. GPS Software
 *
 *    Copyright (c) 2006-2009 by SiRF Technology, Inc.  All rights reserved.
 *
 *    This Software is protected by United States copyright laws and
 *    international treaties.  You may not reverse engineer, decompile
 *    or disassemble this Software.
 *
 *    WARNING:
 *    This Software contains SiRF Technology Inc.’s confidential and
 *    proprietary information. UNAUTHORIZED COPYING, USE, DISTRIBUTION,
 *    PUBLICATION, TRANSFER, SALE, RENTAL OR DISCLOSURE IS PROHIBITED
 *    AND MAY RESULT IN SERIOUS LEGAL CONSEQUENCES.  Do not copy this
 *    Software without SiRF Technology, Inc.’s  express written
 *    permission.   Use of any portion of the contents of this Software
 *    is subject to and restricted by your signed written agreement with
 *    SiRF Technology, Inc.
 *
 */

/**
 * @file   sirf_pal_com.h
 *
 * @brief  SiRF PAL communications API.
 *
 * Terminologies
 *    HW - Hardware
 *    FC - Flow Control
 */

#ifndef SIRF_PAL_COM_H_INCLUDED
#define SIRF_PAL_COM_H_INCLUDED
#include "sirf_types.h"



/* ----------------------------------------------------------------------------
 *   Preprocessor Definitions
 * ------------------------------------------------------------------------- */



/* SiRF PAL communications error codes */
#define SIRF_PAL_COM_ERROR                    0x2301
#define SIRF_PAL_COM_DATA_NOT_AVAILABLE       0x2302
#define SIRF_PAL_COM_TIMEOUT                  0x2303
#define SIRF_PAL_COM_RESUMED_POWER            0x2304
#define SIRF_PAL_COM_INVALID_PORT_BAUD_RATE   0x2305
#define SIRF_PAL_COM_WAIT_TERMINATED          0x2306
#define SIRF_PAL_COM_INVALIDARG               0x2307
#define SIRF_PAL_COM_STATE                    0x2308

/* Hardware flow control values */
#define SIRF_PAL_COM_FC_HW      0x1
#define SIRF_PAL_COM_FC_NONE    0x0


/* ----------------------------------------------------------------------------
 *   Types
 * ------------------------------------------------------------------------- */



/** SiRF PAL communications handle. */
typedef tSIRF_HANDLE tSIRF_COM_HANDLE;


/* ----------------------------------------------------------------------------
 *   Function Prototypes
 * ------------------------------------------------------------------------- */



/* Enter C naming convention */
#ifdef __cplusplus
extern "C" {
#endif



/**
 * @brief Create a serial port and return a handle to it.
 *
 * This function creates a serial port object and returns a handle to it. The
 * serial port may then be opened and closed an arbitrary number of times.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - A valid port handle was returned.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - Out of resources.
 *
 * Once this function returns successfully, the following operations
 * may be performed:
 *   - SIRF_PAL_COM_Open
 *   - SIRF_PAL_COM_Delete
 *
 * @param[out] port_handle    Returned serial port handle.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_Create(
      tSIRF_COM_HANDLE * port_handle);

/**
 * @brief Delete a serial port.
 *
 * This function deletes a serial port created by the SIRF_PAL_COM_Create
 * function. If the serial port handle is invalid, or the port is still 
 * open then this function returns SIRF_PAL_COM_ERROR.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The port was successfully deleted.
 *   - SIRF_PAL_COM_ERROR      - An error occured while deleting.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_STATE      - The port is still open.
 *
 * @param[in] port_handle     Serial port to delete.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_Delete(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Open the serial port.
 *
 * This function opens a serial port created by the SIRF_PAL_COM_Create 
 * function.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The device was opened.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred opening the device.
 *   - SIRF_PAL_COM_STATE      - The port is already open.
 *
 * Once this function returns successfully, the following operations
 * may be performed on the handle:
 *   - SIRF_PAL_COM_SetUart
 *   - SIRF_PAL_COM_Wait
 *   - SIRF_PAL_COM_Read
 *   - SIRF_PAL_COM_Write
 *   - SIRF_PAL_COM_Close
 *   - SIRF_PAL_COM_ClrRTS
 *   - SIRF_PAL_COM_SetRTS
 *
 * @param[in] port_handle     Handle to the serial port.
 * @param[in] port            Hardware port number (1 = COM1, 2 = COM2, ...)
 * @param[in] flow_ctrl       flow control. 0 = no flow control, 1 = hardware fc
 *                            2 and above are reserved for future use
 * @param[in] baudrate        Serial baud rate (e.g 115200)
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_Open(
      tSIRF_COM_HANDLE port_handle, 
      tSIRF_UINT32     port, 
      tSIRF_UINT32     flow_ctrl,
      tSIRF_UINT32     baudrate );

/**
 * @brief Re-open the serial port.
 *
 * This function closes (if necessary) the serial port, and re-opens it with
 * the saved parameters from the last time the port was opened.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The device was reopened.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred re-opening the device.
 *   - SIRF_PAL_COM_STATE      - The port was never opened.
 *
 * @param[in] port_handle     Handle to the serial port.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_Reopen(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Close the open serial port.
 *
 * This function closes the open serial port handle. It does so by halting and
 * blocking all I/O operations (Wait / Read / Write) and once all active I/O
 * has halted, it closes the port.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The device was reopened.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred closing the device.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * Once this function returns successfully, the following operations
 * may be performed:
 *   - SIRF_PAL_COM_Open
 *   - SIRF_PAL_COM_Delete
 *
 * @param[in] port_handle     Handle to the port to close.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_Close(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Configure UART port.
 *
 * This function configures UART port.
 *
 * The possible return values are:
 *   - SIRF_SUCCESS            - The device was reconfigured.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred opening the device.
 *   - SIRF_PAL_COM_STATE      - The port is not open.
 *
 * @param[in] port_handle     Handle to the serial port.
 * @param[in] flow_ctrl       Hardware flow control.
 * @param[in] baud_rate       New serial baud rate. See SIRF_PAL_COM_Open
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_SetUart(
      tSIRF_COM_HANDLE port_handle, 
      tSIRF_UINT32     flow_ctrl,
      tSIRF_UINT32     baud_rate);

/**
 * @brief Wait for input on the serial port.
 *
 * This function waits up to the specified timeout period for activity on
 * the serial port. It acquires a semaphore lock for the duration of its
 * work to ensure that SIRF_PAL_COM_Close() does not close the serial port
 * while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Input ready for reading.
 *   - SIRF_PAL_COM_TIMEOUT    - Timed out waiting.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the wait.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @note If the port is not open then this function may return immediately 
 *       without yielding. Therefore threads calling this in a loop should
 *       protect themselves from consuming 100% of the processor.
 *
 * @param[in] port_handle     Handle to the port to wait on.
 * @param[in] timeout         Milliseconds to wait for.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_Wait(
      tSIRF_COM_HANDLE port_handle, 
      tSIRF_UINT32     timeout);

/**
 * @brief Read data from the serial port.
 *
 * This function reads data from the serial port. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in]  port_handle    Handle to the port to read from.
 * @param[out] data           Buffer to hold the read data.
 * @param[in]  length         Amount of data to read.
 * @param[out] bytes_read     Number of bytes read from the port.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_Read(
      tSIRF_COM_HANDLE   port_handle, 
      tSIRF_UINT8      * data, 
      tSIRF_UINT32       length, 
      tSIRF_UINT32     * bytes_read);

/**
 * @brief Write data to the serial port.
 *
 * This function writes data to the serial port. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @param[in] data            Pointer to the data to write.
 * @param[in] length          Amount of data to write.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_Write(
      tSIRF_COM_HANDLE   port_handle, 
      tSIRF_UINT8      * data, 
      tSIRF_UINT32       length);

/**
 * @brief Clear the serial port RTS line.
 *
 * This function clears the serial port RTS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_ClrRTS(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Set the serial port RTS line.
 *
 * This function sets the serial port RTS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_SetRTS(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Clear the serial port DTR line.
 *
 * This function clears the serial port DTR line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_ClrDTR(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Set the serial port DTR line.
 *
 * This function sets the serial port DTR line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_SetDTR(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Clear the serial port CTS line.
 *
 * This function clears the serial port CTS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_ClrCTS(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Set the serial port CTS line.
 *
 * This function sets the serial port CTS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_SetCTS(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Clear the SPI port SS line.
 *
 * This function clears the serial port SS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_ClrSS(
      tSIRF_COM_HANDLE port_handle);

/**
 * @brief Set the SPI port SS line.
 *
 * This function sets the serial port SS line. It acquires a semaphore 
 * lock for the duration of its work to ensure that SIRF_PAL_COM_Close() 
 * does not close the serial port while this function is using it.
 *
 * The three possible return values are:
 *   - SIRF_SUCCESS            - Data read successfully.
 *   - SIRF_PAL_COM_INVALIDARG - Invalid argument to function.
 *   - SIRF_PAL_COM_ERROR      - An error occurred during the read.
 *   - SIRF_PAL_COM_STATE      - The port was not open.
 *
 * @param[in] port_handle     Handle to the port to write to.
 * @return                    Return value (see table).
 */
tSIRF_RESULT SIRF_PAL_COM_SetSS(
      tSIRF_COM_HANDLE port_handle);

/* Leave C naming convention */
#ifdef __cplusplus
}
#endif

#endif /* !SIRF_PAL_COM_H_INCLUDED */

/**
 * @}
 * End of file.
 */


