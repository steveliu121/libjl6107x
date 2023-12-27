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

#include "jl6107x/jl6107x_drv_port.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"
#include "portable/jl_portable.h"


jl_ret_t jl6107x_port_phy_autoneg_ability_set(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	JL_CHECK_POINTER(pability);

	if (pability->half_duplex_10 >= JL_ENABLE_END || pability->full_duplex_10 >= JL_ENABLE_END || \
			pability->half_duplex_100 >= JL_ENABLE_END || pability->full_duplex_100 >= JL_ENABLE_END || \
			pability->full_duplex_1000 >= JL_ENABLE_END || pability->flow_control >= JL_ENABLE_END || \
			pability->asym_flow_control >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	REGISTER_READ(device, PHG_GEPHY, IEEE_ANAR, anar, INDEX_ZERO, jlxx_port_l2p(device, port));
	REGISTER_READ(device, PHG_GEPHY, IEEE_GBCR, gbcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	REGISTER_READ(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	/*for PHY auto mode setup*/
	pability->auto_negotiation = 1;
	bmcr.BF.ieee_restart_an = 1;
	bmcr.BF.ieee_an_en = 1;
	linkctrl.BF.cfg_force_rx_pause_en = 0;
	linkctrl.BF.cfg_force_tx_pause_en = 0;

	if (pability->full_duplex_1000) {
		gbcr.BF.ieee_baset1000_full_duplex_adv = 1;
		gbcr.BF.ieee_baset1000_half_duplex_adv = 1;
	} else {
		gbcr.BF.ieee_baset1000_full_duplex_adv = 0;
		gbcr.BF.ieee_baset1000_half_duplex_adv = 0;
	}
	if (pability->full_duplex_100)
		anar.BF.ieee_an_100basetx_full_duplex = 1;
	else
		anar.BF.ieee_an_100basetx_full_duplex = 0;
	if (pability->half_duplex_100)
		anar.BF.ieee_an_100basetx_half_duplex = 1;
	else
		anar.BF.ieee_an_100basetx_half_duplex = 0;
	if (pability->full_duplex_10)
		anar.BF.ieee_an_10baset_full_duplex = 1;
	else
		anar.BF.ieee_an_10baset_full_duplex = 0;
	if (pability->half_duplex_10)
		anar.BF.ieee_an_10baset_half_duplex = 1;
	else
		anar.BF.ieee_an_10baset_half_duplex = 0;

	if (pability->flow_control)
		anar.BF.ieee_an_pause = 1;
	else
		anar.BF.ieee_an_pause = 0;
	if (pability->asym_flow_control)
		anar.BF.ieee_an_asym_pause = 1;
	else
		anar.BF.ieee_an_asym_pause = 0;

	REGISTER_WRITE(device, LMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	REGISTER_WRITE(device, PHG_GEPHY, IEEE_ANAR, anar, INDEX_ZERO, jlxx_port_l2p(device, port));
	REGISTER_WRITE(device, PHG_GEPHY, IEEE_GBCR, gbcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	REGISTER_WRITE(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: anar: 0x%x, gbcr:0x%x, linkctrl:0x%x, bmcr:0x%x \n", \
					anar.val[0], gbcr.val[0], linkctrl.val[0], bmcr.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_phy_autoneg_ability_get(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	JL_CHECK_POINTER(pability);

	/* Auto-Negotiation 1000M Advertisement Register setting*/
	REGISTER_READ(device, PHG_GEPHY, IEEE_GBCR, gbcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* Auto-Negotiation Advertisement Register setting*/
	REGISTER_READ(device, PHG_GEPHY, IEEE_ANAR, anar, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* Basic Mode Control Register setting */
	REGISTER_READ(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (gbcr.BF.ieee_baset1000_full_duplex_adv)
		pability->full_duplex_1000 = 1;
	else
		pability->full_duplex_1000 = 0;

	if (anar.BF.ieee_an_asym_pause)
		pability->asym_flow_control = 1;
	else
		pability->asym_flow_control = 0;

	if (anar.BF.ieee_an_pause)
		pability->flow_control = 1;
	else
		pability->flow_control = 0;

	if (anar.BF.ieee_an_100basetx_full_duplex)
		pability->full_duplex_100 = 1;
	else
		pability->full_duplex_100 = 0;

	if (anar.BF.ieee_an_100basetx_half_duplex)
		pability->half_duplex_100 = 1;
	else
		pability->half_duplex_100 = 0;

	if (anar.BF.ieee_an_10baset_full_duplex)
		pability->full_duplex_10 = 1;
	else
		pability->full_duplex_10 = 0;

	if (anar.BF.ieee_an_10baset_half_duplex)
		pability->half_duplex_10 = 1;
	else
		pability->half_duplex_10 = 0;

	if (bmcr.BF.ieee_an_en)
		pability->auto_negotiation = 1;
	else
		pability->auto_negotiation = 0;

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: anar: 0x%x, gbcr:0x%x, bmcr:0x%x \n", \
					anar.val[0], gbcr.val[0], bmcr.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_phy_force_ability_set(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	JL_CHECK_POINTER(pability);

	if (pability->half_duplex_10 >= JL_ENABLE_END || pability->full_duplex_10 >= JL_ENABLE_END || \
			pability->half_duplex_100 >= JL_ENABLE_END || pability->full_duplex_100 >= JL_ENABLE_END || \
			pability->full_duplex_1000 >= JL_ENABLE_END || pability->flow_control >= JL_ENABLE_END || \
			pability->asym_flow_control >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/* Mac Link Control Register setting */
	REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* Basic Mode Control Register setting */
	REGISTER_READ(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	/*for PHY auto mode setup*/
	pability->auto_negotiation = 0;
	bmcr.BF.ieee_restart_an = 1;
	bmcr.BF.ieee_an_en = 0;

	if (pability->full_duplex_1000) {
		bmcr.BF.ieee_speed_sel_msb = 1;
		bmcr.BF.ieee_speed_sel_lsb = 0;
		bmcr.BF.ieee_duplex_mode = 1;
	} else if (pability->full_duplex_100) {
		bmcr.BF.ieee_speed_sel_msb = 0;
		bmcr.BF.ieee_speed_sel_lsb = 1;
		bmcr.BF.ieee_duplex_mode = 1;
	} else if (pability->half_duplex_100) {
		bmcr.BF.ieee_speed_sel_msb = 0;
		bmcr.BF.ieee_speed_sel_lsb = 1;
		bmcr.BF.ieee_duplex_mode = 0;
	} else if (pability->full_duplex_10) {
		bmcr.BF.ieee_speed_sel_msb = 0;
		bmcr.BF.ieee_speed_sel_lsb = 0;
		bmcr.BF.ieee_duplex_mode = 1;
	} else {
		bmcr.BF.ieee_speed_sel_msb = 0;
		bmcr.BF.ieee_speed_sel_lsb = 0;
		bmcr.BF.ieee_duplex_mode = 0;
	}

	linkctrl.BF.cfg_force_rx_pause_en = 1;
	linkctrl.BF.cfg_force_tx_pause_en = 1;
	if (pability->flow_control) {
		linkctrl.BF.cfg_force_rx_pause_en_value = 1;
		linkctrl.BF.cfg_force_tx_pause_en_value = 1;
	} else {
		linkctrl.BF.cfg_force_rx_pause_en_value = 0;
		linkctrl.BF.cfg_force_tx_pause_en_value = 0;
	}

	/* Mac Link Control Register setting */
	REGISTER_WRITE(device, LMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* Basic Mode Control Register setting */
	REGISTER_WRITE(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: linkctrl: 0x%x, bmcr:0x%x \n", \
					linkctrl.val[0], bmcr.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_phy_force_ability_get(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	JL_CHECK_POINTER(pability);

	/* Mac Link Control Register setting */
	REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* Basic Mode Control Register setting */
	REGISTER_READ(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (bmcr.BF.ieee_restart_an)
		pability->auto_negotiation = 1;
	else
		pability->auto_negotiation = 0;

	if (bmcr.BF.ieee_speed_sel_msb && (!bmcr.BF.ieee_speed_sel_lsb)) {
		pability->full_duplex_1000 = 1;
	} else if ((!bmcr.BF.ieee_speed_sel_msb) && bmcr.BF.ieee_speed_sel_lsb) {
		if (bmcr.BF.ieee_duplex_mode)
			pability->full_duplex_100 = 1;
		else
			pability->half_duplex_100 = 1;
	} else {
		if (bmcr.BF.ieee_duplex_mode)
			pability->full_duplex_10 = 1;
		else
			pability->half_duplex_10 = 1;
	}
	if (linkctrl.BF.cfg_force_tx_pause_en && linkctrl.BF.cfg_force_tx_pause_en_value)
		pability->flow_control = 1;
	else
		pability->flow_control = 0;

	pability->asym_flow_control = 0;

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: linkctrl: 0x%x, bmcr:0x%x \n", \
					linkctrl.val[0], bmcr.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_phy_status_get(jl_device_t *device, jl_port_t port, jl_port_link_status_t *pstatus, jl_port_duplex_t *pduplex, jl_port_speed_t *pspeed)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pstatus);

	JL_CHECK_POINTER(pduplex);

	JL_CHECK_POINTER(pspeed);

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, TX_MII_MON, lphystatus, INDEX_ZERO, jlxx_port_l2p(device, port));
		if (lphystatus.BF.dbg_mii_link_status)
			*pstatus = PORT_LINKUP;
		else
			*pstatus = PORT_LINKDOWN;
		if (lphystatus.BF.dbg_mii_duplex)
			*pduplex = PORT_FULL_DUPLEX;
		else
			*pduplex = PORT_HALF_DUPLEX;
		if (lphystatus.BF.dbg_mii_speed == PORT_SPEED_1000M)
			*pspeed = PORT_SPEED_1000M;
		else if (lphystatus.BF.dbg_mii_speed == PORT_SPEED_100M)
			*pspeed = PORT_SPEED_100M;
		else
			*pspeed = PORT_SPEED_10M;

		JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: lphystatus: 0x%x \n", lphystatus.val[0]);
	} else {
		REGISTER_READ(device, SMAC_ADP, TX_MII_MON, sphystatus, INDEX_ZERO, jlxx_port_l2p(device, port));
		if (sphystatus.BF.dbg_mii_link_status)
			*pstatus = PORT_LINKUP;
		else
			*pstatus = PORT_LINKDOWN;
		if (sphystatus.BF.dbg_mii_duplex)
			*pduplex = PORT_FULL_DUPLEX;
		else
			*pduplex = PORT_HALF_DUPLEX;
		if (sphystatus.BF.dbg_mii_speed == PORT_SPEED_2500M)
			*pspeed = PORT_SPEED_2500M;
		else if (sphystatus.BF.dbg_mii_speed == PORT_SPEED_1000M)
			*pspeed = PORT_SPEED_1000M;
		else if (sphystatus.BF.dbg_mii_speed == PORT_SPEED_100M)
			*pspeed = PORT_SPEED_100M;
		else
			*pspeed = PORT_SPEED_10M;

		JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: sphystatus: 0x%x \n", sphystatus.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_port_phy_isolate_set(jl_device_t *device, jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/* Basic Mode Control Register setting */
	REGISTER_READ(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (enable)
		bmcr.BF.ieee_isolate_en = 1;
	else
		bmcr.BF.ieee_isolate_en = 0;

	/* Basic Mode Control Register setting */
	REGISTER_WRITE(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: bmcr: 0x%x \n", \
					bmcr.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_phy_isolate_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	JL_CHECK_POINTER(penable);

	/* Basic Mode Control Register setting */
	REGISTER_READ(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (bmcr.BF.ieee_isolate_en)
		*penable = ENABLED;
	else
		*penable = DISABLED;

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: bmcr: 0x%x \n", \
					bmcr.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_phy_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/* Basic Mode Control Register setting */
	REGISTER_READ(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (enable)
		bmcr.BF.ieee_power_down = 0;
	else
		bmcr.BF.ieee_power_down = 1;

	/* Basic Mode Control Register setting */
	REGISTER_WRITE(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: bmcr: 0x%x \n", \
					bmcr.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_phy_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	JL_CHECK_POINTER(penable);

	/* Basic Mode Control Register setting */
	REGISTER_READ(device, PHG_GEPHY, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (bmcr.BF.ieee_power_down)
		*penable = DISABLED;
	else
		*penable = ENABLED;

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: bmcr: 0x%x \n", \
					bmcr.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_phy_all_enable_set(jl_device_t *device, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;
	jl_port_t port;

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	JL_FOR_EACH_UTP_PORT(device, port) {
		ret = jl6107x_port_phy_enable_set(device, port, enable);
		if (ret != JL_ERR_OK)
			break;
	}

	return ret;
}

jl_ret_t jl6107x_port_mac_force_link_set(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	JL_CHECK_POINTER(pability);

	if (pability->speed > PORT_SPEED_1000M || pability->force_mode >= JL_ENABLE_END || \
			pability->duplex >= JL_ENABLE_END || pability->link >= JL_ENABLE_END || \
			pability->tx_pause >= JL_ENABLE_END || pability->rx_pause >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/* LMAC ifm control setting */
	REGISTER_READ(device, LMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* Mac Link Control Register setting */
	REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));

	ifmctl.BF.cfg_set_link_status_value = PORT_LINKDOWN;
	ifmctl.BF.cfg_set_link_status = ENABLED;
	/* LMAC ifm control setting */
	REGISTER_WRITE(device, LMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	port_udelay(100);

	if (pability->force_mode) {
		ifmctl.BF.cfg_set_link_status = ENABLED;
		ifmctl.BF.cfg_set_duplex = ENABLED;
		ifmctl.BF.cfg_set_speed = ENABLED;
		linkctrl.BF.cfg_force_rx_pause_en = ENABLED;
		linkctrl.BF.cfg_force_tx_pause_en = ENABLED;
	} else {
		ifmctl.BF.cfg_set_link_status = DISABLED;
		ifmctl.BF.cfg_set_duplex = DISABLED;
		ifmctl.BF.cfg_set_speed = DISABLED;
		linkctrl.BF.cfg_force_rx_pause_en = DISABLED;
		linkctrl.BF.cfg_force_tx_pause_en = DISABLED;
	}
	if (pability->link)
		ifmctl.BF.cfg_set_link_status_value = PORT_LINKUP;
	else
		ifmctl.BF.cfg_set_link_status_value = PORT_LINKDOWN;
	if (pability->duplex)
		ifmctl.BF.cfg_set_duplex_value = PORT_FULL_DUPLEX;
	else
		ifmctl.BF.cfg_set_duplex_value = PORT_HALF_DUPLEX;
	if (pability->speed == PORT_SPEED_1000M) {
		ifmctl.BF.cfg_set_speed_value = 0xe;
		ifmctl.BF.cfg_set_duplex_value = PORT_FULL_DUPLEX;
	} else if (pability->speed == PORT_SPEED_100M) {
		ifmctl.BF.cfg_set_speed_value = 0xd;
	} else {
		ifmctl.BF.cfg_set_speed_value = 0xc;
	}
	if (pability->tx_pause)
		linkctrl.BF.cfg_force_tx_pause_en_value = ENABLED;
	else
		linkctrl.BF.cfg_force_tx_pause_en_value = DISABLED;
	if (pability->rx_pause)
		linkctrl.BF.cfg_force_rx_pause_en_value = ENABLED;
	else
		linkctrl.BF.cfg_force_rx_pause_en_value = DISABLED;

	/* LMAC ifm control setting */
	REGISTER_WRITE(device, LMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* Mac Link Control Register setting */
	REGISTER_WRITE(device, LMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: ifmctl: 0x%x, linkctrl: 0x%x \n", \
					ifmctl.val[0], linkctrl.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_mac_force_link_get(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_PORT(device, port);

	JL_CHECK_POINTER(pability);

	/* LMAC ifm control setting */
	REGISTER_READ(device, LMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* Mac Link Control Register setting */
	REGISTER_READ(device, LMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (ifmctl.BF.cfg_set_link_status)
		pability->force_mode = ENABLED;
	else
		pability->force_mode = DISABLED;

	if (ifmctl.BF.cfg_set_link_status_value)
		pability->link = PORT_LINKUP;
	else
		pability->link = PORT_LINKDOWN;

	if (ifmctl.BF.cfg_set_speed_value == 0xe)
		pability->speed = PORT_SPEED_1000M;
	else if (ifmctl.BF.cfg_set_speed_value == 0xd)
		pability->speed = PORT_SPEED_100M;
	else
		pability->speed = PORT_SPEED_10M;
	if (ifmctl.BF.cfg_set_duplex_value)
		pability->duplex = PORT_FULL_DUPLEX;
	else
		pability->duplex = PORT_HALF_DUPLEX;

	if (linkctrl.BF.cfg_force_tx_pause_en_value)
		pability->tx_pause = ENABLED;
	else
		pability->tx_pause = DISABLED;

	if (linkctrl.BF.cfg_force_rx_pause_en_value)
		pability->rx_pause = ENABLED;
	else
		pability->rx_pause = DISABLED;

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: ifmctl: 0x%x, linkctrl: 0x%x \n", \
					ifmctl.val[0], linkctrl.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_mac_dis_set(jl_device_t *device, jl_port_t port)
{
	jl_ret_t ret = JL_ERR_OK;

	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_READ(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* SMAC ifm control setting */
	REGISTER_READ(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));

	sdsrsvd0.BF.sds_rsvd0 = SERDES_MODE_POWER_DOWN;
	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_WRITE(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));

	bmcr.BF.ieee_power_down = ENABLED;
	bmcr.BF.ieee_software_reset = ENABLED;
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_WRITE(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: sdsrsvd0: 0x%x, bmcr: 0x%x \n", \
					sdsrsvd0.val[0], bmcr.val[0]);

	ifmctl.BF.cfg_set_link_status_value = PORT_LINKDOWN;
	ifmctl.BF.cfg_set_link_status = ENABLED;
	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	port_udelay(100);

	ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_DISABLE;
	ifmctl.BF.cfg_set_speed = ENABLED;
	ifmctl.BF.cfg_set_link_status_value = PORT_LINKUP;
	ifmctl.BF.cfg_set_link_status = ENABLED;
	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	port_udelay(100);

	ifmctl.BF.cfg_set_link_status_value = PORT_LINKDOWN;
	ifmctl.BF.cfg_set_link_status = ENABLED;
	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: ifmctl: 0x%x \n", \
					ifmctl.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_mac_sys_set(jl_device_t *device, jl_port_t port, jl_mode_ext_t mode, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_READ(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* SMAC ifm control setting */
	REGISTER_READ(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* SMAC Link Control Register setting */
	REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));

	sdsrsvd0.BF.sds_rsvd0 = SERDES_MODE_POWER_DOWN;
	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_WRITE(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));

	bmcr.BF.ieee_power_down = ENABLED;
	bmcr.BF.ieee_software_reset = ENABLED;
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_WRITE(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: sdsrsvd0: 0x%x, bmcr: 0x%x \n", \
					sdsrsvd0.val[0], bmcr.val[0]);

	ifmctl.BF.cfg_set_link_status_value = PORT_LINKDOWN;
	ifmctl.BF.cfg_set_link_status = ENABLED;
	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	port_udelay(100);

	ifmctl.BF.cfg_set_link_status = ENABLED;
	ifmctl.BF.cfg_set_duplex = ENABLED;
	ifmctl.BF.cfg_set_speed = ENABLED;
	linkctrl.BF.cfg_force_rx_pause_en = ENABLED;
	linkctrl.BF.cfg_force_tx_pause_en = ENABLED;
	if (pability->link)
		ifmctl.BF.cfg_set_link_status_value = PORT_LINKUP;
	else
		ifmctl.BF.cfg_set_link_status_value = PORT_LINKDOWN;
	if (pability->duplex)
		ifmctl.BF.cfg_set_duplex_value = PORT_FULL_DUPLEX;
	else
		ifmctl.BF.cfg_set_duplex_value = PORT_HALF_DUPLEX;
	if (pability->tx_pause)
		linkctrl.BF.cfg_force_tx_pause_en_value = ENABLED;
	else
		linkctrl.BF.cfg_force_tx_pause_en_value = DISABLED;
	if (pability->rx_pause)
		linkctrl.BF.cfg_force_rx_pause_en_value = ENABLED;
	else
		linkctrl.BF.cfg_force_rx_pause_en_value = DISABLED;
	if (mode == MODE_EXT_RGMII) {
		if (pability->speed == PORT_SPEED_1000M) {
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_RGMII_1000M;
			ifmctl.BF.cfg_set_duplex_value = PORT_FULL_DUPLEX;
		} else if (pability->speed == PORT_SPEED_100M) {
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_RGMII_100M;
		} else {
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_RGMII_10M;
		}
	} else if (mode == MODE_EXT_MII_MAC) {
		if (pability->speed == PORT_SPEED_100M)
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_MII_MAC_100M;
		else
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_MII_MAC_10M;
	} else if (mode == MODE_EXT_MII_PHY) {
		if (pability->speed == PORT_SPEED_100M)
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_MII_PHY_100M;
		else
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_MII_PHY_10M;
	} else if (mode == MODE_EXT_RMII_MAC) {
		if (pability->speed == PORT_SPEED_100M)
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_RMII_MAC_100M;
		else
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_RMII_MAC_10M;
	} else {
		if (pability->speed == PORT_SPEED_100M)
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_RMII_PHY_100M;
		else
			ifmctl.BF.cfg_set_speed_value = SMAC_SPEED_RMII_PHY_10M;
	}

	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* SMAC Link Control Register setting */
	REGISTER_WRITE(device, SMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: ifmctl: 0x%x, linkctrl: 0x%x \n", \
					ifmctl.val[0], linkctrl.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_mac_sds_set(jl_device_t *device, jl_port_t port, jl_mode_ext_t mode, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* SMAC Link Control Register setting */
	REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* SMAC ifm control setting */
	REGISTER_READ(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_READ(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	ifmctl.BF.cfg_set_link_status_value = PORT_LINKDOWN;
	ifmctl.BF.cfg_set_link_status = ENABLED;
	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	port_udelay(100);

	ifmctl.BF.cfg_set_link_status = DISABLED;
	ifmctl.BF.cfg_set_rx_pause_en = DISABLED;
	ifmctl.BF.cfg_set_tx_pause_en = DISABLED;
	ifmctl.BF.cfg_set_duplex = DISABLED;
	ifmctl.BF.cfg_set_speed = DISABLED;
	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (mode == MODE_EXT_SGMII_MAC) {
		sdsrsvd0.BF.sds_rsvd0 = SERDES_MODE_SGMII_MAC;
		bmcr.BF.ieee_an_en = ENABLED;
		bmcr.BF.ieee_restart_an = ENABLED;
	} else if (mode == MODE_EXT_SGMII_PHY) {
		sdsrsvd0.BF.sds_rsvd0 = SERDES_MODE_SGMII_PHY;
		bmcr.BF.ieee_an_en = ENABLED;
		bmcr.BF.ieee_restart_an = ENABLED;
		if (pability->duplex)
			bmcr.BF.ieee_duplex_mode = PORT_FULL_DUPLEX;
		else
			bmcr.BF.ieee_duplex_mode = PORT_HALF_DUPLEX;
		if (pability->speed == PORT_SPEED_1000M) {
			bmcr.BF.ieee_speed_sel_msb = 1;
			bmcr.BF.ieee_speed_sel_lsb = 0;
			bmcr.BF.ieee_duplex_mode = PORT_FULL_DUPLEX;
		} else if (pability->speed == PORT_SPEED_100M) {
			bmcr.BF.ieee_speed_sel_msb = 0;
			bmcr.BF.ieee_speed_sel_lsb = 1;
		} else {
			bmcr.BF.ieee_speed_sel_msb = 0;
			bmcr.BF.ieee_speed_sel_lsb = 0;
		}
	} else {
		sdsrsvd0.BF.sds_rsvd0 = SERDES_MODE_HSGMII;
		bmcr.BF.ieee_an_en = DISABLED;
		bmcr.BF.ieee_restart_an = ENABLED;
		bmcr.BF.ieee_duplex_mode = PORT_FULL_DUPLEX;
	}
	if (pability->link)
		bmcr.BF.ieee_power_down = 0;
	else
		bmcr.BF.ieee_power_down = 1;

	linkctrl.BF.cfg_force_tx_pause_en = ENABLED;
	linkctrl.BF.cfg_force_rx_pause_en = ENABLED;
	if (pability->tx_pause)
		linkctrl.BF.cfg_force_tx_pause_en_value = ENABLED;
	else
		linkctrl.BF.cfg_force_tx_pause_en_value = DISABLED;
	if (pability->rx_pause)
		linkctrl.BF.cfg_force_rx_pause_en_value = ENABLED;
	else
		linkctrl.BF.cfg_force_rx_pause_en_value = DISABLED;

	/* SMAC Link Control Register setting */
	REGISTER_WRITE(device, SMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_WRITE(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_WRITE(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	bmcr.BF.ieee_software_reset = 1;
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_WRITE(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: bmcr: 0x%x, sdsrsvd0: 0x%x \n", \
					bmcr.val[0], sdsrsvd0.val[0]);
	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: linkctrl: 0x%x \n", linkctrl.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_mac_force_link_ext_set(jl_device_t *device, jl_port_t port, jl_mode_ext_t mode, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Ext Port */
	JL_CHECK_EXT_PORT(device, port);

	if (mode >= MODE_EXT_END)
		return JL_ERR_PARAM;

	JL_CHECK_POINTER(pability);

	if (pability->speed >= PORT_SPEED_END || pability->force_mode >= JL_ENABLE_END || \
			pability->duplex >= JL_ENABLE_END || pability->link >= JL_ENABLE_END || \
			pability->tx_pause >= JL_ENABLE_END || pability->rx_pause >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/* fiber ability not config from here */
	if (mode == MODE_EXT_FIBER)
		return JL_ERR_NOT_FOUND;

	if (mode == MODE_EXT_DISABLE)
		ret = jl6107x_port_mac_dis_set(device, port);
	else if ((mode >= MODE_EXT_RGMII) && (mode <= MODE_EXT_RMII_PHY))
		ret = jl6107x_port_mac_sys_set(device, port, mode, pability);
	else
		ret = jl6107x_port_mac_sds_set(device, port, mode, pability);

	return ret;
}

jl_ret_t jl6107x_port_mac_force_link_ext_get(jl_device_t *device, jl_port_t port, jl_mode_ext_t *pmode, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Ext Port */
	JL_CHECK_EXT_PORT(device, port);

	JL_CHECK_POINTER(pmode);

	JL_CHECK_POINTER(pability);

	/* SMAC Link Control Register setting */
	REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* SMAC ifm control setting */
	REGISTER_READ(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_READ(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES SDS_STATUS_VECTOR setting */
	REGISTER_READ(device, PHG_SERDES, SDS_STATUS_VECTOR, sdsstatus, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (!ifmctl.BF.cfg_set_link_status) {
		switch (sdsrsvd0.BF.sds_rsvd0) {
		case SERDES_MODE_POWER_DOWN:
			*pmode = MODE_EXT_DISABLE;
			break;
		case SERDES_MODE_SGMII_MAC:
			*pmode = MODE_EXT_SGMII_MAC;
			pability->speed = (jl_uint32)sdsstatus.BF.speed_mode_reg;
			pability->duplex = (jl_uint32)sdsstatus.BF.duplex_mode_reg;
			break;
		case SERDES_MODE_SGMII_PHY:
			*pmode = MODE_EXT_SGMII_PHY;
			if ((bmcr.BF.ieee_speed_sel_msb) && (!bmcr.BF.ieee_speed_sel_lsb))
				pability->speed = PORT_SPEED_1000M;
			else if ((!bmcr.BF.ieee_speed_sel_msb) && (bmcr.BF.ieee_speed_sel_lsb))
				pability->speed = PORT_SPEED_100M;
			else
				pability->speed = PORT_SPEED_10M;
			pability->duplex = (jl_uint32)bmcr.BF.ieee_duplex_mode;
			break;
		case SERDES_MODE_HSGMII:
			*pmode = MODE_EXT_HSGMII;
			pability->speed = PORT_SPEED_2500M;
			pability->duplex = (jl_uint32)bmcr.BF.ieee_duplex_mode;
			break;
		default:
			ret = JL_ERR_PARAM;
			break;
		}
		pability->force_mode = DISABLED;
		if (bmcr.BF.ieee_power_down)
			pability->link = PORT_LINKDOWN;
		else
			pability->link = PORT_LINKUP;
	} else {
		switch (ifmctl.BF.cfg_set_speed_value) {
		case SMAC_SPEED_DISABLE:
			*pmode = MODE_EXT_DISABLE;
			break;
		case SMAC_SPEED_RGMII_10M:
			*pmode = MODE_EXT_RGMII;
			pability->speed = PORT_SPEED_10M;
			break;
		case SMAC_SPEED_RGMII_100M:
			*pmode = MODE_EXT_RGMII;
			pability->speed = PORT_SPEED_100M;
			break;
		case SMAC_SPEED_RGMII_1000M:
			*pmode = MODE_EXT_RGMII;
			pability->speed = PORT_SPEED_1000M;
			break;
		case SMAC_SPEED_MII_MAC_10M:
			*pmode = MODE_EXT_MII_MAC;
			pability->speed = PORT_SPEED_10M;
			break;
		case SMAC_SPEED_MII_MAC_100M:
			*pmode = MODE_EXT_MII_MAC;
			pability->speed = PORT_SPEED_100M;
			break;
		case SMAC_SPEED_MII_PHY_10M:
			*pmode = MODE_EXT_MII_PHY;
			pability->speed = PORT_SPEED_10M;
			break;
		case SMAC_SPEED_MII_PHY_100M:
			*pmode = MODE_EXT_MII_PHY;
			pability->speed = PORT_SPEED_100M;
			break;
		case SMAC_SPEED_RMII_MAC_10M:
			*pmode = MODE_EXT_RMII_MAC;
			pability->speed = PORT_SPEED_10M;
			break;
		case SMAC_SPEED_RMII_MAC_100M:
			*pmode = MODE_EXT_RMII_MAC;
			pability->speed = PORT_SPEED_100M;
			break;
		case SMAC_SPEED_RMII_PHY_10M:
			*pmode = MODE_EXT_RMII_PHY;
			pability->speed = PORT_SPEED_10M;
			break;
		case SMAC_SPEED_RMII_PHY_100M:
			*pmode = MODE_EXT_RMII_PHY;
			pability->speed = PORT_SPEED_100M;
			break;
		default:
			ret = JL_ERR_PARAM;
			break;
		}
		pability->force_mode = ENABLED;
		pability->link = (jl_uint32)ifmctl.BF.cfg_set_link_status_value;
		pability->duplex = (jl_uint32)ifmctl.BF.cfg_set_duplex_value;
	}
	pability->tx_pause = (jl_uint32)linkctrl.BF.cfg_force_tx_pause_en_value;
	pability->rx_pause = (jl_uint32)linkctrl.BF.cfg_force_rx_pause_en_value;

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: sdsrsvd0: 0x%x, bmcr: 0x%x, sdsstatus: 0x%x \n", \
					sdsrsvd0.val[0], bmcr.val[0], sdsstatus.val[0]);
	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: linkctrl: 0x%x, ifmctl: 0x%x \n", \
					linkctrl.val[0], ifmctl.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_mac_status_get(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pability);

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, TX_IFM_MON, lmacstatus, INDEX_ZERO, jlxx_port_l2p(device, port));
		REGISTER_READ(device, LMAC_ADP, IFM_CONTROL, lmifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
		pability->link = (jl_uint32)lmacstatus.BF.dbg_mac_link_status;
		pability->duplex = (jl_uint32)lmacstatus.BF.dbg_mac_duplex_mode;
		pability->speed = (jl_uint32)lmacstatus.BF.dbg_mac_speed;
		pability->tx_pause = (jl_uint32)lmacstatus.BF.dbg_mac_tx_pause_en;
		pability->rx_pause = (jl_uint32)lmacstatus.BF.dbg_mac_rx_pause_en;
		pability->force_mode = (jl_uint32)lmifmctl.BF.cfg_set_link_status;

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: lmacstatus: 0x%x, lmifmctl: 0x%x \n", \
						lmacstatus.val[0], lmifmctl.val[0]);
	} else {
		REGISTER_READ(device, SMAC_ADP, TX_IFM_MON, smacstatus, INDEX_ZERO, jlxx_port_l2p(device, port));
		REGISTER_READ(device, SMAC_ADP, IFM_CONTROL, smifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
		pability->link = (jl_uint32)smacstatus.BF.dbg_mac_link_status;
		pability->duplex = (jl_uint32)smacstatus.BF.dbg_mac_duplex_mode;
		pability->speed = (jl_uint32)smacstatus.BF.dbg_mac_speed;
		pability->tx_pause = (jl_uint32)smacstatus.BF.dbg_mac_tx_pause_en;
		pability->rx_pause = (jl_uint32)smacstatus.BF.dbg_mac_rx_pause_en;
		pability->force_mode = (jl_uint32)smifmctl.BF.cfg_set_link_status;

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: smacstatus: 0x%x, smifmctl: 0x%x \n", \
						smacstatus.val[0], smifmctl.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_port_mac_loopback_set(jl_device_t *device, jl_port_t port, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		/* lmac config setting */
		REGISTER_READ(device, LMAC, MAC_CONFIG, lmaccfg, INDEX_ZERO, jlxx_port_l2p(device, port));
		lmaccfg.BF.cfg_loopback_mode = enable;
		/* lmac config setting */
		REGISTER_WRITE(device, LMAC, MAC_CONFIG, lmaccfg, INDEX_ZERO, jlxx_port_l2p(device, port));

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: lmaccfg: 0x%x \n", \
						lmaccfg.val[0]);
	} else {
		/* smac config setting */
		REGISTER_READ(device, SMAC, MAC_CONFIG, smaccfg, INDEX_ZERO, jlxx_port_l2p(device, port));
		smaccfg.BF.cfg_loopback_mode = enable;
		/* smac config setting */
		REGISTER_WRITE(device, SMAC, MAC_CONFIG, smaccfg, INDEX_ZERO, jlxx_port_l2p(device, port));

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: smaccfg: 0x%x \n", \
						smaccfg.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_port_mac_loopback_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(penable);

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		/* lmac config setting */
		REGISTER_READ(device, LMAC, MAC_CONFIG, lmaccfg, INDEX_ZERO, jlxx_port_l2p(device, port));
		*penable = lmaccfg.BF.cfg_loopback_mode;

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: lmaccfg: 0x%x \n", \
						lmaccfg.val[0]);
	} else {
		/* smac config setting */
		REGISTER_READ(device, SMAC, MAC_CONFIG, smaccfg, INDEX_ZERO, jlxx_port_l2p(device, port));
		*penable = smaccfg.BF.cfg_loopback_mode;

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: smaccfg: 0x%x \n", \
						smaccfg.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_port_fiber_ability_set(jl_device_t *device, jl_port_t port, jl_port_fiber_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Ext Port */
	JL_CHECK_EXT_PORT(device, port);

	JL_CHECK_POINTER(pability);

	if (pability->auto_negotiation >= JL_ENABLE_END || pability->full_duplex_100 >= JL_ENABLE_END || \
			pability->full_duplex_1000 >= JL_ENABLE_END || pability->flow_control >= JL_ENABLE_END || \
			pability->asym_flow_control >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	/* SMAC ifm control setting */
	REGISTER_READ(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* SMAC Link Control Register setting */
	REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY TOP_RSVD0 setting */
	REGISTER_READ(device, TOP, SADP_SPEED_CTRL_MODE, spctrlmode, INDEX_ZERO, INDEX_ZERO);
	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_READ(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_ANAR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_ANAR, anar, INDEX_ZERO, jlxx_port_l2p(device, port));

	ifmctl.BF.cfg_set_link_status_value = PORT_LINKDOWN;
	ifmctl.BF.cfg_set_link_status = ENABLED;
	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));

	ifmctl.BF.cfg_set_link_status = DISABLED;
	ifmctl.BF.cfg_set_rx_pause_en = DISABLED;
	ifmctl.BF.cfg_set_tx_pause_en = DISABLED;
	ifmctl.BF.cfg_set_duplex = DISABLED;
	ifmctl.BF.cfg_set_speed = DISABLED;
	/* SMAC ifm control setting */
	REGISTER_WRITE(device, SMAC_ADP, IFM_CONTROL, ifmctl, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: ifmctl: 0x%x \n", \
					ifmctl.val[0]);

	sdsrsvd0.BF.sds_rsvd0 = SERDES_MODE_FIBER;

	bmcr.BF.ieee_power_down = DISABLED;
	bmcr.BF.ieee_duplex_mode = PORT_FULL_DUPLEX;
	anar.BF.ieee_an_full_duplex = PORT_FULL_DUPLEX;
	anar.BF.ieee_an_pause = ENABLED;
	anar.BF.ieee_an_asym_pause = ENABLED;

	if (pability->full_duplex_1000) {
		bmcr.BF.ieee_speed_sel_msb = 1;
		bmcr.BF.ieee_speed_sel_lsb = 0;
		if (pability->auto_negotiation) {
			bmcr.BF.ieee_an_en = ENABLED;
			bmcr.BF.ieee_restart_an = ENABLED;
			linkctrl.BF.cfg_force_rx_pause_en = DISABLED;
			linkctrl.BF.cfg_force_tx_pause_en = DISABLED;
			if (pability->flow_control)
				anar.BF.ieee_an_pause = ENABLED;
			else
				anar.BF.ieee_an_pause = DISABLED;
			if (pability->asym_flow_control)
				anar.BF.ieee_an_asym_pause = ENABLED;
			else
				anar.BF.ieee_an_asym_pause = DISABLED;
		} else {
			bmcr.BF.ieee_an_en = DISABLED;
			bmcr.BF.ieee_restart_an = ENABLED;
			linkctrl.BF.cfg_force_rx_pause_en = ENABLED;
			linkctrl.BF.cfg_force_tx_pause_en = ENABLED;
			if (pability->flow_control) {
				linkctrl.BF.cfg_force_rx_pause_en_value = ENABLED;
				linkctrl.BF.cfg_force_tx_pause_en_value = ENABLED;
			} else {
				linkctrl.BF.cfg_force_rx_pause_en_value = DISABLED;
				linkctrl.BF.cfg_force_tx_pause_en_value = DISABLED;
			}
		}
	} else if (pability->full_duplex_100) {
		bmcr.BF.ieee_speed_sel_msb = 0;
		bmcr.BF.ieee_speed_sel_lsb = 1;
		bmcr.BF.ieee_an_en = DISABLED;
		bmcr.BF.ieee_restart_an = ENABLED;
		if (pability->flow_control) {
			linkctrl.BF.cfg_force_rx_pause_en = ENABLED;
			linkctrl.BF.cfg_force_tx_pause_en = ENABLED;
			linkctrl.BF.cfg_force_rx_pause_en_value = ENABLED;
			linkctrl.BF.cfg_force_tx_pause_en_value = ENABLED;
		} else {
			linkctrl.BF.cfg_force_rx_pause_en = ENABLED;
			linkctrl.BF.cfg_force_tx_pause_en = ENABLED;
			linkctrl.BF.cfg_force_rx_pause_en_value = DISABLED;
			linkctrl.BF.cfg_force_tx_pause_en_value = DISABLED;
		}
	} else {
		bmcr.BF.ieee_speed_sel_msb = 0;
		bmcr.BF.ieee_speed_sel_lsb = 0;
		bmcr.BF.ieee_an_en = DISABLED;
		bmcr.BF.ieee_restart_an = ENABLED;
		linkctrl.BF.cfg_force_rx_pause_en = DISABLED;
		linkctrl.BF.cfg_force_tx_pause_en = DISABLED;
	}

	/* SMAC Link Control Register setting */
	REGISTER_WRITE(device, SMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES SDS_RSVD0 setting */
	REGISTER_WRITE(device, PHG_SERDES, SDS_RSVD0, sdsrsvd0, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_ANAR setting */
	REGISTER_WRITE(device, PHG_SERDES, IEEE_ANAR, anar, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_WRITE(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	bmcr.BF.ieee_software_reset = 1;
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_WRITE(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: sdsrsvd0: 0x%x, anar: 0x%x, bmcr: 0x%x \n", \
					sdsrsvd0.val[0], anar.val[0], bmcr.val[0]);
	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: linkctrl: 0x%x \n", linkctrl.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_fiber_ability_get(jl_device_t *device, jl_port_t port, jl_port_fiber_ability_t *pability)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Ext Port */
	JL_CHECK_EXT_PORT(device, port);

	JL_CHECK_POINTER(pability);

	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_ANAR, anar, INDEX_ZERO, jlxx_port_l2p(device, port));
	/* SMAC Link Control Register setting */
	REGISTER_READ(device, SMAC_ADP, LINK_CONTROL, linkctrl, INDEX_ZERO, jlxx_port_l2p(device, port));

	if (bmcr.BF.ieee_an_en)
		pability->auto_negotiation = ENABLED;
	else
		pability->auto_negotiation = DISABLED;
	if ((bmcr.BF.ieee_speed_sel_msb) && (!bmcr.BF.ieee_speed_sel_lsb)) {
		pability->full_duplex_1000 = ENABLED;
	} else if ((!bmcr.BF.ieee_speed_sel_msb) && (bmcr.BF.ieee_speed_sel_lsb)) {
		pability->full_duplex_100 = ENABLED;
	} else {
		pability->full_duplex_1000 = DISABLED;
		pability->full_duplex_100 = DISABLED;
	}
	if (anar.BF.ieee_an_pause && bmcr.BF.ieee_an_en)
		pability->flow_control = ENABLED;
	else if (linkctrl.BF.cfg_force_tx_pause_en && linkctrl.BF.cfg_force_tx_pause_en_value)
		pability->flow_control = ENABLED;
	else
		pability->flow_control = DISABLED;
	if (anar.BF.ieee_an_asym_pause && bmcr.BF.ieee_an_en)
		pability->asym_flow_control = ENABLED;
	else
		pability->asym_flow_control = DISABLED;

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: anar: 0x%x, bmcr: 0x%x \n", \
					anar.val[0], bmcr.val[0]);
	JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: linkctrl: 0x%x \n", \
					linkctrl.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_sds_reset(jl_device_t *device, jl_port_t port)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Ext Port */
	JL_CHECK_EXT_PORT(device, port);

	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_READ(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	bmcr.BF.ieee_software_reset = ENABLED;

	/* PHY SERDES IEEE_BMCR setting */
	REGISTER_WRITE(device, PHG_SERDES, IEEE_BMCR, bmcr, INDEX_ZERO, jlxx_port_l2p(device, port));

	JL_DBG_MSG(JL_FLAG_PHY, _DBG_ON, "PHY: bmcr: 0x%x \n", \
					bmcr.val[0]);

	return ret;
}
