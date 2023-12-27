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

#include "jl6107x/jl6107x_drv_interrupt.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"

jl_ret_t jl6107x_int_polarity_set(jl_device_t *device, jl_int_polarity_t polarity)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_ASSERT(polarity < INT_POLAR_END);

	REGISTER_READ(device, CCS, CCS_INT_CFG, val, INDEX_ZERO, INDEX_ZERO);
	val.BF.ccs_int_polarity = polarity;
	REGISTER_WRITE(device, CCS, CCS_INT_CFG, val, INDEX_ZERO, INDEX_ZERO);
	return ret;
}

jl_ret_t jl6107x_int_polarity_get(jl_device_t *device, jl_int_polarity_t *ppolarity)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);

	REGISTER_READ(device, CCS, CCS_INT_CFG, val, INDEX_ZERO, INDEX_ZERO);
	*ppolarity = val.BF.ccs_int_polarity;
	return ret;
}

jl_ret_t jl6107x_int_port_phy_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_UTP_PORT(device, port);
	JL_ASSERT(enable < JL_ENABLE_END);

	REGISTER_READ(device, PHG_GEPHY, INER, val, INDEX_ZERO, jlxx_port_l2p(device, port));
	val.BF.ieee_link_chg_int_en = enable;

	REGISTER_WRITE(device, PHG_GEPHY, INER, val, INDEX_ZERO, jlxx_port_l2p(device, port));
	return ret;
}

jl_ret_t jl6107x_int_port_phy_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_UTP_PORT(device, port);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(device, PHG_GEPHY, INER, val, INDEX_ZERO, jlxx_port_l2p(device, port));
	*penable = val.BF.ieee_link_chg_int_en;
	return ret;
}

jl_ret_t jl6107x_int_port_phy_status_get(jl_device_t *device, jl_port_t port, jl_uint32 *pstatus)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_CHECK_UTP_PORT(device, port);
	JL_CHECK_POINTER(pstatus);

	REGISTER_READ(device, PHG_GEPHY, INSR, val, INDEX_ZERO, jlxx_port_l2p(device, port));
	*pstatus = val.BF.ieee_link_chg_int;
	return ret;
}

