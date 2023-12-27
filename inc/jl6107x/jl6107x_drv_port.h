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

#ifndef __jl6107x_DRV_PORT_H__
#define __jl6107x_DRV_PORT_H__

#include "jl_base.h"
#include "jl_port.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum jl_smac_speed_e {
	SMAC_SPEED_DISABLE = 0,
	SMAC_SPEED_RGMII_10M,
	SMAC_SPEED_RGMII_100M,
	SMAC_SPEED_RGMII_1000M,
	SMAC_SPEED_MII_MAC_10M,
	SMAC_SPEED_MII_MAC_100M,
	SMAC_SPEED_MII_PHY_10M,
	SMAC_SPEED_MII_PHY_100M,
	SMAC_SPEED_RMII_MAC_10M,
	SMAC_SPEED_RMII_MAC_100M,
	SMAC_SPEED_RMII_PHY_10M,
	SMAC_SPEED_RMII_PHY_100M,
	SMAC_SPEED_END
} jl_smac_speed_t;

typedef enum jl_serdes_mode_e {
	SERDES_MODE_POWER_DOWN = 0,
	SERDES_MODE_SGMII_MAC,
	SERDES_MODE_SGMII_PHY,
	SERDES_MODE_HSGMII,
	SERDES_MODE_FIBER,
	SERDES_MODE_END
} jl_serdes_mode_t;

jl_ret_t jl6107x_port_phy_autoneg_ability_set(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability);
jl_ret_t jl6107x_port_phy_autoneg_ability_get(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability);
jl_ret_t jl6107x_port_phy_force_ability_set(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability);
jl_ret_t jl6107x_port_phy_force_ability_get(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability);
jl_ret_t jl6107x_port_phy_status_get(jl_device_t *device, jl_port_t port, jl_port_link_status_t *pstatus, jl_port_duplex_t *pduplex, jl_port_speed_t *pspeed);
jl_ret_t jl6107x_port_phy_isolate_set(jl_device_t *device, jl_port_t port, jl_enable_t enable);
jl_ret_t jl6107x_port_phy_isolate_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
jl_ret_t jl6107x_port_phy_enable_set(jl_device_t *device, jl_port_t port, jl_enable_t enable);
jl_ret_t jl6107x_port_phy_enable_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
jl_ret_t jl6107x_port_phy_all_enable_set(jl_device_t *device, jl_enable_t enable);
jl_ret_t jl6107x_port_mac_force_link_set(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability);
jl_ret_t jl6107x_port_mac_force_link_get(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability);
jl_ret_t jl6107x_port_mac_force_link_ext_set(jl_device_t *device, jl_port_t port, jl_mode_ext_t mode, jl_port_mac_ability_t *pability);
jl_ret_t jl6107x_port_mac_force_link_ext_get(jl_device_t *device, jl_port_t port, jl_mode_ext_t *pmode, jl_port_mac_ability_t *pability);
jl_ret_t jl6107x_port_mac_status_get(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability);
jl_ret_t jl6107x_port_mac_loopback_set(jl_device_t *device, jl_port_t port, jl_enable_t enable);
jl_ret_t jl6107x_port_mac_loopback_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
jl_ret_t jl6107x_port_fiber_ability_set(jl_device_t *device, jl_port_t port, jl_port_fiber_ability_t *pability);
jl_ret_t jl6107x_port_fiber_ability_get(jl_device_t *device, jl_port_t port, jl_port_fiber_ability_t *pability);
jl_ret_t jl6107x_port_sds_reset(jl_device_t *device, jl_port_t port);

#ifdef __cplusplus
}
#endif

#endif /* __jl6107x_DRV_PORT_H__ */

