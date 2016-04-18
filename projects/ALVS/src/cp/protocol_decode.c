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

#include "protocol_decode.h"
#include <stdio.h>
#include <string.h>
#include <EZapiChannel.h>

bool configure_my_mac(void)
{
	EZstatus retVal;
	FILE* fd;
	EZapiChannel_ProtocolDecoderParams sProtocolDecoderParams;

	memset(&sProtocolDecoderParams, 0, sizeof( sProtocolDecoderParams));

	sProtocolDecoderParams.uiProfile = 0;

	retVal = EZapiChannel_Status(0, EZapiChannel_StatCmd_GetProtocolDecoderParams, &sProtocolDecoderParams);
	if (EZrc_IS_ERROR(retVal)) {
		return false;
	}

	fd = fopen("/sys/class/net/eth0/address","r");
	if(fd == NULL) {
		return false;
	}
	fscanf(fd, "%2hhx%*c%2hhx%*c%2hhx%*c%2hhx%*c%2hhx%*c%2hhx",
	       &sProtocolDecoderParams.aucDestMACAddressLow[0],
	       &sProtocolDecoderParams.aucDestMACAddressLow[1],
	       &sProtocolDecoderParams.aucDestMACAddressLow[2],
	       &sProtocolDecoderParams.aucDestMACAddressLow[3],
	       &sProtocolDecoderParams.aucDestMACAddressLow[4],
	       &sProtocolDecoderParams.aucDestMACAddressLow[5]);
	fclose(fd);

	sProtocolDecoderParams.aucDestMACAddressHigh[0] = sProtocolDecoderParams.aucDestMACAddressLow[0];
	sProtocolDecoderParams.aucDestMACAddressHigh[1] = sProtocolDecoderParams.aucDestMACAddressLow[1];
	sProtocolDecoderParams.aucDestMACAddressHigh[2] = sProtocolDecoderParams.aucDestMACAddressLow[2];
	sProtocolDecoderParams.aucDestMACAddressHigh[3] = sProtocolDecoderParams.aucDestMACAddressLow[3];
	sProtocolDecoderParams.aucDestMACAddressHigh[4] = sProtocolDecoderParams.aucDestMACAddressLow[4];
	sProtocolDecoderParams.aucDestMACAddressHigh[5] = sProtocolDecoderParams.aucDestMACAddressLow[5];

	retVal = EZapiChannel_Config(0, EZapiChannel_ConfigCmd_SetProtocolDecoderParams, &sProtocolDecoderParams);

	if (EZrc_IS_ERROR(retVal)) {
		return false;
	}

	return true;
}
