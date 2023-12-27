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

#ifndef __JL_OPS_H__
#define __JL_OPS_H__

#include "jl_types.h"
#include "jl_kit.h"
#include "jl_device.h"
#include "jl_switch.h"
#include "jl_rwreg.h"
#include "jl_port.h"
#include "jl_mib.h"
#include "jl_lpd.h"
#include "jl_pti.h"
#include "jl_stp.h"
#include "jl_storm.h"
#include "jl_cpu.h"
#include "jl_fc.h"
#include "jl_l2.h"
#include "jl_acl.h"
#include "jl_led.h"
#include "jl_vlan.h"
#include "jl_trunk.h"
#include "jl_mirror.h"
#include "jl_qos.h"
#include "jl_interrupt.h"

extern struct jl_module_s g_module;

typedef struct jl_ops_s {
	/* TODO add your APIs template here */
	/* TODO for example */
	/* port */
	/* jl_ret_t port_phy_autoneg_ability_set(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability); */
	/* switch */
	jl_ret_t (*switch_info)(jl_device_t *device, jl_switch_info_t *pinfo);
	jl_ret_t (*lport_check)(jl_device_t *device, jl_port_t lport);
	jl_ret_t (*utpport_check)(jl_device_t *device, jl_port_t lport);
	jl_ret_t (*extport_check)(jl_device_t *device, jl_port_t lport);
	jl_ret_t (*cpuport_check)(jl_device_t *device, jl_port_t lport);
	jl_uint32 (*port_l2c_get)(jl_device_t *device, jl_port_t lport);
	jl_port_t (*port_c2l_get)(jl_device_t *device, jl_port_t cport);
	jl_ret_t (*lportmask_get)(jl_device_t *device, jl_portmask_t *lportmask);
	jl_ret_t (*cportmask_get)(jl_device_t *device, jl_portmask_t *cportmask);
	jl_ret_t (*portmask_l2c_get)(jl_device_t *device, jl_portmask_t lportmask, jl_portmask_t *cportmask);
	jl_ret_t (*portmask_c2l_get)(jl_device_t *device, jl_portmask_t cportmask, jl_portmask_t *lportmask);
	jl_ret_t (*show_switchport_info)(jl_device_t *device);
	/* test */

	/* Register IO */
	jl_ret_t (*reg_io_write)(jl_device_t *device, jl_uint32 addr, jl_uint32 val);
	jl_ret_t (*reg_io_read)(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval);
	jl_ret_t (*reg_io_burst_write)(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size);
	jl_ret_t (*reg_io_burst_read)(jl_device_t *device, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size);

	/* Port */
	jl_ret_t (*port_phy_autoneg_ability_set)(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability);
	jl_ret_t (*port_phy_autoneg_ability_get)(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability);
	jl_ret_t (*port_phy_force_ability_set)(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability);
	jl_ret_t (*port_phy_force_ability_get)(jl_device_t *device, jl_port_t port, jl_port_phy_ability_t *pability);
	jl_ret_t (*port_phy_status_get)(jl_device_t *device, jl_port_t port, jl_port_link_status_t *pstatus, jl_port_duplex_t *pduplex, jl_port_speed_t *pspeed);
	jl_ret_t (*port_phy_isolate_set)(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t (*port_phy_isolate_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*port_phy_enable_set)(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t (*port_phy_enable_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*port_phy_all_enable_set)(jl_device_t *device, jl_enable_t enable);
	jl_ret_t (*port_mac_force_link_set)(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability);
	jl_ret_t (*port_mac_force_link_get)(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability);
	jl_ret_t (*port_mac_force_link_ext_set)(jl_device_t *device, jl_port_t port, jl_mode_ext_t mode, jl_port_mac_ability_t *pability);
	jl_ret_t (*port_mac_force_link_ext_get)(jl_device_t *device, jl_port_t port, jl_mode_ext_t *pmode, jl_port_mac_ability_t *pability);
	jl_ret_t (*port_mac_status_get)(jl_device_t *device, jl_port_t port, jl_port_mac_ability_t *pability);
	jl_ret_t (*port_mac_loopback_set)(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t (*port_mac_loopback_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*port_fiber_ability_set)(jl_device_t *device, jl_port_t port, jl_port_fiber_ability_t *pability);
	jl_ret_t (*port_fiber_ability_get)(jl_device_t *device, jl_port_t port, jl_port_fiber_ability_t *pability);
	jl_ret_t (*port_sds_reset)(jl_device_t *device, jl_port_t port);

	/* MIB */
	jl_ret_t (*mib_port_rc_mode_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*mib_port_rc_mode_set)(jl_device_t *device, jl_port_t port, jl_uint32 enable);
	jl_ret_t (*mib_port_clear)(jl_device_t *device, jl_port_t port, jl_port_mib_type_t type);
	jl_ret_t (*mib_port_clear_all)(jl_device_t *device, jl_port_t port);
	jl_ret_t (*mib_port_get)(jl_device_t *device, jl_port_t port, jl_port_mib_type_t type, jl_mib_counter_t *pcnt);
	jl_ret_t (*mib_port_get_all)(jl_device_t *device, jl_port_t port, jl_port_mib_cnt_t *pport_cnt);
	jl_ret_t (*mib_swc_global_drop_get)(jl_device_t *device, jl_mib_swc_glb_drop_t *pport_cnt);
	jl_ret_t (*mib_swc_global_drop_clear)(jl_device_t *device);
	jl_ret_t (*mib_swc_port_drop_get)(jl_device_t *device, jl_port_t port, jl_mib_swc_port_drop_t *pport_cnt);
	jl_ret_t (*mib_swc_port_drop_clear)(jl_device_t *device, jl_port_t port);

	/* Loop Detection */
	jl_ret_t (*lpd_config_get)(jl_device_t *device, jl_lpd_config_t *lpd_config);
	jl_ret_t (*lpd_config_set)(jl_device_t *device, jl_lpd_config_t *lpd_config);
	jl_ret_t (*lpd_enable_set)(jl_device_t *device, jl_enable_t enable);
	jl_ret_t (*lpd_enable_get)(jl_device_t *device, jl_enable_t *penable);
	jl_ret_t (*lpd_port_enable_set)(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t (*lpd_port_enable_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*lpd_port_status_get)(jl_device_t *device, jl_port_t port, jl_lpd_port_status_t *pstatus);
	jl_ret_t (*lpd_status_history_get)(jl_device_t *device, jl_portmask_t *pmaskstatus);

	/* Port Isolation */
	jl_ret_t (*port_isolation_dst_set)(jl_device_t *device, jl_port_t dstport, jl_portmask_t portmask);
	jl_ret_t (*port_isolation_dst_get)(jl_device_t *device, jl_port_t dstport, jl_portmask_t *pportmask);
	jl_ret_t (*port_isolation_src_set)(jl_device_t *device, jl_port_t srcport, jl_portmask_t portmask);
	jl_ret_t (*port_isolation_src_get)(jl_device_t *device, jl_port_t srcport, jl_portmask_t *pportmask);
	jl_ret_t (*port_isolation_group_set)(jl_device_t *device, jl_pti_group_t group_id, jl_portmask_t portmask);
	jl_ret_t (*port_isolation_group_get)(jl_device_t *device, jl_pti_group_t group_id, jl_portmask_t *pportmask);
	jl_ret_t (*port_isolation_group_clear)(jl_device_t *device, jl_pti_group_t group_id);

	/* STP */
	jl_ret_t (*stp_port_ingress_state_set)(jl_device_t *device, jl_port_t port, jl_stp_state_t stp_state);
	jl_ret_t (*stp_port_ingress_state_get)(jl_device_t *device, jl_port_t port, jl_stp_state_t *pstp_state);
	jl_ret_t (*stp_port_egress_state_set)(jl_device_t *device, jl_port_t port, jl_stp_state_t stp_state);
	jl_ret_t (*stp_port_egress_state_get)(jl_device_t *device, jl_port_t port, jl_stp_state_t *pstp_state);
	jl_ret_t (*stp_ingress_state_drop_get)(jl_device_t *device, jl_stp_state_t stp_state, jl_uint32 *pcntr);
	jl_ret_t (*stp_ingress_state_drop_clear)(jl_device_t *device, jl_stp_state_t stp_state);
	jl_ret_t (*stp_port_egress_drop_get)(jl_device_t *device, jl_port_t port, jl_uint32 *pcntr);
	jl_ret_t (*stp_port_egress_drop_clear)(jl_device_t *device, jl_port_t port);
	jl_ret_t (*mstp_port_ingress_state_set)(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state);
	jl_ret_t (*mstp_port_ingress_state_get)(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state);
	jl_ret_t (*mstp_port_egress_state_set)(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state);
	jl_ret_t (*mstp_port_egress_state_get)(jl_device_t *device, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state);
	jl_ret_t (*mstp_ingress_state_drop_get)(jl_device_t *device, jl_mstp_state_t mstp_state, jl_uint32 *pcntr);
	jl_ret_t (*mstp_ingress_state_drop_clear)(jl_device_t *device, jl_mstp_state_t mstp_state);
	jl_ret_t (*mstp_port_egress_drop_get)(jl_device_t *device, jl_port_t port, jl_uint32 *pcntr);
	jl_ret_t (*mstp_port_egress_drop_clear)(jl_device_t *device, jl_port_t port);
	jl_ret_t (*mstp_vlan_msti_set)(jl_device_t *device, jl_uint16 vid, jl_mstp_msti_t msti);
	jl_ret_t (*mstp_vlan_msti_get)(jl_device_t *device, jl_uint16 vid, jl_mstp_msti_t *pmsti);

	/* Storm Control */
	jl_ret_t (*stormctrl_enable_set)(jl_device_t *pdevice, const jl_port_t egress_port, const jl_storm_type_t type, const jl_bool enable);

	jl_ret_t (*stormctrl_enable_get)(jl_device_t *pdevice, const jl_port_t egress_port, const jl_storm_type_t type, jl_bool *penable);

	jl_ret_t (*stormctrl_rate_set)(
			jl_device_t *pdevice,
			const jl_port_t egress_port,
			const jl_storm_type_t type,
			const jl_storm_rate_mode_t mode,
			const jl_uint32 rate);

	jl_ret_t (*stormctrl_rate_get)(
			jl_device_t *pdevice,
			const jl_port_t egress_port,
			const jl_storm_type_t type,
			jl_storm_rate_mode_t *pmode,
			jl_uint32 *prate);

	jl_ret_t (*stormctrl_drop_counter_get)(jl_device_t *pdevice, const jl_port_t egress_port, jl_uint32 *pcntr);

	/* CPU */
	jl_api_ret_t (*cpu_allow_bpdu_enable_set)(jl_device_t *device, jl_enable_t enable);
	jl_api_ret_t (*cpu_allow_bpdu_enable_get)(jl_device_t *device, jl_enable_t *penable);
	jl_api_ret_t (*cpu_allow_rst_bpdu_enable_set)(jl_device_t *device, jl_enable_t enable);
	jl_api_ret_t (*cpu_allow_rst_bpdu_enable_get)(jl_device_t *device, jl_enable_t *penable);
	jl_api_ret_t (*cpu_tag_enable_set)(jl_device_t *device, jl_enable_t enable);
	jl_api_ret_t (*cpu_tag_enable_get)(jl_device_t *device, jl_enable_t *penable);
	jl_api_ret_t (*cpu_allow_lldp_enable_set)(jl_device_t *device, jl_enable_t enable);
	jl_api_ret_t (*cpu_allow_lldp_enable_get)(jl_device_t *device, jl_enable_t *penable);
	jl_api_ret_t (*cpu_rma_action_set)(jl_device_t *device, jl_uint32 rma_mac_L8, jl_rma_action_t rma_action);
	jl_api_ret_t (*cpu_rma_action_get)(jl_device_t *device, jl_uint32 rma_mac_L8, jl_rma_action_t *prma_action);
	jl_api_ret_t (*cpu_cpumac_set)(jl_device_t *device, jl_mac_addr_t cpu_mac);
	jl_api_ret_t (*cpu_cpumac_get)(jl_device_t *device, jl_mac_addr_t *pcpu_mac);
	jl_api_ret_t (*cpu_detection_cpumac_enable_set)(jl_device_t *device, jl_enable_t enable);
	jl_api_ret_t (*cpu_detection_cpumac_enable_get)(jl_device_t *device, jl_enable_t *penable);

	/* Flow Control */
	jl_ret_t (*port_fc_force_mode_set)(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t (*port_fc_force_mode_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*port_fc_force_enable_set)(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t (*port_fc_force_enable_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*port_fc_threshold_set)(jl_device_t *device, jl_port_t port, jl_uint32 threshold_on, jl_uint32 threshold_off);
	jl_ret_t (*port_fc_threshold_get)(jl_device_t *device, jl_port_t port, jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off);
	jl_ret_t (*taildrop_enable_set)(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t (*taildrop_enable_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*taildrop_counter_get)(jl_device_t *device, jl_uint32 *pcntr);

	/* L2 */
	jl_ret_t (*l2_init)(jl_device_t *pdevice);
	jl_ret_t (*l2_deinit)(jl_device_t *pdevice);
	jl_ret_t (*l2_table_set)(jl_device_t *pdevice, jl_l2_tbl_t *pcfg);
	jl_ret_t (*l2_table_search)(jl_device_t *pdevice, const jl_l2_sel_mode_t mode, jl_l2_tbl_t *pcfg);
	jl_ret_t (*l2_table_clear)(jl_device_t *pdevice, const jl_l2_sel_mode_t mode, jl_l2_tbl_t *pcfg);
	jl_ret_t (*l2_dynamic_fast_clear)(jl_device_t *pdevice);
	jl_ret_t (*l2_hash_col_table_set)(jl_device_t *pdevice, const jl_uint32 idx, jl_l2_tbl_t *pcfg);
	jl_ret_t (*l2_hash_col_table_get)(jl_device_t *pdevice, const jl_uint32 idx, jl_l2_tbl_t *pcfg);
	jl_ret_t (*l2_set_agetime)(jl_device_t *pdevice, const jl_uint32 time);
	jl_ret_t (*l2_get_agetime)(jl_device_t *pdevice, jl_uint32 *ptime);
	jl_ret_t (*l2_mc_table_set)(jl_device_t *pdevice, const jl_uint32 idx, const jl_uint32 portMask);
	jl_ret_t (*l2_mc_table_get)(jl_device_t *pdevice, const jl_uint32 idx, jl_uint32 *pPortMask);
	jl_ret_t (*l2_mc_handle_set)(jl_device_t *pdevice, jl_l2_mc_handle_t mcHandle, jl_bool enable);

	/* ACL */
	jl_ret_t (*acl_template_set)(jl_device_t *device, jl_uint8 index, jl_acl_template_t *ptempl);
	jl_ret_t (*acl_template_get)(jl_device_t *device, jl_uint8 index, jl_acl_template_t *ptempl);
	jl_ret_t (*acl_rule_add)(jl_device_t *device, jl_uint8 index, jl_acl_rule_t *prule, jl_acl_action_t *paction);
	jl_ret_t (*acl_rule_get)(jl_device_t *device, jl_uint8 index, jl_acl_rule_t *prule,jl_acl_action_t *paction);
	jl_ret_t (*acl_rule_del)(jl_device_t *device, jl_uint8 index);
	jl_ret_t (*acl_field_insert)(jl_device_t *device, jl_acl_field_data_t *pfield, jl_acl_rule_t *prule);
	jl_ret_t (*acl_field_clear)(jl_device_t *device, jl_acl_rule_t *prule);
	jl_ret_t (*acl_selection_set)(jl_device_t *device, jl_uint8 val);
	jl_ret_t (*acl_selection_get)(jl_device_t *device, jl_uint8 *pval);
	jl_ret_t (*acl_stat_enable)(jl_device_t *device, jl_uint8 index, jl_uint8 counter_id);
	jl_ret_t (*acl_stat_disable)(jl_device_t *device, jl_uint8 index);
	jl_ret_t (*acl_stat_read)(jl_device_t *device, jl_uint8 counter_id, jl_uint32 *ppackets);
	jl_ret_t (*acl_stat_write)(jl_device_t *device, jl_uint8 counter_id, jl_uint32 packets);
	jl_ret_t (*acl_stat_counter_get)(jl_device_t *device, jl_uint8 *pcounter_id);
	jl_ret_t (*acl_mac_rule_add)(jl_device_t *device, jl_uint8 index,
			jl_acl_mac_range_t *pmac, jl_acl_mac_action_t *paction);
	jl_ret_t (*acl_mac_rule_get)(jl_device_t *device, jl_uint8 index, jl_acl_mac_type_t type,
			jl_acl_mac_range_t *pmac, jl_acl_mac_action_t *paction);
	jl_ret_t (*acl_mac_rule_del)(jl_device_t *device, jl_uint8 index, jl_acl_mac_type_t type);
	jl_ret_t (*acl_source_port_set)(jl_device_t *device, jl_uint8 index, jl_acl_port_t *pstate);
	jl_ret_t (*acl_source_port_get)(jl_device_t *device, jl_uint8 index, jl_acl_port_t *pstate);

	/* LED */
	jl_ret_t (*led_enable_set)(jl_device_t *device, jl_led_group_t group, jl_portmask_t portmask);
	jl_ret_t (*led_enable_get)(jl_device_t *device, jl_led_group_t group, jl_portmask_t *pportmask);
	jl_ret_t (*led_group_config_set)(jl_device_t *device, jl_led_group_t group, jl_led_config_t config);
	jl_ret_t (*led_group_config_get)(jl_device_t *device, jl_led_group_t group, jl_led_config_t *pconfig);
	jl_ret_t (*led_blink_rate_set)(jl_device_t *device, jl_led_blink_rate_t blink_rate);
	jl_ret_t (*led_blink_rate_get)(jl_device_t *device, jl_led_blink_rate_t *pblink_rate);
	jl_ret_t (*led_group_active_high_set)(jl_device_t *device, jl_led_group_t group, jl_portmask_t portmask);
	jl_ret_t (*led_group_active_high_get)(jl_device_t *device, jl_led_group_t group, jl_portmask_t *pportmask);

	/* VLAN */
	jl_ret_t (*vlan_init)(jl_device_t *pdevice);
	jl_ret_t (*vlan_deinit)(jl_device_t *pdevice);
	jl_ret_t (*vlan_ingressport_mode_set)(
			jl_device_t *pdevice,
			const jl_uint32 sportidx,
			const jl_vlan_operation_t opt,
			const jl_vlan_val_sel_t vidsel,
			const jl_vlan_val_sel_t cfisel,
			const jl_vlan_val_sel_t pcpsel,
			const jl_vlan_tpid_sel_t ethertype,
			const jl_sport_vlan_assign_t mode);
	jl_ret_t (*vlan_ingressport_default_set)(
			jl_device_t *pdevice,
			const jl_uint32 sportidx,
			const jl_uint32 default_vid,
			const jl_uint32 default_cfi,
			const jl_uint32 default_pcp,
			const jl_uint8  default_order);
	jl_ret_t (*vlan_egressport_mode_set)(
			jl_device_t *pdevice,
			const jl_uint32 dportidx,
			const jl_vlan_operation_t opt,
			const jl_vlan_val_sel_t vidsel,
			const jl_vlan_val_sel_t cfisel,
			const jl_vlan_val_sel_t pcpsel,
			const jl_vlan_tpid_sel_t ethertype);
	jl_ret_t (*vlan_egressport_default_set)(
			jl_device_t *pdevice,
			const jl_uint32 dportidx,
			const jl_uint32 default_vid,
			const jl_uint32 default_cfi,
			const jl_uint32 default_pcp);
	jl_ret_t (*vlan_preprocess_mac_set)(
			jl_device_t *pdevice,
			const jl_uint8 idx,
			jl_vlan_macbase_tbl_t *pcfg);
	jl_ret_t (*vlan_preprocess_mac_get)(
			jl_device_t *pdevice,
			const jl_uint8 idx,
			jl_vlan_macbase_tbl_t *pcfg);
	jl_ret_t (*vlan_preprocess_outvid_set)(
			jl_device_t *pdevice,
			const jl_uint8 idx,
			jl_vlan_outvidbase_tbl_t *pcfg);
	jl_ret_t (*vlan_preprocess_outvid_get)(
			jl_device_t *pdevice,
			const jl_uint8 idx,
			jl_vlan_outvidbase_tbl_t *pcfg);
	jl_ret_t (*vlan_preprocess_invid_set)(
			jl_device_t *pdevice,
			const jl_uint8 idx,
			jl_vlan_invidbase_tbl_t *pcfg);
	jl_ret_t (*vlan_preprocess_invid_get)(
			jl_device_t *pdevice,
			const jl_uint8 idx,
			jl_vlan_invidbase_tbl_t *pcfg);
	jl_ret_t (*vlan_preprocess_ethtype_set)(
			jl_device_t *pdevice,
			const jl_uint8 idx,
			jl_vlan_ethertypebase_tbl_t *pcfg);
	jl_ret_t (*vlan_preprocess_ethtype_get)(
			jl_device_t *pdevice,
			const jl_uint8 idx,
			jl_vlan_ethertypebase_tbl_t *pcfg);
	jl_ret_t (*vlan_tbl_set)(
			jl_device_t *pdevice,
			const jl_uint32 vid,
			jl_vlan_tbl_t *pcfg);
	jl_ret_t (*vlan_tbl_get)(
			jl_device_t *pdevice,
			const jl_uint32 vid,
			jl_vlan_tbl_t *pcfg);
	jl_ret_t (*vlan_tbl_del)(
			jl_device_t *pdevice,
			const jl_uint32 vid);
	jl_ret_t (*vlan_tbl_set_fid)(
			jl_device_t *pdevice,
			const jl_uint32 vid,
			const jl_uint32 fid);
	jl_ret_t (*vlan_tbl_get_fid)(
			jl_device_t *pdevice,
			const jl_uint32 vid,
			jl_uint32 *fid);
	jl_ret_t (*vlan_tbl_set_memberPortmsk)(
			jl_device_t *pdevice,
			const jl_uint32 vid,
			const jl_uint32 portmsk);
	jl_ret_t (*vlan_tbl_get_memberPortmsk)(
			jl_device_t *pdevice,
			const jl_uint32 vid,
			jl_uint32 *pportmsk);
	jl_ret_t (*vlan_set_operation_selection)(
			jl_device_t *pdevice,
			const jl_uint32 idx,
			const jl_vlan_sel_operation_t selmode,
			const jl_vlan_operation_t value);
	jl_ret_t (*vlan_get_operation_selection)(
			jl_device_t *pdevice,
			const jl_uint32 idx,
			const jl_vlan_sel_operation_t selmode,
			jl_vlan_operation_t *pvalue);
	jl_ret_t (*vlan_set_tpid_val)(
			jl_device_t         *pdevice,
			const jl_uint32     val);
	jl_ret_t (*vlan_get_tpid_val)(
			jl_device_t     *pdevice,
			jl_uint32       *pval);

	/* Mirror */
	jl_ret_t (*mirror_input_mr_get)(jl_device_t *device, jl_port_t mirrored_port, jl_port_t *pmirroring_port);
	jl_ret_t (*mirror_input_mr_set)(jl_device_t *device, jl_port_t mirrored_port, jl_port_t mirroring_port);
	jl_ret_t (*mirror_output_mr_set)(jl_device_t *device, jl_port_t mirrored_port, jl_port_t mirroring_port);
	jl_ret_t (*mirror_output_mr_get)(jl_device_t *device, jl_port_t mirrored_port, jl_port_t *pmirroring_port);
	jl_ret_t (*mirror_input_mr_clear)(jl_device_t *device, jl_port_t mirrored_port);
	jl_ret_t (*mirror_output_mr_clear)(jl_device_t *device, jl_port_t mirrored_port);

	/* Link Aggregation */
	jl_ret_t (*link_aggregation_enable_set)(jl_device_t *device, jl_enable_t enable);
	jl_ret_t (*link_aggregation_enable_get)(jl_device_t *device, jl_enable_t *penable);
	jl_ret_t (*link_aggregation_portmask_set)(jl_device_t *device, jl_la_group_t la_id, jl_portmask_t la_portmask);
	jl_ret_t (*link_aggregation_portmask_get)(jl_device_t *device, jl_la_group_t la_id, jl_portmask_t *pla_portmask);
	jl_ret_t (*link_aggregation_port_del)(jl_device_t *device, jl_la_group_t la_id, jl_port_t port_id);
	jl_ret_t (*link_aggregation_group_del)(jl_device_t *device, jl_la_group_t la_id);
	jl_ret_t (*link_aggregation_hashfactor_set)(jl_device_t *device, jl_uint32 la_hashfactor);
	jl_ret_t (*link_aggregation_hashfactor_get)(jl_device_t *device, jl_uint32 *pla_hashfactor);
	jl_ret_t (*link_aggregation_weight_set)(jl_device_t *device, jl_la_group_t la_id, jl_portmask_t la_portmask);
	jl_ret_t (*link_aggregation_weight_get)(jl_device_t *device, jl_la_hash2ports_t *pla_hash2ports);
	jl_ret_t (*link_aggregation_weight_clear)(jl_device_t *device, jl_la_group_t la_id, jl_portmask_t la_portmask);
	jl_ret_t (*link_aggregation_clear)(jl_device_t *device, jl_la_group_t la_id);

	/* QoS */
	jl_ret_t (*qos_nonvlan_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 queue);
	jl_ret_t (*qos_nonvlan_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_uint8 *pqueue);
	jl_ret_t (*qos_unknownl3_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 queue);
	jl_ret_t (*qos_unknownl3_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_uint8 *pqueue);
	jl_ret_t (*qos_mac_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_mac_map_t *pmap);
	jl_ret_t (*qos_mac_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_mac_map_t *pmap);
	jl_ret_t (*qos_vid_map2q_set)(jl_device_t *dev,
			jl_uint8 eport,
			jl_uint8 rule_idx,
			jl_qos_vid_map_t *pmap);
	jl_ret_t (*qos_vid_map2q_get)(jl_device_t *dev,
			jl_uint8 eport,
			jl_uint8 rule_idx,
			jl_qos_vid_map_t *pmap);
	jl_ret_t (*qos_ip_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_ip_map_t *pmap);
	jl_ret_t (*qos_ip_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_ip_map_t *pmap);
	jl_ret_t (*qos_ethtype_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_ethtype_map_t *pmap);
	jl_ret_t (*qos_ethtype_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_ethtype_map_t *pmap);
	jl_ret_t (*qos_tos_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint16 tos,
			const jl_uint8 queue);
	jl_ret_t (*qos_tos_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint16 tos,
			jl_uint8 *pqueue);
	jl_ret_t (*qos_exp_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 exp,
			const jl_uint8 queue);
	jl_ret_t (*qos_exp_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 exp,
			jl_uint8 *pqueue);
	jl_ret_t (*qos_pcp_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 pcp,
			const jl_uint8 queue);
	jl_ret_t (*qos_pcp_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 pcp,
			jl_uint8 *pqueue);
	jl_ret_t (*qos_l4port_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_l4_port_range_map_t *pmap);
	jl_ret_t (*qos_l4port_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_l4_port_range_map_t *pmap);
	jl_ret_t (*qos_l4protocol_map2q_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_l4_protocol_map_t *pmap);
	jl_ret_t (*qos_l4protocol_map2q_get)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 rule_idx,
			jl_qos_l4_protocol_map_t *pmap);
	jl_ret_t (*qos_nonvlan_map2color_set)(jl_device_t *device,
			const jl_uint8 color,
			const jl_uint8 enable);
	jl_ret_t (*qos_nonvlan_map2color_get)(jl_device_t *device,
			jl_uint8 *pcolor,
			jl_uint8 *penable);
	jl_ret_t (*qos_unknownl3_map2color_set)(jl_device_t *device,
			const jl_uint8 color,
			const jl_uint8 enable);
	jl_ret_t (*qos_unknownl3_map2color_get)(jl_device_t *device,
			jl_uint8 *pcolor,
			jl_uint8 *penable);
	jl_ret_t (*qos_tos_map2color_set)(jl_device_t *device,
			const jl_uint8 tos,
			const jl_uint8 color);
	jl_ret_t (*qos_tos_map2color_get)(jl_device_t *device,
			const jl_uint8 tos,
			jl_uint8 *pcolor);
	jl_ret_t (*qos_exp_map2color_set)(jl_device_t *device,
			const jl_uint8 exp,
			const jl_uint8 color);
	jl_ret_t (*qos_exp_map2color_get)(jl_device_t *device,
			const jl_uint8 exp,
			jl_uint8 *pcolor);
	jl_ret_t (*qos_pcpdei_map2color_set)(jl_device_t *device,
			const jl_uint8 pcp,
			const jl_uint8 dei,
			const jl_uint8 color);
	jl_ret_t (*qos_pcpdei_map2color_get)(jl_device_t *device,
			const jl_uint8 pcp,
			const jl_uint8 dei,
			jl_uint8 *pcolor);
	jl_ret_t (*qos_remap_vlan_set)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_remap_vlan_t *pcfg);
	jl_ret_t (*qos_remap_vlan_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_remap_vlan_t *pcfg);
	jl_ret_t (*qos_color_remap_set)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_color_remap_t *pcfg);
	jl_ret_t (*qos_color_remap_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_color_remap_t *pcfg);
	jl_ret_t (*qos_mmp_cfg_set)(jl_device_t *device,
			const jl_uint8 pointer,
			jl_qos_mmp_cfg_t *pcfg);
	jl_ret_t (*qos_mmp_cfg_get)(jl_device_t *device,
			const jl_uint8 pointer,
			jl_qos_mmp_cfg_t *pcfg);
	jl_ret_t (*qos_mmp_unused_pointer_get)(jl_device_t *device,
			jl_uint8 *ppointer);
	jl_ret_t (*qos_mmp_used_pointer_info_get)(jl_device_t *device,
			const jl_uint8 pointer,
			jl_qos_mmp_used_info_t *ppointer_info);
	jl_ret_t (*qos_ingress_initial_mmp_set)(jl_device_t *device,
			const jl_uint8 inport,
			const jl_uint8 pcp,
			jl_qos_mmp_pointer_t *ppointer);
	jl_ret_t (*qos_ingress_initial_mmp_get)(jl_device_t *device,
			const jl_uint8 inport,
			const jl_uint8 pcp,
			jl_qos_mmp_pointer_t *ppointer);
	jl_ret_t (*qos_queue_map_priority_set)(jl_device_t *device,
			const jl_uint8 eport,
			const jl_uint8 enable);
	jl_ret_t (*qos_queue_map_priority_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_uint8 *penable);
	jl_ret_t (*qos_color_map_priority_set)(jl_device_t *device,
			const jl_uint8 inport,
			const jl_uint8 enable);
	jl_ret_t (*qos_color_map_priority_get)(jl_device_t *device,
			const jl_uint8 inport,
			jl_uint8 *penable);
	jl_ret_t (*qos_priority_to_queue_set)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_queue_priority_t *pqueue_pri);
	jl_ret_t (*qos_priority_to_queue_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_queue_priority_t *pqueue_pri);
	jl_ret_t (*qos_queue_shaper_set)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_queue_shaper_t *pshaper);
	jl_ret_t (*qos_queue_shaper_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_queue_shaper_t *pshaper);
	jl_ret_t (*qos_priority_shaper_set)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_priority_shaper_t *pshaper);
	jl_ret_t (*qos_priority_shaper_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_priority_shaper_t *pshaper);
	jl_ret_t (*qos_port_shaper_set)(jl_device_t *dev,
			jl_uint8 port,
			jl_qos_port_shaper_t *pshaper);
	jl_ret_t (*qos_port_shaper_get)(jl_device_t *dev,
			jl_uint8 port,
			jl_qos_port_shaper_t *pshaper);
	jl_ret_t (*qos_queue_dwrr_set)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_queue_dwrr_t *pdwrr);
	jl_ret_t (*qos_queue_dwrr_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_queue_dwrr_t *pdwrr);
	jl_ret_t (*qos_enqueue_enable_set)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_enqueue_state_t *penqueue);
	jl_ret_t (*qos_enqueue_enable_get)(jl_device_t *device,
			const jl_uint8 eport,
			jl_qos_enqueue_state_t *penqueue);
	jl_ret_t (*qos_schedule_set)(jl_device_t *dev,
			const jl_uint8 eport,
			jl_qos_schedule_t *psche);
	jl_ret_t (*qos_schedule_get)(jl_device_t *dev,
			const jl_uint8 eport,
			jl_qos_schedule_t *psche);
	jl_ret_t (*qos_res_init)(jl_device_t *dev, jl_uint8 queue_num);
	jl_ret_t (*qos_res_deinit)(jl_device_t *dev);
	/*interrupt */
	jl_ret_t (*interrupt_polarity_set)(jl_device_t *device, jl_int_polarity_t polarity);
	jl_ret_t (*interrupt_polarity_get)(jl_device_t *device, jl_int_polarity_t *ppolarity);
	jl_ret_t (*interrupt_port_phy_int_enable_set)(jl_device_t *device, jl_port_t port, jl_enable_t enable);
	jl_ret_t (*interrupt_port_phy_int_enable_get)(jl_device_t *device, jl_port_t port, jl_enable_t *penable);
	jl_ret_t (*interrupt_port_phy_int_status_get)(jl_device_t *device, jl_port_t port, jl_uint32 *pstatus);
	jl_ret_t (*interrupt_enable_set)(jl_device_t *device, jl_int_type_t type, jl_enable_t enable);
	jl_ret_t (*interrupt_enable_get)(jl_device_t *device, jl_int_type_t type, jl_enable_t *penable);
	jl_ret_t (*interrupt_status_clean)(jl_device_t *device, jl_int_type_t type);
	jl_ret_t (*interrupt_status_get)(jl_device_t *device, jl_int_status_t *pstatusmask);
	jl_ret_t (*interrupt_detail_get)(jl_device_t *device, jl_int_type_t type, jl_int_info_t *pinfo);
} jl_ops_t;


