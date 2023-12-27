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

#ifndef __JL61XX_DRV_VLAN_H__
#define __JL61XX_DRV_VLAN_H__

#include "jl_base.h"
#include "drv_common/jl_drv_common.h"
#include "jl_vlan.h"
#include "jl6107x/reg_swcore.h"

/****Macro Define****/
#define     VLAN_INGRESS_TBL_MAX_ORDER_VAL      (4)
#define     VLAN_INGRESS_TBL_MAX_ENTRY_VAL      (4)
#define     VLAN_TBL_FID_DEFAULT_VAL            (0)
#define     VLAN_TBL_FID_MAX_USER_VAL           (4095)

#define     VLAN_TBL_MMP_MAX_ORDER_VAL          (4)
#define     VLAN_TBL_DEFAULT_PMSK               (0xff)
#define     VLAN_TBL_DEFAULT_VID                (0)

#define     VLAN_TBL_MMP_MAX_PTR_VAL            (16)
#define     VLAN_TBL_MSPT_MAX_PTR_VAL           (16)

typedef struct JL_VLAN_IGS_SPEC_ETHERTYPE_s {
	jl_uint16 value;
#define JL_VLAN_IGS_SPEC_ETHERTYPE_CVLAN    0
#define JL_VLAN_IGS_SPEC_ETHERTYPE_SVLAN    1
	jl_uint8  type;
	jl_uint8  valid;
} JL_VLAN_IGS_SPEC_ETHERTYPE_t;


/****Type Define****/

jl_ret_t jl6107x_vlan_init(
	jl_device_t    *pdevice
);

jl_ret_t jl6107x_vlan_deinit(
	jl_device_t    *pdevice
);


jl_ret_t jl6107x_vlan_ingressport_mode_set(
	jl_device_t                    *pdevice,
	jl_uint32                       sportidx,
	const jl_vlan_operation_t       opt,
	const jl_vlan_val_sel_t         vidsel,
	const jl_vlan_val_sel_t         cfisel,
	const jl_vlan_val_sel_t         pcpsel,
	const jl_vlan_tpid_sel_t        ethertype,
	const jl_sport_vlan_assign_t    mode
);



jl_ret_t jl6107x_vlan_ingressport_default_set(
	jl_device_t            *pdevice,
	jl_uint32                       sportidx,
	const jl_uint32         default_vid,
	const jl_uint32         default_cfi,
	const jl_uint32         default_pcp,
	const jl_uint8          default_order

);


jl_ret_t jl6107x_vlan_egressport_mode_set(
	jl_device_t                    *pdevice,
	jl_uint32                       dportidx,
	const jl_vlan_operation_t       opt,
	const jl_vlan_val_sel_t         vidsel,
	const jl_vlan_val_sel_t         cfisel,
	const jl_vlan_val_sel_t         pcpsel,
	const jl_vlan_tpid_sel_t        ethertype

);

jl_ret_t jl6107x_vlan_egressport_default_set(
	jl_device_t            *pdevice,
	jl_uint32                       dportidx,
	const jl_uint32         default_vid,
	const jl_uint32         default_cfi,
	const jl_uint32         default_pcp

);

jl_ret_t jl6107x_vlan_preprocess_mac_set(
	jl_device_t            *pdevice,
	const jl_uint8          idx,
	jl_vlan_macbase_tbl_t  *pcfg
);


jl_ret_t jl6107x_vlan_preprocess_mac_get(
	jl_device_t            *pdevice,
	const jl_uint8          idx,
	jl_vlan_macbase_tbl_t  *pcfg
);


jl_ret_t jl6107x_vlan_preprocess_outvid_set(
	jl_device_t                *pdevice,
	const jl_uint8              idx,
	jl_vlan_outvidbase_tbl_t   *pcfg

);


jl_ret_t jl6107x_vlan_preprocess_outvid_get(
	jl_device_t                *pdevice,
	const jl_uint8              idx,
	jl_vlan_outvidbase_tbl_t   *pcfg

);


jl_ret_t jl6107x_vlan_preprocess_invid_set(
	jl_device_t                *pdevice,
	const jl_uint8              idx,
	jl_vlan_invidbase_tbl_t    *pcfg

);

jl_ret_t jl6107x_vlan_preprocess_invid_get(
	jl_device_t                *pdevice,
	const jl_uint8              idx,
	jl_vlan_invidbase_tbl_t    *pcfg

);


jl_ret_t jl6107x_vlan_preprocess_ethtype_set(
	jl_device_t                    *pdevice,
	const jl_uint8                  idx,
	jl_vlan_ethertypebase_tbl_t    *pcfg

);

jl_ret_t jl6107x_vlan_preprocess_ethtype_get(
	jl_device_t                    *pdevice,
	const jl_uint8                  idx,
	jl_vlan_ethertypebase_tbl_t    *pcfg

);

jl_ret_t jl6107x_vlan_tbl_set(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_vlan_tbl_t      *pcfg

);

jl_ret_t jl6107x_vlan_tbl_get(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_vlan_tbl_t      *pcfg

);

jl_ret_t jl6107x_vlan_tbl_del(
	jl_device_t        *pdevice,
	const jl_uint32     vid
);

jl_ret_t jl6107x_vlan_tbl_set_fid(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	const jl_uint32     fid
);

jl_ret_t jl6107x_vlan_tbl_get_fid(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	jl_uint32          *pfid
);

jl_ret_t jl6107x_vlan_tbl_set_memberPortmsk(
	jl_device_t        *pdevice,
	const jl_uint32     vid,
	const jl_uint32     portmsk
);

jl_ret_t jl6107x_vlan_tbl_get_memberPortmsk(
	jl_device_t         *pdevice,
	const jl_uint32     vid,
	jl_uint32           *pportmsk
);

jl_ret_t jl6107x_vlan_set_operation_selection(
	jl_device_t                     *pdevice,
	const jl_uint32                 idx,
	const jl_vlan_sel_operation_t   selmode,
	const jl_vlan_operation_t       value
);

jl_ret_t jl6107x_vlan_get_operation_selection(
	jl_device_t                     *pdevice,
	const jl_uint32                 idx,
	const jl_vlan_sel_operation_t   selmode,
	jl_vlan_operation_t             *pvalue
);

jl_ret_t jl6107x_vlan_set_tpid_val(
	jl_device_t        *pdevice,
	const jl_uint32    val
);

jl_ret_t jl6107x_vlan_get_tpid_val(
	jl_device_t        *pdevice,
	jl_uint32          *pval
);
#endif /* __JL61XX_DRV_VLAN_H__ */
