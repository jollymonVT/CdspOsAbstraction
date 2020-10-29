/**************************************************************************//**
* ## COPYRIGHT 2020 CONNECTED DEVELOPMENT, INC.
*
* ALL RIGHTS RESERVED BY AND FOR THE EXCLUSIVE BENEFIT OF
* CONNECTED DEVELOPMENT, INC.
*
* CONNECTED DEVELOPMENT, INC - CONFIDENTIAL AND PROPRIETARY
* INFORMATION AND/OR TRADE SECRET.
*
* NOTICE: ALL CODE, PROGRAM, INFORMATION, SCRIPT, INSTRUCTION,
* DATA, AND COMMENT HEREIN IS AND SHALL REMAIN THE CONFIDENTIAL
* INFORMATION AND PROPERTY OF CONNECTED DEVELOPMENT, INC.
* USE AND DISCLOSURE THEREOF, EXCEPT AS STRICTLY AUTHORIZED IN A
* WRITTEN AGREEMENT SIGNED BY CONNECTED DEVELOPMENT, INC IS PROHIBITED.
*
*******************************************************************************
* @file
* @brief Logging functions.
******************************************************************************/

#include "Logs.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <ctype.h>
#include "CdspOsAbstraction.h"

// -----------------------------------------------------------------------------
// Local defines and typedefs

/// Length of the log buffer.
#define LOG_BUFFER_LENGTH           256

/// Length of the hex dump line buffer.
#define HEX_DUMP_LINE_LENGTH        80

/// Offset in the hex dump line to the ASCII field separator '|'.
#define HEX_DUMP_ASCII_SEPARATOR    48

/// Offset in the hex dump line to the start of the ASCII field.
#define HEX_DUMP_ASCII_OFFSET       49

// -----------------------------------------------------------------------------
// Local variables

/// The currently enabled log levels.
static uint32_t       activeLogLevels = LOG_LEVEL_DEFAULT;

/// Buffer for log messages.
static char logBuffer[LOG_BUFFER_LENGTH];

// -----------------------------------------------------------------------------
// Local functions

/**************************************************************************//**
* @brief  Print a buffer.
*
*   Having this separate function allows for easier replacement, if needed.
*
* @param buffer  The buffer to print.
******************************************************************************/
static void WriteLog(char const *buffer)
{
   printf("%s",buffer);
}


// ----------------------------------------------------------------------------
// Global functions

/**************************************************************************//**
* @brief
*   Sets the enabled log levels to one or more levels.
*
* @param logLevels
*   The bitmap of log levels to be enabled.
******************************************************************************/
void LogEnableLevels(LogLevel_t logLevels)
{
   LOG_DEBUG("Set bitmap: 0x%08X", logLevels);
   activeLogLevels = logLevels;
}

/**************************************************************************//**
* @brief  Returns the currently enabled log levels.
* @return The bitmap of enabled log levels.
******************************************************************************/
uint32_t LogGetLevels(void)
{
   LOG_DEBUG("Active bitmap: 0x%08X", activeLogLevels);
   return activeLogLevels;
}

/**************************************************************************//**
* @brief
*   Print a formatted log message, if its level is enabled.
*
* @param logLevel
*   The log level of the message.
*
* @param functionName
*   The name of the calling function.
*
* @param format
*   A printf formatted string
******************************************************************************/
void LogPrintf(LogLevel_t logLevel,
#if (LOG_FUNCTION_NAME == 1)
               const char *functionName,
#endif
               const char *format, ...)
{
   if ((logLevel & activeLogLevels) != 0)
   {
      va_list argptr;
      va_start(argptr, format);
      
#if (LOG_TIMESTAMP == 1)
	  uint32_t currentTime = GetRunningTime();    // in msec
#endif

      // Aquire the logging semaphore.

      // Logs are formatted as follows:
      //   "LOG:timeInMsec:logLevel:function: user string\n"

      // Begin log with prefix, log level, and function name.
      snprintf(logBuffer, sizeof(logBuffer), "LOG:"
#if (LOG_TIMESTAMP == 1)
               "%d:"    // timestasmp
#endif
               "%X:"    // log level bitmap in hex

#if (LOG_FUNCTION_NAME == 1)
               "%s:"    // function name
#endif
               " "
#if (LOG_TIMESTAMP == 1)
               , currentTime
#endif
               , logLevel

#if (LOG_FUNCTION_NAME == 1)
               , functionName
#endif
              );

      WriteLog(logBuffer);

      // Append the caller's log data. Keeping this separate allows the user data
      // to have the full log buffer size.
      vsnprintf(logBuffer, sizeof(logBuffer), format, argptr);
      va_end(argptr);
      WriteLog(logBuffer);

      // Terminate the line.
      WriteLog("\n");

      // Release the semaphore.

   }
}

/**************************************************************************//**
* @brief
*   Print a log message in ASCII hex format, if its level is enabled.
*
* @param logLevel  The log level of the message.
* @param functionName  The name of the calling function.
* @param buffer  Pointer to the buffer
* @param bufferSize  Number of bytes to dump
******************************************************************************/
void LogHexData(LogLevel_t logLevel,
#if (LOG_FUNCTION_NAME == 1)
                const char *functionName,
#endif
                uint8_t const * const buffer, uint32_t const bufferSize)
{
   uint32_t n;
   char outBuf[HEX_DUMP_LINE_LENGTH];
   int a = 0;  // Character index in the ASCII field; 0-15
   int d = 0;  // Number of bytes in the hex data section; 0-48

   CdspOsAssert((NULL != buffer), 1);

   if ((logLevel & activeLogLevels) != 0)
   {
      memset(outBuf, ' ', sizeof(outBuf));

      // Lines are formatted as in the following sample:
      //   LOG:91948:100:LogLevelCmd: 40 41 42 43 44 45 46 47 48 4A 4B 4C 4D 4E 4E 4F |@ABCDEFGHJKLMNNO
      //   LOG:91958:100:LogLevelCmd: 50 51 52 53 54 55 56 57 58 5A 5B 5C 5D 5E 5E 5F |PQRSTUVWXZ[\]^^_
      //   LOG:91967:100:LogLevelCmd: 60 61 62 63 64 65 66 67 68 6A 6B 6C 6D 6E 6E 6F |`abcdefghjklmnno

      for (n = 0; n < bufferSize;)
      {
         d += sprintf(&outBuf[d], "%02X ", buffer[n]);
         if (buffer[n] == '%')
         {
            // We need to double '%' characters so they print.
            outBuf[HEX_DUMP_ASCII_OFFSET + a++] = '%';
            outBuf[HEX_DUMP_ASCII_OFFSET + a] = '%';
         }
         else if (isprint(buffer[n]))
         {
            outBuf[HEX_DUMP_ASCII_OFFSET + a] = buffer[n];
         }
         else
         {
            outBuf[HEX_DUMP_ASCII_OFFSET + a] = '.';
         }

         a++;
         n++;

         // End of a line?
         if ((n % 16 == 0) || (n >= bufferSize))
         {
            outBuf[d] = ' ';  // Overwrite the null terminator left by sprintf.
            outBuf[HEX_DUMP_ASCII_SEPARATOR] = '|';
            outBuf[HEX_DUMP_ASCII_OFFSET + a] = 0;

            LogPrintf(logLevel,
#if (LOG_FUNCTION_NAME == 1)
                      functionName,
#endif
                      "%s", outBuf);
            a = 0;
            d = 0;
            memset(outBuf, ' ', sizeof(outBuf));
         }
      }
   }
}

