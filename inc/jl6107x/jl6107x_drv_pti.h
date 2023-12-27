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

#ifndef __jl6107x_DRV_PTI_H__
#define __jl6107x_DRV_PTI_H__

#include "jl_base.h"
#include "jl_pti.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JL_PTI_CHECK_GRP_ID(group_id)				\
	do {							\
		if (group_id >= PTI_GROUP_END)			\
			return JL_ERR_PARAM;			\
	} while (0)

jl_ret_t jl6107x_port_isolation_dst_set(jl_device_t *device, jl_port_t dstport, jl_portmask_t portmask);
jl_ret_t jl6107x_port_isolation_dst_get(jl_device_t *device, jl_port_t dstport, jl_portmask_t *pportmask);
jl_ret_t jl6107x_port_isolation_src_set(jl_device_t *device, jl_port_t srcport, jl_portmask_t portmask);
jl_ret_t jl6107x_port_isolation_src_get(jl_device_t *device, jl_port_t srcport, jl_portmask_t *pportmask);
jl_ret_t jl6107x_port_isolation_group_set(jl_device_t *device, jl_pti_group_t group_id, jl_portmask_t portmask);
jl_ret_t jl6107x_port_isolation_group_get(jl_device_t *device, jl_pti_group_t group_id, jl_portmask_t *pportmask);
jl_ret_t jl6107x_port_isolation_group_clear(jl_device_t *device, jl_pti_group_t group_id);

#ifdef __cplusplus
}
#endif

#endif /* __jl6107x_DRV_PTI_H__ */

