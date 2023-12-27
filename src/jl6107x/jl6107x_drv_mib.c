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

#include "jl6107x/jl6107x_drv_mib.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"

jl_ret_t jl6107x_mib_port_rc_mode_get(jl_device_t *device, jl_port_t port, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(penable);

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, STATISTIC_MODE_0, lmode0, INDEX_ZERO, jlxx_port_l2p(device, port));
		*penable = lmode0.BF.car_en;

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: phy port: %u, lmode0: 0x%x \n", \
						jlxx_port_l2p(device, port), lmode0.val[0]);
	} else {
		REGISTER_READ(device, SMAC_ADP, STATISTIC_MODE_0, smode0, INDEX_ZERO, jlxx_port_l2p(device, port));
		*penable = smode0.BF.car_en;

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: phy port: %u, smode0: 0x%x \n", \
						jlxx_port_l2p(device, port), smode0.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_mib_port_rc_mode_set(jl_device_t *device, jl_port_t port, jl_uint32 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, STATISTIC_MODE_0, lmode0, INDEX_ZERO, jlxx_port_l2p(device, port));
		lmode0.BF.car_en = enable;
		REGISTER_WRITE(device, LMAC_ADP, STATISTIC_MODE_0, lmode0, INDEX_ZERO, jlxx_port_l2p(device, port));

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: phy port: %u, lmode0: 0x%x \n", \
						jlxx_port_l2p(device, port), lmode0.val[0]);
	} else {
		REGISTER_READ(device, SMAC_ADP, STATISTIC_MODE_0, smode0, INDEX_ZERO, jlxx_port_l2p(device, port));
		smode0.BF.car_en = enable;
		REGISTER_WRITE(device, SMAC_ADP, STATISTIC_MODE_0, smode0, INDEX_ZERO, jlxx_port_l2p(device, port));

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: phy port: %u, smode0: 0x%x \n", \
						jlxx_port_l2p(device, port), smode0.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_mib_port_clear(jl_device_t *device, jl_port_t port, jl_port_mib_type_t type)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_MIB_CHECK_TYPE(type);

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REGISTER_READ(device, LMAC_ADP, STATISTIC_MODE_1, lmode1, INDEX_ZERO, jlxx_port_l2p(device, port));
		lmode1.BF.mib_clear = ENABLED;
		lmode1.BF.mib_id = type;
		REGISTER_WRITE(device, LMAC_ADP, STATISTIC_MODE_1, lmode1, INDEX_ZERO, jlxx_port_l2p(device, port));

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: phy port: %u, lmode1: 0x%x \n", \
						jlxx_port_l2p(device, port), lmode1.val[0]);
	} else {
		REGISTER_READ(device, SMAC_ADP, STATISTIC_MODE_1, smode1, INDEX_ZERO, jlxx_port_l2p(device, port));
		smode1.BF.mib_clear = ENABLED;
		smode1.BF.mib_id = type;
		REGISTER_WRITE(device, SMAC_ADP, STATISTIC_MODE_1, smode1, INDEX_ZERO, jlxx_port_l2p(device, port));

		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ON, "MAC: phy port: %u, smode1: 0x%x \n", \
						jlxx_port_l2p(device, port), smode1.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_mib_port_clear_all(jl_device_t *device, jl_port_t port)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 type;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	for (type = 0; type < PORT_MIB_TYPE_END; type++) {
		ret = jl6107x_mib_port_clear(device, port, type);
		if (ret != JL_ERR_OK)
			break;
	}

	return ret;
}

