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

#include "jl_vlan.h"
#include "jl_ops.h"

jl_api_ret_t jl_vlan_init(
	const jl_uint32     chip_id
)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, vlan_init, chip_id);
	return ret;
}
jl_api_ret_t jl_vlan_deinit(
	const jl_uint32     chip_id
)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, vlan_deinit, chip_id);
	return ret;
}

jl_api_ret_t jl_vlan_ingressport_mode_set(
	const jl_uint32                 chip_id,
	const jl_uint32                 sportidx,
	const jl_vlan_operation_t       opt,
	const jl_vlan_val_sel_t         vidsel,
	const jl_vlan_val_sel_t         cfisel,
	const jl_vlan_val_sel_t         pcpsel,
	const jl_vlan_tpid_sel_t        ethertype,
	const jl_sport_vlan_assign_t    mode
)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(opt, JL_VLAN_OPERATION_END);
	JL_CHECK_ENUM(vidsel, JL_VLAN_VAL_SEL_END);
	JL_CHECK_ENUM(cfisel, JL_VLAN_VAL_SEL_END);
	JL_CHECK_ENUM(pcpsel, JL_VLAN_VAL_SEL_END);
	JL_CHECK_ENUM(ethertype, JL_VLAN_TPID_SEL_END);
	JL_CHECK_ENUM(mode, JL_SPORT_VLAN_ASSIGN_END);
	JL_DRV_SAFE_CALL(ret, vlan_ingressport_mode_set, chip_id, sportidx, opt, vidsel, cfisel, pcpsel, ethertype,
							mode);
	return ret;
}


jl_api_ret_t jl_vlan_ingressport_default_set(
	const jl_uint32         chip_id,
	const jl_uint32         sportidx,
	const jl_uint32         default_vid,
	const jl_uint32         default_cfi,
	const jl_uint32         default_pcp,
	const jl_uint8          default_order
)
{
	jl_api_ret_t ret;

	JL_CHECK_MIN_MAX_RANGE(default_vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);
	JL_CHECK_ENUM(default_cfi, (VLAN_MAX_CFI_DEI_VAL + 1));
	JL_CHECK_ENUM(default_pcp, (VLAN_MAX_PCP_VAL + 1));

	JL_DRV_SAFE_CALL(ret, vlan_ingressport_default_set, chip_id, sportidx, default_vid, default_cfi, default_pcp,
							default_order);
	return ret;
}

jl_api_ret_t jl_vlan_egressport_mode_set(
	const jl_uint32                 chip_id,
	const jl_uint32                 dportidx,
	const jl_vlan_operation_t       opt,
	const jl_vlan_val_sel_t         vidsel,
	const jl_vlan_val_sel_t         cfisel,
	const jl_vlan_val_sel_t         pcpsel,
	const jl_vlan_tpid_sel_t        ethertype
)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(opt, JL_VLAN_OPERATION_END);
	JL_CHECK_ENUM(vidsel, JL_VLAN_VAL_SEL_END);
	JL_CHECK_ENUM(cfisel, JL_VLAN_VAL_SEL_END);
	JL_CHECK_ENUM(pcpsel, JL_VLAN_VAL_SEL_END);
	JL_CHECK_ENUM(ethertype, JL_VLAN_TPID_SEL_END);

	JL_DRV_SAFE_CALL(ret, vlan_egressport_mode_set, chip_id, dportidx, opt, vidsel, cfisel, pcpsel, ethertype);
	return ret;

}

jl_api_ret_t jl_vlan_egressport_default_set(
	const jl_uint32         chip_id,
	const jl_uint32         dportidx,
	const jl_uint32         default_vid,
	const jl_uint32         default_cfi,
	const jl_uint32         default_pcp
)
{
	jl_api_ret_t ret;

	JL_CHECK_MIN_MAX_RANGE(default_vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);
	JL_CHECK_ENUM(default_cfi, (VLAN_MAX_CFI_DEI_VAL + 1));
	JL_CHECK_ENUM(default_pcp, (VLAN_MAX_PCP_VAL + 1));

	JL_DRV_SAFE_CALL(ret, vlan_egressport_default_set, chip_id, dportidx, default_vid, default_cfi, default_pcp);
	return ret;
}

