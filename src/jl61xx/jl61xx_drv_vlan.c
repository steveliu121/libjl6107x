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
#include "jl61xx/jl61xx_drv_vlan.h"
#include "jl61xx/jl61xx_drv_switch.h"
#include "jl61xx/jl61xx_chip.h"


/****Macro Define****/

/****Type Define****/
/*for svlan 9100 old standard*/
jl_ret_t _jl61xx_vlan_igs_spec_ethertype_set(
	jl_device_t *pdevice,
	const JL_VLAN_IGS_SPEC_ETHERTYPE_t vtag)
{
	jl_ret_t    ret = JL_ERR_OK;
	SWCORE_INGRESS_ETHERNET_TYPE_FOR_VLAN_TAG_t cfg;

	cfg.BF.type_value   = vtag.value;

	if (JL_VLAN_IGS_SPEC_ETHERTYPE_CVLAN == vtag.type)
		cfg.BF.type = 0;
	else if (JL_VLAN_IGS_SPEC_ETHERTYPE_SVLAN == vtag.type)
		cfg.BF.type = 1;
	else
		return JL_ERR_PARAM;

	cfg.BF.valid = vtag.valid;

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_ETHERNET_TYPE_FOR_VLAN_TAG, INDEX_ZERO, INDEX_ZERO, cfg.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set Ethertype %x tbl as %s %s.",
			   vtag.value, vtag.type ? "SVLAN" : "CVLAN", vtag.valid ? "enable" : "disable");
	return ret;
}


/*for pritag vlan processing*/
jl_ret_t _jl61xx_vlan_ingress_pretag_set(
	jl_device_t        *pdevice,
	const jl_uint32     portid,
	const jl_bool       valid,
	const jl_uint32     vid)
{
	jl_ret_t        ret = JL_ERR_OK;
	jl_uint32       mac_port;

	SWCORE_SOURCE_PORT_TABLE_t cfg;

	/*read from driver*/
	JL_PORT_L2C_MACRO(pdevice, portid, mac_port);
	REG_BURST_READ(pdevice, SWCORE, SOURCE_PORT_TABLE, mac_port, INDEX_ZERO, cfg.val);

	cfg.BF.enable_specific_vid = valid;
	cfg.BF.specific_vid_as_non_vid_0_2 = vid & BITS(0, 2);
	cfg.BF.specific_vid_as_non_vid_3_11 = BITS_OFFSET_R(vid,
										SWCORE_SOURCE_PORT_TABLE_OFFSET_SPECIFIC_VID_AS_NON_VID_0_2_WIDTH,
										SWCORE_SOURCE_PORT_TABLE_OFFSET_SPECIFIC_VID_AS_NON_VID_3_11_WIDTH);

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, SOURCE_PORT_TABLE, mac_port, INDEX_ZERO, cfg.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set pri tag vid %d mode %s\n", vid,
			   cfg.BF.enable_specific_vid == TRUE ? "enable" : "disable");

	return ret;
}


jl_ret_t _jl61xx_vlan_mstp_set(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	const jl_uint32     mstpPtr)
{
	jl_ret_t        ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t vlan_tbl;

	/*read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	vlan_tbl.BF.mspt_ptr_0_1 = BITS_OFFSET_R(mstpPtr, 0, SWCORE_VLAN_TABLE_OFFSET_MSPT_PTR_0_1_WIDTH);
	vlan_tbl.BF.mspt_ptr_2_3 = BITS_OFFSET_R(mstpPtr, SWCORE_VLAN_TABLE_OFFSET_MSPT_PTR_0_1_WIDTH,
							   SWCORE_VLAN_TABLE_OFFSET_MSPT_PTR_2_3_WIDTH);

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set mspt Ptr value %d success.\n", mstpPtr);

	return ret;
}


jl_ret_t _jl61xx_vlan_mstp_get(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_uint32          *mstpPtr)
{
	jl_ret_t        ret = JL_ERR_OK;

	SWCORE_VLAN_TABLE_t vlan_tbl;

	/*read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	*mstpPtr = vlan_tbl.BF.mspt_ptr_0_1 + BITS_OFFSET_L(vlan_tbl.BF.mspt_ptr_2_3, SWCORE_VLAN_TABLE_OFFSET_MSPT_PTR_0_1_WIDTH,
			   SWCORE_VLAN_TABLE_OFFSET_MSPT_PTR_2_3_WIDTH);

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get mstp Ptr value %d success.\n", *mstpPtr);
	return ret;
}


jl_ret_t _jl61xx_vlan_mmp_set(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	const jl_bool       mmpValid,
	const jl_uint8      mmpPtr,
	const jl_uint8      mmpOrder)
{
	jl_ret_t        ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t vlan_tbl;

	/*read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	vlan_tbl.BF.mmp_valid   = mmpValid;
	vlan_tbl.BF.mmp_ptr     = mmpPtr;
	vlan_tbl.BF.mmp_order   = mmpOrder;

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set mmp value ptr:%d order:%d mode %s\n", mmpPtr, mmpOrder,
			   mmpValid == TRUE ? "enable" : "disable");
	return ret;
}


jl_ret_t _jl61xx_vlan_mmp_get(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_uint8           *pmmpValid,
	jl_uint8           *pmmpPtr,
	jl_uint8           *pmmpOrder)
{
	jl_ret_t ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t vlan_tbl;

	/*read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	*pmmpValid  = vlan_tbl.BF.mmp_valid;
	*pmmpPtr    = vlan_tbl.BF.mmp_ptr;
	*pmmpOrder  = vlan_tbl.BF.mmp_order;

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get mmp value ptr:%d order:%d mode %s\n", *pmmpPtr, *pmmpOrder,
			   *pmmpValid == TRUE ? "enable" : "disable");
	return ret;
}


/*init :
 *for svlan 9100 old standard
 *for pritag vlan processing
 */
jl_ret_t jl61xx_vlan_init(jl_device_t *pdevice)
{
	jl_ret_t    ret = JL_ERR_OK;

	if (NULL == pdevice)
		return JL_ERR_PARAM;
	/*set vlan init as follows*/
	return ret;
}


jl_ret_t jl61xx_vlan_deinit(jl_device_t *pdevice)
{
	jl_ret_t    ret = JL_ERR_OK;

	if (NULL == pdevice)
		return JL_ERR_PARAM;

	/*set vlan deinit as follows*/
	return ret;
}



jl_ret_t jl61xx_vlan_ingressport_mode_set(
	jl_device_t                    *pdevice,
	const jl_uint32                 sportidx,
	const jl_vlan_operation_t       opt,
	const jl_vlan_val_sel_t         vidsel,
	const jl_vlan_val_sel_t         cfisel,
	const jl_vlan_val_sel_t         pcpsel,
	const jl_vlan_tpid_sel_t        ethertypesel,
	const jl_sport_vlan_assign_t    mode)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   macport;
	SWCORE_SOURCE_PORT_TABLE_t sportTbl;

	/*para check*/
	JL_CHECK_PORT(pdevice, sportidx);

	/*read from driver*/
	JL_PORT_L2C_MACRO(pdevice, sportidx, macport);
	REG_BURST_READ(pdevice, SWCORE, SOURCE_PORT_TABLE, macport, INDEX_ZERO, sportTbl.val);

	sportTbl.BF.vlan_single_op = opt;
	sportTbl.BF.vid_sel = vidsel;
	sportTbl.BF.cfi_dei_sel = cfisel;
	sportTbl.BF.pcp_sel = pcpsel;
	sportTbl.BF.type_sel = ethertypesel;
	sportTbl.BF.vlan_assignment = mode;

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, SOURCE_PORT_TABLE, macport, INDEX_ZERO, sportTbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set ingress sport %d vlan mode value %08x-%08x-%08x-%08x sucessed.\n", sportidx,
			   sportTbl.val[0], sportTbl.val[1], sportTbl.val[2], sportTbl.val[3]);

	return ret;
}


