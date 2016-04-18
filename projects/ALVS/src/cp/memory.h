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
*/

#ifndef _MEMORY_H_
#define _MEMORY_H_

#include <stdint.h>
#include <stdbool.h>
#include <EZapiChannel.h>

#define NUM_OF_MSIDS               12
#define HALF_CLUSTER_CODE_SIZE     0
#define HALF_CLUSTER_DATA_SIZE     0
#define X1_CLUSTER_CODE_SIZE       0
#define X1_CLUSTER_DATA_SIZE       0
#define X2_CLUSTER_CODE_SIZE       0
#define X2_CLUSTER_DATA_SIZE       0
#define X4_CLUSTER_CODE_SIZE       0
#define X4_CLUSTER_DATA_SIZE       0
#define X16_CLUSTER_CODE_SIZE      0
#define X16_CLUSTER_DATA_SIZE      0
#define ALL_CLUSTER_CODE_SIZE      0
#define ALL_CLUSTER_DATA_SIZE      0

static
uint32_t imem_sizes[NUM_OF_MSIDS][2] = {
		{EZapiChannel_IntMemSpaceType_HALF_CLUSTER_CODE, HALF_CLUSTER_CODE_SIZE},
		{EZapiChannel_IntMemSpaceType_HALF_CLUSTER_DATA, HALF_CLUSTER_DATA_SIZE},
		{EZapiChannel_IntMemSpaceType_1_CLUSTER_CODE, X1_CLUSTER_CODE_SIZE},
		{EZapiChannel_IntMemSpaceType_1_CLUSTER_DATA, X1_CLUSTER_DATA_SIZE},
		{EZapiChannel_IntMemSpaceType_2_CLUSTER_CODE, X2_CLUSTER_CODE_SIZE},
		{EZapiChannel_IntMemSpaceType_2_CLUSTER_DATA, X2_CLUSTER_DATA_SIZE},
		{EZapiChannel_IntMemSpaceType_4_CLUSTER_CODE, X4_CLUSTER_CODE_SIZE},
		{EZapiChannel_IntMemSpaceType_4_CLUSTER_DATA, X4_CLUSTER_DATA_SIZE},
		{EZapiChannel_IntMemSpaceType_16_CLUSTER_CODE, X16_CLUSTER_CODE_SIZE},
		{EZapiChannel_IntMemSpaceType_16_CLUSTER_DATA, X16_CLUSTER_DATA_SIZE},
		{EZapiChannel_IntMemSpaceType_ALL_CLUSTER_CODE, ALL_CLUSTER_CODE_SIZE},
		{EZapiChannel_IntMemSpaceType_ALL_CLUSTER_DATA, ALL_CLUSTER_DATA_SIZE} };

bool create_mem_partition(void);

uint32_t get_imem_index(void);


#endif /* _MEMORY_H_ */
