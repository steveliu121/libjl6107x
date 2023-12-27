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

#include "jl_mib.h"
#include "jl_ops.h"

jl_api_ret_t jl_mib_port_rc_mode_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_port_rc_mode_get, chip_id, port, penable);

	return ret;
}

jl_api_ret_t jl_mib_port_rc_mode_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_port_rc_mode_set, chip_id, port, enable);

	return ret;
}

jl_api_ret_t jl_mib_port_clear(jl_uint32 chip_id, jl_port_t port, jl_port_mib_type_t type)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_port_clear, chip_id, port, type);

	return ret;
}

jl_api_ret_t jl_mib_port_clear_all(jl_uint32 chip_id, jl_port_t port)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_port_clear_all, chip_id, port);

	return ret;
}

jl_api_ret_t jl_mib_port_get(jl_uint32 chip_id, jl_port_t port, jl_port_mib_type_t type, jl_mib_counter_t *pcnt)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_port_get, chip_id, port, type, pcnt);

	return ret;
}

jl_api_ret_t jl_mib_port_get_all(jl_uint32 chip_id, jl_port_t port, jl_port_mib_cnt_t *pport_cnt)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_port_get_all, chip_id, port, pport_cnt);

	return ret;
}

jl_api_ret_t jl_mib_swc_global_drop_get(jl_uint32 chip_id, jl_mib_swc_glb_drop_t *pport_cnt)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_swc_global_drop_get, chip_id, pport_cnt);

	return ret;
}

jl_api_ret_t jl_mib_swc_global_drop_clear(jl_uint32 chip_id)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_swc_global_drop_clear, chip_id);

	return ret;
}

jl_api_ret_t jl_mib_swc_port_drop_get(jl_uint32 chip_id, jl_port_t port, jl_mib_swc_port_drop_t *pport_cnt)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_swc_port_drop_get, chip_id, port, pport_cnt);

	return ret;
}

jl_api_ret_t jl_mib_swc_port_drop_clear(jl_uint32 chip_id, jl_port_t port)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mib_swc_port_drop_clear, chip_id, port);

	return ret;
}