jl_ret_t jl61xx_vlan_ingressport_default_set(
	jl_device_t                *pdevice,
	const jl_uint32             sportidx,
	const jl_uint32             default_VID,
	const jl_uint32             default_CFI,
	const jl_uint32             default_PCP,
	const jl_uint8              default_order)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   macport;
	SWCORE_SOURCE_PORT_TABLE_t sportTbl;

	/*para check*/
	JL_CHECK_PORT(pdevice, sportidx);
	JL_CHECK_ENUM(default_order, VLAN_INGRESS_TBL_MAX_ORDER_VAL);

	/*read from driver*/
	JL_PORT_L2C_MACRO(pdevice, sportidx, macport);
	REG_BURST_READ(pdevice, SWCORE, SOURCE_PORT_TABLE, macport, INDEX_ZERO, sportTbl.val);

	sportTbl.BF.default_vid_0_2 = BITS_OFFSET_R(default_VID, 0, SWCORE_SOURCE_PORT_TABLE_OFFSET_DEFAULT_VID_0_2_WIDTH);
	sportTbl.BF.default_vid_3_11 = BITS_OFFSET_R(default_VID, SWCORE_SOURCE_PORT_TABLE_OFFSET_DEFAULT_VID_0_2_WIDTH,
								   SWCORE_SOURCE_PORT_TABLE_OFFSET_DEFAULT_VID_3_11_WIDTH);
	sportTbl.BF.default_cfi_dei = default_CFI;
	sportTbl.BF.default_pcp = default_PCP;
	sportTbl.BF.default_vid_order = default_order;

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, SOURCE_PORT_TABLE, macport, INDEX_ZERO, sportTbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set ingress sport %d default vlan hex value %08x-%08x-%08x-%08x sucessed.\n",
			   sportidx,
			   sportTbl.val[0], sportTbl.val[1], sportTbl.val[2], sportTbl.val[3]);

	return ret;
}


jl_ret_t jl61xx_vlan_egressport_mode_set(
	jl_device_t                    *pdevice,
	const jl_uint32                 dportidx,
	const jl_vlan_operation_t       opt,
	const jl_vlan_val_sel_t         VIDSel,
	const jl_vlan_val_sel_t         CFISel,
	const jl_vlan_val_sel_t         PCPSel,
	const jl_vlan_tpid_sel_t        EthertypeSel)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   macport;
	SWCORE_EGRESS_PORT_CONFIGURATION_t dportTbl;

	/*para check*/
	JL_CHECK_PORT(pdevice, dportidx);

	/*read from driver*/
	JL_PORT_L2C_MACRO(pdevice, dportidx, macport);
	REG_BURST_READ(pdevice, SWCORE, EGRESS_PORT_CONFIGURATION, macport, INDEX_ZERO, dportTbl.val);

	dportTbl.BF.vlan_single_op = opt;
	dportTbl.BF.vid_sel = VIDSel;
	dportTbl.BF.cfi_dei_sel = CFISel;
	dportTbl.BF.pcp_sel = PCPSel;
	dportTbl.BF.type_sel = EthertypeSel;

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, EGRESS_PORT_CONFIGURATION, macport, INDEX_ZERO, dportTbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set egress sport %d vlan mode %08x-%08x sucessed.\n", dportidx,
			   dportTbl.val[0], dportTbl.val[1]);

	return ret;
}


jl_ret_t jl61xx_vlan_egressport_default_set(
	jl_device_t        *pdevice,
	const jl_uint32     dportidx,
	const jl_uint32     default_VID,
	const jl_uint32     default_CFI,
	const jl_uint32     default_PCP)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   macport;
	SWCORE_EGRESS_PORT_CONFIGURATION_t dportTbl;

	/*para check*/
	JL_CHECK_PORT(pdevice, dportidx);

	/*read from driver*/
	JL_PORT_L2C_MACRO(pdevice, dportidx, macport);
	REG_BURST_READ(pdevice, SWCORE, EGRESS_PORT_CONFIGURATION, macport, INDEX_ZERO, dportTbl.val);
	dportTbl.BF.vid     = default_VID;
	dportTbl.BF.cfi_dei = default_CFI;
	dportTbl.BF.pcp     = default_PCP;

	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, EGRESS_PORT_CONFIGURATION, macport, INDEX_ZERO, dportTbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set egress sport default vlan value %08x-%08x sucessed.\n",
			   dportTbl.val[0], dportTbl.val[1]);

	return ret;
}


