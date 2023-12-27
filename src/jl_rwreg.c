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

#include "jl_rwreg.h"
#include "jl_ops.h"


jl_api_ret_t
jl_reg_io_write(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 val)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, reg_io_write, chip_id, addr, val);

	return ret;
}

jl_api_ret_t
jl_reg_io_read(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 *pval)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, reg_io_read, chip_id, addr, pval);

	return ret;
}

jl_api_ret_t
jl_reg_io_burst_write(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, reg_io_burst_write, chip_id, addr, pval, size);

	return ret;
}

jl_api_ret_t
jl_reg_io_burst_read(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, reg_io_burst_read, chip_id, addr, pval, size);

	return ret;
}

