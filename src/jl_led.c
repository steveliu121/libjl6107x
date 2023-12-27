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

#include "jl_led.h"
#include "jl_ops.h"

jl_api_ret_t jl_led_enable_set(jl_uint32 chip_id, jl_led_group_t group, jl_portmask_t portmask)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, led_enable_set, chip_id, group, portmask);

	return ret;
}

jl_api_ret_t jl_led_enable_get(jl_uint32 chip_id, jl_led_group_t group, jl_portmask_t *pportmask)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, led_enable_get, chip_id, group, pportmask);

	return ret;
}

jl_api_ret_t jl_led_group_config_set(jl_uint32 chip_id, jl_led_group_t group, jl_led_config_t config)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, led_group_config_set, chip_id, group, config);

	return ret;
}

jl_api_ret_t jl_led_group_config_get(jl_uint32 chip_id, jl_led_group_t group, jl_led_config_t *pconfig)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, led_group_config_get, chip_id, group, pconfig);

	return ret;
}

jl_api_ret_t jl_led_blink_rate_set(jl_uint32 chip_id, jl_led_blink_rate_t blink_rate)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, led_blink_rate_set, chip_id, blink_rate);

	return ret;
}

jl_api_ret_t jl_led_blink_rate_get(jl_uint32 chip_id, jl_led_blink_rate_t *pblink_rate)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, led_blink_rate_get, chip_id, pblink_rate);

	return ret;
}

jl_api_ret_t jl_led_group_active_high_set(jl_uint32 chip_id, jl_led_group_t group, jl_portmask_t portmask)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, led_group_active_high_set, chip_id, group, portmask);

	return ret;
}

jl_api_ret_t jl_led_group_active_high_get(jl_uint32 chip_id, jl_led_group_t group, jl_portmask_t *pportmask)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, led_group_active_high_get, chip_id, group, pportmask);

	return ret;

}

