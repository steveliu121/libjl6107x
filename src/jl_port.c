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

#include "jl_port.h"
#include "jl_ops.h"

jl_api_ret_t jl_port_phy_autoneg_ability_set(jl_uint32 chip_id, jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_autoneg_ability_set, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_phy_autoneg_ability_get(jl_uint32 chip_id, jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_autoneg_ability_get, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_phy_force_ability_set(jl_uint32 chip_id, jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_force_ability_set, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_phy_force_ability_get(jl_uint32 chip_id, jl_port_t port, jl_port_phy_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_force_ability_get, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_phy_status_get(jl_uint32 chip_id, jl_port_t port, jl_port_link_status_t *pstatus, jl_port_duplex_t *pduplex, jl_port_speed_t *pspeed)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_status_get, chip_id, port, pstatus, pduplex, pspeed);

	return ret;
}

jl_api_ret_t jl_port_phy_isolate_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t isolate)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_isolate_set, chip_id, port, isolate);

	return ret;
}

jl_api_ret_t jl_port_phy_isolate_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *pisolate)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_isolate_get, chip_id, port, pisolate);

	return ret;
}

jl_api_ret_t jl_port_phy_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_enable_set, chip_id, port, enable);

	return ret;
}

jl_api_ret_t jl_port_phy_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_enable_get, chip_id, port, penable);

	return ret;
}


jl_api_ret_t jl_port_phy_all_enable_set(jl_uint32 chip_id, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_phy_all_enable_set, chip_id, enable);

	return ret;
}

jl_api_ret_t jl_port_mac_force_link_set(jl_uint32 chip_id, jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_mac_force_link_set, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_mac_force_link_get(jl_uint32 chip_id, jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_mac_force_link_get, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_mac_force_link_ext_set(jl_uint32 chip_id, jl_port_t port, jl_mode_ext_t mode, jl_port_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_mac_force_link_ext_set, chip_id, port, mode, pability);

	return ret;
}

jl_api_ret_t jl_port_mac_force_link_ext_get(jl_uint32 chip_id, jl_port_t port, jl_mode_ext_t *pmode, jl_port_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_mac_force_link_ext_get, chip_id, port, pmode, pability);

	return ret;
}

jl_api_ret_t jl_port_mac_status_get(jl_uint32 chip_id, jl_port_t port, jl_port_mac_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_mac_status_get, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_mac_loopback_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_mac_loopback_set, chip_id, port, enable);

	return ret;
}

jl_api_ret_t jl_port_mac_loopback_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_mac_loopback_get, chip_id, port, penable);

	return ret;
}

jl_api_ret_t jl_port_fiber_ability_set(jl_uint32 chip_id, jl_port_t port, jl_port_fiber_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_fiber_ability_set, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_fiber_ability_get(jl_uint32 chip_id, jl_port_t port, jl_port_fiber_ability_t *pability)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_fiber_ability_get, chip_id, port, pability);

	return ret;
}

jl_api_ret_t jl_port_sds_reset(jl_uint32 chip_id, jl_port_t port)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, port_sds_reset, chip_id, port);

	return ret;
}
