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

#include "jl_pti.h"
#include "jl_ops.h"

jl_api_ret_t jl_port_isolation_dst_set(jl_uint32 chip_id, jl_port_t dstport, jl_portmask_t portmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_isolation_dst_set, chip_id, dstport, portmask);

	return ret;
}

jl_api_ret_t jl_port_isolation_dst_get(jl_uint32 chip_id, jl_port_t dstport, jl_portmask_t *pportmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_isolation_dst_get, chip_id, dstport, pportmask);

	return ret;
}

jl_api_ret_t jl_port_isolation_src_set(jl_uint32 chip_id, jl_port_t srcport, jl_portmask_t portmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_isolation_src_set, chip_id, srcport, portmask);

	return ret;
}

jl_api_ret_t jl_port_isolation_src_get(jl_uint32 chip_id, jl_port_t srcport, jl_portmask_t *pportmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_isolation_src_get, chip_id, srcport, pportmask);

	return ret;
}

jl_api_ret_t jl_port_isolation_group_set(jl_uint32 chip_id, jl_pti_group_t group_id, jl_portmask_t portmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_isolation_group_set, chip_id, group_id, portmask);

	return ret;
}

jl_api_ret_t jl_port_isolation_group_get(jl_uint32 chip_id, jl_pti_group_t group_id, jl_portmask_t *pportmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_isolation_group_get, chip_id, group_id, pportmask);

	return ret;
}

jl_api_ret_t jl_port_isolation_group_clear(jl_uint32 chip_id, jl_pti_group_t group_id)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_isolation_group_clear, chip_id, group_id);

	return ret;
}