jl_ret_t jl61xx_vlan_preprocess_mac_set(
	jl_device_t                *pdevice,
	const jl_uint8              idx,
	jl_vlan_macbase_tbl_t      *pcfg
)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   macpmsk;

	SWCORE_INGRESS_VID_MAC_RANGE_ASSIGNMENT_ANSWER_t ans;
	SWCORE_INGRESS_VID_MAC_RANGE_SEARCH_DATA_t data;

	/*para check*/
	JL_CHECK_PORTMASK(pdevice, pcfg->portmsk);
	JL_CHECK_ENUM(idx, SWCORE_INGRESS_VID_MAC_RANGE_ASSIGNMENT_ANSWER_NUM);
	JL_CHECK_ENUM(pcfg->order, VLAN_INGRESS_TBL_MAX_ORDER_VAL);

	memset((void *)&ans, 0, sizeof(SWCORE_INGRESS_VID_MAC_RANGE_ASSIGNMENT_ANSWER_t));
	memset((void *)&data, 0, sizeof(SWCORE_INGRESS_VID_MAC_RANGE_SEARCH_DATA_t));


	ans.BF.ingress_vid  = pcfg->vid;
	ans.BF.order        = pcfg->order;

	JL_PORT_LMAP_TO_CMAP(pdevice, pcfg->portmsk, macpmsk);
	data.BF.ports       = macpmsk;
	data.BF.sa_or_da    = pcfg->mode;

	data.BF.start_0_19  = pcfg->mac_range_start.addr[5] +
						  BITS_OFFSET_L(pcfg->mac_range_start.addr[4], 8, 8) +
						  BITS_OFFSET_L(pcfg->mac_range_start.addr[3], 16, 4);

	data.BF.start_20_47 = BITS_OFFSET_R(pcfg->mac_range_start.addr[3], 4, 4) +
						  BITS_OFFSET_L(pcfg->mac_range_start.addr[2], 4, 8) +
						  BITS_OFFSET_L(pcfg->mac_range_start.addr[1], 12, 8) +
						  BITS_OFFSET_L(pcfg->mac_range_start.addr[0], 20, 8);

	data.BF.end_0_3     = BITS_OFFSET_R(pcfg->mac_range_end.addr[5], 0, 4);

	data.BF.end_4_35    = BITS_OFFSET_R(pcfg->mac_range_end.addr[5], 4, 4) +
						  BITS_OFFSET_L(pcfg->mac_range_end.addr[4], 4, 8) +
						  BITS_OFFSET_L(pcfg->mac_range_end.addr[3], 12, 8) +
						  BITS_OFFSET_L(pcfg->mac_range_end.addr[2], 20, 8) +
						  BITS_OFFSET_L(pcfg->mac_range_end.addr[1], 28, 4);

	data.BF.end_36_47   = BITS_OFFSET_R(pcfg->mac_range_end.addr[1], 4, 4) +
						  BITS_OFFSET_L(pcfg->mac_range_end.addr[0], 4, 8);


	/*send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_VID_MAC_RANGE_ASSIGNMENT_ANSWER, idx, INDEX_ZERO, ans.val);
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_VID_MAC_RANGE_SEARCH_DATA, idx, INDEX_ZERO, data.val);


	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set preprocess mac table value: portmsk:%08x, mode:%d, idx:%d, vid:%d, order:%d\n",
			   pcfg->portmsk, pcfg->mode, idx, pcfg->vid, pcfg->order);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "MAC start is: %02x-%02x-%02x-%02x-%02x-%02x\n",
			   pcfg->mac_range_start.addr[0], pcfg->mac_range_start.addr[1], pcfg->mac_range_start.addr[2],
			   pcfg->mac_range_start.addr[3], pcfg->mac_range_start.addr[4], pcfg->mac_range_start.addr[5]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "MAC end is: %02x-%02x-%02x-%02x-%02x-%02x\n",
			   pcfg->mac_range_end.addr[0], pcfg->mac_range_end.addr[1], pcfg->mac_range_end.addr[2], pcfg->mac_range_end.addr[3],
			   pcfg->mac_range_end.addr[4], pcfg->mac_range_end.addr[5]);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table data reg is %08x-%08x-%08x-%08x\n", data.val[0], data.val[1],
			   data.val[2], data.val[3]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table ans reg is %08x\n", ans.val[0]);

	return ret;
}


jl_ret_t jl61xx_vlan_preprocess_mac_get(
	jl_device_t                *pdevice,
	const jl_uint8              idx,
	jl_vlan_macbase_tbl_t      *pcfg
)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   logicpmsk;
	SWCORE_INGRESS_VID_MAC_RANGE_ASSIGNMENT_ANSWER_t ans;
	SWCORE_INGRESS_VID_MAC_RANGE_SEARCH_DATA_t data;

	/*para check*/
	JL_CHECK_ENUM(idx, SWCORE_INGRESS_VID_MAC_RANGE_ASSIGNMENT_ANSWER_NUM);

	/*read from driver*/
	REG_BURST_READ(pdevice, SWCORE, INGRESS_VID_MAC_RANGE_ASSIGNMENT_ANSWER, idx, INDEX_ZERO, ans.val);
	REG_BURST_READ(pdevice, SWCORE, INGRESS_VID_MAC_RANGE_SEARCH_DATA, idx, INDEX_ZERO, data.val);


	pcfg->vid           = ans.BF.ingress_vid;
	pcfg->order         = ans.BF.order;

	JL_PORT_CMAP_TO_LMAP(pdevice, data.BF.ports, logicpmsk);
	pcfg->portmsk       = logicpmsk;
	pcfg->mode          = data.BF.sa_or_da;
	pcfg->mac_range_start.addr[5] = BITS_OFFSET_R(data.BF.start_0_19, 0, 8);
	pcfg->mac_range_start.addr[4] = BITS_OFFSET_R(data.BF.start_0_19, 8, 8);
	pcfg->mac_range_start.addr[3] = BITS_OFFSET_R(data.BF.start_0_19, 16, 4) + BITS_OFFSET_L(data.BF.start_20_47, 4, 4);
	pcfg->mac_range_start.addr[2] = BITS_OFFSET_R(data.BF.start_20_47, 4, 8);
	pcfg->mac_range_start.addr[1] = BITS_OFFSET_R(data.BF.start_20_47, 12, 8);
	pcfg->mac_range_start.addr[0] = BITS_OFFSET_R(data.BF.start_20_47, 20, 8);

	pcfg->mac_range_end.addr[5] = BITS_OFFSET_L(data.BF.end_4_35, 4, 4) + data.BF.end_0_3;
	pcfg->mac_range_end.addr[4] = BITS_OFFSET_R(data.BF.end_4_35, 4, 8);
	pcfg->mac_range_end.addr[3] = BITS_OFFSET_R(data.BF.end_4_35, 12, 8);
	pcfg->mac_range_end.addr[2] = BITS_OFFSET_R(data.BF.end_4_35, 20, 8);
	pcfg->mac_range_end.addr[1] = BITS_OFFSET_R(data.BF.end_4_35, 28, 4) + BITS_OFFSET_L(data.BF.end_36_47, 4, 4);
	pcfg->mac_range_end.addr[0] = BITS_OFFSET_R(data.BF.end_36_47, 4, 8);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get preprocess mac table value: portmsk:%08x, mode:%d, idx:%d, vid:%d, order:%d\n",
			   pcfg->portmsk, pcfg->mode, idx, pcfg->vid, pcfg->order);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "MAC start is: %02x-%02x-%02x-%02x-%02x-%02x\n",
			   pcfg->mac_range_start.addr[0], pcfg->mac_range_start.addr[1], pcfg->mac_range_start.addr[2],
			   pcfg->mac_range_start.addr[3], pcfg->mac_range_start.addr[4], pcfg->mac_range_start.addr[5]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "MAC end is: %02x-%02x-%02x-%02x-%02x-%02x\n",
			   pcfg->mac_range_end.addr[0], pcfg->mac_range_end.addr[1], pcfg->mac_range_end.addr[2], pcfg->mac_range_end.addr[3],
			   pcfg->mac_range_end.addr[4], pcfg->mac_range_end.addr[5]);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table data reg is %08x-%08x-%08x-%08x\n", data.val[0], data.val[1],
			   data.val[2], data.val[3]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table ans reg is %08x\n", ans.val[0]);

	return ret;
}


