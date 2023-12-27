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

#include "jl_mirror.h"
#include "jl_ops.h"

jl_api_ret_t jl_mirror_input_mr_get(jl_uint32 chip_id, jl_port_t mirrored_port, jl_port_t *pmirroring_port)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, mirror_input_mr_get, chip_id, mirrored_port, pmirroring_port);

	return ret;
}

jl_api_ret_t jl_mirror_input_mr_set(jl_uint32 chip_id, jl_port_t mirrored_port, jl_port_t mirroring_port)
{

	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, mirror_input_mr_set, chip_id, mirrored_port, mirroring_port);

	return ret;
}

jl_api_ret_t jl_mirror_input_mr_clear(jl_uint32 chip_id, jl_port_t mirrored_port)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, mirror_input_mr_clear, chip_id, mirrored_port);

	return ret;
}

jl_api_ret_t jl_mirror_output_mr_get(jl_uint32 chip_id, jl_port_t mirrored_port, jl_port_t *pmirroring_port)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, mirror_output_mr_get, chip_id, mirrored_port, pmirroring_port);

	return ret;
}

jl_api_ret_t jl_mirror_output_mr_set(jl_uint32 chip_id, jl_port_t mirrored_port, jl_port_t mirroring_port)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, mirror_output_mr_set, chip_id, mirrored_port, mirroring_port);

	return ret;
}

jl_api_ret_t jl_mirror_output_mr_clear(jl_uint32 chip_id, jl_port_t mirrored_port)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, mirror_output_mr_clear, chip_id, mirrored_port);

	return ret;
}


