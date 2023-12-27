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

#include "jl6107x/jl6107x_drv_rwreg.h"
#include "jl6107x/jl6107x_reg.h"

jl_ret_t jl6107x_reg_io_write(jl_device_t *device, jl_uint32 addr, jl_uint32 val)
{
	jl_ret_t ret;

	ret = jl_reg_write(&device->io_desc, addr, val);

	return ret;
}

jl_ret_t jl6107x_reg_io_read(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval)
{
	jl_ret_t ret;

	ret = jl_reg_read(&device->io_desc, addr, pval);

	return ret;
}

jl_ret_t jl6107x_reg_io_burst_write(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size)
{
	jl_ret_t ret;

	ret = jl_reg_burst_write(&device->io_desc, addr, pval, size);

	return ret;
}

jl_ret_t jl6107x_reg_io_burst_read(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size)
{
	jl_ret_t ret;

	ret = jl_reg_burst_read(&device->io_desc, addr, pval, size);

	return ret;
}