jl_ret_t jl61xx_vlan_preprocess_outvid_set(
	jl_device_t                *pdevice,
	const jl_uint8              idx,
	jl_vlan_outvidbase_tbl_t   *pcfg
)
{
	jl_ret_t        ret = JL_ERR_OK;
	jl_uint32   macpmsk;
	SWCORE_INGRESS_VID_OUTER_VID_RANGE_ASSIGNMENT_ANSWER_t ans;
	SWCORE_INGRESS_VID_OUTER_VID_RANGE_SEARCH_DATA_t data;

	/*para check*/
	JL_CHECK_PORTMASK(pdevice, pcfg->portmsk);
	JL_CHECK_ENUM(idx, SWCORE_INGRESS_VID_OUTER_VID_RANGE_ASSIGNMENT_ANSWER_NUM);
	JL_CHECK_ENUM(pcfg->order, VLAN_INGRESS_TBL_MAX_ORDER_VAL);

	memset((void *)&ans, 0, sizeof(SWCORE_INGRESS_VID_OUTER_VID_RANGE_ASSIGNMENT_ANSWER_t));
	memset((void *)&data, 0, sizeof(SWCORE_INGRESS_VID_OUTER_VID_RANGE_SEARCH_DATA_t));


	ans.BF.ingress_vid  = pcfg->vid;
	ans.BF.order        = pcfg->order;

	JL_PORT_LMAP_TO_CMAP(pdevice, pcfg->portmsk, macpmsk);
	data.BF.ports       = macpmsk;
	data.BF.vtype       = pcfg->mode;
	data.BF.start       = pcfg->checkvidstart;
	data.BF.end_0_7     = BITS_OFFSET_R(pcfg->checkvidend, 0, 8);
	data.BF.end_8_11    = BITS_OFFSET_R(pcfg->checkvidend, 8, 4);

	/*send to data*/
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_VID_OUTER_VID_RANGE_ASSIGNMENT_ANSWER, idx, INDEX_ZERO, ans.val);
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_VID_OUTER_VID_RANGE_SEARCH_DATA, idx, INDEX_ZERO, data.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
			   "Set preprocess outvid table value: portmsk:%08x, mode:%d, idx:%d, vid:%d, order:%d\n", pcfg->portmsk, pcfg->mode, idx,
			   pcfg->vid, pcfg->order);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Outvid start is %d, end is %d\n", pcfg->checkvidstart, pcfg->checkvidend);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table data reg is %08x-%08x\n", data.val[0], data.val[1]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table ans reg is %08x\n", ans.val[0]);

	return ret;
}


jl_ret_t jl61xx_vlan_preprocess_outvid_get(
	jl_device_t                            *pdevice,
	const jl_uint8                          idx,
	jl_vlan_outvidbase_tbl_t               *pcfg
)
{
	jl_ret_t        ret = JL_ERR_OK;
	jl_uint32   logicpmsk;
	SWCORE_INGRESS_VID_OUTER_VID_RANGE_ASSIGNMENT_ANSWER_t ans;
	SWCORE_INGRESS_VID_OUTER_VID_RANGE_SEARCH_DATA_t data;

	/*para check*/
	JL_CHECK_ENUM(idx, SWCORE_INGRESS_VID_OUTER_VID_RANGE_ASSIGNMENT_ANSWER_NUM);

	/*read from data*/
	REG_BURST_READ(pdevice, SWCORE, INGRESS_VID_OUTER_VID_RANGE_ASSIGNMENT_ANSWER, idx, INDEX_ZERO, ans.val);
	REG_BURST_READ(pdevice, SWCORE, INGRESS_VID_OUTER_VID_RANGE_SEARCH_DATA, idx, INDEX_ZERO, data.val);


	pcfg->vid           = ans.BF.ingress_vid;
	pcfg->order         = ans.BF.order;

	JL_PORT_CMAP_TO_LMAP(pdevice, data.BF.ports, logicpmsk);
	pcfg->portmsk       = logicpmsk;
	pcfg->mode          = data.BF.vtype;
	pcfg->checkvidstart = data.BF.start;
	pcfg->checkvidend   = data.BF.end_0_7 + BITS_OFFSET_L(data.BF.end_8_11, 8, 4);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
			   "Get preprocess outvid table value: portmsk:%08x, mode:%d, idx:%d, vid:%d, order:%d\n",
			   pcfg->portmsk, pcfg->mode, idx, pcfg->vid, pcfg->order);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Outvid start is %d, end is %d\n", pcfg->checkvidstart, pcfg->checkvidend);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table data reg is %08x-%08x\n", data.val[0], data.val[1]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table ans reg is %08x\n", ans.val[0]);

	return ret;
}


