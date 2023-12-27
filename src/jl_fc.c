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

#include "jl_fc.h"
#include "jl_ops.h"

jl_ret_t jl_port_fc_force_mode_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, port_fc_force_mode_set, chip_id, port, enable);

	return ret;
}

jl_ret_t jl_port_fc_force_mode_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, port_fc_force_mode_get, chip_id, port, penable);

	return ret;
}

jl_ret_t jl_port_fc_force_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, port_fc_force_enable_set, chip_id, port, enable);

	return ret;
}

jl_ret_t jl_port_fc_force_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, port_fc_force_enable_get, chip_id, port, penable);

	return ret;
}

jl_ret_t jl_port_fc_threshold_set(jl_uint32 chip_id, jl_port_t port,
			jl_uint32 threshold_on, jl_uint32 threshold_off)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, port_fc_threshold_set, chip_id, port, threshold_on, threshold_off);

	return ret;
}

jl_ret_t jl_port_fc_threshold_get(jl_uint32 chip_id, jl_port_t port,
			jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, port_fc_threshold_get, chip_id, port, pthreshold_on, pthreshold_off);

	return ret;
}

jl_api_ret_t jl_taildrop_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, taildrop_enable_set, chip_id, port, enable);

	return ret;
}

jl_api_ret_t jl_taildrop_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, taildrop_enable_get, chip_id, port, penable);

	return ret;
}

jl_api_ret_t jl_taildrop_counter_get(jl_uint32 chip_id, jl_uint32 *pcntr)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, taildrop_counter_get, chip_id, pcntr);

	return ret;
}

