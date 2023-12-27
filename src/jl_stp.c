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

#include "jl_stp.h"
#include "jl_ops.h"

jl_api_ret_t jl_stp_port_egress_state_set(jl_uint32 chip_id, jl_port_t port, jl_stp_state_t stp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, stp_port_egress_state_set, chip_id, port, stp_state);

	return ret;
}

jl_api_ret_t jl_stp_port_egress_state_get(jl_uint32 chip_id, jl_port_t port, jl_stp_state_t *pstp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, stp_port_egress_state_get, chip_id, port, pstp_state);

	return ret;
}

jl_api_ret_t jl_stp_port_ingress_state_set(jl_uint32 chip_id, jl_port_t port, jl_stp_state_t stp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, stp_port_ingress_state_set, chip_id, port, stp_state);

	return ret;
}

jl_api_ret_t jl_stp_port_ingress_state_get(jl_uint32 chip_id, jl_port_t port, jl_stp_state_t *pstp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, stp_port_ingress_state_get, chip_id, port, pstp_state);

	return ret;
}

jl_api_ret_t jl_stp_ingress_state_drop_get(jl_uint32 chip_id, jl_stp_state_t stp_state, jl_uint32 *pcntr)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, stp_ingress_state_drop_get, chip_id, stp_state, pcntr);

	return ret;
}

jl_api_ret_t jl_stp_ingress_state_drop_clear(jl_uint32 chip_id, jl_stp_state_t stp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, stp_ingress_state_drop_clear, chip_id, stp_state);

	return ret;
}

jl_api_ret_t jl_stp_port_egress_drop_get(jl_uint32 chip_id, jl_port_t port, jl_uint32 *pcntr)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, stp_port_egress_drop_get, chip_id, port, pcntr);

	return ret;
}

jl_api_ret_t jl_stp_port_egress_drop_clear(jl_uint32 chip_id, jl_port_t port)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, stp_port_egress_drop_clear, chip_id, port);

	return ret;
}

jl_api_ret_t jl_mstp_port_egress_state_set(jl_uint32 chip_id, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_port_egress_state_set, chip_id, msti, port, mstp_state);

	return ret;
}

jl_api_ret_t jl_mstp_port_egress_state_get(jl_uint32 chip_id, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_port_egress_state_get, chip_id, msti, port, pmstp_state);

	return ret;
}

jl_api_ret_t jl_mstp_port_ingress_state_set(jl_uint32 chip_id, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_port_ingress_state_set, chip_id, msti, port, mstp_state);

	return ret;
}

jl_api_ret_t jl_mstp_port_ingress_state_get(jl_uint32 chip_id, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_port_ingress_state_get, chip_id, msti, port, pmstp_state);

	return ret;
}

jl_api_ret_t jl_mstp_ingress_state_drop_get(jl_uint32 chip_id, jl_mstp_state_t mstp_state, jl_uint32 *pcntr)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_ingress_state_drop_get, chip_id, mstp_state, pcntr);

	return ret;
}

jl_api_ret_t jl_mstp_ingress_state_drop_clear(jl_uint32 chip_id, jl_mstp_state_t mstp_state)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_ingress_state_drop_clear, chip_id, mstp_state);

	return ret;
}

jl_api_ret_t jl_mstp_port_egress_drop_get(jl_uint32 chip_id, jl_port_t port, jl_uint32 *pcntr)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_port_egress_drop_get, chip_id, port, pcntr);

	return ret;
}

jl_api_ret_t jl_mstp_port_egress_drop_clear(jl_uint32 chip_id, jl_port_t port)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_port_egress_drop_clear, chip_id, port);

	return ret;
}

jl_api_ret_t jl_mstp_vlan_msti_set(jl_uint32 chip_id, jl_uint16 vid, jl_mstp_msti_t msti)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_vlan_msti_set, chip_id, vid, msti);

	return ret;
}

jl_api_ret_t jl_mstp_vlan_msti_get(jl_uint32 chip_id, jl_uint16 vid, jl_mstp_msti_t *pmsti)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, mstp_vlan_msti_get, chip_id, vid, pmsti);

	return ret;
}
