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

#include "jl6107x/jl6107x_drv_mirror.h"
#include "jl6107x/jl6107x_chip.h"


jl_ret_t jl6107x_mirror_input_mr_get(jl_device_t *device, jl_port_t mirrored_port, jl_port_t *pmirroring_port)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 cmirrored_port;

	JL_CHECK_POINTER(pmirroring_port);
	JL_CHECK_PORT(device, mirrored_port);

	cmirrored_port = jlxx_port_l2c(device, mirrored_port);

	REGISTER_READ(device, SWCORE, SOURCE_PORT_TABLE, source_tbl, cmirrored_port, INDEX_ZERO);

	if (source_tbl.BF.input_mirror_enabled) {
		*pmirroring_port = jlxx_port_c2l(device, source_tbl.BF.dest_input_mirror);
		return JL_ERR_OK;
	}

	*pmirroring_port = UNDEF_PORT;

	return JL_ERR_UNAVAIL;

}

jl_ret_t jl6107x_mirror_input_mr_set(jl_device_t *device, jl_port_t mirrored_port, jl_port_t mirroring_port)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 cmirrored_port, cmirroring_port;

	JL_CHECK_PORT(device, mirrored_port);
	JL_CHECK_PORT(device, mirroring_port);

	cmirrored_port = jlxx_port_l2c(device, mirrored_port);
	cmirroring_port = jlxx_port_l2c(device, mirroring_port);

	REGISTER_READ(device, SWCORE, SOURCE_PORT_TABLE, source_tbl, cmirrored_port, INDEX_ZERO);

	source_tbl.BF.dest_input_mirror = cmirroring_port;
	source_tbl.BF.input_mirror_enabled = 1;

	REGISTER_WRITE(device, SWCORE, SOURCE_PORT_TABLE, source_tbl, cmirrored_port, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_mirror_output_mr_get(jl_device_t *device, jl_port_t mirrored_port, jl_port_t *pmirroring_port)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 cmirrored_port;

	JL_CHECK_POINTER(pmirroring_port);
	JL_CHECK_PORT(device, mirrored_port);

	cmirrored_port = jlxx_port_l2c(device, mirrored_port);

	REGISTER_READ(device, SWCORE, OUTPUT_MIRRORING_TABLE, mirr_tbl, cmirrored_port, INDEX_ZERO);

	if (mirr_tbl.BF.output_mirror_enabled) {
		*pmirroring_port = jlxx_port_c2l(device, mirr_tbl.BF.output_mirror_port);
		return JL_ERR_OK;
	}

	*pmirroring_port = UNDEF_PORT;

	return JL_ERR_UNAVAIL;
}

jl_ret_t jl6107x_mirror_output_mr_set(jl_device_t *device, jl_port_t mirrored_port, jl_port_t mirroring_port)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 cmirrored_port, cmirroring_port, cport;

	JL_CHECK_PORT(device, mirrored_port);
	JL_CHECK_PORT(device, mirroring_port);

	cmirrored_port = jlxx_port_l2c(device, mirrored_port);
	cmirroring_port = jlxx_port_l2c(device, mirroring_port);

	REGISTER_READ(device, SWCORE, OUTPUT_MIRRORING_TABLE, mirr_tbl, cmirrored_port, INDEX_ZERO);

	mirr_tbl.BF.output_mirror_port = cmirroring_port;
	mirr_tbl.BF.output_mirror_enabled = 1;

	REGISTER_WRITE(device, SWCORE, OUTPUT_MIRRORING_TABLE, mirr_tbl, cmirrored_port, INDEX_ZERO);

	JL_FOR_EACH_CPORT(device, cport) {
		if (cmirrored_port != cport) {
			REGISTER_READ(device, SWCORE, OUTPUT_MIRRORING_TABLE, mirr_tbl, cport, INDEX_ZERO);
			if (mirr_tbl.BF.output_mirror_enabled == 1)
				return ret;
		}
	}
	REGISTER_READ(device, SWC, MC_ERM_EN, erm_en, INDEX_ZERO, INDEX_ZERO);

	erm_en.BF.mc_erm_en = 0;

	REGISTER_WRITE(device, SWC, MC_ERM_EN, erm_en, INDEX_ZERO, INDEX_ZERO);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "it's first time to set mirror, disable erm\n");

	return ret;
}

jl_ret_t jl6107x_mirror_input_mr_clear(jl_device_t *device, jl_port_t mirrored_port)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 cmirrored_port;

	JL_CHECK_PORT(device, mirrored_port);
	cmirrored_port = jlxx_port_l2c(device, mirrored_port);

	REGISTER_READ(device, SWCORE, SOURCE_PORT_TABLE, source_tbl, cmirrored_port, INDEX_ZERO);

	source_tbl.BF.input_mirror_enabled = 0;
	source_tbl.BF.dest_input_mirror = 0;

	REGISTER_WRITE(device, SWCORE, SOURCE_PORT_TABLE, source_tbl, cmirrored_port, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_mirror_output_mr_clear(jl_device_t *device, jl_port_t mirrored_port)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 cmirrored_port, cport;

	JL_CHECK_PORT(device, mirrored_port);

	cmirrored_port = jlxx_port_l2c(device, mirrored_port);

	REGISTER_READ(device, SWCORE, OUTPUT_MIRRORING_TABLE, mirr_tbl, cmirrored_port, INDEX_ZERO);

	mirr_tbl.BF.output_mirror_enabled = 0;
	mirr_tbl.BF.output_mirror_port = 0;

	REGISTER_WRITE(device, SWCORE, OUTPUT_MIRRORING_TABLE, mirr_tbl, cmirrored_port, INDEX_ZERO);

	JL_FOR_EACH_CPORT(device, cport) {
		REGISTER_READ(device, SWCORE, OUTPUT_MIRRORING_TABLE, mirr_tbl, cport, INDEX_ZERO);
		if (mirr_tbl.BF.output_mirror_enabled == 1)
			return ret;
	}
	REGISTER_READ(device, SWC, MC_ERM_EN, erm_en, INDEX_ZERO, INDEX_ZERO);

	erm_en.BF.mc_erm_en = 0;
	REGISTER_WRITE(device, SWC, MC_ERM_EN, erm_en, INDEX_ZERO, INDEX_ZERO);

	erm_en.BF.mc_erm_en = 1;
	REGISTER_WRITE(device, SWC, MC_ERM_EN, erm_en, INDEX_ZERO, INDEX_ZERO);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "all mirror is cleared, enable erm.\n");

	return ret;
}


