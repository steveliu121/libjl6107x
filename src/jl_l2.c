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

#include "jl_l2.h"
#include "jl_ops.h"

/*L2 Multicast TBL 64 entry for hal*/

jl_api_ret_t jl_l2_init(const jl_uint32 chip_id)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, l2_init, chip_id);
	return ret;
}
jl_api_ret_t jl_l2_deinit(const jl_uint32 chip_id)
{

	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, l2_deinit, chip_id);
	return ret;
}

/*fill hash tbl & shadow tbl in hal driver*/
jl_api_ret_t jl_l2_table_set(const jl_uint32 chip_id, jl_l2_tbl_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_BOOL(pcfg->valid);
	JL_CHECK_BOOL(pcfg->stat);

	JL_DRV_SAFE_CALL(ret, l2_table_set, chip_id, pcfg);
	return ret;
}
jl_api_ret_t jl_l2_table_search(const jl_uint32 chip_id, const jl_l2_sel_mode_t mode,
			jl_l2_tbl_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_ENUM(mode, JL_L2_SEL_MODE_END);

	JL_DRV_SAFE_CALL(ret, l2_table_search, chip_id, mode, pcfg);
	return ret;
}

jl_api_ret_t jl_l2_table_clear(const jl_uint32 chip_id, const jl_l2_sel_mode_t mode,
				  jl_l2_tbl_t *pcfg)
{

	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_ENUM(mode, JL_L2_SEL_MODE_END);

	JL_DRV_SAFE_CALL(ret, l2_table_clear, chip_id, mode, pcfg);
	return ret;
}

jl_api_ret_t jl_l2_hash_col_table_set(const jl_uint32 chip_id, const jl_uint32 idx, jl_l2_tbl_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_BOOL(pcfg->valid);
	JL_CHECK_BOOL(pcfg->stat);

	JL_DRV_SAFE_CALL(ret, l2_hash_col_table_set, chip_id, idx, pcfg);
	return ret;
}


jl_api_ret_t jl_l2_hash_col_table_get(const jl_uint32 chip_id, const jl_uint32 idx, jl_l2_tbl_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);

	JL_DRV_SAFE_CALL(ret, l2_hash_col_table_get, chip_id, idx, pcfg);
	return ret;
}

/*  age time = sys_tick_mode * tick_cnt
 *   This will auto cal in driver
 *   Suggest do not change sys clk , so age time will have deviation (round up)
 *   Unit:ms
 */
jl_api_ret_t jl_l2_set_agetime(const jl_uint32 chip_id, const jl_uint32 time)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, l2_set_agetime, chip_id, time);
	return ret;
}

jl_api_ret_t jl_l2_get_agetime(const jl_uint32 chip_id, jl_uint32 *ptime)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, l2_get_agetime, chip_id, ptime);
	return ret;
}

jl_api_ret_t jl_l2_mc_table_set(const jl_uint32 chip_id, const jl_uint32 idx, jl_uint32 portMask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, l2_mc_table_set, chip_id, idx, portMask);
	return ret;
}

jl_api_ret_t jl_l2_mc_table_get(const jl_uint32 chip_id, const jl_uint32 idx, jl_uint32 *pportMask)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pportMask);
	JL_DRV_SAFE_CALL(ret, l2_mc_table_get, chip_id, idx, pportMask);
	return ret;
}

/*This function use JL_L2_MC_HANDLE_xx*/

jl_api_ret_t jl_l2_mc_handle_set(const jl_uint32 chip_id, jl_l2_mc_handle_t mcHandle, jl_bool enable)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(mcHandle, JL_L2_MC_HANDLE_END);
	JL_CHECK_BOOL(enable);

	JL_DRV_SAFE_CALL(ret, l2_mc_handle_set, chip_id, mcHandle, enable);
	return ret;
}