jl_api_ret_t jl_vlan_preprocess_mac_set(
	const jl_uint32         chip_id,
	const jl_uint8          idx,
	jl_vlan_macbase_tbl_t  *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_ENUM(pcfg->vid, VLAN_MAX_VID_VALUE);
	JL_DRV_SAFE_CALL(ret, vlan_preprocess_mac_set, chip_id, idx, pcfg);
	return ret;
}

jl_api_ret_t jl_vlan_preprocess_mac_get(
	const jl_uint32         chip_id,
	const jl_uint8          idx,
	jl_vlan_macbase_tbl_t  *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_DRV_SAFE_CALL(ret, vlan_preprocess_mac_get, chip_id, idx, pcfg);
	return ret;
}


jl_api_ret_t jl_vlan_preprocess_outvid_set(
	const jl_uint32             chip_id,
	const jl_uint8              idx,
	jl_vlan_outvidbase_tbl_t   *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_ENUM(pcfg->vid, VLAN_MAX_VID_VALUE);
	JL_CHECK_ENUM(pcfg->checkvidstart, VLAN_MAX_VID_VALUE);
	JL_CHECK_ENUM(pcfg->checkvidend, VLAN_MAX_VID_VALUE);
	JL_DRV_SAFE_CALL(ret, vlan_preprocess_outvid_set, chip_id, idx, pcfg);
	return ret;
}

jl_api_ret_t jl_vlan_preprocess_outvid_get(
	const jl_uint32             chip_id,
	const jl_uint8              idx,
	jl_vlan_outvidbase_tbl_t   *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_DRV_SAFE_CALL(ret, vlan_preprocess_outvid_get, chip_id, idx, pcfg);
	return ret;
}

jl_api_ret_t jl_vlan_preprocess_invid_set(
	const jl_uint32             chip_id,
	const jl_uint8              idx,
	jl_vlan_invidbase_tbl_t    *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_ENUM(pcfg->vid, VLAN_MAX_VID_VALUE);
	JL_CHECK_ENUM(pcfg->checkvidstart, VLAN_MAX_VID_VALUE);
	JL_CHECK_ENUM(pcfg->checkvidend, VLAN_MAX_VID_VALUE);
	JL_DRV_SAFE_CALL(ret, vlan_preprocess_invid_set, chip_id, idx, pcfg);
	return ret;
}

jl_api_ret_t jl_vlan_preprocess_invid_get(
	const jl_uint32             chip_id,
	const jl_uint8              idx,
	jl_vlan_invidbase_tbl_t    *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_DRV_SAFE_CALL(ret, vlan_preprocess_invid_get, chip_id, idx, pcfg);
	return ret;
}

jl_api_ret_t jl_vlan_preprocess_ethtype_set(
	const jl_uint32                 chip_id,
	const jl_uint8                  idx,
	jl_vlan_ethertypebase_tbl_t    *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_ENUM(pcfg->vid, VLAN_MAX_VID_VALUE);
	JL_DRV_SAFE_CALL(ret, vlan_preprocess_ethtype_set, chip_id, idx, pcfg);
	return ret;
}

jl_api_ret_t jl_vlan_preprocess_ethtype_get(
	const jl_uint32                 chip_id,
	const jl_uint8                  idx,
	jl_vlan_ethertypebase_tbl_t    *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_DRV_SAFE_CALL(ret, vlan_preprocess_ethtype_get, chip_id, idx, pcfg);
	return ret;
}


/*  for IVL/SVL setting
 *   When setting VLAN tbl, the FID value should be set same as VLAN ID
 *   When using SVL mode, plz change bigger VID tbl FID value to smaller VID value
 */
jl_api_ret_t jl_vlan_tbl_set(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	jl_vlan_tbl_t      *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_MIN_MAX_RANGE(vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);
	JL_CHECK_ENUM(pcfg->cfi, (VLAN_MAX_CFI_DEI_VAL + 1));
	JL_CHECK_ENUM(pcfg->pcp, (VLAN_MAX_PCP_VAL + 1));
	JL_CHECK_BOOL(pcfg->mmpvalid);
	JL_DRV_SAFE_CALL(ret, vlan_tbl_set, chip_id, vid, pcfg);
	return ret;
}

