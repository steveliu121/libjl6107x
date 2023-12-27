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

#ifndef __JL6107X_DRV_CPU_H__
#define __JL6107X_DRV_CPU_H__

#include "jl_base.h"
#include "jl_cpu.h"


#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl6107x_cpu_allow_bpdu_enable_set(jl_device_t *device, jl_enable_t enable);
jl_ret_t jl6107x_cpu_allow_bpdu_enable_get(jl_device_t *device, jl_enable_t *penable);

jl_ret_t jl6107x_cpu_allow_rst_bpdu_enable_set(jl_device_t *device, jl_enable_t enable);
jl_ret_t jl6107x_cpu_allow_rst_bpdu_enable_get(jl_device_t *device, jl_enable_t *penable);

jl_ret_t jl6107x_cpu_tag_enable_get(jl_device_t *device, jl_enable_t *penable);
jl_ret_t jl6107x_cpu_tag_enable_set(jl_device_t *device, jl_enable_t enable);

jl_ret_t jl6107x_cpu_allow_lldp_enable_set(jl_device_t *device, jl_enable_t enable);
jl_ret_t jl6107x_cpu_allow_lldp_enable_get(jl_device_t *device, jl_enable_t *penable);

jl_ret_t jl6107x_cpu_rma_action_set(jl_device_t *device, jl_uint32 rma_mac_L8, jl_rma_action_t rma_action);
jl_ret_t jl6107x_cpu_rma_action_get(jl_device_t *device, jl_uint32 rma_mac_L8, jl_rma_action_t *prma_action);

jl_ret_t jl6107x_cpu_cpumac_set(jl_device_t *device, jl_mac_addr_t cpu_mac);
jl_ret_t jl6107x_cpu_cpumac_get(jl_device_t *device, jl_mac_addr_t *pcpu_mac);

jl_ret_t jl6107x_cpu_detection_cpumac_enable_set(jl_device_t *device, jl_enable_t enable);
jl_ret_t jl6107x_cpu_detection_cpumac_enable_get(jl_device_t *device, jl_enable_t *penable);


#ifdef __cplusplus
}
#endif

#endif /* __jl6107X_DRV_CPU_H__ */