jl_ret_t jl6107x_int_enable_set(jl_device_t *device, jl_int_type_t type, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t port = 0;

	JL_CHECK_POINTER(device);
	JL_ASSERT(type < INT_TYPE_END);
	JL_ASSERT(enable < JL_ENABLE_END);

	switch (type) {
	case INT_PORT_LINKDOWN:
		JL_FOR_EACH_UTP_PORT(device, port) {
			REGISTER_READ(device, LMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			val.BF.int_link_down_en = enable;
			REGISTER_WRITE(device, LMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		}

		JL_FOR_EACH_EXT_PORT(device, port) {
			REGISTER_READ(device, SMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			val.BF.int_link_down_en = enable;
			REGISTER_WRITE(device, SMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		}
		break;
	case INT_PORT_LINKUP:
		JL_FOR_EACH_UTP_PORT(device, port) {
			REGISTER_READ(device, LMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			val.BF.int_link_up_en = enable;
			REGISTER_WRITE(device, LMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		}

		JL_FOR_EACH_EXT_PORT(device, port) {
			REGISTER_READ(device, SMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			val.BF.int_link_up_en = enable;
			REGISTER_WRITE(device, SMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		}
		break;
	case INT_TYPE_LOOP_DETECT:
		REGISTER_READ(device, MAG, INT_ENABLE, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.int_lp_detect_0_en = enable;
		val.BF.int_lp_detect_1_en = enable;
		val.BF.int_lp_detect_2_en = enable;
		val.BF.int_lp_detect_3_en = enable;
		val.BF.int_lp_detect_4_en = enable;
		val.BF.int_lp_detect_5_en = enable;
		val.BF.int_lp_detect_6_en = enable;
		REGISTER_WRITE(device, MAG, INT_ENABLE, val, INDEX_ZERO, INDEX_ZERO);
		break;
	default:
		return JL_ERR_PARAM;
	}
	return ret;
}

jl_ret_t jl6107x_int_enable_get(jl_device_t *device, jl_int_type_t type, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_enable_t en = ENABLED;
	jl_port_t port = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(penable);
	JL_ASSERT(type < INT_TYPE_END);

	switch (type) {
	case INT_PORT_LINKDOWN:
		JL_FOR_EACH_UTP_PORT(device, port) {
			REGISTER_READ(device, LMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			en &= val.BF.int_link_down_en;
		}

		JL_FOR_EACH_EXT_PORT(device, port) {
			REGISTER_READ(device, SMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			en &= val.BF.int_link_down_en;
		}
		*penable = en;
		break;
	case INT_PORT_LINKUP:
		JL_FOR_EACH_UTP_PORT(device, port) {
			REGISTER_READ(device, LMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			en &= val.BF.int_link_up_en;
		}

		JL_FOR_EACH_EXT_PORT(device, port) {
			REGISTER_READ(device, SMAC_ADP, INT_ENABLE, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			en &= val.BF.int_link_up_en;
		}
		*penable = en;
		break;
	case INT_TYPE_LOOP_DETECT:
		REGISTER_READ(device, MAG, INT_ENABLE, val, INDEX_ZERO, INDEX_ZERO);
		en = val.BF.int_lp_detect_0_en & val.BF.int_lp_detect_1_en &
		    val.BF.int_lp_detect_2_en & val.BF.int_lp_detect_3_en &
		    val.BF.int_lp_detect_4_en & val.BF.int_lp_detect_5_en & val.BF.int_lp_detect_6_en;
		*penable = en;
		break;
	default:
		return JL_ERR_PARAM;
	}
	return ret;
}

jl_ret_t jl6107x_int_status_clean(jl_device_t *device, jl_int_type_t type)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t port = 0;

	JL_CHECK_POINTER(device);
	JL_ASSERT(type < INT_TYPE_END);

	switch (type) {
	case INT_PORT_LINKDOWN:
		JL_FOR_EACH_UTP_PORT(device, port) {
			REGISTER_READ(device, LMAC_ADP, INT_STATUS, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			val.BF.int_link_down = 1;
			val.BF.int_link_up = 0;
			REGISTER_WRITE(device, LMAC_ADP, INT_STATUS, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		}

		JL_FOR_EACH_EXT_PORT(device, port) {
			REGISTER_READ(device, SMAC_ADP, INT_STATUS, intval, INDEX_ZERO, jlxx_port_l2p(device, port));
			intval.BF.int_link_down = 1;
			intval.BF.int_link_up = 0;
			REGISTER_WRITE(device, SMAC_ADP, INT_STATUS, intval, INDEX_ZERO, jlxx_port_l2p(device, port));
		}
		break;
	case INT_PORT_LINKUP:
		JL_FOR_EACH_UTP_PORT(device, port) {
			REGISTER_READ(device, LMAC_ADP, INT_STATUS, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			val.BF.int_link_up = 1;
			val.BF.int_link_down = 0;
			REGISTER_WRITE(device, LMAC_ADP, INT_STATUS, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		}

		JL_FOR_EACH_EXT_PORT(device, port) {
			REGISTER_READ(device, SMAC_ADP, INT_STATUS, intval, INDEX_ZERO, jlxx_port_l2p(device, port));
			intval.BF.int_link_up = 1;
			intval.BF.int_link_down = 0;
			REGISTER_WRITE(device, SMAC_ADP, INT_STATUS, intval, INDEX_ZERO, jlxx_port_l2p(device, port));
		}
		break;
	case INT_TYPE_LOOP_DETECT:
		REGISTER_READ(device, MAG, INT_STATUS, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.int_lp_detect_0 = 1;
		val.BF.int_lp_detect_1 = 1;
		val.BF.int_lp_detect_2 = 1;
		val.BF.int_lp_detect_3 = 1;
		val.BF.int_lp_detect_4 = 1;
		val.BF.int_lp_detect_5 = 1;
		val.BF.int_lp_detect_6 = 1;
		REGISTER_WRITE(device, MAG, INT_STATUS, val, INDEX_ZERO, INDEX_ZERO);
		break;
	default:
		return JL_ERR_PARAM;
	}
	return ret;
}

jl_ret_t jl6107x_int_status_get(jl_device_t *device, jl_int_status_t *pstatusmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 loopdect = 0;
	jl_port_t port = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pstatusmask);

	JL_FOR_EACH_UTP_PORT(device, port) {
		REGISTER_READ(device, LMAC_ADP, INT_STATUS, val, INDEX_ZERO, jlxx_port_l2p(device, port));
		if (val.BF.int_link_up == 1)
			SET_BIT(pstatusmask->mask, INT_PORT_LINKUP);

		if (val.BF.int_link_down == 1)
			SET_BIT(pstatusmask->mask, INT_PORT_LINKDOWN);
	}

	JL_FOR_EACH_EXT_PORT(device, port) {
		REGISTER_READ(device, SMAC_ADP, INT_STATUS, intval, INDEX_ZERO, jlxx_port_l2p(device, port));
		if (intval.BF.int_link_up == 1)
			SET_BIT(pstatusmask->mask, INT_PORT_LINKUP);

		if (intval.BF.int_link_down == 1)
			SET_BIT(pstatusmask->mask, INT_PORT_LINKDOWN);
	}

	REGISTER_READ(device, MAG, INT_STATUS, val, INDEX_ZERO, INDEX_ZERO);
	loopdect = val.BF.int_lp_detect_0 | val.BF.int_lp_detect_1 |
	    val.BF.int_lp_detect_2 | val.BF.int_lp_detect_3 |
	    val.BF.int_lp_detect_4 | val.BF.int_lp_detect_5 | val.BF.int_lp_detect_6;

	if (loopdect)
		SET_BIT(pstatusmask->mask, INT_TYPE_LOOP_DETECT);

	return ret;
}

jl_ret_t jl6107x_int_detail_get(jl_device_t *device, jl_int_type_t type, jl_int_info_t *pinfo)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t port = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pinfo);
	JL_ASSERT(type < INT_TYPE_END);

	switch (type) {
	case INT_PORT_LINKDOWN:
		JL_FOR_EACH_UTP_PORT(device, port) {
			REGISTER_READ(device, LMAC_ADP, INT_STATUS, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			WRITE_BIT(pinfo->port_mask, port, val.BF.int_link_down);
		}

		JL_FOR_EACH_EXT_PORT(device, port) {
			REGISTER_READ(device, SMAC_ADP, INT_STATUS, intval, INDEX_ZERO, jlxx_port_l2p(device, port));
			WRITE_BIT(pinfo->port_mask, port, intval.BF.int_link_down);
		}
		break;
	case INT_PORT_LINKUP:
		JL_FOR_EACH_UTP_PORT(device, port) {
			REGISTER_READ(device, LMAC_ADP, INT_STATUS, val, INDEX_ZERO, jlxx_port_l2p(device, port));
			WRITE_BIT(pinfo->port_mask, port, val.BF.int_link_up);
		}

		JL_FOR_EACH_EXT_PORT(device, port) {
			REGISTER_READ(device, SMAC_ADP, INT_STATUS, intval, INDEX_ZERO, jlxx_port_l2p(device, port));
			WRITE_BIT(pinfo->port_mask, port, intval.BF.int_link_up);
		}
		break;
	case INT_TYPE_LOOP_DETECT:
		REGISTER_READ(device, MAG, INT_STATUS, val, INDEX_ZERO, INDEX_ZERO);
		WRITE_BIT(pinfo->port_mask, EXT_PORT0, val.BF.int_lp_detect_0);
		WRITE_BIT(pinfo->port_mask, EXT_PORT1, val.BF.int_lp_detect_1);
		WRITE_BIT(pinfo->port_mask, UTP_PORT0, val.BF.int_lp_detect_2);
		WRITE_BIT(pinfo->port_mask, UTP_PORT1, val.BF.int_lp_detect_3);
		WRITE_BIT(pinfo->port_mask, UTP_PORT2, val.BF.int_lp_detect_4);
		WRITE_BIT(pinfo->port_mask, UTP_PORT3, val.BF.int_lp_detect_5);
		WRITE_BIT(pinfo->port_mask, UTP_PORT4, val.BF.int_lp_detect_6);
		break;
	default:
		return JL_ERR_PARAM;
	}
	return ret;
}
