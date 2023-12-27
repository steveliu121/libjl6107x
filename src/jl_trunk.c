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

#include "jl_trunk.h"
#include "jl_ops.h"


jl_api_ret_t jl_link_aggregation_enable_set(jl_uint32 chip_id, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_enable_set, chip_id, enable);

	return ret;
}

jl_api_ret_t jl_link_aggregation_enable_get(jl_uint32 chip_id, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_enable_get, chip_id, penable);

	return ret;
}

jl_api_ret_t jl_link_aggregation_portmask_set(jl_uint32 chip_id, jl_la_group_t la_id, jl_portmask_t la_portmask)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_portmask_set, chip_id, la_id, la_portmask);

	return ret;
}

jl_api_ret_t jl_link_aggregation_portmask_get(jl_uint32 chip_id, jl_la_group_t la_id, jl_portmask_t *pla_portmask)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_portmask_get, chip_id, la_id, pla_portmask);

	return ret;
}

jl_api_ret_t jl_link_aggregation_port_del(jl_uint32 chip_id, jl_la_group_t la_id, jl_port_t port_id)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_port_del, chip_id, la_id, port_id);

	return ret;
}

jl_api_ret_t jl_link_aggregation_group_del(jl_uint32 chip_id, jl_la_group_t la_id)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_group_del, chip_id, la_id);

	return ret;
}

jl_api_ret_t jl_link_aggregation_hashfactor_set(jl_uint32 chip_id, jl_uint32 la_hashfactor_bitmask)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_hashfactor_set, chip_id, la_hashfactor_bitmask);

	return ret;
}

jl_api_ret_t jl_link_aggregation_hashfactor_get(jl_uint32 chip_id, jl_uint32 *pla_hashfactor_bitmask)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_hashfactor_get, chip_id, pla_hashfactor_bitmask);

	return ret;
}

jl_api_ret_t jl_link_aggregation_hash_map_set(jl_uint32 chip_id, jl_la_group_t la_id, jl_portmask_t la_portmask)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_weight_set, chip_id, la_id, la_portmask);

	return ret;
}

jl_api_ret_t jl_link_aggregation_hash_map_get(jl_uint32 chip_id, jl_la_hash2ports_t *pla_hash2ports)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_weight_get, chip_id, pla_hash2ports);

	return ret;
}
jl_api_ret_t jl_link_aggregation_hash_map_clear(jl_uint32 chip_id, jl_la_group_t la_id, jl_portmask_t la_portmask)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, link_aggregation_weight_clear, chip_id, la_id, la_portmask);

	return ret;
}