jl_ret_t jl6107x_mib_port_get(jl_device_t *device, jl_port_t port, jl_port_mib_type_t type, jl_mib_counter_t *pcnt)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 burst = 0;
	jl_uint32 pval[2] = {0};
	jl_uint32 *ptr = NULL;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_MIB_CHECK_TYPE(type);

	JL_CHECK_POINTER(pcnt);

	CONST_T jl_uint8 mib_reg_offset[PORT_MIB_TYPE_END] = {
		0, 2, 4, 6, 8, 10, 12, 14, 16, 17,
		18, 20, 22, 24, 26, 28, 30, 32, 34, 35, 36 };

	ptr = (jl_uint32 *)pcnt;

	switch (type) {
	case PORT_MIB_TX_PKT_CNT:
	case PORT_MIB_TX_PKT_BYTE_CNT:
	case PORT_MIB_TX_PKT_UC_CNT:
	case PORT_MIB_TX_PKT_UC_BYTE_CNT:
	case PORT_MIB_TX_PKT_MC_CNT:
	case PORT_MIB_TX_PKT_MC_BYTE_CNT:
	case PORT_MIB_TX_PKT_BC_CNT:
	case PORT_MIB_TX_PKT_BC_BYTE_CNT:
	case PORT_MIB_RX_PKT_CNT:
	case PORT_MIB_RX_PKT_BYTE_CNT:
	case PORT_MIB_RX_PKT_UC_CNT:
	case PORT_MIB_RX_PKT_UC_BYTE_CNT:
	case PORT_MIB_RX_PKT_MC_CNT:
	case PORT_MIB_RX_PKT_MC_BYTE_CNT:
	case PORT_MIB_RX_PKT_BC_CNT:
	case PORT_MIB_RX_PKT_BC_BYTE_CNT:
		burst = 2;
		break;
	case PORT_MIB_TX_PKT_DRP_CNT:
	case PORT_MIB_TX_PKT_ERR_CNT:
	case PORT_MIB_RX_PKT_DRP_CNT:
	case PORT_MIB_RX_PKT_CRC_ERR_CNT:
	case PORT_MIB_RX_FRAG_ERR_CNT:
		burst = 1;
		break;
	default:
		JL_DBG_MSG(JL_FLAG_EMAC, _DBG_ERROR, "mib type overflow!\n");
		ret = JL_ERR_PARAM;
		break;
	}

	if (jlxx_utpport_check(device, port) == JL_ERR_OK) {
		REG_BURST_READ_OFFSET(device, LMAC_ADP, TX_PKT_CNT, mib_reg_offset[type], jlxx_port_l2p(device, port), burst, pval);
	} else {
		REG_BURST_READ_OFFSET(device, SMAC_ADP, TX_PKT_CNT, mib_reg_offset[type], jlxx_port_l2p(device, port), burst, pval);
	}

	*ptr++ = pval[0];
	*ptr = pval[1];

	return ret;
}

jl_ret_t jl6107x_mib_port_get_all(jl_device_t *device, jl_port_t port, jl_port_mib_cnt_t *pport_cnt)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_enable_t rc = 0;
	jl_mib_counter_t counter = 0;
	jl_uint32 *ptr = NULL;
	jl_port_mib_type_t idx = 0;

	/* Check Port Valid */
	JL_CHECK_UTP_EXT_PORT(device, port);

	JL_CHECK_POINTER(pport_cnt);

	CONST_T jl_uint8 mib_reg_width[PORT_MIB_TYPE_END] = {
		2, 2, 2, 2, 2, 2, 2, 2, 1, 1,
		2, 2, 2, 2, 2, 2, 2, 2, 1, 1, 1 };

	ret = jl6107x_mib_port_rc_mode_get(device, port, &rc);
	if (ret != JL_ERR_OK)
		return ret;

	pport_cnt->rc_enable = rc;

	ptr = (jl_uint32 *)(&(pport_cnt->tx_pkt_cnt));

	for (idx = 0; idx < PORT_MIB_TYPE_END; idx++) {
		counter = 0;
		ret = jl6107x_mib_port_get(device, port, idx, &counter);
		if (ret != JL_ERR_OK)
			return ret;
		if (mib_reg_width[idx] == 2)
			*((jl_uint64 *)ptr) = counter;
		else if (mib_reg_width[idx] == 1)
			*ptr = (counter & 0xFFFFFFFF);
		else
			return JL_ERR_FAIL;
		ptr += mib_reg_width[idx];
	}

	return ret;
}

