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

#include "infrastructure.h"
#include "infrastructure_conf.h"
#include "memory_spaces_msids.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <EZapiChannel.h>
#include <EZapiStat.h>

enum infra_imem_spaces_params {
	INFRA_IMEM_SPACES_PARAMS_TYPE             = 0,
	INFRA_IMEM_SPACES_PARAMS_SIZE             = 1,
	INFRA_IMEM_SPACES_PARAMS_MSID             = 2,
	INFRA_NUM_OF_IMEM_SPACES_PARAMS
};

enum infra_emem_spaces_params {
	INFRA_EMEM_SPACES_PARAMS_TYPE             = 0,
	INFRA_EMEM_SPACES_PARAMS_PROTECTION       = 1,
	INFRA_EMEM_SPACES_PARAMS_SIZE             = 2,
	INFRA_EMEM_SPACES_PARAMS_MSID             = 3,
	INFRA_NUM_OF_EMEM_SPACES_PARAMS
};

#define INFRA_HALF_CLUSTER_CODE_SIZE 0
#define INFRA_X1_CLUSTER_CODE_SIZE 0
#define INFRA_X2_CLUSTER_CODE_SIZE 0
#define INFRA_X4_CLUSTER_CODE_SIZE 0
#define INFRA_X16_CLUSTER_CODE_SIZE 0
#define INFRA_ALL_CLUSTER_CODE_SIZE 0

#define HALF_CLUSTER_CODE_MSID        0x3
#define X1_CLUSTER_CODE_MSID          0x5
#define X2_CLUSTER_CODE_MSID          0x7
#define X4_CLUSTER_CODE_MSID          0x9
#define X16_CLUSTER_CODE_MSID         0xb
#define ALL_CLUSTER_CODE_MSID         0xd

#define NUM_OF_INT_MEMORY_SPACES 18
#define NUM_OF_EXT_MEMORY_SPACES 4

static
uint32_t imem_spaces_params[NUM_OF_INT_MEMORY_SPACES][INFRA_NUM_OF_IMEM_SPACES_PARAMS] = {
		{EZapiChannel_IntMemSpaceType_HALF_CLUSTER_CODE, INFRA_HALF_CLUSTER_CODE_SIZE, HALF_CLUSTER_CODE_MSID},
		{EZapiChannel_IntMemSpaceType_HALF_CLUSTER_DATA, INFRA_HALF_CLUSTER_DATA_SIZE, HALF_CLUSTER_DATA_MSID},
		{EZapiChannel_IntMemSpaceType_HALF_CLUSTER_SEARCH, INFRA_HALF_CLUSTER_SEARCH_SIZE, 0},
		{EZapiChannel_IntMemSpaceType_1_CLUSTER_CODE, INFRA_X1_CLUSTER_CODE_SIZE, X1_CLUSTER_CODE_MSID},
		{EZapiChannel_IntMemSpaceType_1_CLUSTER_DATA, INFRA_X1_CLUSTER_DATA_SIZE, X1_CLUSTER_DATA_MSID},
		{EZapiChannel_IntMemSpaceType_1_CLUSTER_SEARCH, INFRA_X1_CLUSTER_SEARCH_SIZE, 0},
		{EZapiChannel_IntMemSpaceType_2_CLUSTER_CODE, INFRA_X2_CLUSTER_CODE_SIZE, X2_CLUSTER_CODE_MSID},
		{EZapiChannel_IntMemSpaceType_2_CLUSTER_DATA, INFRA_X2_CLUSTER_DATA_SIZE, X2_CLUSTER_DATA_MSID},
		{EZapiChannel_IntMemSpaceType_2_CLUSTER_SEARCH, INFRA_X2_CLUSTER_SEARCH_SIZE, 0},
		{EZapiChannel_IntMemSpaceType_4_CLUSTER_CODE, INFRA_X4_CLUSTER_CODE_SIZE, X4_CLUSTER_CODE_MSID},
		{EZapiChannel_IntMemSpaceType_4_CLUSTER_DATA, INFRA_X4_CLUSTER_DATA_SIZE, X4_CLUSTER_DATA_MSID},
		{EZapiChannel_IntMemSpaceType_4_CLUSTER_SEARCH, INFRA_X4_CLUSTER_SEARCH_SIZE, 0},
		{EZapiChannel_IntMemSpaceType_16_CLUSTER_CODE, INFRA_X16_CLUSTER_CODE_SIZE, X16_CLUSTER_CODE_MSID},
		{EZapiChannel_IntMemSpaceType_16_CLUSTER_DATA, INFRA_X16_CLUSTER_DATA_SIZE, X16_CLUSTER_DATA_MSID},
		{EZapiChannel_IntMemSpaceType_16_CLUSTER_SEARCH, INFRA_X16_CLUSTER_SEARCH_SIZE, 0},
		{EZapiChannel_IntMemSpaceType_ALL_CLUSTER_CODE, INFRA_ALL_CLUSTER_CODE_SIZE, ALL_CLUSTER_CODE_MSID},
		{EZapiChannel_IntMemSpaceType_ALL_CLUSTER_DATA, INFRA_ALL_CLUSTER_DATA_SIZE, ALL_CLUSTER_DATA_MSID},
		{EZapiChannel_IntMemSpaceType_ALL_CLUSTER_SEARCH, INFRA_ALL_CLUSTER_SEARCH_SIZE, 0}
};

