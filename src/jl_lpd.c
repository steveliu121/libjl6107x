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

#include "jl_lpd.h"
#include "jl_ops.h"

jl_api_ret_t jl_lpd_config_get(jl_uint32 chip_id, jl_lpd_config_t *lpd_config)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lpd_config_get, chip_id, lpd_config);

	return ret;
}

jl_api_ret_t jl_lpd_config_set(jl_uint32 chip_id, jl_lpd_config_t *lpd_config)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lpd_config_set, chip_id, lpd_config);

	return ret;
}

jl_api_ret_t jl_lpd_enable_set(jl_uint32 chip_id, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lpd_enable_set, chip_id, enable);

	return ret;
}

jl_api_ret_t jl_lpd_enable_get(jl_uint32 chip_id, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lpd_enable_get, chip_id, penable);

	return ret;
}

jl_api_ret_t jl_lpd_port_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lpd_port_enable_set, chip_id, port, enable);

	return ret;
}

jl_api_ret_t jl_lpd_port_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lpd_port_enable_get, chip_id, port, penable);

	return ret;
}

jl_api_ret_t jl_lpd_port_status_get(jl_uint32 chip_id, jl_port_t port, jl_lpd_port_status_t *pstatus)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lpd_port_status_get, chip_id, port, pstatus);

	return ret;
}

jl_api_ret_t jl_lpd_status_history_get(jl_uint32 chip_id, jl_portmask_t *pmaskstatus)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lpd_status_history_get, chip_id, pmaskstatus);

	return ret;
}