jl_ret_t jl6107x_mib_swc_global_drop_get(jl_device_t *device, jl_mib_swc_glb_drop_t *pport_cnt)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(pport_cnt);

	REGISTER_READ(device, SWCORE, IPP_BROKEN_PACKETS, ippbrkpkt, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, IPP_PM_DROP, ipppmdrp, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EPP_PM_DROP, epppmdrp, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, UNKNOWN_INGRESS_DROP, unkowningdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EMPTY_MASK_DROP, empmskdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_PACKET_FILTERING_DROP, inpktfilterdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LISTEN, instpdroplisten, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, instpdroplearn, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, instpdropblk, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, L2_LOOKUP_DROP, l2lookupdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, L2_RESERVED_MULTICAST_ADDRESS_DROP, l2rsvmultiaddrdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, RESERVED_MAC_SA_DROP, rsvmacsadrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, RESERVED_MAC_DA_DROP, rsvmacdadrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_CONFIGURABLE_ACL_DROP, inconfigacldrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, VLAN_MEMBER_DROP, vlanmemdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MINIMUM_ALLOWED_VLAN_DROP, minivlandrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MAXIMUM_ALLOWED_VLAN_DROP, maxvlandrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, BUFFER_OVERFLOW_DROP, bufoverdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_RESOURCE_MANAGER_DROP, inresmangdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, RE_QUEUE_OVERFLOW_DROP, reqoverdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_EGRESS_ADMISSION_CONTROL_DROP, inegadctldrop, INDEX_ZERO, INDEX_ZERO);

	pport_cnt->ipp_broken_pkt = ippbrkpkt.BF.packets;
	pport_cnt->ipp_pm_drop = ipppmdrp.BF.packets;
	pport_cnt->epp_pm_drop = epppmdrp.BF.packets;
	pport_cnt->unknown_ingress_drop = unkowningdrop.BF.packets;
	pport_cnt->empty_mask_drop = empmskdrop.BF.packets;
	pport_cnt->ingress_pkt_filter_drop = inpktfilterdrop.BF.packets;
	pport_cnt->ingress_stp_listen_drop = instpdroplisten.BF.packets;
	pport_cnt->ingress_stp_learn_drop = instpdroplearn.BF.packets;
	pport_cnt->ingress_stp_block_drop = instpdropblk.BF.packets;
	pport_cnt->l2_lookup_drop = l2lookupdrop.BF.packets;
	pport_cnt->l2_rsvd_multi_addr_drop = l2rsvmultiaddrdrop.BF.packets;
	pport_cnt->rsvd_mac_sa_drop = rsvmacsadrop.BF.packets;
	pport_cnt->rsvd_mac_da_drop = rsvmacdadrop.BF.packets;
	pport_cnt->ingress_config_acl_drop = inconfigacldrop.BF.packets;
	pport_cnt->vlan_member_drop = vlanmemdrop.BF.packets;
	pport_cnt->mini_allow_vlan_drop = minivlandrop.BF.packets;
	pport_cnt->max_allow_vlan_drop = maxvlandrop.BF.packets;
	pport_cnt->buf_overflow_drop = bufoverdrop.BF.packets;
	pport_cnt->ingress_res_manager_drop = inresmangdrop.BF.packets;
	pport_cnt->re_queue_overflow_drop = reqoverdrop.BF.packets;
	pport_cnt->ingress_egress_admin_ctrl_drop = inegadctldrop.BF.packets;

	return ret;
}

