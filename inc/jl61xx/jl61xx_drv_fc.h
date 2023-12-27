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

#ifndef __JL61XX_DRV_FC_H__
#define __JL61XX_DRV_FC_H__

#include "jl_base.h"
#include "jl_fc.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DEFAULT_CELLS (0x390U)
#define TAIL_DROP_CELLS (70U)


jl_ret_t jl61xx_taildrop_enable_set(jl_device_t *device, jl_port_t phyport, jl_enable_t enable);
jl_ret_t jl61xx_taildrop_enable_get(jl_device_t *device, jl_port_t phyport, jl_enable_t *penable);

jl_ret_t jl61xx_ingress_resource_manager_drop_counter_get(jl_device_t *device, jl_uint32 *pcntr);

jl_ret_t jl61xx_port_fc_force_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable);
jl_ret_t jl61xx_port_fc_force_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
jl_ret_t jl61xx_port_fc_force_mode_set(jl_device_t *device, jl_port_t port, jl_enable_t enable);
jl_ret_t jl61xx_port_fc_force_mode_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable);

jl_ret_t jl61xx_port_fc_threshold_set(jl_device_t *device, jl_port_t port,
			jl_uint32 threshold_on, jl_uint32 threshold_off);
jl_ret_t jl61xx_port_fc_threshold_get(jl_device_t *device, jl_port_t port,
			jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off);

#ifdef __cplusplus
}
#endif

#endif /* __JL61XX_DRV_FC_H__ */
