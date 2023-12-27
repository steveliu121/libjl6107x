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
#ifndef __JL61XX_DRV_L2_H__
#define __JL61XX_DRV_L2_H__

#include "jl_base.h"
#include "jl_l2.h"

/****Macro Define****/
#define L2_TBL_MAX_DEPTH                (4096)
#define L2_HASH_XOR_FACTOR              (0x3ff)
#define L2_HASH_WIDTH                   (10)
#define L2_HASH_INPUT_LENGTH            (60)
#define L2_HASH_WAY_VALUE               (4)

#define L2_AGE_TIME_MAX_STEP            (5U)
#define L2_AGE_TIME_DEFAULT_MS_LEVEL    (3U)
#define L2_MULTICAST_PORTMSK_DEFAULT    (0x7ff)

#define L2_AGE_TIME_BASE                (2U)
#define L2_HW_TIME_BASE                 (3U)

#define L2_FID_MAX_VALUE                (4095)
#define L2_COL_TBL_MAX_DEPTH            (SWCORE_L2_LOOKUP_COLLISION_TABLE_NUM - 1)
/****Type Define****/
typedef enum L2_HASH_TABLE_MODE_E {
	L2_HASH_TABLE_MODE_LOOKUP,
	L2_HASH_TABLE_MODE_AGE,
	L2_HASH_TABLE_MODE_DESTINATION,
	L2_HASH_TABLE_MODE_END,
} L2_HASH_TABLE_MODE_T;


/* For firmware , these case will be set below
 * Learning And Aging Enable
 * Source port learning/dst Port
 * Hairpin Enable
 * Default Learning Status
 */
jl_ret_t jl61xx_l2_init(jl_device_t *pdevice);

jl_ret_t jl61xx_l2_deinit(jl_device_t *pdevice);

jl_ret_t jl61xx_l2_table_set(jl_device_t *pdevice, jl_l2_tbl_t *pcfg);

jl_ret_t jl61xx_l2_table_search(jl_device_t *pdevice, const jl_l2_sel_mode_t mode, jl_l2_tbl_t *pcfg);

jl_ret_t jl61xx_l2_table_clear(jl_device_t *pdevice, const jl_l2_sel_mode_t mode, jl_l2_tbl_t *pcfg);

jl_ret_t jl61xx_l2_dynamic_fast_clear(jl_device_t *pdevice);

jl_ret_t jl61xx_l2_hash_col_table_set(jl_device_t *pdevice, const jl_uint32 idx, jl_l2_tbl_t *pcfg);

jl_ret_t jl61xx_l2_hash_col_table_get(jl_device_t *pdevice, const jl_uint32 idx, jl_l2_tbl_t *pcfg);

jl_ret_t jl61xx_l2_set_agetime(jl_device_t *pdevice, const jl_uint32 time);

jl_ret_t jl61xx_l2_get_agetime(jl_device_t *pdevice, jl_uint32 *ptime);

jl_ret_t jl61xx_l2_mc_table_set(jl_device_t *pdevice, const jl_uint32 idx, const jl_uint32 portMask);

jl_ret_t jl61xx_l2_mc_table_get(jl_device_t *pdevice, const jl_uint32 idx, jl_uint32 *pPortMask);

jl_ret_t jl61xx_l2_mc_handle_set(jl_device_t *pdevice, jl_l2_mc_handle_t mcHandle, jl_bool enable);

jl_ret_t jl61xx_l2_set_spec_hash_tbl(jl_device_t *pdevice,
	const jl_uint32 hash_idx, const L2_HASH_TABLE_MODE_T mode, void *data);

#endif /* __JL61XX_DRV_L2_H__ */
