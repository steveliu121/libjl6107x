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

#include "jl6107x/jl6107x_drv_stp.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"

jl_ret_t jl6107x_stp_port_ingress_state_set(jl_device_t *device, jl_port_t port, jl_stp_state_t stp_state)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	if (stp_state >= STP_STATE_END)
		return JL_ERR_PARAM;

	REGISTER_READ(device, SWCORE, SOURCE_PORT_TABLE, srctbl, jlxx_port_l2c(device, port), INDEX_ZERO);

	srctbl.BF.spt = stp_state;

	REGISTER_WRITE(device, SWCORE, SOURCE_PORT_TABLE, srctbl, jlxx_port_l2c(device, port), INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, srctbl[0]: 0x%x, srctbl[1]: 0x%x, srctbl[2]: 0x%x, srctbl[3]: 0x%x \n", \
					jlxx_port_l2c(device, port), srctbl.val[0], srctbl.val[1], srctbl.val[2], srctbl.val[3]);

	return ret;
}

jl_ret_t jl6107x_stp_port_ingress_state_get(jl_device_t *device, jl_port_t port, jl_stp_state_t *pstp_state)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pstp_state);

	REGISTER_READ(device, SWCORE, SOURCE_PORT_TABLE, srctbl, jlxx_port_l2c(device, port), INDEX_ZERO);

	*pstp_state = srctbl.BF.spt;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, srctbl[0]: 0x%x, srctbl[1]: 0x%x, srctbl[2]: 0x%x, srctbl[3]: 0x%x \n", \
					jlxx_port_l2c(device, port), srctbl.val[0], srctbl.val[1], srctbl.val[2], srctbl.val[3]);

	return ret;
}

jl_ret_t jl6107x_stp_port_egress_state_set(jl_device_t *device, jl_port_t port, jl_stp_state_t stp_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport = 0;
	jl_uint32 portstart = 0;
	jl_uint32 portend = 0;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	if (stp_state >= STP_STATE_END)
		return JL_ERR_PARAM;

	REGISTER_READ(device, SWCORE, EGRESS_SPANNING_TREE_STATE, estpst, INDEX_ZERO, INDEX_ZERO);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PARAM;
	portstart = cport * 3;
	portend = portstart + 2;
	WRITE_BITS(estpst.BF.spt_state, portstart, portend, stp_state);

	REGISTER_WRITE(device, SWCORE, EGRESS_SPANNING_TREE_STATE, estpst, INDEX_ZERO, INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, estpst[0]: 0x%x, estpst[1]: 0x%x \n", \
					jlxx_port_l2c(device, port), estpst.val[0], estpst.val[1]);

	return ret;
}

jl_ret_t jl6107x_stp_port_egress_state_get(jl_device_t *device, jl_port_t port, jl_stp_state_t *pstp_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport = 0;
	jl_uint32 portstart = 0;
	jl_uint32 portend = 0;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pstp_state);

	REGISTER_READ(device, SWCORE, EGRESS_SPANNING_TREE_STATE, estpst, INDEX_ZERO, INDEX_ZERO);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PARAM;
	portstart = cport * 3;
	portend = portstart + 2;
	*pstp_state = GET_BITS(estpst.BF.spt_state, portstart, portend);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, estpst[0]: 0x%x, estpst[1]: 0x%x \n", \
					jlxx_port_l2c(device, port), estpst.val[0], estpst.val[1]);

	return ret;
}

jl_ret_t jl6107x_stp_ingress_state_drop_get(jl_device_t *device, jl_stp_state_t stp_state, jl_uint32 *pcntr)
{
	jl_ret_t ret = JL_ERR_OK;

	if (stp_state >= STP_STATE_END)
		return JL_ERR_PARAM;

	JL_CHECK_POINTER(pcntr);

	/* Packets will be forwarded as normal when set to
	 * STP_STATE_DISABLED or STP_STATE_FORWARDING
	 */
	if (stp_state == STP_STATE_DISABLED || stp_state == STP_STATE_FORWARDING)
		return JL_ERR_PARAM;

	if (stp_state == STP_STATE_BLOCKING) {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, istpblock, INDEX_ZERO, INDEX_ZERO);
		*pcntr = (jl_uint32)istpblock.BF.packets;

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: istpblock: 0x%x \n", \
						istpblock.val[0]);
	} else if (stp_state == STP_STATE_LISTENING) {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LISTEN, istplisten, INDEX_ZERO, INDEX_ZERO);
		*pcntr = (jl_uint32)istplisten.BF.packets;

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: istplisten: 0x%x \n", \
						istplisten.val[0]);
	} else {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, istplearn, INDEX_ZERO, INDEX_ZERO);
		*pcntr = (jl_uint32)istplearn.BF.packets;

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: istplearn: 0x%x \n", \
						istplearn.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_stp_ingress_state_drop_clear(jl_device_t *device, jl_stp_state_t stp_state)
{
	jl_ret_t ret = JL_ERR_OK;

	if (stp_state >= STP_STATE_END)
		return JL_ERR_PARAM;

	/* Packets will be forwarded as normal when set to
	 * STP_STATE_DISABLED or STP_STATE_FORWARDING
	 */
	if (stp_state == STP_STATE_DISABLED || stp_state == STP_STATE_FORWARDING)
		return JL_ERR_PARAM;

	if (stp_state == STP_STATE_BLOCKING) {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, istpblock, INDEX_ZERO, INDEX_ZERO);
		istpblock.BF.packets = 0;
		REGISTER_WRITE(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, istpblock, INDEX_ZERO, INDEX_ZERO);

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: istpblock: 0x%x \n", \
						istpblock.val[0]);
	} else if (stp_state == STP_STATE_LISTENING) {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LISTEN, istplisten, INDEX_ZERO, INDEX_ZERO);
		istplisten.BF.packets = 0;
		REGISTER_WRITE(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LISTEN, istplisten, INDEX_ZERO, INDEX_ZERO);

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: istplisten: 0x%x \n", \
						istplisten.val[0]);
	} else {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, istplearn, INDEX_ZERO, INDEX_ZERO);
		istplearn.BF.packets = 0;
		REGISTER_WRITE(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, istplearn, INDEX_ZERO, INDEX_ZERO);

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: istplearn: 0x%x \n", \
						istplearn.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_stp_port_egress_drop_get(jl_device_t *device, jl_port_t port, jl_uint32 *pcntr)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pcntr);

	REGISTER_READ(device, SWCORE, EGRESS_SPANNING_TREE_DROP, estpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	*pcntr = (jl_uint32)estpdrop.BF.packets;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, estpdrop: 0x%x \n", \
					jlxx_port_l2c(device, port), estpdrop.val[0]);

	return ret;
}