jl_ret_t jl61xx_vlan_preprocess_invid_set(
	jl_device_t                *pdevice,
	const jl_uint8              idx,
	jl_vlan_invidbase_tbl_t    *pcfg
)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   macpmsk;
	SWCORE_INGRESS_VID_INNER_VID_RANGE_ASSIGNMENT_ANSWER_t  ans;
	SWCORE_INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA_t        data;

	/*para check*/
	JL_CHECK_PORTMASK(pdevice, pcfg->portmsk);
	JL_CHECK_ENUM(idx, SWCORE_INGRESS_VID_INNER_VID_RANGE_ASSIGNMENT_ANSWER_NUM);
	JL_CHECK_ENUM(pcfg->order, VLAN_INGRESS_TBL_MAX_ORDER_VAL);

	memset((void *)&ans, 0, sizeof(SWCORE_INGRESS_VID_INNER_VID_RANGE_ASSIGNMENT_ANSWER_t));
	memset((void *)&data, 0, sizeof(SWCORE_INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA_t));


	ans.BF.ingress_vid  = pcfg->vid;
	ans.BF.order            = pcfg->order;

	JL_PORT_LMAP_TO_CMAP(pdevice, pcfg->portmsk, macpmsk);
	data.BF.ports       = macpmsk;
	data.BF.vtype       = pcfg->mode;
	data.BF.start       = pcfg->checkvidstart;
	data.BF.end_0_7     = BITS_OFFSET_R(pcfg->checkvidend, 0,
										SWCORE_INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA_OFFSET_END_0_7_WIDTH);
	data.BF.end_8_11    = BITS_OFFSET_R(pcfg->checkvidend,
										SWCORE_INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA_OFFSET_END_0_7_WIDTH,
										SWCORE_INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA_OFFSET_END_8_11_WIDTH);

	/*send to data*/
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_VID_INNER_VID_RANGE_ASSIGNMENT_ANSWER, idx, INDEX_ZERO, ans.val);
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA, idx, INDEX_ZERO, data.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set preprocess invid table value: portmsk:%08x, mode:%d, idx:%d, vid:%d, order:%d\n",
			   pcfg->portmsk, pcfg->mode, idx, pcfg->vid, pcfg->order);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Invid start is %d, end is %d\n", pcfg->checkvidstart, pcfg->checkvidend);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table data reg is %08x-%08x\n", data.val[0], data.val[1]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table ans reg is %08x\n", ans.val[0]);

	return ret;
}


jl_ret_t jl61xx_vlan_preprocess_invid_get(
	jl_device_t                        *pdevice,
	const jl_uint8                      idx,
	jl_vlan_invidbase_tbl_t            *pcfg
)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   logicpmsk;
	SWCORE_INGRESS_VID_INNER_VID_RANGE_ASSIGNMENT_ANSWER_t  ans;
	SWCORE_INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA_t        data;

	/*para check*/
	JL_CHECK_ENUM(idx, SWCORE_INGRESS_VID_INNER_VID_RANGE_ASSIGNMENT_ANSWER_NUM);

	/*read from data*/
	REG_BURST_READ(pdevice, SWCORE, INGRESS_VID_INNER_VID_RANGE_ASSIGNMENT_ANSWER, idx, INDEX_ZERO, ans.val);
	REG_BURST_READ(pdevice, SWCORE, INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA, idx, INDEX_ZERO, data.val);


	pcfg->vid           = ans.BF.ingress_vid;
	pcfg->order         = ans.BF.order;

	JL_PORT_CMAP_TO_LMAP(pdevice, data.BF.ports, logicpmsk);
	pcfg->portmsk       = logicpmsk;
	pcfg->mode          = data.BF.vtype;
	pcfg->checkvidstart = data.BF.start;
	pcfg->checkvidend   = data.BF.end_0_7 + BITS_OFFSET_L(data.BF.end_8_11,
						  SWCORE_INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA_OFFSET_END_0_7_WIDTH,
						  SWCORE_INGRESS_VID_INNER_VID_RANGE_SEARCH_DATA_OFFSET_END_8_11_WIDTH);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get preprocess invid table value: portmsk:%08x, mode:%d, idx:%d, vid:%d, order:%d\n",
			   pcfg->portmsk, pcfg->mode, idx, pcfg->vid, pcfg->order);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Invid start is %d, end is %d\n", pcfg->checkvidstart, pcfg->checkvidend);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table data reg is %08x-%08x\n", data.val[0], data.val[1]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table ans reg is %08x\n", ans.val[0]);

	return ret;
}


