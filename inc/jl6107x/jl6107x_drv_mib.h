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

#ifndef __jl6107x_DRV_MIB_H__
#define __jl6107x_DRV_MIB_H__

#include "jl_base.h"
#include "jl_mib.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JL_MIB_CHECK_TYPE(type)				\
	do {						\
		if (type >= PORT_MIB_TYPE_END)		\
			return JL_ERR_PARAM;		\
	} while (0)

jl_ret_t jl6107x_mib_port_rc_mode_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
jl_ret_t jl6107x_mib_port_rc_mode_set(jl_device_t *device, jl_port_t port, jl_uint32 enable);
jl_ret_t jl6107x_mib_port_clear(jl_device_t *device, jl_port_t port, jl_port_mib_type_t type);
jl_ret_t jl6107x_mib_port_clear_all(jl_device_t *device, jl_port_t port);
jl_ret_t jl6107x_mib_port_get(jl_device_t *device, jl_port_t port, jl_port_mib_type_t type, jl_mib_counter_t *pcnt);
jl_ret_t jl6107x_mib_port_get_all(jl_device_t *device, jl_port_t port, jl_port_mib_cnt_t *pport_cnt);
jl_ret_t jl6107x_mib_swc_global_drop_get(jl_device_t *device, jl_mib_swc_glb_drop_t *pport_cnt);
jl_ret_t jl6107x_mib_swc_global_drop_clear(jl_device_t *device);
jl_ret_t jl6107x_mib_swc_port_drop_get(jl_device_t *device, jl_port_t port, jl_mib_swc_port_drop_t *pport_cnt);
jl_ret_t jl6107x_mib_swc_port_drop_clear(jl_device_t *device, jl_port_t port);

#ifdef __cplusplus
}
#endif

#endif /* __jl6107x_DRV_MIB_H__ */
