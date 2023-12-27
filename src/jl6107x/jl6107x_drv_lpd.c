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

#include "jl6107x/jl6107x_drv_lpd.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"

jl_ret_t jl6107x_lpd_config_get(jl_device_t *device, jl_lpd_config_t *lpd_config)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint64 tmpval = 0;
	jl_uint32 tmpaddr[2] = {0};

	JL_CHECK_POINTER(lpd_config);

	/* MAG LOOP_FRAME_SA Register setting */
	REGISTER_READ(device, MAG, LOOP_FRAME_SA, sa, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_FRAME_TYPE Register setting */
	REGISTER_READ(device, MAG, LOOP_FRAME_TYPE, type, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_FRAME_TTL Register setting */
	REGISTER_READ(device, MAG, LOOP_FRAME_TTL, ttl, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_INSERT_TIMESCALE Register setting */
	REGISTER_READ(device, MAG, LOOP_INSERT_TIMESCALE, timescale, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_INSERT_TIME Register setting */
	REGISTER_READ(device, MAG, LOOP_INSERT_TIME, time, INDEX_ZERO, INDEX_ZERO);

	REGISTER_READ_BITS(sa.val, &tmpval, MAG_LOOP_FRAME_SA_OFFSET_CFG_LP_FRAME_SA_0_31);
	tmpaddr[0] = (jl_uint32)tmpval;
	REGISTER_READ_BITS(sa.val, &tmpval, MAG_LOOP_FRAME_SA_OFFSET_CFG_LP_FRAME_SA_32_47);
	tmpaddr[1] = (jl_uint32)tmpval;
	memcpy(&(lpd_config->src_mac.addr[0]), &tmpaddr[0], ETHER_ADDR_LEN);
	lpd_config->eth_type = (jl_uint16)GET_BITS(type.BF.cfg_lp_frame_type, 16, 31);
	lpd_config->ttl = (jl_uint16)ttl.BF.cfg_lp_frame_ttl;
	lpd_config->timescale = (jl_uint16)timescale.BF.cfg_timescale;
	lpd_config->trig_time = (jl_uint16)time.BF.cfg_lp_trig_time;

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: sa[0]: 0x%x, sa[1]: 0x%x, type: 0x%x, ttl: 0x%x, timescale: 0x%x, time: 0x%x \n", \
					sa.val[0], sa.val[1], type.val[0], ttl.val[0], timescale.val[0], time.val[0]);

	return ret;
}

jl_ret_t jl6107x_lpd_config_set(jl_device_t *device, jl_lpd_config_t *lpd_config)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 tmpaddr[2] = {0};
	jl_uint64 tmpval = 0;
	jl_port_t port;

	JL_CHECK_POINTER(lpd_config);

	JL_CHECK_POINTER(lpd_config->src_mac.addr);

	if (lpd_config->ttl > JL_LPD_TTL_MAX || lpd_config->timescale >= LPD_TIMESCALE_END || \
		lpd_config->trig_time > JL_LPD_TIME_MAX)
		return JL_ERR_PARAM;

	/* MAG LOOP_FRAME_SA Register setting */
	REGISTER_READ(device, MAG, LOOP_FRAME_SA, sa, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_FRAME_TYPE Register setting */
	REGISTER_READ(device, MAG, LOOP_FRAME_TYPE, type, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_FRAME_TTL Register setting */
	REGISTER_READ(device, MAG, LOOP_FRAME_TTL, ttl, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_INSERT_TIMESCALE Register setting */
	REGISTER_READ(device, MAG, LOOP_INSERT_TIMESCALE, timescale, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_INSERT_TIME Register setting */
	REGISTER_READ(device, MAG, LOOP_INSERT_TIME, time, INDEX_ZERO, INDEX_ZERO);

	memcpy(&tmpaddr[0], &(lpd_config->src_mac.addr[0]), ETHER_ADDR_LEN);
	tmpval = tmpaddr[0];
	REGISTER_WRITE_BITS(sa.val, tmpval, MAG_LOOP_FRAME_SA_OFFSET_CFG_LP_FRAME_SA_0_31);
	tmpval = tmpaddr[1];
	REGISTER_WRITE_BITS(sa.val, tmpval, MAG_LOOP_FRAME_SA_OFFSET_CFG_LP_FRAME_SA_32_47);
	WRITE_BITS(type.BF.cfg_lp_frame_type, 16, 31, (jl_uint32)lpd_config->eth_type);
	ttl.BF.cfg_lp_frame_ttl = lpd_config->ttl;
	if (lpd_config->timescale == LPD_TIMESCALE_US)
		timescale.BF.cfg_timescale = LPD_TIMESCALE_US;
	else if (lpd_config->timescale == LPD_TIMESCALE_MS)
		timescale.BF.cfg_timescale = LPD_TIMESCALE_MS;
	else
		timescale.BF.cfg_timescale = LPD_TIMESCALE_S;
	time.BF.cfg_lp_trig_time = lpd_config->trig_time;

	/* MAG LOOP_FRAME_SA Register setting */
	REGISTER_WRITE(device, MAG, LOOP_FRAME_SA, sa, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_FRAME_TYPE Register setting */
	REGISTER_WRITE(device, MAG, LOOP_FRAME_TYPE, type, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_FRAME_TTL Register setting */
	REGISTER_WRITE(device, MAG, LOOP_FRAME_TTL, ttl, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_INSERT_TIMESCALE Register setting */
	REGISTER_WRITE(device, MAG, LOOP_INSERT_TIMESCALE, timescale, INDEX_ZERO, INDEX_ZERO);
	/* MAG LOOP_INSERT_TIME Register setting */
	REGISTER_WRITE(device, MAG, LOOP_INSERT_TIME, time, INDEX_ZERO, INDEX_ZERO);

	JL_FOR_EACH_UTP_PORT(device, port) {
		REGISTER_READ(device, LMAC_ADP, RX_LOOP_FRAME_CONTROL, llpdctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
		llpdctrl.BF.cfg_lp_frm_etype = lpd_config->eth_type;
		REGISTER_WRITE(device, LMAC_ADP, RX_LOOP_FRAME_CONTROL, llpdctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	}

	JL_FOR_EACH_EXT_PORT(device, port) {
		REGISTER_READ(device, SMAC_ADP, RX_LOOP_FRAME_CONTROL, slpdctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
		slpdctrl.BF.cfg_lp_frm_etype = lpd_config->eth_type;
		REGISTER_WRITE(device, SMAC_ADP, RX_LOOP_FRAME_CONTROL, slpdctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	}

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: sa[0]: 0x%x, sa[1]: 0x%x, type: 0x%x, ttl: 0x%x, timescale: 0x%x, time: 0x%x \n", \
					sa.val[0], sa.val[1], type.val[0], ttl.val[0], timescale.val[0], time.val[0]);

	return ret;
}

jl_ret_t jl6107x_lpd_enable_set(jl_device_t *device, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/* MAG LOOP_INSERT_ENABLE Register setting */
	REGISTER_READ(device, MAG, LOOP_INSERT_ENABLE, ldpen, INDEX_ZERO, INDEX_ZERO);

	ldpen.BF.cfg_lp_insert_en = enable;

	/* MAG LOOP_INSERT_ENABLE Register setting */
	REGISTER_WRITE(device, MAG, LOOP_INSERT_ENABLE, ldpen, INDEX_ZERO, INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: ldpen: 0x%x \n", \
					ldpen.val[0]);

	return ret;
}

jl_ret_t jl6107x_lpd_enable_get(jl_device_t *device, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(penable);

	/* MAG LOOP_INSERT_ENABLE Register setting */
	REGISTER_READ(device, MAG, LOOP_INSERT_ENABLE, ldpen, INDEX_ZERO, INDEX_ZERO);

	*penable = ldpen.BF.cfg_lp_insert_en;

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: ldpen: 0x%x \n", \
					ldpen.val[0]);

	return ret;
}

jl_ret_t jl6107x_lpd_port_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PORT;

	/* MAG LOOP_DETECT_CONTROL Register setting */
	REGISTER_READ(device, MAG, LOOP_DETECT_CONTROL, lpdctrl, INDEX_ZERO, INDEX_ZERO);

	WRITE_BIT(lpdctrl.BF.cfg_lp_detect_en, cport, enable);

	/* MAG LOOP_DETECT_CONTROL Register setting */
	REGISTER_WRITE(device, MAG, LOOP_DETECT_CONTROL, lpdctrl, INDEX_ZERO, INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: cport: %u, lpdctrl: 0x%x \n", \
					jlxx_port_l2c(device, port), lpdctrl.val[0]);

	return ret;
}

jl_ret_t jl6107x_lpd_port_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(penable);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PORT;

	/* MAG LOOP_DETECT_CONTROL Register setting */
	REGISTER_READ(device, MAG, LOOP_DETECT_CONTROL, lpdctrl, INDEX_ZERO, INDEX_ZERO);

	*penable = GET_BIT(lpdctrl.BF.cfg_lp_detect_en, cport);

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: cport: %u, lpdctrl: 0x%x \n", \
					jlxx_port_l2c(device, port), lpdctrl.val[0]);

	return ret;
}

jl_ret_t jl6107x_lpd_port_status_get(jl_device_t *device, jl_port_t port, jl_lpd_port_status_t *pstatus)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pstatus);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PORT;

	/* MAG LOOP_DETECT_STATUS Register setting */
	REGISTER_READ(device, MAG, LOOP_DETECT_STATUS, lpdsts, INDEX_ZERO, INDEX_ZERO);

	*pstatus = GET_BIT(lpdsts.BF.lp_detect_sta, cport);

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: cport: %u, lpdsts: 0x%x \n", \
					jlxx_port_l2c(device, port), lpdsts.val[0]);

	return ret;
}

jl_ret_t jl6107x_lpd_status_history_get(jl_device_t *device, jl_portmask_t *pmaskstatus)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_portmask_t tmpmask = 0;

	JL_CHECK_POINTER(pmaskstatus);

	*pmaskstatus = 0;

	/* MAG LOOP_DETECT_CONTROL Register setting */
	REGISTER_READ(device, MAG, LOOP_DETECT_HISTORY, lpdhis, INDEX_ZERO, INDEX_ZERO);

	tmpmask = lpdhis.val[0];
	ret = jlxx_portmask_c2l(device, tmpmask, pmaskstatus);

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: lpdhis: 0x%x \n", \
					lpdhis.val[0]);

	return ret;
}