jl_ret_t jl61xx_vlan_preprocess_ethtype_set(
	jl_device_t                        *pdevice,
	const jl_uint8                      idx,
	jl_vlan_ethertypebase_tbl_t        *pcfg
)
{
	jl_ret_t        ret = JL_ERR_OK;
	jl_uint32       macpmsk;
	SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_ASSIGNMENT_ANSWER_t ans;
	SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA_t data;

	/*para check*/
	JL_CHECK_PORTMASK(pdevice, pcfg->portmsk);
	JL_CHECK_ENUM(idx, SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_ASSIGNMENT_ANSWER_NUM);
	JL_CHECK_ENUM(pcfg->order, VLAN_INGRESS_TBL_MAX_ORDER_VAL);

	memset((void *)&ans, 0, sizeof(SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_ASSIGNMENT_ANSWER_t));
	memset((void *)&data, 0, sizeof(SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA_t));

	ans.BF.ingress_vid  = pcfg->vid;
	ans.BF.order            = pcfg->order;

	JL_PORT_LMAP_TO_CMAP(pdevice, pcfg->portmsk, macpmsk);
	data.BF.ports       = macpmsk;
	data.BF.start       = pcfg->ethertype_range_start;
	data.BF.end_0_4     = BITS_OFFSET_R(pcfg->ethertype_range_end, 0,
										SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA_OFFSET_END_0_4_WIDTH);
	data.BF.end_5_15    = BITS_OFFSET_R(pcfg->ethertype_range_end,
										SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA_OFFSET_END_0_4_WIDTH,
										SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA_OFFSET_END_5_15_WIDTH);

	/*send to data*/
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_VID_ETHERNET_TYPE_RANGE_ASSIGNMENT_ANSWER, idx, INDEX_ZERO, ans.val);
	REG_BURST_WRITE(pdevice, SWCORE, INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA, idx, INDEX_ZERO, data.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set preprocess ethertype table value: portmsk:%08x, idx:%d, vid:%d, order:%d\n",
			   pcfg->portmsk, idx, pcfg->vid, pcfg->order);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Ethertype start is %04x, end is %04x\n", pcfg->ethertype_range_start,
			   pcfg->ethertype_range_end);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table data reg is %08x-%08x\n", data.val[0], data.val[1]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table ans reg is %08x\n", ans.val[0]);

	return ret;
}


jl_ret_t jl61xx_vlan_preprocess_ethtype_get(
	jl_device_t                        *pdevice,
	const jl_uint8                      idx,
	jl_vlan_ethertypebase_tbl_t        *pcfg
)
{
	jl_ret_t        ret = JL_ERR_OK;
	jl_uint32   logicpmsk;
	SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_ASSIGNMENT_ANSWER_t ans;
	SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA_t data;

	/*para check*/
	JL_CHECK_ENUM(idx, SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_ASSIGNMENT_ANSWER_NUM);


	/*read from data*/
	REG_BURST_READ(pdevice, SWCORE, INGRESS_VID_ETHERNET_TYPE_RANGE_ASSIGNMENT_ANSWER, idx, INDEX_ZERO, ans.val);
	REG_BURST_READ(pdevice, SWCORE, INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA, idx, INDEX_ZERO, data.val);


	pcfg->vid           = ans.BF.ingress_vid;
	pcfg->order         = ans.BF.order;

	JL_PORT_CMAP_TO_LMAP(pdevice, data.BF.ports, logicpmsk);
	pcfg->portmsk       = logicpmsk;
	pcfg->ethertype_range_start = data.BF.start;
	pcfg->ethertype_range_end = BITS_OFFSET_L(data.BF.end_5_15,
								SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA_OFFSET_END_0_4_WIDTH,
								SWCORE_INGRESS_VID_ETHERNET_TYPE_RANGE_SEARCH_DATA_OFFSET_END_5_15_WIDTH) +
								data.BF.end_0_4;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get preprocess ethertype table value: portmsk:%08x, idx:%d, vid:%d, order:%d\n",
			   pcfg->portmsk, idx, pcfg->vid, pcfg->order);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Ethertype start is %04x, end is %04x\n", pcfg->ethertype_range_start,
			   pcfg->ethertype_range_end);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table data reg is %08x-%08x\n", data.val[0], data.val[1]);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Preprocess mac table ans reg is %08x\n", ans.val[0]);

	return ret;
}

