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

#include "jl_interrupt.h"
#include "jl_ops.h"

jl_ret_t jl_interrupt_polarity_set(jl_uint32 chip_id, jl_int_polarity_t polarity)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_polarity_set, chip_id, polarity);

	return ret;
}

jl_ret_t jl_interrupt_polarity_get(jl_uint32 chip_id, jl_int_polarity_t *ppolarity)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_polarity_get, chip_id, ppolarity);

	return ret;
}

jl_ret_t jl_interrupt_port_phy_int_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_port_phy_int_enable_set, chip_id, port, enable);

	return ret;
}

jl_ret_t jl_interrupt_port_phy_int_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_port_phy_int_enable_get, chip_id, port, penable);

	return ret;
}

jl_ret_t jl_interrupt_port_phy_int_status_get(jl_uint32 chip_id, jl_port_t port, jl_uint32 *pstatus)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_port_phy_int_status_get, chip_id, port, pstatus);

	return ret;
}

jl_ret_t jl_interrupt_enable_set(jl_uint32 chip_id, jl_int_type_t type, jl_enable_t enable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_enable_set, chip_id, type, enable);

	return ret;
}

jl_ret_t jl_interrupt_enable_get(jl_uint32 chip_id, jl_int_type_t type, jl_enable_t *penable)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_enable_get, chip_id, type, penable);

	return ret;
}

jl_ret_t jl_interrupt_status_clean(jl_uint32 chip_id, jl_int_type_t type)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_status_clean, chip_id, type);

	return ret;
}

jl_ret_t jl_interrupt_status_get(jl_uint32 chip_id, jl_int_status_t *pstatusmask)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_status_get, chip_id, pstatusmask);

	return ret;
}

jl_ret_t jl_interrupt_detail_get(jl_uint32 chip_id, jl_int_type_t type, jl_int_info_t *pinfo)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_DRV_SAFE_CALL(ret, interrupt_detail_get, chip_id, type, pinfo);

	return ret;
}
