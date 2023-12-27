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

#include "jl_storm.h"
#include "jl_ops.h"

jl_api_ret_t jl_storm_control_enable_set(const jl_uint32 chip_id,
					 const jl_port_t egress_port,
					 const jl_storm_type_t type, const jl_bool enable) {
	jl_api_ret_t ret;

	JL_CHECK_ENUM(type, JL_STORM_TYPE_END);
	JL_CHECK_BOOL(enable);

	JL_DRV_SAFE_CALL(ret, stormctrl_enable_set, chip_id, egress_port, type, enable);
    return ret;
}

jl_api_ret_t jl_storm_control_enable_get(const jl_uint32 chip_id,
					   const jl_port_t egress_port,
					   const jl_storm_type_t type, jl_bool *penable) {
	jl_api_ret_t ret;

	JL_CHECK_POINTER(penable);
	JL_CHECK_ENUM(type, JL_STORM_TYPE_END);

	JL_DRV_SAFE_CALL(ret, stormctrl_enable_get, chip_id, egress_port, type, penable);
	return ret;
}

jl_api_ret_t jl_storm_control_rate_set(const jl_uint32 chip_id,
				       const jl_port_t egress_port,
				       const jl_storm_type_t type,
				       const jl_storm_rate_mode_t mode, const jl_uint32 rate) {
	jl_api_ret_t ret;

	JL_CHECK_ENUM(type, JL_STORM_TYPE_END);
	JL_CHECK_ENUM(mode, JL_STORM_RATE_MODE_END);

	JL_DRV_SAFE_CALL(ret, stormctrl_rate_set, chip_id, egress_port, type, mode, rate);
	return ret;
}

jl_api_ret_t jl_storm_control_rate_get(const jl_uint32 chip_id,
				       const jl_port_t egress_port,
				       const jl_storm_type_t type,
				       jl_storm_rate_mode_t *pmode, jl_uint32 *prate) {
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmode);
	JL_CHECK_POINTER(prate);
	JL_CHECK_ENUM(type, JL_STORM_TYPE_END);

	JL_DRV_SAFE_CALL(ret, stormctrl_rate_get, chip_id, egress_port, type, pmode, prate);
	return ret;
}
