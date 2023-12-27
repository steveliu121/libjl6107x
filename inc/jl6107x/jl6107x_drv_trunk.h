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

#ifndef __JL61XX_DRV_TRUNK_H__
#define __JL61XX_DRV_TRUNK_H__

#include "jl_base.h"
#include "jl_trunk.h"
#include "drv_common/jl_drv_common.h"

#ifdef cplusplus
extern "C" {
#endif

jl_ret_t jl6107x_link_aggregation_enable_set(jl_device_t *pdevice, jl_enable_t enable);
jl_ret_t jl6107x_link_aggregation_enable_get(jl_device_t *pdevice, jl_enable_t *penable);

jl_ret_t jl6107x_link_aggregation_portmask_set(jl_device_t *pdevice, jl_la_group_t la_id, jl_portmask_t la_portmask);
jl_ret_t jl6107x_link_aggregation_portmask_get(jl_device_t *pdevice, jl_la_group_t la_id, jl_portmask_t *pla_portmask);
jl_ret_t jl6107x_link_aggregation_port_del(jl_device_t *pdevice, jl_la_group_t la_id, jl_port_t port_id);
jl_ret_t jl6107x_link_aggregation_group_del(jl_device_t *pdevice, jl_la_group_t la_id);


jl_ret_t jl6107x_link_aggregation_hashfactor_set(jl_device_t *pdevice, jl_uint32 la_hashfactor_bitmask);
jl_ret_t jl6107x_link_aggregation_hashfactor_get(jl_device_t *pdevice, jl_uint32 *pla_hashfactor_bitmask);

jl_ret_t jl6107x_link_aggregation_hash_map_set(jl_device_t *pdevice, jl_la_group_t la_id, jl_portmask_t la_portmask);
jl_ret_t jl6107x_link_aggregation_hash_map_get(jl_device_t *pdevice, jl_la_hash2ports_t *pla_hash2ports);
jl_ret_t jl6107x_link_aggregation_hash_map_clear(jl_device_t *pdevice, jl_la_group_t la_id, jl_portmask_t la_portmask);

#ifdef cplusplus
extern }
#endif
#endif
