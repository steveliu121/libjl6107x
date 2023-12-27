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

#ifndef __JL6107X_DRV_SWITCH_H__
#define __JL6107X_DRV_SWITCH_H__

#include "jl_types.h"
#include "jl_device.h"
#include "jl_switch.h"
#include "jl6107x/jl6107x_reg.h"

#ifdef __cplusplus
extern "C" {
#endif

/*macro define*/
/*sys time base for 61XX: 25Mhz*/
#define JL_SYSTIME_BASE                             (25000000)
#define JL_SYSTIME_MAX_STEP                         (5)

#define JL_SYSTIME_DEFAULT_DIV                      (0x19)
#define JL_SYSTIME_DEFAULT_STEP                     (0xa)

#define JL_SYSTEM_JUMBO_LENGTH                      (16369)

#define JL_MMP_POINTER_NUM                          (16)

enum jl_mmp_pointer_module_e {
    JL_MMP_MODULE_INITIAL_ADM = 1,
    JL_MMP_MODULE_SOURCE_MAC,
    JL_MMP_MODULE_DEST_MAC,
    JL_MMP_MODULE_ACL,
    JL_MMP_MODULE_VID
};

typedef struct jl_mmp_used_info_s {
	jl_uint8 is_used;
	jl_uint8 adm_used_num;
	jl_uint8 acl_used_num;
	jl_uint8 smac_used_num;
	jl_uint8 dmac_used_num;
	jl_uint8 vlan_used_num;
} jl_mmp_used_info_t;

typedef struct jl_mmp_pointer_s {
	jl_mmp_used_info_t used_info[JL_MMP_POINTER_NUM];
} jl_mmp_pointer_t;

extern jl_uint32 g_tick_freq_list_6107[];

jl_ret_t jl6107x_calc_tick_freq(jl_device_t *pDevice);
jl_ret_t jl6107x_get_unused_mmp_pointer(jl_device_t *device, jl_uint8 *pptr);
jl_ret_t jl6107x_set_mmp_pointer_info(jl_device_t *device, jl_uint8 module, jl_uint8 mmp_ptr);
jl_ret_t jl6107x_get_mmp_pointer_info(jl_device_t *device, jl_uint8 mmp_ptr, jl_mmp_used_info_t *pptr_info);
jl_ret_t jl6107x_clear_mmp_pointer_info(jl_device_t *device, jl_uint8 module, jl_uint8 mmp_ptr);

jl_ret_t jl6107x_switch_info(jl_device_t *device, jl_switch_info_t *pinfo);
jl_ret_t jl6107x_chip_probe(jl_device_t *device);
jl_ret_t jl6107x_chip_remove(jl_device_t *device);
jl_ret_t jl6107x_load_patch(jl_device_t *device);
jl_ret_t jl6107x_show_switchport_info(jl_device_t *pdevice);

#ifdef __cplusplus
}
#endif
#endif /* __JL6107X_DRV_SWITCH_H__ */