jl_ret_t jl6107x_mib_swc_global_drop_clear(jl_device_t *device)
{
	jl_ret_t ret = JL_ERR_OK;

	REGISTER_READ(device, SWCORE, IPP_BROKEN_PACKETS, ippbrkpkt, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, IPP_PM_DROP, ipppmdrp, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EPP_PM_DROP, epppmdrp, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, UNKNOWN_INGRESS_DROP, unkowningdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EMPTY_MASK_DROP, empmskdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_PACKET_FILTERING_DROP, inpktfilterdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LISTEN, instpdroplisten, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, instpdroplearn, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, instpdropblk, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, L2_LOOKUP_DROP, l2lookupdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, L2_RESERVED_MULTICAST_ADDRESS_DROP, l2rsvmultiaddrdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, RESERVED_MAC_SA_DROP, rsvmacsadrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, RESERVED_MAC_DA_DROP, rsvmacdadrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_CONFIGURABLE_ACL_DROP, inconfigacldrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, VLAN_MEMBER_DROP, vlanmemdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MINIMUM_ALLOWED_VLAN_DROP, minivlandrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MAXIMUM_ALLOWED_VLAN_DROP, maxvlandrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, BUFFER_OVERFLOW_DROP, bufoverdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_RESOURCE_MANAGER_DROP, inresmangdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, RE_QUEUE_OVERFLOW_DROP, reqoverdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_EGRESS_ADMISSION_CONTROL_DROP, inegadctldrop, INDEX_ZERO, INDEX_ZERO);

	ippbrkpkt.BF.packets = 0;
	ipppmdrp.BF.packets = 0;
	epppmdrp.BF.packets = 0;
	unkowningdrop.BF.packets = 0;
	empmskdrop.BF.packets = 0;
	inpktfilterdrop.BF.packets = 0;
	instpdroplisten.BF.packets = 0;
	instpdroplearn.BF.packets = 0;
	instpdropblk.BF.packets = 0;
	l2lookupdrop.BF.packets = 0;
	l2rsvmultiaddrdrop.BF.packets = 0;
	rsvmacsadrop.BF.packets = 0;
	rsvmacdadrop.BF.packets = 0;
	inconfigacldrop.BF.packets = 0;
	vlanmemdrop.BF.packets = 0;
	minivlandrop.BF.packets = 0;
	maxvlandrop.BF.packets = 0;
	bufoverdrop.BF.packets = 0;
	inresmangdrop.BF.packets = 0;
	reqoverdrop.BF.packets = 0;
	inegadctldrop.BF.packets = 0;

	REGISTER_WRITE(device, SWCORE, IPP_BROKEN_PACKETS, ippbrkpkt, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, IPP_PM_DROP, ipppmdrp, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, EPP_PM_DROP, epppmdrp, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, UNKNOWN_INGRESS_DROP, unkowningdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, EMPTY_MASK_DROP, empmskdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, INGRESS_PACKET_FILTERING_DROP, inpktfilterdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LISTEN, instpdroplisten, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, INGRESS_SPANNING_TREE_DROP_LEARNING, instpdroplearn, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, INGRESS_SPANNING_TREE_DROP_BLOCKING, instpdropblk, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, L2_LOOKUP_DROP, l2lookupdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, L2_RESERVED_MULTICAST_ADDRESS_DROP, l2rsvmultiaddrdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, RESERVED_MAC_SA_DROP, rsvmacsadrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, RESERVED_MAC_DA_DROP, rsvmacdadrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, INGRESS_CONFIGURABLE_ACL_DROP, inconfigacldrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, VLAN_MEMBER_DROP, vlanmemdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, MINIMUM_ALLOWED_VLAN_DROP, minivlandrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, MAXIMUM_ALLOWED_VLAN_DROP, maxvlandrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, BUFFER_OVERFLOW_DROP, bufoverdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, INGRESS_RESOURCE_MANAGER_DROP, inresmangdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, RE_QUEUE_OVERFLOW_DROP, reqoverdrop, INDEX_ZERO, INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, INGRESS_EGRESS_ADMISSION_CONTROL_DROP, inegadctldrop, INDEX_ZERO, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_mib_swc_port_drop_get(jl_device_t *device, jl_port_t port, jl_mib_swc_port_drop_t *pport_cnt)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 tmpval0 = 0;
	jl_uint32 tmpval1 = 0;

	/* Check Port Valid */
	JL_CHECK_PORT(device, port);

	JL_CHECK_POINTER(pport_cnt);

	REGISTER_READ(device, SWCORE, SP_OVERFLOW_DROP, spoverdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, PS_ERROR_COUNTER, pserrcnt, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, UNKNOWN_EGRESS_DROP, unkwegrdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EGRESS_PORT_DISABLED_DROP, egrdisdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EGRESS_PORT_FILTERING_DROP, egrfilterdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, QUEUE_OFF_DROP, queueoffdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EGRESS_SPANNING_TREE_DROP, egrstpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MBSC_DROP, mbscdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_EGRESS_PACKET_FILTERING_DROP, ingegrfilterdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, DRAIN_PORT_DROP, drainportdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MAC_RX_BROKEN_PACKETS, macrxbrkpkt, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MAC_RX_SHORT_PACKET_DROP, macrxshortpktdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MAC_RX_LONG_PACKET_DROP, macrxlongpktdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EGRESS_RESOURCE_MANAGEMENT_DROP, egresmangdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	pport_cnt->sp_overflow_drop = spoverdrop.BF.packets;
	pport_cnt->ps_err_underrun = pserrcnt.BF.underrun;
	tmpval0 = pserrcnt.BF.overflow_0_7;
	tmpval1 = pserrcnt.BF.overflow_8_23;
	pport_cnt->ps_err_overflow  = tmpval0 | (tmpval1 << SWCORE_PS_ERROR_COUNTER_OFFSET_OVERFLOW_0_7_WIDTH);
	pport_cnt->unknown_egress_drop = unkwegrdrop.BF.packets;
	pport_cnt->egress_port_dis_drop = egrdisdrop.BF.packets;
	pport_cnt->egress_port_filter_drop = egrfilterdrop.BF.packets;
	pport_cnt->queue_off_drop = queueoffdrop.BF.packets;
	pport_cnt->egress_stp_drop = egrstpdrop.BF.packets;
	pport_cnt->mbsc_drop = mbscdrop.BF.packets;
	pport_cnt->ingress_egress_filter_drop = ingegrfilterdrop.BF.packets;
	pport_cnt->drain_port_drop = drainportdrop.BF.packets;
	pport_cnt->mac_rx_broken_pkt = macrxbrkpkt.BF.packets;
	pport_cnt->mac_rx_short_pkt_drop = macrxshortpktdrop.BF.packets;
	pport_cnt->mac_rx_long_packet_drop = macrxlongpktdrop.BF.packets;
	pport_cnt->egress_res_manage_drop = egresmangdrop.BF.packets;

	return ret;
}

jl_ret_t jl6107x_mib_swc_port_drop_clear(jl_device_t *device, jl_port_t port)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_PORT(device, port);

	REGISTER_READ(device, SWCORE, SP_OVERFLOW_DROP, spoverdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, PS_ERROR_COUNTER, pserrcnt, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, UNKNOWN_EGRESS_DROP, unkwegrdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EGRESS_PORT_DISABLED_DROP, egrdisdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EGRESS_PORT_FILTERING_DROP, egrfilterdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, QUEUE_OFF_DROP, queueoffdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EGRESS_SPANNING_TREE_DROP, egrstpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MBSC_DROP, mbscdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, INGRESS_EGRESS_PACKET_FILTERING_DROP, ingegrfilterdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, DRAIN_PORT_DROP, drainportdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MAC_RX_BROKEN_PACKETS, macrxbrkpkt, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MAC_RX_SHORT_PACKET_DROP, macrxshortpktdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, MAC_RX_LONG_PACKET_DROP, macrxlongpktdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_READ(device, SWCORE, EGRESS_RESOURCE_MANAGEMENT_DROP, egresmangdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	spoverdrop.BF.packets = 0;
	pserrcnt.BF.underrun = 0;
	pserrcnt.BF.overflow_0_7 = 0;
	pserrcnt.BF.overflow_8_23 = 0;
	unkwegrdrop.BF.packets = 0;
	egrdisdrop.BF.packets = 0;
	egrfilterdrop.BF.packets = 0;
	queueoffdrop.BF.packets = 0;
	egrstpdrop.BF.packets = 0;
	mbscdrop.BF.packets = 0;
	ingegrfilterdrop.BF.packets = 0;
	drainportdrop.BF.packets = 0;
	macrxbrkpkt.BF.packets = 0;
	macrxshortpktdrop.BF.packets = 0;
	macrxlongpktdrop.BF.packets = 0;
	egresmangdrop.BF.packets = 0;

	REGISTER_WRITE(device, SWCORE, SP_OVERFLOW_DROP, spoverdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, PS_ERROR_COUNTER, pserrcnt, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, UNKNOWN_EGRESS_DROP, unkwegrdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, EGRESS_PORT_DISABLED_DROP, egrdisdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, EGRESS_PORT_FILTERING_DROP, egrfilterdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, QUEUE_OFF_DROP, queueoffdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, EGRESS_SPANNING_TREE_DROP, egrstpdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, MBSC_DROP, mbscdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, INGRESS_EGRESS_PACKET_FILTERING_DROP, ingegrfilterdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, DRAIN_PORT_DROP, drainportdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, MAC_RX_BROKEN_PACKETS, macrxbrkpkt, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, MAC_RX_SHORT_PACKET_DROP, macrxshortpktdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, MAC_RX_LONG_PACKET_DROP, macrxlongpktdrop, jlxx_port_l2c(device, port), INDEX_ZERO);
	REGISTER_WRITE(device, SWCORE, EGRESS_RESOURCE_MANAGEMENT_DROP, egresmangdrop, jlxx_port_l2c(device, port), INDEX_ZERO);

	return ret;
}