jl_ret_t jl61xx_vlan_tbl_set(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_vlan_tbl_t      *pcfg
)
{
	jl_ret_t            ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t     vlan_tbl;
	jl_ret_t            mac_portmask;

	/*para check*/
	JL_CHECK_PORTMASK(pdevice, pcfg->portmsk);
	/*para check*/
	JL_CHECK_ENUM(pcfg->fid, VLAN_TBL_FID_MAX_USER_VAL);

	JL_CHECK_ENUM(pcfg->mmporder, VLAN_TBL_MMP_MAX_ORDER_VAL);
	JL_CHECK_ENUM(pcfg->mmpptr, VLAN_TBL_MMP_MAX_PTR_VAL);
	JL_CHECK_ENUM(pcfg->msptptr, VLAN_TBL_MSPT_MAX_PTR_VAL);

	if (pcfg->mmpvalid)
		_jl61xx_vlan_mmp_set(pdevice, vid, pcfg->mmpvalid, pcfg->mmpptr, pcfg->mmporder);

	if (pcfg->msptptr)
		_jl61xx_vlan_mstp_set(pdevice, vid, pcfg->msptptr);

	/*read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
			   "Get VLAN table info: portmsk:%08x, fid:%d, vlanopt:%d, pcpsel:%d, pcp:%d, cfisel:%d, cfi:%d, vidsel:%d, vid:%d, typesel:%d\n",
			   pcfg->portmsk, pcfg->fid, pcfg->vlanopt, pcfg->pcpsel, pcfg->pcp, pcfg->cfisel, pcfg->cfi, pcfg->vidsel, pcfg->vid,
			   pcfg->typesel);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
			   "Get VLAN table reg: portmsk:%08x, fid:%d, vlanopt:%d, pcpsel:%d, pcp:%d, cfisel:%d, cfi:%d, vidsel:%d, vid:%d, typesel:%d\n",
			   vlan_tbl.BF.vlan_port_mask, vlan_tbl.BF.gid, vlan_tbl.BF.vlan_single_op, vlan_tbl.BF.pcp_sel,
			   vlan_tbl.BF.pcp, vlan_tbl.BF.cfi_dei_sel, vlan_tbl.BF.cfi_dei, vlan_tbl.BF.vid_sel, vlan_tbl.BF.vid,
			   vlan_tbl.BF.type_sel);

	JL_PORT_LMAP_TO_CMAP(pdevice, pcfg->portmsk, mac_portmask);
	vlan_tbl.BF.vlan_port_mask = mac_portmask;
	vlan_tbl.BF.gid = pcfg->fid;

	vlan_tbl.BF.vlan_single_op = pcfg->vlanopt;

	vlan_tbl.BF.pcp_sel = pcfg->pcpsel;
	vlan_tbl.BF.pcp = pcfg->pcp;


	vlan_tbl.BF.cfi_dei_sel = pcfg->cfisel;
	vlan_tbl.BF.cfi_dei = pcfg->cfi;


	vlan_tbl.BF.vid_sel = pcfg->vidsel;
	vlan_tbl.BF.vid = pcfg->vid;


	vlan_tbl.BF.type_sel = pcfg->typesel;

	/*Send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
			   "Set VLAN table info: portmsk:%08x, fid:%d, vlanopt:%d, pcpsel:%d, pcp:%d, cfisel:%d, cfi:%d, vidsel:%d, vid:%d, typesel:%d\n",
			   pcfg->portmsk, pcfg->fid, pcfg->vlanopt, pcfg->pcpsel, pcfg->pcp, pcfg->cfisel, pcfg->cfi, pcfg->vidsel, pcfg->vid,
			   pcfg->typesel);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "VLAN reg hex val is %08x-%08x\n", vlan_tbl.val[0], vlan_tbl.val[1]);

	if (pcfg->mmpvalid)
		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "mmp setting: mmpptr:%d, mmporder:%d\n", pcfg->mmpptr, pcfg->mmporder);

	if (pcfg->msptptr)
		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "mspt setting: mmpptr:%d\n", pcfg->msptptr);

	return ret;
}


jl_ret_t jl61xx_vlan_tbl_get(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_vlan_tbl_t      *pcfg
)
{
	jl_ret_t            ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t     vlan_tbl;

	/*Read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_PORT_CMAP_TO_LMAP(pdevice, vlan_tbl.BF.vlan_port_mask, pcfg->portmsk);
	pcfg->fid           = vlan_tbl.BF.gid;

	pcfg->mmpvalid      = vlan_tbl.BF.mmp_valid;
	pcfg->mmpptr        = vlan_tbl.BF.mmp_ptr;
	pcfg->mmporder      = vlan_tbl.BF.mmp_order;

	pcfg->msptptr       = vlan_tbl.BF.mspt_ptr_0_1 + BITS_OFFSET_L(vlan_tbl.BF.mspt_ptr_2_3,
						  SWCORE_VLAN_TABLE_OFFSET_MSPT_PTR_0_1_WIDTH, SWCORE_VLAN_TABLE_OFFSET_MSPT_PTR_2_3_WIDTH);

	pcfg->vlanopt       = vlan_tbl.BF.vlan_single_op;

	pcfg->pcpsel        = vlan_tbl.BF.pcp_sel;
	pcfg->pcp           = vlan_tbl.BF.pcp;

	pcfg->cfisel        = vlan_tbl.BF.cfi_dei_sel;
	pcfg->cfi           = vlan_tbl.BF.cfi_dei;

	pcfg->vidsel        = vlan_tbl.BF.vid_sel;
	pcfg->vid           = vlan_tbl.BF.vid;
	pcfg->typesel       = vlan_tbl.BF.type_sel;


	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "VLAN %d table info:\n", vid);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "port msk %08x, fid:%d\n", pcfg->portmsk, pcfg->fid);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "MMP setting: valid:%d, ptr:%d, order:%d\n", pcfg->mmpvalid, pcfg->mmpptr,
			   pcfg->mmporder);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Mspt setting: ptr:%d\n", pcfg->msptptr);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO,
			   "VLAN relate info: vlanopt:%d, pcpsel:%d, pcp:%d, cfgsel:%d, cfi:%d, vidsel:%d, vid:%d, typesel:%d\n",
			   pcfg->vlanopt, pcfg->pcpsel, pcfg->pcp, pcfg->cfisel, pcfg->cfi, pcfg->vidsel, pcfg->vid, pcfg->typesel);

	return ret;
}


jl_ret_t jl61xx_vlan_tbl_del(
	jl_device_t        *pdevice,
	const jl_uint32     vid
)
{
	jl_ret_t        ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t vlan_tbl;

	memset((void *)&vlan_tbl, 0, sizeof(SWCORE_VLAN_TABLE_t));
	vlan_tbl.BF.vlan_port_mask = VLAN_TBL_DEFAULT_PMSK;

	/*Send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Delete VLAN %d tbl sucessed.\n", vid);
	return ret;
}


jl_ret_t jl61xx_vlan_tbl_set_fid(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	const jl_uint32     fid
)
{
	jl_ret_t        ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t vlan_tbl;

	/*para check*/
	JL_CHECK_ENUM(fid, VLAN_TBL_FID_MAX_USER_VAL);

	/*Read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	vlan_tbl.BF.gid = fid;

	/*Send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set VLAN %d tbl FID %d sucessed.\n", vid, fid);
	return ret;
}


jl_ret_t jl61xx_vlan_tbl_get_fid(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_uint32          *pfid
)
{
	jl_ret_t    ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t vlan_tbl;

	/*Read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);
	*pfid = vlan_tbl.BF.gid;
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get VLAN %d tbl FID %d sucessed.\n", vid, *pfid);
	return ret;
}

jl_ret_t jl61xx_vlan_tbl_set_memberPortmsk(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	const jl_uint32     portmsk
)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32       macpmsk;

	SWCORE_VLAN_TABLE_t vlan_tbl;

	/*para check*/
	JL_CHECK_PORTMASK(pdevice, portmsk);

	/*Read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_PORT_LMAP_TO_CMAP(pdevice, portmsk, macpmsk);
	vlan_tbl.BF.vlan_port_mask = macpmsk;
	/*Send to driver*/
	REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set VLAN %d tbl portmsk %x sucessed.\n", vid, portmsk);
	return ret;
}