static
uint32_t emem_spaces_params[NUM_OF_EXT_MEMORY_SPACES][INFRA_NUM_OF_EMEM_SPACES_PARAMS] = {
		{EZapiChannel_ExtMemSpaceType_GENERAL, EZapiChannel_ExtMemSpaceECCType_NONE, INFRA_EMEM_DATA_NO_ECC_SIZE, EMEM_DATA_NO_ECC_MSID},
		{EZapiChannel_ExtMemSpaceType_GENERAL, EZapiChannel_ExtMemSpaceECCType_IN_BAND, INFRA_EMEM_DATA_IN_BAND_SIZE, EMEM_DATA_IN_BAND_MSID},
		{EZapiChannel_ExtMemSpaceType_GENERAL, EZapiChannel_ExtMemSpaceECCType_OUT_OF_BAND, INFRA_EMEM_DATA_OUT_OF_BAND_SIZE, EMEM_DATA_OUT_OF_BAND_MSID},
		{EZapiChannel_ExtMemSpaceType_SEARCH, 0, INFRA_EMEM_SEARCH_SIZE, EMEM_SEARCH_MSID}
};

bool infra_create_if_mapping(void)
{
	uint32_t ind;
	EZstatus ret_val;
	EZapiChannel_EthIFParams eth_if_params;
	EZapiChannel_EthRXChannelParams eth_rx_channel_params;

	/* Configure external interfaces */
	for(ind = 0; ind < INFRA_NW_IF_NUM; ind++) {
		eth_if_params.uiSide = INFRA_NW_IF_SIDE;
		eth_if_params.uiIFEngine = ind;
		eth_if_params.eEthIFType = INFRA_NW_IF_TYPE;
		eth_if_params.uiIFNumber = 0;

		ret_val = EZapiChannel_Status(0, EZapiChannel_StatCmd_GetEthIFParams, &eth_if_params);
		if (EZrc_IS_ERROR(ret_val)) {
			return false;
		}

		eth_if_params.bRXEnable = true;
		eth_if_params.bTXEnable = true;
		eth_if_params.bTXTMBypass = true;

		ret_val = EZapiChannel_Config(0, EZapiChannel_ConfigCmd_SetEthIFParams, &eth_if_params);
		if (EZrc_IS_ERROR(ret_val)) {
			return false;
		}


		eth_rx_channel_params.uiSide = INFRA_NW_IF_SIDE;
		eth_rx_channel_params.uiIFEngine  = ind;
		eth_rx_channel_params.eEthIFType  = INFRA_NW_IF_TYPE;
		eth_rx_channel_params.uiIFNumber  = 0;
		eth_rx_channel_params.uiRXChannel  = 0;

		ret_val = EZapiChannel_Status(0, EZapiChannel_StatCmd_GetEthRXChannelParams, &eth_rx_channel_params);
		if (EZrc_IS_ERROR (ret_val)) {
			return false;
		}

		eth_rx_channel_params.uiLogicalID = INFRA_NW_IF_BASE_LOGICAL_ID + ind;

		ret_val = EZapiChannel_Config(0, EZapiChannel_ConfigCmd_SetEthRXChannelParams, &eth_rx_channel_params);
		if (EZrc_IS_ERROR(ret_val)) {
			return false;
		}
	}

	/* Configure interface to host */
	eth_if_params.uiSide = INFRA_HOST_IF_SIDE;
	eth_if_params.uiIFEngine = INFRA_HOST_IF_ENGINE;
	eth_if_params.eEthIFType = EZapiChannel_EthIFType_10GE;
	eth_if_params.uiIFNumber = INFRA_HOST_IF_NUMBER;

	ret_val = EZapiChannel_Status(0, EZapiChannel_StatCmd_GetEthIFParams, &eth_if_params);
	if (EZrc_IS_ERROR(ret_val)) {
		return false;
	}

	eth_if_params.bRXEnable = true;
	eth_if_params.bTXEnable = true;
	eth_if_params.bTXTMBypass = true;

	ret_val = EZapiChannel_Config(0, EZapiChannel_ConfigCmd_SetEthIFParams, &eth_if_params);
	if (EZrc_IS_ERROR(ret_val)) {
		return false;
	}

	eth_rx_channel_params.uiSide = INFRA_HOST_IF_SIDE;
	eth_rx_channel_params.uiIFEngine  = INFRA_HOST_IF_ENGINE;
	eth_rx_channel_params.eEthIFType  = EZapiChannel_EthIFType_10GE;
	eth_rx_channel_params.uiIFNumber  = INFRA_HOST_IF_NUMBER;
	eth_rx_channel_params.uiRXChannel  = 0;

	ret_val = EZapiChannel_Status(0, EZapiChannel_StatCmd_GetEthRXChannelParams, &eth_rx_channel_params);
	if (EZrc_IS_ERROR (ret_val)) {
		return false;
	}

	eth_rx_channel_params.uiLogicalID = INFRA_HOST_IF_LOGICAL_ID;

	ret_val = EZapiChannel_Config(0, EZapiChannel_ConfigCmd_SetEthRXChannelParams, &eth_rx_channel_params);
	if (EZrc_IS_ERROR(ret_val)) {
		return false;
	}

	return true;
}

