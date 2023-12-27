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

#ifndef __JL6107X_DRV_RWREG_H__
#define __JL6107X_DRV_RWREG_H__

#include "jl_base.h"
#include "jl_rwreg.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl6107x_reg_io_write(jl_device_t *device, jl_uint32 addr, jl_uint32 val);
jl_ret_t jl6107x_reg_io_read(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval);
jl_ret_t jl6107x_reg_io_burst_write(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size);
jl_ret_t jl6107x_reg_io_burst_read(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size);

#ifdef __cplusplus
}
#endif

#endif /* __JL6107X_DRV_RWREG_H__ */