jl_api_ret_t jl_vlan_tbl_get(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	jl_vlan_tbl_t      *pcfg
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_MIN_MAX_RANGE(vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);
	JL_DRV_SAFE_CALL(ret, vlan_tbl_get, chip_id, vid, pcfg);
	return ret;
}
jl_api_ret_t jl_vlan_tbl_del(
	const jl_uint32     chip_id,
	const jl_uint32     vid
)
{
	jl_api_ret_t ret;

	JL_CHECK_MIN_MAX_RANGE(vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);
	JL_DRV_SAFE_CALL(ret, vlan_tbl_del, chip_id, vid);
	return ret;
}
jl_api_ret_t jl_vlan_tbl_set_fid(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	const jl_uint32     fid
)
{
	jl_api_ret_t ret;

	JL_CHECK_MIN_MAX_RANGE(vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);
	JL_DRV_SAFE_CALL(ret, vlan_tbl_set_fid, chip_id, vid, fid);
	return ret;

}

jl_api_ret_t jl_vlan_tbl_get_fid(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	jl_uint32          *pfid
)
{
	jl_api_ret_t ret;

	JL_CHECK_MIN_MAX_RANGE(vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);
	JL_CHECK_POINTER(pfid);
	JL_DRV_SAFE_CALL(ret, vlan_tbl_get_fid, chip_id, vid, pfid);
	return ret;
}

jl_api_ret_t jl_vlan_tbl_set_memberPortmsk(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	const jl_uint32     portmsk
)
{
	jl_api_ret_t ret;

	JL_CHECK_MIN_MAX_RANGE(vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);

	JL_DRV_SAFE_CALL(ret, vlan_tbl_set_memberPortmsk, chip_id, vid, portmsk);
	return ret;
}


jl_api_ret_t jl_vlan_tbl_get_memberPortmsk(
	const jl_uint32     chip_id,
	const jl_uint32     vid,
	jl_uint32           *pportmsk
)
{
	jl_api_ret_t ret;

	JL_CHECK_MIN_MAX_RANGE(vid, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);
	JL_CHECK_POINTER(pportmsk);
	JL_DRV_SAFE_CALL(ret, vlan_tbl_get_memberPortmsk, chip_id, vid, pportmsk);
	return ret;
}

jl_api_ret_t jl_vlan_set_operation_selection(
	const jl_uint32                 chip_id,
	const jl_uint32                 idx,
	const jl_vlan_sel_operation_t   selmode,
	const jl_vlan_operation_t       value
)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(selmode, JL_VLAN_SEL_OPERATION_END);
	JL_CHECK_ENUM(value, JL_VLAN_OPERATION_END);
	if (JL_VLAN_SEL_OPERATION_VLANTBL == selmode)
		JL_CHECK_MIN_MAX_RANGE(idx, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);

	JL_DRV_SAFE_CALL(ret, vlan_set_operation_selection, chip_id, idx, selmode, value);
	return ret;
}


jl_api_ret_t jl_vlan_get_operation_selection(
	const jl_uint32                 chip_id,
	const jl_uint32                 idx,
	const jl_vlan_sel_operation_t   selmode,
	jl_vlan_operation_t             *pvalue
)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(selmode, JL_VLAN_SEL_OPERATION_END);
	if (JL_VLAN_SEL_OPERATION_VLANTBL == selmode)
		JL_CHECK_MIN_MAX_RANGE(idx, VLAN_MIN_VID_VALUE, VLAN_MAX_VID_VALUE);

	JL_DRV_SAFE_CALL(ret, vlan_get_operation_selection, chip_id, idx, selmode, pvalue);
	return ret;
}


jl_api_ret_t jl_vlan_set_tpid_val(
	const jl_uint32     chip_id,
	const jl_uint32     val
)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(val, VLAN_MAX_TPID_VAL + 1);

	JL_DRV_SAFE_CALL(ret, vlan_set_tpid_val, chip_id, val);
	return ret;
}


jl_api_ret_t jl_vlan_get_tpid_val(
	const jl_uint32     chip_id,
	jl_uint32           *pval
)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pval);

	JL_DRV_SAFE_CALL(ret, vlan_get_tpid_val, chip_id, pval);
	return ret;
}