uint32_t get_imem_index(void)
{
	static uint32_t index = 0;
	return index++;
}

uint32_t get_emem_index(void)
{
	static uint32_t index = 0;
	return index++;
}

bool infra_create_mem_partition(void)
{
	EZstatus ret_val;
	EZapiChannel_IntMemSpaceParams int_mem_space_params;
	EZapiChannel_ExtMemSpaceParams ext_mem_space_params;
	uint32_t ind;

	for (ind = 0; ind < NUM_OF_INT_MEMORY_SPACES; ind++)
	{
		if (imem_spaces_params[ind][INFRA_IMEM_SPACES_PARAMS_SIZE] > 0) {
			memset(&int_mem_space_params, 0, sizeof(int_mem_space_params));

			int_mem_space_params.uiIndex = get_imem_index();
			ret_val = EZapiChannel_Status(0, EZapiChannel_StatCmd_GetIntMemSpaceParams, &int_mem_space_params);
			if (EZrc_IS_ERROR(ret_val)) {
				return false;
			}

			int_mem_space_params.bEnable = true;
			int_mem_space_params.eType = imem_spaces_params[ind][INFRA_IMEM_SPACES_PARAMS_TYPE];
			int_mem_space_params.uiSize = imem_spaces_params[ind][INFRA_IMEM_SPACES_PARAMS_SIZE];

			ret_val = EZapiChannel_Config(0, EZapiChannel_ConfigCmd_SetIntMemSpaceParams, &int_mem_space_params);
			if (EZrc_IS_ERROR(ret_val)) {
				return false;
			}
		}
	}

	for (ind = 0; ind < NUM_OF_EXT_MEMORY_SPACES; ind++)
	{
		if (emem_spaces_params[ind][INFRA_EMEM_SPACES_PARAMS_SIZE] > 0) {
			memset(&ext_mem_space_params, 0, sizeof(ext_mem_space_params));

			ext_mem_space_params.uiIndex = get_emem_index();
			ret_val = EZapiChannel_Status(0, EZapiChannel_StatCmd_GetExtMemSpaceParams, &ext_mem_space_params);
			if (EZrc_IS_ERROR(ret_val)) {
				return false;
			}

			ext_mem_space_params.bEnable = true;
			ext_mem_space_params.eType = emem_spaces_params[ind][INFRA_EMEM_SPACES_PARAMS_TYPE];
			ext_mem_space_params.uiSize = emem_spaces_params[ind][INFRA_EMEM_SPACES_PARAMS_SIZE];
			ext_mem_space_params.eECCType = emem_spaces_params[ind][INFRA_EMEM_SPACES_PARAMS_PROTECTION];
			ext_mem_space_params.uiMSID = emem_spaces_params[ind][INFRA_EMEM_SPACES_PARAMS_MSID];

			ret_val = EZapiChannel_Config(0, EZapiChannel_ConfigCmd_SetExtMemSpaceParams, &ext_mem_space_params);
			if (EZrc_IS_ERROR(ret_val)) {
				return false;
			}
		}
	}

	return true;
}

