/* Copyright (c) 2016 Mellanox Technologies, Ltd. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright
*    notice, this list of conditions and the following disclaimer.
* 2. Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the
*    documentation and/or other materials provided with the distribution.
* 3. Neither the names of the copyright holders nor the names of its
*    contributors may be used to endorse or promote products derived from
*    this software without specific prior written permission.
*
* Alternatively, this software may be distributed under the terms of the
* GNU General Public License ("GPL") version 2 as published by the Free
* Software Foundation.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*       Created on: May 18, 2016
*       file - alvs_log.h
*       description - contains definitions for alvs_log.c
*/

#ifndef ALVS_LOG_H_
#define ALVS_LOG_H_

#include <ezdp.h>
#include <ezframe.h>
#include "log.h"
#include "defs.h"
#include <arpa/inet.h>


#define UDP_SYSLOG_SERVER	514
#define ALVS_SYSLOG_WA	EZDP_SYSLOG_WA

#ifndef NDEBUG
#define alvs_write_log(priority, str, ...) \
		write_log_macro(priority, &cmem_wa.syslog_work_area, sizeof(cmem_wa.syslog_work_area), str, ##__VA_ARGS__)

#define alvs_write_log_simple(priority, str)

#else
#define ALVS_LOGMASK  LOG_UPTO(LOG_INFO)
#define alvs_write_log(priority, str, ...) { \
	if (LOG_MASK(priority) & ALVS_LOGMASK) { \
		write_log_macro(priority, &cmem_wa.syslog_work_area, sizeof(cmem_wa.syslog_work_area), str, ##__VA_ARGS__); \
	} \
}

#define alvs_write_log_simple(priority, str) { \
	if (LOG_MASK(priority) & ALVS_LOGMASK) { \
		write_log(priority, str, sizeof(str), cmem_wa.syslog_work_area, EZDP_SYSLOG_WA); \
	} \
}

#endif
/*****************************************************************************/
/*! \fn void alvs_open_log()
 * \brief  create syslog DP utility.
 * \param[in] s -  none
 * \return TRUE on SUCCESS, FALSE - otherwise.
 */
bool alvs_open_log(void);

/*****************************************************************************/
/*! \fn void alvs_send()
 * \brief  passes as send_cb function in open_log middle ware
 * \param[in]  s -  frame - frame to be sent
 *             wa_frame - WA for working with frame
 *             wa_frame_size - WA sizeof the provided wa_frame
 * \return 0 on SUCESS, otherwise 1.
 */
int alvs_send(ezframe_t  __cmem * frame) __slow_path_code;

#endif /* ALVS_LOG_H_ */
