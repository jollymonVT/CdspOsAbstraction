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
* @brief Header file for the logging functions.
******************************************************************************/

#ifndef LOGS_H
#define LOGS_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// -----------------------------------------------------------------------------
// Defines and typedefs

/// Define this macro to include a timestamp in the log.
#ifndef LOG_TIMESTAMP
   #define LOG_TIMESTAMP               0
#endif

/// Define this macro to include the calling function name in the log.
#ifndef LOG_FUNCTION_NAME
   #define LOG_FUNCTION_NAME           0
#endif

#if (LOG_FUNCTION_NAME == 1)
   /// Macros to print common log messages.
   #define LOG_ERROR(...)                    LogPrintf(LOG_LEVEL_ERROR, __func__, __VA_ARGS__)
   #define LOG_WARNING(...)                  LogPrintf(LOG_LEVEL_WARNING, __func__, __VA_ARGS__)
   #define LOG_INFO(...)                     LogPrintf(LOG_LEVEL_INFO, __func__, __VA_ARGS__)

   /// Only include DEBUG and VERBOSE logs in Debug builds.
   #if (DEBUG_BUILD == 1)
      #define LOG_DEBUG(...)                 LogPrintf(LOG_LEVEL_DEBUG, __func__, __VA_ARGS__)

      /// Macro to print verbose logs by specifying a custom log level.
      #define LOG_LVL(_lvl_, ...)            LogPrintf(_lvl_, __func__, __VA_ARGS__)

      /// Macro to print hex data dump logs by specifying a custom log level.
      #define LOG_LVL_HEX(_lvl_, _buf_, _cnt_)    LogHexData(_lvl_, __func__,  _buf_, _cnt_)
   #endif // (DEBUG_BUILD == 1)
#else
   /// Macros to print common log messages.
   #define LOG_ERROR(...)                    LogPrintf(LOG_LEVEL_ERROR, __VA_ARGS__)
   #define LOG_WARNING(...)                  LogPrintf(LOG_LEVEL_WARNING, __VA_ARGS__)
   #define LOG_INFO(...)                     LogPrintf(LOG_LEVEL_INFO, __VA_ARGS__)

   /// Only include DEBUG and VERBOSE logs in Debug builds.
   #if (DEBUG_BUILD == 1)
      #define LOG_DEBUG(...)                 LogPrintf(LOG_LEVEL_DEBUG, __VA_ARGS__)

      /// Macro to print verbose logs by specifying a custom log level.
      #define LOG_LVL(_lvl_, ...)            LogPrintf(_lvl_, __VA_ARGS__)

      /// Macro to print hex data dump logs by specifying a custom log level.
      #define LOG_LVL_HEX(_lvl_, ...)        LogHexData(_lvl_, __VA_ARGS__)
   #endif // (DEBUG_BUILD == 1)
#endif // (LOG_FUNCTION_NAME == 1)

#if (DEBUG_BUILD != 1)
   // Compile out DEBUG and VERBOSE logs in Release builds.
   #define LOG_DEBUG(...)
   #define LOG_LVL(...)
   #define LOG_LVL_HEX(...)
#endif // (DEBUG_BUILD != 1)

/// Bit map to define the log priority levels.
typedef enum
{
   LOG_LEVEL_NONE          = 0x00000000,
   LOG_LEVEL_ERROR         = 0x00000001,
   LOG_LEVEL_WARNING       = 0x00000002,
   LOG_LEVEL_INFO          = 0x00000004,
   LOG_LEVEL_DEBUG         = 0x00000008,

   // Additional levels can be defined for more verbose logging for a
   // specific subsystem or task.

   LOG_LEVEL_FLASH         = 0x00000100,        // Hex data dump of flash read and write

   LOG_LEVEL_MAX           = 0xffffffff
} LogLevel_t;

/// The default bitmap of enabled logs levels.
#define LOG_LEVEL_DEFAULT  (LOG_LEVEL_ERROR |    \
                            LOG_LEVEL_WARNING |  \
                            LOG_LEVEL_INFO |     \
                            LOG_LEVEL_DEBUG)

// ----------------------------------------------------------------------------
// Global functions

void LogEnableLevels(LogLevel_t logLevel);
uint32_t LogGetLevels(void);

void LogPrintf(LogLevel_t logLevel,
#if (LOG_FUNCTION_NAME == 1)
               const char *functionName,
#endif
               const char *format, ...);

void LogHexData(LogLevel_t logLevel,
#if (LOG_FUNCTION_NAME == 1)
                const char *functionName,
#endif
                uint8_t const * const buffer, uint32_t const bufferSize);

#ifdef __cplusplus
}
#endif

#endif /* LOGS_H */
