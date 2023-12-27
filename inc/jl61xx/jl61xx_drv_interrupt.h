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

#ifndef __JL61XX_DRV_INTERRUPT_H__
#define __JL61XX_DRV_INTERRUPT_H__

#include "jl_base.h"
#include "jl_interrupt.h"

#ifdef __cplusplus
extern "C" {
#endif

	jl_ret_t jl61xx_int_port_phy_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t jl61xx_int_port_phy_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t jl61xx_int_port_phy_status_get(jl_device_t *device, jl_port_t port, jl_uint32 *pstatus);
	jl_ret_t jl61xx_int_enable_set(jl_device_t *device, jl_int_type_t type, jl_enable_t enable);
	jl_ret_t jl61xx_int_enable_get(jl_device_t *device, jl_int_type_t type, jl_enable_t *penable);
	jl_ret_t jl61xx_int_status_clean(jl_device_t *device, jl_int_type_t type);
	jl_ret_t jl61xx_int_status_get(jl_device_t *device, jl_int_status_t *pstatusmask);
	jl_ret_t jl61xx_int_detail_get(jl_device_t *device, jl_int_type_t type, jl_int_info_t *pinfo);
#ifdef __cplusplus
}
#endif
#endif				/* __JL6107X_DRV_INTERRUPT_H__ */
