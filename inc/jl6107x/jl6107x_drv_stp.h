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

#ifndef __jl6107x_DRV_STP_H__
#define __jl6107x_DRV_STP_H__

#include "jl_base.h"
#include "jl_stp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JL_MSTP_MSTI_MAX_NUM	16
#define JL_MSTP_VID_MAX_NUM	4096

jl_ret_t jl6107x_stp_port_ingress_state_set(jl_device_t *device, jl_port_t port, jl_stp_state_t stp_state);
jl_ret_t jl6107x_stp_port_ingress_state_get(jl_device_t *device, jl_port_t port, jl_stp_state_t *pstp_state);
jl_ret_t jl6107x_stp_port_egress_state_set(jl_device_t *device, jl_port_t port, jl_stp_state_t stp_state);
jl_ret_t jl6107x_stp_port_egress_state_get(jl_device_t *device, jl_port_t port, jl_stp_state_t *pstp_state);
jl_ret_t jl6107x_stp_ingress_state_drop_get(jl_device_t *device, jl_stp_state_t stp_state, jl_uint32 *pcntr);
jl_ret_t jl6107x_stp_ingress_state_drop_clear(jl_device_t *device, jl_stp_state_t stp_state);
jl_ret_t jl6107x_stp_port_egress_drop_get(jl_device_t *device, jl_port_t port, jl_uint32 *pcntr);
jl_ret_t jl6107x_stp_port_egress_drop_clear(jl_device_t *device, jl_port_t port);
jl_ret_t jl6107x_mstp_port_ingress_state_set(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state);
jl_ret_t jl6107x_mstp_port_ingress_state_get(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state);
jl_ret_t jl6107x_mstp_port_egress_state_set(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state);
jl_ret_t jl6107x_mstp_port_egress_state_get(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state);
jl_ret_t jl6107x_mstp_ingress_state_drop_get(jl_device_t *device, jl_mstp_state_t mstp_state, jl_uint32 *pcntr);
jl_ret_t jl6107x_mstp_ingress_state_drop_clear(jl_device_t *device, jl_mstp_state_t mstp_state);
jl_ret_t jl6107x_mstp_port_egress_drop_get(jl_device_t *device, jl_port_t port, jl_uint32 *pcntr);
jl_ret_t jl6107x_mstp_port_egress_drop_clear(jl_device_t *device, jl_port_t port);
jl_ret_t jl6107x_mstp_vlan_msti_set(jl_device_t *device, jl_uint16 vid, jl_mstp_msti_t msti);
jl_ret_t jl6107x_mstp_vlan_msti_get(jl_device_t *device, jl_uint16 vid, jl_mstp_msti_t *pmsti);

#ifdef __cplusplus
}
#endif

#endif /* __jl6107x_DRV_STP_H__ */
