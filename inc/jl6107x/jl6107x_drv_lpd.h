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

#ifndef __jl6107x_DRV_LPD_H__
#define __jl6107x_DRV_LPD_H__

#include "jl_base.h"
#include "jl_lpd.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JL_LPD_TTL_MAX		0xf
#define JL_LPD_TIME_MAX		0xff

typedef enum jl_lpd_timescale_s {
	LPD_TIMESCALE_US = 0,
	LPD_TIMESCALE_MS,
	LPD_TIMESCALE_S,
	LPD_TIMESCALE_END
} jl_lpd_timescale_t;

jl_ret_t jl6107x_lpd_config_get(jl_device_t *device, jl_lpd_config_t *lpd_config);
jl_ret_t jl6107x_lpd_config_set(jl_device_t *device, jl_lpd_config_t *lpd_config);
jl_ret_t jl6107x_lpd_enable_set(jl_device_t *device, jl_enable_t enable);
jl_ret_t jl6107x_lpd_enable_get(jl_device_t *device, jl_enable_t *penable);
jl_ret_t jl6107x_lpd_port_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable);
jl_ret_t jl6107x_lpd_port_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
jl_ret_t jl6107x_lpd_port_status_get(jl_device_t *device, jl_port_t port, jl_lpd_port_status_t *pstatus);
jl_ret_t jl6107x_lpd_status_history_get(jl_device_t *device, jl_portmask_t *pmaskstatus);

#ifdef __cplusplus
}
#endif

#endif /* __jl6107x_DRV_LPD_H__ */
