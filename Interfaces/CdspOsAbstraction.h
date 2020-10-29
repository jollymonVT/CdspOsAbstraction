/***************************************************************************//**
 * COPYRIGHT 2020 CONNECTED DEVELOPMENT, INC
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
 * @brief  Header file for modem hardware specific routines
 ******************************************************************************/

#ifndef CDSP_OS_ABSTRACTION_H_
#define CDSP_OS_ABSTRACTION_H__

#ifdef __cplusplus
extern "C" {
#endif

void SetStartTime (void);
uint32_t GetRunningTime (void);
void CdspOsAssert (bool condition, uint32_t arg);
void CdspOsDelay (unsigned int ms);
	

#ifdef __cplusplus
}
#endif

#endif /* CELLMODEM_MODEMHAL_H_ */
