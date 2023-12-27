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

#include "jl_cpu.h"
#include "jl_ops.h"


jl_api_ret_t jl_cpu_allow_bpdu_enable_set(jl_uint32 chip_id, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_allow_bpdu_enable_set, chip_id, enable);

	return ret;
}

jl_api_ret_t jl_cpu_allow_bpdu_enable_get(jl_uint32 chip_id, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_allow_bpdu_enable_get, chip_id, penable);

	return ret;
}

jl_api_ret_t jl_cpu_allow_rst_bpdu_enable_set(jl_uint32 chip_id, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_allow_rst_bpdu_enable_set, chip_id, enable);

	return ret;
}

jl_api_ret_t jl_cpu_allow_rst_bpdu_enable_get(jl_uint32 chip_id, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_allow_rst_bpdu_enable_get, chip_id, penable);

	return ret;
}

jl_api_ret_t jl_cpu_tag_enable_set(jl_uint32 chip_id, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_tag_enable_set, chip_id, enable);

	return ret;
}

jl_api_ret_t jl_cpu_tag_enable_get(jl_uint32 chip_id, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_tag_enable_get, chip_id, penable);

	return ret;
}

jl_api_ret_t jl_cpu_allow_lldp_enable_set(jl_uint32 chip_id, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_allow_lldp_enable_set, chip_id, enable);

	return ret;
}

jl_api_ret_t jl_cpu_allow_lldp_enable_get(jl_uint32 chip_id, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_allow_lldp_enable_get, chip_id, penable);

	return ret;
}


jl_api_ret_t jl_cpu_rma_action_set(jl_uint32 chip_id, jl_uint32 rma_mac_L8, jl_rma_action_t rma_action)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_rma_action_set, chip_id, rma_mac_L8, rma_action);

	return ret;

}

jl_api_ret_t jl_cpu_rma_action_get(jl_uint32 chip_id, jl_uint32 rma_mac_L8, jl_rma_action_t *prma_action)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_rma_action_get, chip_id, rma_mac_L8, prma_action);

	return ret;

}

jl_api_ret_t jl_cpu_cpumac_set(jl_uint32 chip_id, jl_mac_addr_t cpu_mac)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_cpumac_set, chip_id, cpu_mac);

	return ret;

}

jl_api_ret_t jl_cpu_cpumac_get(jl_uint32 chip_id, jl_mac_addr_t *pcpu_mac)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_cpumac_get, chip_id, pcpu_mac);

	return ret;

}

jl_api_ret_t jl_cpu_detection_cpumac_enable_set(jl_uint32 chip_id, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_detection_cpumac_enable_set, chip_id, enable);

	return ret;

}

jl_api_ret_t jl_cpu_detection_cpumac_enable_get(jl_uint32 chip_id, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, cpu_detection_cpumac_enable_get, chip_id, penable);

	return ret;

}