bool infra_configure_protocol_decode(void)
{
	EZstatus ret_val;
	FILE* fd;
	EZapiChannel_ProtocolDecoderParams protocol_decoder_params;

	memset(&protocol_decoder_params, 0, sizeof(protocol_decoder_params));

	protocol_decoder_params.uiProfile = 0;

	ret_val = EZapiChannel_Status(0, EZapiChannel_StatCmd_GetProtocolDecoderParams, &protocol_decoder_params);
	if (EZrc_IS_ERROR(ret_val)) {
		return false;
	}

	fd = fopen("/sys/class/net/"INFRA_HOST_INTERFACE"/address","r");
	if(fd == NULL) {
		return false;
	}
	fscanf(fd, "%2hhx%*c%2hhx%*c%2hhx%*c%2hhx%*c%2hhx%*c%2hhx",
	       &protocol_decoder_params.aucDestMACAddressLow[0],
	       &protocol_decoder_params.aucDestMACAddressLow[1],
	       &protocol_decoder_params.aucDestMACAddressLow[2],
	       &protocol_decoder_params.aucDestMACAddressLow[3],
	       &protocol_decoder_params.aucDestMACAddressLow[4],
	       &protocol_decoder_params.aucDestMACAddressLow[5]);
	fclose(fd);

	protocol_decoder_params.aucDestMACAddressHigh[0] = protocol_decoder_params.aucDestMACAddressLow[0];
	protocol_decoder_params.aucDestMACAddressHigh[1] = protocol_decoder_params.aucDestMACAddressLow[1];
	protocol_decoder_params.aucDestMACAddressHigh[2] = protocol_decoder_params.aucDestMACAddressLow[2];
	protocol_decoder_params.aucDestMACAddressHigh[3] = protocol_decoder_params.aucDestMACAddressLow[3];
	protocol_decoder_params.aucDestMACAddressHigh[4] = protocol_decoder_params.aucDestMACAddressLow[4];
	protocol_decoder_params.aucDestMACAddressHigh[5] = protocol_decoder_params.aucDestMACAddressLow[5];

	ret_val = EZapiChannel_Config(0, EZapiChannel_ConfigCmd_SetProtocolDecoderParams, &protocol_decoder_params);
	if (EZrc_IS_ERROR(ret_val)) {
		return false;
	}

	return true;
}

