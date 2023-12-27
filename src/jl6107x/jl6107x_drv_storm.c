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

#include "jl6107x/jl6107x_drv_storm.h"
#include "jl6107x/jl6107x_chip.h"
#include "driver/jl_reg_io.h"
#include "jl6107x/jl6107x_drv_switch.h"

/****Type Define****/
static jl_ret_t _jl_stormctrl_bucket_set(jl_device_t *pDevice,
				  const jl_uint32 egress_port,
				  const jl_storm_type_t type, const jl_storm_rate_mode_t mode, const jl_uint32 tokens)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 mac_port;

	SWCORE_L2_BROADCAST_STORM_CONTROL_BUCKET_THRESHOLD_CONFIGURATION_t bcThrshld;
	SWCORE_L2_MULTICAST_STORM_CONTROL_BUCKET_THRESHOLD_CONFIGURATION_t mcThrshld;
	SWCORE_L2_FLOODING_STORM_CONTROL_BUCKET_THRESHOLD_CONFIGURATION_t floodThrshld;

	SWCORE_L2_BROADCAST_STORM_CONTROL_BUCKET_CAPACITY_CONFIGURATION_t bcCap;
	SWCORE_L2_MULTICAST_STORM_CONTROL_BUCKET_CAPACITY_CONFIGURATION_t mcCap;
	SWCORE_L2_FLOODING_STORM_CONTROL_BUCKET_CAPACITY_CONFIGURATION_t floodCap;

	/*for bucket size in PPS, need over max frame length * 10 bytes */
	/*16369B for XY chip */
	switch (type) {
	case JL_STORM_TYPE_BROADCAST:{
			if (JL_STORM_RATE_MODE_KBPS == mode) {
				if (JL_STORMCTRL_BUCKET_MINDEPTH_VAL_BPS >
				    tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_BPS) {
					bcThrshld.BF.threshold = JL_STORMCTRL_BUCKET_MINDEPTH_VAL_BPS;
				} else
					bcThrshld.BF.threshold = tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_BPS;
				bcCap.BF.bucket_capacity =
				    bcThrshld.BF.threshold * JL_STORMCTRL_BUCKET_BURSTSIZE_MULTIPLE_BPS;
			} else if (JL_STORM_RATE_MODE_PPS == mode) {
				bcThrshld.BF.threshold = tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_PPS;
				bcCap.BF.bucket_capacity =
				    bcThrshld.BF.threshold * JL_STORMCTRL_BUCKET_BURSTSIZE_MULTIPLE_PPS;
			}
			/*Send to driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_WRITE(pDevice, SWCORE, L2_BROADCAST_STORM_CONTROL_BUCKET_THRESHOLD_CONFIGURATION,
					mac_port, INDEX_ZERO, bcThrshld.val);
			REG_BURST_WRITE(pDevice, SWCORE, L2_BROADCAST_STORM_CONTROL_BUCKET_CAPACITY_CONFIGURATION, mac_port,
					INDEX_ZERO, bcCap.val);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: BC: mac port: %x, cap hex:%x, thrshld hex:%x\n",
			mac_port, bcCap.val[0], bcThrshld.val[0]);
			break;
		}
	case JL_STORM_TYPE_MULTICAST:{
			if (JL_STORM_RATE_MODE_KBPS == mode) {
				if (JL_STORMCTRL_BUCKET_MINDEPTH_VAL_BPS >
				    tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_BPS) {
					mcThrshld.BF.threshold = JL_STORMCTRL_BUCKET_MINDEPTH_VAL_BPS;
				} else
					mcThrshld.BF.threshold = tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_BPS;
				mcCap.BF.bucket_capacity =
				    mcThrshld.BF.threshold * JL_STORMCTRL_BUCKET_BURSTSIZE_MULTIPLE_BPS;
			} else if (JL_STORM_RATE_MODE_PPS == mode) {
				mcThrshld.BF.threshold = tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_PPS;
				mcCap.BF.bucket_capacity =
				    mcThrshld.BF.threshold * JL_STORMCTRL_BUCKET_BURSTSIZE_MULTIPLE_PPS;
			}
			/*Send to driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_WRITE(pDevice, SWCORE, L2_MULTICAST_STORM_CONTROL_BUCKET_THRESHOLD_CONFIGURATION,
					mac_port, INDEX_ZERO, mcThrshld.val);
			REG_BURST_WRITE(pDevice, SWCORE, L2_MULTICAST_STORM_CONTROL_BUCKET_CAPACITY_CONFIGURATION, mac_port,
					INDEX_ZERO, mcCap.val);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: MC: mac port: %x, cap hex:%x, thrshld hex:%x\n",
			mac_port, mcCap.val[0], mcThrshld.val[0]);
			break;
		}
	case JL_STORM_TYPE_FLOODING:{
			if (JL_STORM_RATE_MODE_KBPS == mode) {
				if (JL_STORMCTRL_BUCKET_MINDEPTH_VAL_BPS >
				    tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_BPS) {
					floodThrshld.BF.threshold = JL_STORMCTRL_BUCKET_MINDEPTH_VAL_BPS;
				} else
					floodThrshld.BF.threshold = tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_BPS;
				floodCap.BF.bucket_capacity =
				    floodThrshld.BF.threshold * JL_STORMCTRL_BUCKET_BURSTSIZE_MULTIPLE_BPS;
			} else if (JL_STORM_RATE_MODE_PPS == mode) {
				floodThrshld.BF.threshold = tokens * JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_PPS;
				floodCap.BF.bucket_capacity =
				    floodThrshld.BF.threshold * JL_STORMCTRL_BUCKET_BURSTSIZE_MULTIPLE_PPS;
			}
			/*Send to driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_WRITE(pDevice, SWCORE, L2_FLOODING_STORM_CONTROL_BUCKET_THRESHOLD_CONFIGURATION, mac_port,
					INDEX_ZERO, floodThrshld.val);
			REG_BURST_WRITE(pDevice, SWCORE, L2_FLOODING_STORM_CONTROL_BUCKET_CAPACITY_CONFIGURATION, mac_port,
					INDEX_ZERO, floodCap.val);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: FLD: mac port: %x, cap hex:%x, thrshld hex:%x\n",
			mac_port, floodCap.val[0], floodThrshld.val[0]);
			break;
		}
	default:{
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ERROR, "Not support this mode yet\n");
			return JL_ERR_PARAM;
		}
	}
	return ret;
}

jl_ret_t jl6107x_stormctrl_init(jl_device_t *pDevice)
{
	jl_ret_t ret = JL_ERR_OK;

	if (NULL == pDevice)
		return JL_ERR_PARAM;

	return ret;
}

jl_ret_t jl6107x_stormctrl_deinit(jl_device_t *pDevice)
{
	jl_ret_t ret = JL_ERR_OK;

	if (NULL == pDevice)
		return JL_ERR_PARAM;

	return ret;
}

jl_ret_t jl6107x_stormctrl_enable_set(jl_device_t *pDevice,
				     const jl_port_t egress_port, const jl_storm_type_t type, const jl_bool enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg = 0;
	jl_uint32 mac_port;

	/*para check*/
	JL_CHECK_PORT(pDevice, egress_port);

	switch (type) {
	case JL_STORM_TYPE_BROADCAST:{
			/*read from driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_BROADCAST_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);
			if (enable)
				SET_BIT(reg, mac_port);
			else
				CLR_BIT(reg, mac_port);
			/*send to driver */
			REG_BURST_WRITE(pDevice, SWCORE, L2_BROADCAST_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: reg val end is %x\n", reg);
			break;
		}
	case JL_STORM_TYPE_MULTICAST:{
			/*read from driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_MULTICAST_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);
			if (enable)
				SET_BIT(reg, mac_port);
			else
				CLR_BIT(reg, mac_port);
			/*send to driver */
			REG_BURST_WRITE(pDevice, SWCORE, L2_MULTICAST_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: reg val end is %x\n", reg);
			break;
		}
	case JL_STORM_TYPE_FLOODING:
		{
			/*read from driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_FLOODING_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);
			if (enable)
				SET_BIT(reg, mac_port);
			else
				CLR_BIT(reg, mac_port);
			/*send to driver */
			REG_BURST_WRITE(pDevice, SWCORE, L2_FLOODING_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: reg val end is %x\n", reg);
			break;
		}
	default:
		return JL_ERR_PARAM;
	}
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set eg port %d type %d %s success\n",
		   egress_port, type, enable ? "enable" : "disable");
	return ret;
}

jl_ret_t jl6107x_stormctrl_enable_get(jl_device_t *pDevice,
				     const jl_port_t egress_port, const jl_storm_type_t type, jl_bool *penable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 reg = 0;
	jl_uint32 mac_port;

	/*para check*/
	JL_CHECK_PORT(pDevice, egress_port);

	switch (type) {
	case JL_STORM_TYPE_BROADCAST:
		{
			/*read from driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_BROADCAST_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);
			if (CHECK_BIT(reg, mac_port))
				*penable = TRUE;
			else
				*penable = FALSE;

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: reg val end is %x\n", reg);
			break;
		}
	case JL_STORM_TYPE_MULTICAST:
		{
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_MULTICAST_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);
			if (CHECK_BIT(reg, mac_port))
				*penable = TRUE;
			else
				*penable = FALSE;
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: reg val end is %x\n", reg);
			break;
		}
	case JL_STORM_TYPE_FLOODING:
		{
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_FLOODING_STORM_CONTROL_ENABLE, INDEX_ZERO, INDEX_ZERO, &reg);
			if (CHECK_BIT(reg, mac_port))
				*penable = TRUE;
			else
				*penable = FALSE;
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "DBG: reg val end is %x\n", reg);
			break;
		}
	default:
		return JL_ERR_PARAM;
	}
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get eg port %d type %d %s\n", egress_port, type,
		   *penable ? "enable" : "disable");
	return ret;
}

jl_ret_t jl6107x_stormctrl_rate_set(jl_device_t *pDevice,
				   const jl_port_t egress_port,
				   const jl_storm_type_t type, const jl_storm_rate_mode_t mode, const jl_uint32 rate)
{

	jl_ret_t ret = JL_ERR_OK;
	jl_int32 i;
	jl_uint32 mac_port;

	/*para check*/
	JL_CHECK_PORT(pDevice, egress_port);

	SWCORE_L2_BROADCAST_STORM_CONTROL_RATE_CONFIGURATION_t bccfg;
	SWCORE_L2_MULTICAST_STORM_CONTROL_RATE_CONFIGURATION_t mccfg;
	SWCORE_L2_FLOODING_STORM_CONTROL_RATE_CONFIGURATION_t floodcfg;

	/*update tickstep value */
	ret = jl6107x_calc_tick_freq(pDevice);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Input eg port %d type %d rate mode %s val %d\n",
		   egress_port, type, mode == JL_STORM_RATE_MODE_KBPS ? "kbps" : "pps", rate);

	switch (type) {
	case JL_STORM_TYPE_BROADCAST:
		{
			if (JL_STORM_RATE_MODE_KBPS == mode) {
				bccfg.BF.packets_not_bytes = 0;
				for (i = JL_SYSTIME_MAX_STEP - 1; i >= 0; i--) {
					if ((g_tick_freq_list_6107[i] <= (rate * 1000 / 8)) &&
					    ((g_tick_freq_list_6107[i] <<
					      SWCORE_L2_BROADCAST_STORM_CONTROL_RATE_CONFIGURATION_OFFSET_TOKENS_WIDTH) >
					     (rate * 1000 / 8))) {
						bccfg.BF.tick = i;
						break;
					}
					if (!i) {
						JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Rate %d out of range\n", rate);
						return JL_ERR_OUT_OF_RANGE;
					}
				}

				bccfg.BF.tokens = (rate * 1000 / 8) / g_tick_freq_list_6107[i];
				bccfg.BF.ifg_correction = (IFG_DEFAULT_LENGTH + FCS_DEFAULT_LENGTH);

				/*set bucket value */
				_jl_stormctrl_bucket_set(pDevice, egress_port, type, mode, bccfg.BF.tokens);

			} else {
				bccfg.BF.packets_not_bytes = 1;
				for (i = JL_SYSTIME_MAX_STEP - 1; i >= 0; i--) {
					if ((g_tick_freq_list_6107[i] <= rate) &&
					    ((g_tick_freq_list_6107[i] <<
					      SWCORE_L2_BROADCAST_STORM_CONTROL_RATE_CONFIGURATION_OFFSET_TOKENS_WIDTH) >
					     rate)) {
						bccfg.BF.tick = i;
						break;
					}
				}
				if (!i) {
					JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Rate %d out of range\n", rate);
					return JL_ERR_OUT_OF_RANGE;
				}

				bccfg.BF.tokens = rate / g_tick_freq_list_6107[i];
				bccfg.BF.ifg_correction = (IFG_DEFAULT_LENGTH + FCS_DEFAULT_LENGTH);
				_jl_stormctrl_bucket_set(pDevice, egress_port, type, mode, bccfg.BF.tokens);
			}
			/*send to driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
				   "DBG: mac port: %x, mode: %d, token: %d, tick: %d ifg: %d, reg val is %x\n",
				   mac_port, bccfg.BF.packets_not_bytes, bccfg.BF.tokens, bccfg.BF.tick,
				   bccfg.BF.ifg_correction, bccfg.val[0]);
			REG_BURST_WRITE(pDevice, SWCORE, L2_BROADCAST_STORM_CONTROL_RATE_CONFIGURATION, mac_port,
					INDEX_ZERO, bccfg.val);
			break;
		}
	case JL_STORM_TYPE_MULTICAST:
		{
			if (JL_STORM_RATE_MODE_KBPS == mode) {
				mccfg.BF.packets_not_bytes = 0;
				for (i = JL_SYSTIME_MAX_STEP - 1; i >= 0; i--) {
					if ((g_tick_freq_list_6107[i] <= (rate * 1000 / 8)) &&
					    ((g_tick_freq_list_6107[i] <<
					      SWCORE_L2_MULTICAST_STORM_CONTROL_RATE_CONFIGURATION_OFFSET_TOKENS_WIDTH) >
					     (rate * 1000 / 8))) {
						mccfg.BF.tick = i;
						break;
					}
					if (!i) {
						JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Rate %d out of range\n", rate);
						return JL_ERR_OUT_OF_RANGE;
					}
				}

				mccfg.BF.tokens = (rate * 1000 / 8) / g_tick_freq_list_6107[i];
				mccfg.BF.ifg_correction = (IFG_DEFAULT_LENGTH + FCS_DEFAULT_LENGTH);
				_jl_stormctrl_bucket_set(pDevice, egress_port, type, mode, mccfg.BF.tokens);
			} else {
				mccfg.BF.packets_not_bytes = 1;
				for (i = JL_SYSTIME_MAX_STEP - 1; i >= 0; i--) {
					if ((g_tick_freq_list_6107[i] <= rate) &&
					    ((g_tick_freq_list_6107[i] <<
					      SWCORE_L2_MULTICAST_STORM_CONTROL_RATE_CONFIGURATION_OFFSET_TOKENS_WIDTH) >
					     rate)) {
						mccfg.BF.tick = i;
						break;
					}
				}
				if (!i) {
					JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Rate %d out of range\n", rate);
					return JL_ERR_OUT_OF_RANGE;
				}

				mccfg.BF.tokens = rate / g_tick_freq_list_6107[i];
				mccfg.BF.ifg_correction = (IFG_DEFAULT_LENGTH + FCS_DEFAULT_LENGTH);
				_jl_stormctrl_bucket_set(pDevice, egress_port, type, mode, mccfg.BF.tokens);
			}
			/*send to driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
				   "DBG: mac port: %x, mode: %d, token: %d, tick: %d, ifg: %d, reg val is %x\n",
				   mac_port, mccfg.BF.packets_not_bytes, mccfg.BF.tokens, mccfg.BF.tick,
				   mccfg.BF.ifg_correction, mccfg.val[0]);
			REG_BURST_WRITE(pDevice, SWCORE, L2_MULTICAST_STORM_CONTROL_RATE_CONFIGURATION, mac_port,
					INDEX_ZERO, mccfg.val);
			break;
		}
	case JL_STORM_TYPE_FLOODING:
		{
			if (JL_STORM_RATE_MODE_KBPS == mode) {
				floodcfg.BF.packets_not_bytes = 0;
				for (i = JL_SYSTIME_MAX_STEP - 1; i >= 0; i--) {
					if ((g_tick_freq_list_6107[i] <= (rate * 1000 / 8)) &&
					    ((g_tick_freq_list_6107[i] <<
					      SWCORE_L2_FLOODING_STORM_CONTROL_RATE_CONFIGURATION_OFFSET_TOKENS_WIDTH) >
					     (rate * 1000 / 8))) {
						floodcfg.BF.tick = i;
						break;
					}
					if (!i) {
						JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Rate %d out of range\n", rate);
						return JL_ERR_OUT_OF_RANGE;
					}
				}

				floodcfg.BF.tokens = (rate * 1000 / 8) / g_tick_freq_list_6107[i];
				floodcfg.BF.ifg_correction = (IFG_DEFAULT_LENGTH + FCS_DEFAULT_LENGTH);
				_jl_stormctrl_bucket_set(pDevice, egress_port, type, mode, floodcfg.BF.tokens);
			} else {
				floodcfg.BF.packets_not_bytes = 1;
				for (i = JL_SYSTIME_MAX_STEP - 1; i >= 0; i--) {
					if ((g_tick_freq_list_6107[i] <= rate) &&
					    ((g_tick_freq_list_6107[i] <<
					      SWCORE_L2_FLOODING_STORM_CONTROL_RATE_CONFIGURATION_OFFSET_TOKENS_WIDTH) >
					     rate)) {
						floodcfg.BF.tick = i;
						break;
					}
				}
				if (!i) {
					JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Rate %d out of range\n", rate);
					return JL_ERR_OUT_OF_RANGE;
				}

				floodcfg.BF.tokens = rate / g_tick_freq_list_6107[i];
				floodcfg.BF.ifg_correction = (IFG_DEFAULT_LENGTH + FCS_DEFAULT_LENGTH);
				_jl_stormctrl_bucket_set(pDevice, egress_port, type, mode, floodcfg.BF.tokens);
			}
			/*send to driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
				   "DBG: mac port: %x, mode: %d, token: %d, tick: %d, ifg: %d, reg val is %x\n",
				   mac_port, floodcfg.BF.packets_not_bytes, floodcfg.BF.tokens, floodcfg.BF.tick,
				   floodcfg.BF.ifg_correction, floodcfg.val[0]);
			REG_BURST_WRITE(pDevice, SWCORE, L2_FLOODING_STORM_CONTROL_RATE_CONFIGURATION, mac_port, INDEX_ZERO,
					floodcfg.val);
			break;
		}
	default:
		return JL_ERR_PARAM;
	}

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set eg port %d type %d rate mode %s val %d\n",
		   egress_port, type, (JL_STORM_RATE_MODE_KBPS) == (mode) ? "kbps" : "pps", rate);
	return ret;
}

jl_ret_t jl6107x_stormctrl_rate_get(jl_device_t *pDevice,
				   const jl_port_t egress_port,
				   const jl_storm_type_t type, jl_storm_rate_mode_t *pmode, jl_uint32 *prate)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 mac_port;
	SWCORE_L2_BROADCAST_STORM_CONTROL_RATE_CONFIGURATION_t bcCfg;
	SWCORE_L2_MULTICAST_STORM_CONTROL_RATE_CONFIGURATION_t mcCfg;
	SWCORE_L2_FLOODING_STORM_CONTROL_RATE_CONFIGURATION_t UnUniCfg;

	/*para check*/
	JL_CHECK_PORT(pDevice, egress_port);

	ret = jl6107x_calc_tick_freq(pDevice);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	switch (type) {
	case JL_STORM_TYPE_BROADCAST:
		{
			/*read from driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_BROADCAST_STORM_CONTROL_RATE_CONFIGURATION, mac_port, INDEX_ZERO,
				       bcCfg.val);
			*pmode = bcCfg.BF.packets_not_bytes;
			if (bcCfg.BF.packets_not_bytes)
				*prate = bcCfg.BF.tokens * g_tick_freq_list_6107[bcCfg.BF.tick];
			else
				*prate = bcCfg.BF.tokens * g_tick_freq_list_6107[bcCfg.BF.tick] * 8 / 1000;
			break;
		}
	case JL_STORM_TYPE_MULTICAST:
		{
			/*read from driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_MULTICAST_STORM_CONTROL_RATE_CONFIGURATION, mac_port, INDEX_ZERO,
				       mcCfg.val);
			*pmode = mcCfg.BF.packets_not_bytes;
			if (mcCfg.BF.packets_not_bytes)
				*prate = mcCfg.BF.tokens * g_tick_freq_list_6107[mcCfg.BF.tick];
			else
				*prate = mcCfg.BF.tokens * g_tick_freq_list_6107[mcCfg.BF.tick] * 8 / 1000;
			break;
		}
	case JL_STORM_TYPE_FLOODING:
		{
			/*read from driver */
			JL_PORT_L2C_MACRO(pDevice, egress_port, mac_port);
			REG_BURST_READ(pDevice, SWCORE, L2_FLOODING_STORM_CONTROL_RATE_CONFIGURATION, mac_port, INDEX_ZERO,
				       UnUniCfg.val);
			*pmode = UnUniCfg.BF.packets_not_bytes;
			if (UnUniCfg.BF.packets_not_bytes)
				*prate = UnUniCfg.BF.tokens * g_tick_freq_list_6107[UnUniCfg.BF.tick];
			else
				*prate = UnUniCfg.BF.tokens * g_tick_freq_list_6107[UnUniCfg.BF.tick] * 8 / 1000;
			break;
		}
	default:
		return JL_ERR_PARAM;
	}
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get eg port %d type %d rate mode %s val %d\n",
		   egress_port, type, (JL_STORM_RATE_MODE_KBPS) == (*pmode) ? "kbps" : "pps", *prate);
	return ret;
}