jl_ret_t jl6107x_stp_port_egress_drop_clear(jl_device_t *device, jl_port_t port)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	REGISTER_READ(device, SWCORE, EGRESS_SPANNING_TREE_DROP, estpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	estpdrop.BF.packets = 0;

	REGISTER_WRITE(device, SWCORE, EGRESS_SPANNING_TREE_DROP, estpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, estpdrop: 0x%x \n", \
					jlxx_port_l2c(device, port), estpdrop.val[0]);

	return ret;
}

jl_ret_t jl6107x_mstp_port_ingress_state_set(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport = 0;
	jl_uint32 portstart = 0;
	jl_uint32 portend = 0;

	if (msti >= JL_MSTP_MSTI_MAX_NUM)
		return JL_ERR_PARAM;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	if (mstp_state >= MSTP_STATE_END)
		return JL_ERR_PARAM;

	REGISTER_READ(device, SWCORE, INGRESS_MULTIPLE_SPANNING_TREE_STATE, imstpst, msti, INDEX_ZERO);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PARAM;
	portstart = cport * 2;
	portend = portstart + 1;
	WRITE_BITS(imstpst.BF.port_spt_state, portstart, portend, mstp_state);

	REGISTER_WRITE(device, SWCORE, INGRESS_MULTIPLE_SPANNING_TREE_STATE, imstpst, msti, INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, imstpst: 0x%x \n", \
					jlxx_port_l2c(device, port), imstpst.val[0]);

	return ret;
}

jl_ret_t jl6107x_mstp_port_ingress_state_get(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport = 0;
	jl_uint32 portstart = 0;
	jl_uint32 portend = 0;

	if (msti >= JL_MSTP_MSTI_MAX_NUM)
		return JL_ERR_PARAM;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pmstp_state);

	REGISTER_READ(device, SWCORE, INGRESS_MULTIPLE_SPANNING_TREE_STATE, imstpst, msti, INDEX_ZERO);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PARAM;
	portstart = cport * 2;
	portend = portstart + 1;
	*pmstp_state = GET_BITS(imstpst.BF.port_spt_state, portstart, portend);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, imstpst: 0x%x \n", \
					jlxx_port_l2c(device, port), imstpst.val[0]);

	return ret;
}

jl_ret_t jl6107x_mstp_port_egress_state_set(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport = 0;
	jl_uint32 portstart = 0;
	jl_uint32 portend = 0;

	if (msti >= JL_MSTP_MSTI_MAX_NUM)
		return JL_ERR_PARAM;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	if (mstp_state >= MSTP_STATE_END)
		return JL_ERR_PARAM;

	REGISTER_READ(device, SWCORE, EGRESS_MULTIPLE_SPANNING_TREE_STATE, emstpst, msti, INDEX_ZERO);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PARAM;
	portstart = cport * 2;
	portend = portstart + 1;
	WRITE_BITS(emstpst.BF.port_spt_state, portstart, portend, mstp_state);

	REGISTER_WRITE(device, SWCORE, EGRESS_MULTIPLE_SPANNING_TREE_STATE, emstpst, msti, INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, emstpst: 0x%x \n", \
					jlxx_port_l2c(device, port), emstpst.val[0]);

	return ret;
}

jl_ret_t jl6107x_mstp_port_egress_state_get(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport = 0;
	jl_uint32 portstart = 0;
	jl_uint32 portend = 0;

	if (msti >= JL_MSTP_MSTI_MAX_NUM)
		return JL_ERR_PARAM;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pmstp_state);

	REGISTER_READ(device, SWCORE, EGRESS_MULTIPLE_SPANNING_TREE_STATE, emstpst, msti, INDEX_ZERO);

	cport = jlxx_port_l2c(device, port);
	if (cport == UNDEF_PORT)
		return JL_ERR_PARAM;
	portstart = cport * 2;
	portend = portstart + 1;
	*pmstp_state = GET_BITS(emstpst.BF.port_spt_state, portstart, portend);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, emstpst: 0x%x \n", \
					jlxx_port_l2c(device, port), emstpst.val[0]);

	return ret;
}

jl_ret_t jl6107x_mstp_ingress_state_drop_get(jl_device_t *device, jl_mstp_state_t mstp_state, jl_uint32 *pcntr)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Packets will be forwarded as normal when set to MSTP_STATE_FORWARDING */
	if (mstp_state >= MSTP_STATE_END || mstp_state == MSTP_STATE_FORWARDING)
		return JL_ERR_PARAM;

	JL_CHECK_POINTER(pcntr);

	if (mstp_state == MSTP_STATE_DISCARDING) {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, imstpblock, INDEX_ZERO, INDEX_ZERO);
		*pcntr = (jl_uint32)imstpblock.BF.packets;

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: imstpblock: 0x%x \n", \
						imstpblock.val[0]);
	} else {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, imstplearn, INDEX_ZERO, INDEX_ZERO);
		*pcntr = (jl_uint32)imstplearn.BF.packets;

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: imstplearn: 0x%x \n", \
						imstplearn.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_mstp_ingress_state_drop_clear(jl_device_t *device, jl_mstp_state_t mstp_state)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Packets will be forwarded as normal when set to MSTP_STATE_FORWARDING */
	if (mstp_state >= MSTP_STATE_END || mstp_state == MSTP_STATE_FORWARDING)
		return JL_ERR_PARAM;

	if (mstp_state == MSTP_STATE_DISCARDING) {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, imstpblock, INDEX_ZERO, INDEX_ZERO);
		imstpblock.BF.packets = 0;
		REGISTER_WRITE(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, imstpblock, INDEX_ZERO, INDEX_ZERO);

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: imstpblock: 0x%x \n", \
						imstpblock.val[0]);
	} else {
		REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, imstplearn, INDEX_ZERO, INDEX_ZERO);
		imstplearn.BF.packets = 0;
		REGISTER_WRITE(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, imstplearn, INDEX_ZERO, INDEX_ZERO);

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: imstplearn: 0x%x \n", \
						imstplearn.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_mstp_port_egress_drop_get(jl_device_t *device, jl_port_t port, jl_uint32 *pcntr)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pcntr);

	REGISTER_READ(device, SWCORE, EGRESS_SPANNING_TREE_DROP, emstpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	*pcntr = (jl_uint32)emstpdrop.BF.packets;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, emstpdrop: 0x%x \n", \
					jlxx_port_l2c(device, port), emstpdrop.val[0]);

	return ret;
}

