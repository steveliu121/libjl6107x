/*
 * Copyright (c) 2014-2023 JLSemi Limited
 * All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of JLSemi Limited
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * No part of this code may be reproduced, stored in a retrieval system,
 * or transmitted, in any form or by any means, electronic, mechanical,
 * photocopying, recording, or otherwise, without the prior written
 * permission of JLSemi Limited
 */

#include "jl6107x/jl6107x_drv_cpu.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"

jl_ret_t jl6107x_cpu_allow_bpdu_enable_set(jl_device_t *device, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_ASSERT(enable < JL_ENABLE_END);

	REGISTER_READ(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);

	val.BF.allow_bpdu = enable;
	REGISTER_WRITE(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_cpu_allow_bpdu_enable_get(jl_device_t *device, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);
	*penable = val.BF.allow_bpdu;

	return ret;
}

jl_ret_t jl6107x_cpu_allow_rst_bpdu_enable_set(jl_device_t *device, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_ASSERT(enable < JL_ENABLE_END);

	REGISTER_READ(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);

	val.BF.allow_rst_bpdu = enable;
	REGISTER_WRITE(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_cpu_allow_rst_bpdu_enable_get(jl_device_t *device, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);
	*penable = val.BF.allow_rst_bpdu;

	return ret;
}

jl_ret_t jl6107x_cpu_tag_enable_set(jl_device_t *device, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_ASSERT(enable < JL_ENABLE_END);

	REGISTER_READ(device, SWCORE, DISABLE_CPU_TAG_ON_CPU_PORT, val, INDEX_ZERO, INDEX_ZERO);

	if (enable == ENABLED)
		val.BF.disable  = 0;
	else
		val.BF.disable  = 1;

	REGISTER_WRITE(device, SWCORE, DISABLE_CPU_TAG_ON_CPU_PORT, val, INDEX_ZERO, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_cpu_tag_enable_get(jl_device_t *device, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(device, SWCORE, DISABLE_CPU_TAG_ON_CPU_PORT, val, INDEX_ZERO, INDEX_ZERO);

	if (val.BF.disable == 1)
		*penable = 0;
	else
		*penable = 1;

	return ret;
}

jl_ret_t jl6107x_cpu_allow_lldp_enable_set(jl_device_t *device, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 portmask = 0;

	JL_CHECK_POINTER(device);
	JL_ASSERT(enable < JL_ENABLE_END);

	REGISTER_READ(device, SWCORE, LLDP_CONFIGURATION, val, INDEX_ZERO, INDEX_ZERO);

	if (enable == ENABLED)
		SET_BITS(portmask, 0, (SWCORE_LLDP_CONFIGURATION_OFFSET_PORTMASK_WIDTH - 1));
	else
		CLR_BITS(portmask, 0, (SWCORE_LLDP_CONFIGURATION_OFFSET_PORTMASK_WIDTH - 1));

	val.BF.portmask = portmask;
	REGISTER_WRITE(device, SWCORE, LLDP_CONFIGURATION, val, INDEX_ZERO, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_cpu_allow_lldp_enable_get(jl_device_t *device, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 portmask = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(device, SWCORE, LLDP_CONFIGURATION, val, INDEX_ZERO, INDEX_ZERO);
	SET_BITS(portmask, 0, (SWCORE_LLDP_CONFIGURATION_OFFSET_PORTMASK_WIDTH - 1));

	if (val.BF.portmask == portmask)
		*penable = 1;
	else
		*penable = 0;

	return ret;
}

jl_ret_t jl6107x_cpu_rma_action_set(jl_device_t *device, jl_uint32 rma_mac_L8, jl_rma_action_t rma_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 portmask = 0;

	JL_CHECK_POINTER(device);
	JL_ASSERT(rma_mac_L8 <= MAX_RMA_NUM);
	JL_ASSERT(rma_action < RMA_ACTION_END);

	SET_BITS(portmask, 0, (SWCORE_L2_RESERVED_MULTICAST_ADDRESS_ACTION_OFFSET_SEND_TO_CPU_MASK_WIDTH-1));

	REGISTER_READ(device, SWCORE, L2_RESERVED_MULTICAST_ADDRESS_ACTION, val, rma_mac_L8, INDEX_ZERO);

	if (rma_action == RMA_ACTION_DROP) {
		val.BF.drop_mask = portmask;
		val.BF.send_to_cpu_mask = 0;
	} else if (rma_action == RMA_ACTION_SEND2CPU) {
		val.BF.drop_mask = 0;
		val.BF.send_to_cpu_mask = portmask;
	} else {
		val.BF.drop_mask = 0;
		val.BF.send_to_cpu_mask = 0;
	}

	REGISTER_WRITE(device, SWCORE, L2_RESERVED_MULTICAST_ADDRESS_ACTION, val, rma_mac_L8, INDEX_ZERO);
	return ret;
}

jl_ret_t jl6107x_cpu_rma_action_get(jl_device_t *device, jl_uint32 rma_mac_L8, jl_rma_action_t *prma_action)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 portmask = 0;

	JL_CHECK_POINTER(device);
	JL_ASSERT(rma_mac_L8 <= MAX_RMA_NUM);
	JL_CHECK_POINTER(prma_action);

	SET_BITS(portmask, 0, (SWCORE_L2_RESERVED_MULTICAST_ADDRESS_ACTION_OFFSET_SEND_TO_CPU_MASK_WIDTH-1));

	REGISTER_READ(device, SWCORE, L2_RESERVED_MULTICAST_ADDRESS_ACTION, val, rma_mac_L8, INDEX_ZERO);

	if ((val.BF.drop_mask == 0) && (val.BF.send_to_cpu_mask == 0))
		*prma_action = RMA_ACTION_FORWARD;
	else if ((val.BF.drop_mask == portmask) && (val.BF.send_to_cpu_mask == 0))
		*prma_action = RMA_ACTION_DROP;
	else if ((val.BF.drop_mask == 0) && (val.BF.send_to_cpu_mask == portmask))
		*prma_action = RMA_ACTION_SEND2CPU;
	else
		return JL_ERR_PARAM;

	return ret;

}

jl_ret_t jl6107x_cpu_cpumac_set(jl_device_t *device, jl_mac_addr_t cpu_mac)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);

	jl_uint32 lmac29 = ((jl_uint32)(cpu_mac.addr[2] & 0x1F)<<24)+
		BITS_OFFSET_L((jl_uint32)cpu_mac.addr[3], 16, 8) +
		BITS_OFFSET_L((jl_uint32)cpu_mac.addr[4], 8, 8) + cpu_mac.addr[5];
	jl_uint16 hmac19 = ((jl_uint32)cpu_mac.addr[0]<<11) + (cpu_mac.addr[1]<<3) + ((cpu_mac.addr[2]>>5) & 0x7);

	REGISTER_READ(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);
	val.BF.cpu_mac_addr_0_28 = lmac29;
	val.BF.cpu_mac_addr_29_47 = hmac19;
	REGISTER_WRITE(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_cpu_cpumac_get(jl_device_t *device, jl_mac_addr_t *pcpu_mac)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pcpu_mac);
	jl_uint32 lmac29 = 0;
	jl_uint16 hmac19 = 0;

	REGISTER_READ(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);
	lmac29 = val.BF.cpu_mac_addr_0_28;
	hmac19 = val.BF.cpu_mac_addr_29_47;

	pcpu_mac->addr[0] = BITS_OFFSET_R(hmac19, 11, 8);
	pcpu_mac->addr[1] = BITS_OFFSET_R(hmac19, 3, 8);
	pcpu_mac->addr[2] = ((hmac19 & 0x7)<<5) + BITS_OFFSET_R(lmac29, 24, 5);
	pcpu_mac->addr[3] = BITS_OFFSET_R(lmac29, 16, 8);
	pcpu_mac->addr[4] = BITS_OFFSET_R(lmac29, 8, 8);
	pcpu_mac->addr[5] = lmac29 & 0xFF;

	return ret;
}

jl_ret_t jl6107x_cpu_detection_cpumac_enable_set(jl_device_t *device, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);

	REGISTER_READ(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);
	val.BF.unique_cpu_mac = enable;
	REGISTER_WRITE(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_cpu_detection_cpumac_enable_get(jl_device_t *device, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(device, SWCORE, SEND_TO_CPU, val, INDEX_ZERO, INDEX_ZERO);
	*penable = val.BF.unique_cpu_mac;
	return ret;

}