bool infra_create_statistics(void)
{
	EZstatus ret_val;
	EZapiStat_PostedPartitionParams posted_partition_params;
	EZapiStat_PostedGroupParams posted_group_params;

	memset(&posted_partition_params, 0, sizeof(posted_partition_params));

	posted_partition_params.uiPartition = 0;

	ret_val = EZapiStat_Status(0, EZapiStat_StatCmd_GetPostedPartitionParams, &posted_partition_params);
	if (EZrc_IS_ERROR(ret_val)) {
		printf("EZapiStat_Status: EZapiStat_StatCmd_GetPostedPartitionParams failed.\n");
		return false;
	}

	posted_partition_params.bEnable = true;
	posted_partition_params.uiMSID = EMEM_STATISTICS_POSTED_MSID;

	ret_val = EZapiStat_Config(0, EZapiStat_ConfigCmd_SetPostedPartitionParams, &posted_partition_params);
	if (EZrc_IS_ERROR(ret_val)) {
		printf("EZapiStat_Config: EZapiStat_ConfigCmd_SetPostedPartitionParams failed.\n");
		return false;
	}

	memset(&posted_group_params, 0, sizeof(posted_group_params));

	posted_group_params.uiPartition = 0;
	posted_group_params.eGroupType = EZapiStat_PostedGroupType_OPTIMIZED;

	ret_val = EZapiStat_Status(0, EZapiStat_StatCmd_GetPostedGroupParams, &posted_group_params);
	if (EZrc_IS_ERROR(ret_val)) {
		printf("EZapiStat_Status: EZapiStat_StatCmd_GetPostedGroupParams failed.\n");
		return false;
	}

	posted_group_params.eGroupType = EZapiStat_PostedGroupType_OPTIMIZED;
	posted_group_params.uiNumCounters = INFRA_STATS_POSTED_GROUP_SIZE;

	ret_val = EZapiStat_Config (0, EZapiStat_ConfigCmd_SetPostedGroupParams, &posted_group_params );
	if (EZrc_IS_ERROR(ret_val)) {
		printf("EZapiStat_Config: EZapiStat_ConfigCmd_SetPostedGroupParams failed.\n");
		return false;
	}

	return true;
}

bool infra_initialize_statistics(void)
{
	EZstatus ret_val;
	EZapiStat_PostedCounterConfig posted_counter_config;

	memset(&posted_counter_config, 0, sizeof(posted_counter_config));

	posted_counter_config.pasCounters = malloc(sizeof(EZapiStat_PostedCounter));
	memset(posted_counter_config.pasCounters, 0, sizeof(EZapiStat_PostedCounter));
	posted_counter_config.uiPartition = 0;
	posted_counter_config.bRange = true;
	posted_counter_config.uiStartCounter = 0;
	posted_counter_config.uiNumCounters = INFRA_STATS_POSTED_NUM;
	posted_counter_config.pasCounters[0].uiByteValue = 0;
	posted_counter_config.pasCounters[0].uiByteValueMSB = 0;
	posted_counter_config.pasCounters[0].uiFrameValue = 0;
	posted_counter_config.pasCounters[0].uiFrameValueMSB = 0;

	ret_val = EZapiStat_Config(0, EZapiStat_ConfigCmd_SetPostedCounters, &posted_counter_config);
	if (EZrc_IS_ERROR(ret_val)) {
		printf( "EZapiStat_Config: EZapiStat_ConfigCmd_SetPostedCounters failed.\n");
		return false;
	}

	free(posted_counter_config.pasCounters);

	return true;
}