jl_ret_t jl6107x_mstp_port_egress_drop_clear(jl_device_t *device, jl_port_t port)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	REGISTER_READ(device, SWCORE, EGRESS_SPANNING_TREE_DROP, emstpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	emstpdrop.BF.packets = 0;

	REGISTER_WRITE(device, SWCORE, EGRESS_SPANNING_TREE_DROP, emstpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, emstpdrop: 0x%x \n", \
					jlxx_port_l2c(device, port), emstpdrop.val[0]);

	return ret;
}

jl_ret_t jl6107x_mstp_vlan_msti_set(jl_device_t *device, jl_uint16 vid, jl_mstp_msti_t msti)
{
	jl_ret_t ret = JL_ERR_OK;

	if (vid >= JL_MSTP_VID_MAX_NUM)
		return JL_ERR_PARAM;

	if (msti >= JL_MSTP_MSTI_MAX_NUM)
		return JL_ERR_PARAM;

	REGISTER_READ(device, SWCORE, VLAN_TABLE, vlantbl, vid, INDEX_ZERO);

	vlantbl.BF.mspt_ptr = msti;

	REGISTER_WRITE(device, SWCORE, VLAN_TABLE, vlantbl, vid, INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: vlantbl[0]: 0x%x, vlantbl[1]: 0x%x \n", \
					vlantbl.val[0], vlantbl.val[1]);

	return ret;
}

jl_ret_t jl6107x_mstp_vlan_msti_get(jl_device_t *device, jl_uint16 vid, jl_mstp_msti_t *pmsti)
{
	jl_ret_t ret = JL_ERR_OK;

	if (vid >= JL_MSTP_VID_MAX_NUM)
		return JL_ERR_PARAM;

	JL_CHECK_POINTER(pmsti);

	REGISTER_READ(device, SWCORE, VLAN_TABLE, vlantbl, vid, INDEX_ZERO);

	*pmsti = vlantbl.BF.mspt_ptr;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: vlantbl[0]: 0x%x, vlantbl[1]: 0x%x \n", \
					vlantbl.val[0], vlantbl.val[1]);
	return ret;
}

