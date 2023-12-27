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

#include "jl61xx/jl61xx_drv_fc.h"
#include "jl61xx/jl61xx_drv_switch.h"
#include "jl61xx/jl61xx_chip.h"

jl_ret_t jl61xx_taildrop_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_PORT(device, port);
	JL_ASSERT(enable < JL_ENABLE_END);

	REGISTER_READ(device, SWCORE, PORT_TAIL_DROP_SETTINGS, val, jlxx_port_l2c(device, port), INDEX_ZERO);
	val.BF.enable = enable;
	REGISTER_WRITE(device, SWCORE, PORT_TAIL_DROP_SETTINGS, val, jlxx_port_l2c(device, port), INDEX_ZERO);

	REGISTER_READ(device, SWCORE, PORT_TAIL_DROP_FFA_THRESHOLD, ffa_val, jlxx_port_l2c(device, port), INDEX_ZERO);
	ffa_val.BF.enable = enable;
	if (enable == ENABLED)
		ffa_val.BF.cells = TAIL_DROP_CELLS;
	else
		ffa_val.BF.cells = DEFAULT_CELLS;
	REGISTER_WRITE(device, SWCORE, PORT_TAIL_DROP_FFA_THRESHOLD, ffa_val, jlxx_port_l2c(device, port), INDEX_ZERO);

	return ret;
}

jl_ret_t jl61xx_taildrop_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_PORT(device, port);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(device, SWCORE, PORT_TAIL_DROP_SETTINGS, val, jlxx_port_l2c(device, port), INDEX_ZERO);
	*penable = val.BF.enable;

	return ret;
}

jl_ret_t jl61xx_ingress_resource_manager_drop_counter_get(jl_device_t *device, jl_uint32 *pcntr)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pcntr);

	REGISTER_READ(device, SWCORE, INGRESS_RESOURCE_MANAGER_DROP, val, INDEX_ZERO, INDEX_ZERO);
	*pcntr = val.BF.packets;

	return ret;
}

jl_ret_t jl61xx_port_fc_force_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_UTP_EXT_PORT(device, port);
	JL_ASSERT(enable < JL_ENABLE_END);

	/*In forced mode,set fc enable or disable*/
	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));

		val.BF.cfg_force_rx_pause_en_value = enable;
		val.BF.cfg_force_tx_pause_en_value = enable;

		REGISTER_WRITE(device, LMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
	} else {
		REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));

		val.BF.cfg_force_rx_pause_en_value = enable;
		val.BF.cfg_force_tx_pause_en_value = enable;

		REGISTER_WRITE(device, SMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
	}

	return ret;
}

jl_ret_t jl61xx_port_fc_force_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_UTP_EXT_PORT(device, port);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(device, SWCORE, PORT_PAUSE_SETTINGS, val_pa_pause, jlxx_port_l2c(device, port), INDEX_ZERO);

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		*penable = val_pa_pause.BF.enable
					& val.BF.cfg_force_rx_pause_en_value
					& val.BF.cfg_force_tx_pause_en_value;
	} else {
		REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		*penable = val_pa_pause.BF.enable
					& val.BF.cfg_force_rx_pause_en_value
					& val.BF.cfg_force_tx_pause_en_value;
	}

	return ret;
}

jl_ret_t jl61xx_port_fc_force_mode_set(jl_device_t *device, jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_UTP_EXT_PORT(device, port);
	JL_ASSERT(enable < JL_ENABLE_END);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PORT;

	/*enable/disable fw alRun fc func*/
	REGISTER_READ(device, TOP, FW_RESERVED14, fwsrvd, INDEX_ZERO, INDEX_ZERO);
	if (enable == DISABLED)
		CLR_BIT(fwsrvd.BF.fw_reserved14, cport);
	else
		SET_BIT(fwsrvd.BF.fw_reserved14, cport);
	REGISTER_WRITE(device, TOP, FW_RESERVED14, fwsrvd, INDEX_ZERO, INDEX_ZERO);

	/*if enable value is 1,set force mode,else auto mode*/
	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		val.BF.cfg_force_rx_pause_en = enable;
		val.BF.cfg_force_tx_pause_en = enable;

		REGISTER_WRITE(device, LMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
	} else {
		REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		val.BF.cfg_force_rx_pause_en = enable;
		val.BF.cfg_force_tx_pause_en = enable;

		REGISTER_WRITE(device, SMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
	}

	return ret;
}

jl_ret_t jl61xx_port_fc_force_mode_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_UTP_EXT_PORT(device, port);
	JL_CHECK_POINTER(penable);

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		*penable = val.BF.cfg_force_rx_pause_en & val.BF.cfg_force_tx_pause_en;
	} else {
		REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		*penable = val.BF.cfg_force_rx_pause_en & val.BF.cfg_force_tx_pause_en;
	}

	return ret;
}

jl_ret_t jl61xx_port_fc_threshold_set(jl_device_t *device, jl_port_t port,
			jl_uint32 threshold_on, jl_uint32 threshold_off)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_PORT(device, port);

	if (threshold_on >= threshold_off)
		return JL_ERR_PARAM;

	REGISTER_READ(device, SWCORE, PORT_XON_FFA_THRESHOLD, xon_val, jlxx_port_l2c(device, port), INDEX_ZERO);
	xon_val.BF.cells = threshold_on;
	REGISTER_WRITE(device, SWCORE, PORT_XON_FFA_THRESHOLD, xon_val, jlxx_port_l2c(device, port), INDEX_ZERO);

	REGISTER_READ(device, SWCORE, PORT_XOFF_FFA_THRESHOLD, xoff_val, jlxx_port_l2c(device, port), INDEX_ZERO);
	xoff_val.BF.cells  = threshold_off;
	REGISTER_WRITE(device, SWCORE, PORT_XOFF_FFA_THRESHOLD, xoff_val, jlxx_port_l2c(device, port), INDEX_ZERO);

	return ret;
}

jl_ret_t jl61xx_port_fc_threshold_get(jl_device_t *device, jl_port_t port,
			jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_PORT(device, port);
	JL_CHECK_POINTER(pthreshold_on);
	JL_CHECK_POINTER(pthreshold_off);

	REGISTER_READ(device, SWCORE, PORT_XON_FFA_THRESHOLD, xon_val, jlxx_port_l2c(device, port), INDEX_ZERO);
	*pthreshold_on = xon_val.BF.cells;

	REGISTER_READ(device, SWCORE, PORT_XOFF_FFA_THRESHOLD, xoff_val, jlxx_port_l2c(device, port), INDEX_ZERO);
	*pthreshold_off = xoff_val.BF.cells;

	return ret;
}