#define JL_DRV_SAFE_CALL(ret, func, chip_id, ...) \
	do {\
		jl_ops_t *ops = NULL;\
		JL_CHECK_STAT(g_module.stat);\
		JL_ASSERT((chip_id < JL_MAX_CHIP_NUM)\
				&& (g_module.devices[chip_id] != NULL)\
				&& (g_module.devices[chip_id]->driver != NULL)\
				&& (g_module.devices[chip_id]->driver->ops != NULL));\
		ops = (jl_ops_t *)g_module.devices[chip_id]->driver->ops;\
		if (ops->func == NULL)\
			return JL_ERR_OK;\
		JL_API_MUTEX_LOCK(g_module.devices[chip_id]->mutex);\
		ret = ops->func(g_module.devices[chip_id], ##__VA_ARGS__);\
		JL_API_MUTEX_UNLOCK(g_module.devices[chip_id]->mutex);\
	} while (0)

#define JL_DRV_PORT_SAFE_CALL(ret, func, chip_id, ...) \
	do {\
		jl_ops_t *ops = NULL;\
		if (g_module.stat != STAT_READY)\
			return UNDEF_PORT;\
		if ((chip_id >= JL_MAX_CHIP_NUM)\
				|| (g_module.devices[chip_id] == NULL)\
				|| (g_module.devices[chip_id]->driver == NULL)\
				|| (g_module.devices[chip_id]->driver->ops == NULL))\
			return UNDEF_PORT;\
		ops = (jl_ops_t *)g_module.devices[chip_id]->driver->ops;\
		if (ops->func == NULL)\
			return UNDEF_PORT;\
		JL_API_MUTEX_LOCK(g_module.devices[chip_id]->mutex);\
		ret = ops->func(g_module.devices[chip_id], ##__VA_ARGS__);\
		JL_API_MUTEX_UNLOCK(g_module.devices[chip_id]->mutex);\
	} while (0)

#endif				/* __JL_OPS_H__ */