jl_ret_t jl61xx_vlan_tbl_get_memberPortmsk(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_uint32           *pportmsk
)
{
	jl_ret_t    ret = JL_ERR_OK;
	SWCORE_VLAN_TABLE_t vlan_tbl;

	/*Read from driver*/
	REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, vid, INDEX_ZERO, vlan_tbl.val);

	JL_PORT_CMAP_TO_LMAP(pdevice, vlan_tbl.BF.vlan_port_mask, *pportmsk);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get VLAN %d tbl portmsk %x sucessed.\n", vid, *pportmsk);
	return ret;
}
jl_ret_t jl61xx_vlan_set_operation_selection(
	jl_device_t        *pdevice,
	const jl_uint32                 idx,
	const jl_vlan_sel_operation_t   selmode,
	const jl_vlan_operation_t       value
)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   macport;

	SWCORE_SOURCE_PORT_TABLE_t              sportTbl;
	SWCORE_VLAN_TABLE_t                     vlanTbl;
	SWCORE_EGRESS_PORT_CONFIGURATION_t      dportTbl;

	switch (selmode) {
	case JL_VLAN_SEL_OPERATION_INGRESS: {
		JL_CHECK_PORT(pdevice, idx);
		JL_PORT_L2C_MACRO(pdevice, idx, macport);
		/*Read from driver*/
		REG_BURST_READ(pdevice, SWCORE, SOURCE_PORT_TABLE, macport, INDEX_ZERO, sportTbl.val);

		sportTbl.BF.vlan_single_op = value;
		REG_BURST_WRITE(pdevice, SWCORE, SOURCE_PORT_TABLE, macport, INDEX_ZERO, sportTbl.val);
		break;
	}
	case JL_VLAN_SEL_OPERATION_VLANTBL: {
		/*Read from driver*/
		REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, idx, INDEX_ZERO, vlanTbl.val);

		vlanTbl.BF.vlan_single_op = value;
		REG_BURST_WRITE(pdevice, SWCORE, VLAN_TABLE, idx, INDEX_ZERO, vlanTbl.val);
		break;
	}
	case JL_VLAN_SEL_OPERATION_EGRESS: {
		JL_CHECK_PORT(pdevice, idx);
		JL_PORT_L2C_MACRO(pdevice, idx, macport);
		/*Read from driver*/
		REG_BURST_READ(pdevice, SWCORE, EGRESS_PORT_CONFIGURATION, macport, INDEX_ZERO, dportTbl.val);

		dportTbl.BF.vlan_single_op = value;
		REG_BURST_WRITE(pdevice, SWCORE, EGRESS_PORT_CONFIGURATION, macport, INDEX_ZERO, dportTbl.val);
		break;
	}
	default:
		return JL_ERR_PARAM;
	}
	return ret;
}

jl_ret_t jl61xx_vlan_get_operation_selection(
	jl_device_t        *pdevice,
	const jl_uint32                 idx,
	const jl_vlan_sel_operation_t   selmode,
	jl_vlan_operation_t             *pvalue
)
{
	jl_ret_t    ret = JL_ERR_OK;
	jl_uint32   macport;

	SWCORE_SOURCE_PORT_TABLE_t              sportTbl;
	SWCORE_VLAN_TABLE_t                     vlanTbl;
	SWCORE_EGRESS_PORT_CONFIGURATION_t      dportTbl;

	switch (selmode) {
	case JL_VLAN_SEL_OPERATION_INGRESS: {
		JL_CHECK_PORT(pdevice, idx);
		JL_PORT_L2C_MACRO(pdevice, idx, macport);
		/*Read from driver*/
		REG_BURST_READ(pdevice, SWCORE, SOURCE_PORT_TABLE, macport, INDEX_ZERO, sportTbl.val);

		*pvalue = sportTbl.BF.vlan_single_op;
		break;
	}
	case JL_VLAN_SEL_OPERATION_VLANTBL: {
		/*Read from driver*/
		REG_BURST_READ(pdevice, SWCORE, VLAN_TABLE, idx, INDEX_ZERO, vlanTbl.val);

		*pvalue = vlanTbl.BF.vlan_single_op;
		break;
	}
	case JL_VLAN_SEL_OPERATION_EGRESS: {
		JL_CHECK_PORT(pdevice, idx);
		JL_PORT_L2C_MACRO(pdevice, idx, macport);
		/*Read from driver*/
		REG_BURST_READ(pdevice, SWCORE, EGRESS_PORT_CONFIGURATION, macport, INDEX_ZERO, dportTbl.val);

		*pvalue = dportTbl.BF.vlan_single_op;
		break;
	}
	default:
		return JL_ERR_PARAM;
	}

	return ret;
}
jl_ret_t jl61xx_vlan_set_tpid_val(
	jl_device_t        *pdevice,
	const jl_uint32    val
)
{
	jl_ret_t    ret = JL_ERR_OK;
	SWCORE_EGRESS_ETHERNET_TYPE_FOR_VLAN_TAG_t    tblval;

	/*Read from driver*/
	REG_BURST_READ(pdevice, SWCORE, EGRESS_ETHERNET_TYPE_FOR_VLAN_TAG, INDEX_ZERO, INDEX_ZERO, tblval.val);

	tblval.BF.type_value = BITS_OFFSET_R(val,
		SWCORE_EGRESS_ETHERNET_TYPE_FOR_VLAN_TAG_OFFSET_TYPE_VALUE,
		SWCORE_EGRESS_ETHERNET_TYPE_FOR_VLAN_TAG_OFFSET_TYPE_VALUE_WIDTH);

	REG_BURST_WRITE(pdevice, SWCORE, EGRESS_ETHERNET_TYPE_FOR_VLAN_TAG, INDEX_ZERO, INDEX_ZERO, tblval.val);
	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Set VLAN tpid value %x sucessed.\n", tblval.BF.type_value);
	return ret;
}

jl_ret_t jl61xx_vlan_get_tpid_val(
	jl_device_t       *pdevice,
	jl_uint32         *pval
)
{
	jl_ret_t    ret = JL_ERR_OK;
	SWCORE_EGRESS_ETHERNET_TYPE_FOR_VLAN_TAG_t    tblval;

	/*Read from driver*/
	REG_BURST_READ(pdevice, SWCORE, EGRESS_ETHERNET_TYPE_FOR_VLAN_TAG, INDEX_ZERO, INDEX_ZERO, tblval.val);

	*pval = tblval.BF.type_value;

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "Get VLAN tpid value %x sucessed.\n", *pval);
	return ret;
}


