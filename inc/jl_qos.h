/**
 * @file jl_qos.h
 * @brief QoS
 */

/**
 * @defgroup qos_group QoS
 * @{
 */

/**
 * @example example_qos.c
 */

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

#ifndef __JL_API_QOS_H__
#define __JL_API_QOS_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define QOS_PRIORITY_NUM (8) /**< QOS_PRIORITY_NUM */
#define QOS_MMP_POINTER_NUM (16) /**< QOS_MMP_POINTER_NUM */
#define QOS_MAC_TO_QUEUE_RULE_NUM (4) /**< QOS_MAC_TO_QUEUE_RULE_NUM */
#define QOS_VID_TO_QUEUE_RULE_NUM (4) /**< QOS_VID_TO_QUEUE_RULE_NUM */
#define QOS_IP_TO_QUEUE_RULE_NUM (4) /**< QOS_IP_TO_QUEUE_RULE_NUM */
#define QOS_ETHTYPE_TO_QUEUE_RULE_NUM (4) /**< QOS_ETHTYPE_TO_QUEUE_RULE_NUM */
#define QOS_L4_TO_QUEUE_RULE_NUM (4) /**< QOS_L4_TO_QUEUE_RULE_NUM */

/**
 * @enum jl_qos_queue_e
 * @brief Qos queue type.
 */
enum jl_qos_queue_e {
	QOS_QUEUE_0 = 0, /**< 0 */
	QOS_QUEUE_1, /**< 1 */
	QOS_QUEUE_2, /**< 2 */
	QOS_QUEUE_3, /**< 3 */
	QOS_QUEUE_4, /**< 4 */
	QOS_QUEUE_5, /**< 5 */
	QOS_QUEUE_6, /**< 6 */
	QOS_QUEUE_7, /**< 7 */
	QOS_QUEUE_NUM, /**< QOS_QUEUE_NUM */
	QOS_QUEUE_INVALID = 0xff /**< QOS_QUEUE_INVALID */
};

/**
 * @enum jl_qos_dir_type_e
 * @brief Qos mac and ip direction type.
 */
/**
 * @public typedef jl_qos_dir_type_t
 */
typedef enum jl_qos_dir_type_e {
	QOS_DIR_SOURCE = 0, /**< 0 */
	QOS_DIR_DEST, /**< 1 */
	QOS_DIR_NUM /**< Invalid */
} jl_qos_dir_type_t;

/**
 * @enum jl_qos_ip_type_e
 * @brief Qos IP type.
 */
/**
 * @public typedef jl_qos_ip_type_t
 */
typedef enum jl_qos_ip_type_e {
	QOS_IP_V4 = 0, /**< 0 */
	QOS_IP_V6, /**< 1 */
	QOS_IP_NUM /**< Invalid */
} jl_qos_ip_type_t;

/**
 * @enum jl_qos_l4_protcol_e
 * @brief Qos L4 protcol type.
 */
/**
 * @public typedef jl_qos_l4_protcol_t
 */
typedef enum jl_qos_l4_protcol_e {
	QOS_L4_UDP = 0, /**< 0 */
	QOS_L4_TCP, /**< 1 */
	QOS_L4_NUM /**< Invalid */
} jl_qos_l4_protcol_t;

/**
 * @enum jl_qos_rate_unit_e
 * @brief Qos shaper rate unit.
 */
/**
 * @public typedef jl_qos_rate_unit_t
 */
typedef enum jl_qos_rate_unit_e {
	QOS_RATE_UNIT_KBPS = 0, /**< 0 */
	QOS_RATE_UNIT_MBPS, /**< 1 */
	QOS_RATE_UNIT_PPS, /**< 2, only for shaper */
	QOS_RATE_UNIT_NUM, /**< Invalid */
} jl_qos_rate_unit_t;

/**
 * @enum jl_qos_vlan_opt_e
 * @brief QoS egress queue remap VLAN operations.
 */
/**
 * @public typedef jl_qos_vlan_opt_t
 */
typedef enum jl_qos_vlan_opt_e {
	QOS_VLAN_OPT_NO_OPT = 0, /**< 0 */
	QOS_VLAN_OPT_SWAP, /**< 1 */
	QOS_VLAN_OPT_PUSH, /**< 2 */
	QOS_VLAN_OPT_POP, /**< 3 */
	QOS_VLAN_OPT_POP_ALL, /**< 4 */
	QOS_VLAN_OPT_NUM /**< Invalid */
} jl_qos_vlan_opt_t;

/**
 * @enum jl_qos_vlan_type_e
 * @brief QoS VLAN type.
 */
/**
 * @public typedef jl_qos_vlan_type_t
 */
typedef enum jl_qos_vlan_type_e {
	QOS_VLAN_TYPE_CUSTOM = 0, /**< 0, 0x8100 */
	QOS_VLAN_TYPE_SERVICE, /**< 1, 0x88A8 */
	QOS_VLAN_TYPE_USER, /**< 2, User defined type*/
	QOS_VLAN_TYPE_NUM /**< Invalid */
} jl_qos_vlan_type_t;

/**
 * @enum jl_qos_vlan_remap_sel_e
 * @brief Which PCP to use when remapping VLAN.
 */
/**
 * @public typedef jl_qos_vlan_pcp_remap_sel_t
 */
typedef enum jl_qos_vlan_remap_sel_e {
	QOS_REMAP_PCP_FROM_PACKET = 0, /**< 0 */
	QOS_REMAP_PCP_FROM_EPORT, /**< 1 */
	QOS_REMAP_PCP_FROM_QUEUE_MAPPING, /**< 2 */
	QOS_REMAP_PCP_NUM /**< Invalid */
} jl_qos_vlan_pcp_remap_sel_t;

/**
 * @enum jl_qos_vlan_id_remap_sel_e
 * @brief Which VID to use when remapping VLAN.
 */
/**
 * @public typedef jl_qos_vlan_id_remap_sel_t
 */
typedef enum jl_qos_vlan_id_remap_sel_e {
	QOS_REMAP_VID_FROM_PACKET = 0, /**< 0 */
	QOS_REMAP_VID_FROM_EPORT, /**< 1 */
	QOS_REMAP_VID_FROM_SPORT, /**< 2 */
	QOS_REMAP_VID_NUM /**< Invalid */
} jl_qos_vlan_id_remap_sel_t;

/**
 * @enum jl_qos_mmp_mode_e
 * @brief Qos MMP bucket mode.
 */
/**
 * @public typedef jl_qos_mmp_mode_t
 */
typedef enum jl_qos_mmp_mode_e {
	QOS_MMP_MODE_SR = 0, /**< 0, single rate mode */
	QOS_MMP_MODE_TR, /**< 1, two rates mode */
	QOS_MMP_MODE_NUM /**< Invalid */
} jl_qos_mmp_mode_t;

/**
 * @enum jl_qos_mmp_correction_mode_e
 * @brief Qos bytes correction for MMP.
 */
/**
 * @public typedef jl_qos_mmp_correction_mode_t
 */
typedef enum jl_qos_mmp_correction_mode_e {
	QOS_MMP_CORRECTION_ADD = 0, /**< 0, Add extra bytes for metering */
	QOS_MMP_CORRECTION_SUBSTRACT, /**< 1, Substract extra bytes for metering */
	QOS_MMP_CORRECTION_NUM /**< Invalid */
} jl_qos_mmp_correction_mode_t;

/**
 * @enum jl_qos_mmp_remap_mode_e
 * @brief Qos MMP color remap mode.
 */
/**
 * @public typedef jl_qos_mmp_remap_mode_t
 */
typedef enum jl_qos_mmp_remap_mode_e {
	QOS_MMP_REMAP_SKIP = 0, /**< 0, Skip remap color */
	QOS_MMP_REMAP_TO_L3, /**< 1, Remap to L3 only */
	QOS_MMP_REMAP_TO_L2, /**< 2, Remap to L2 only */
	QOS_MMP_REMAP_TO_L2_L3, /**< 3, Remap to L2 and L3 */
	QOS_MMP_REMAP_NUM /**< Invalid */
} jl_qos_mmp_remap_mode_t;

/**
 * @enum jl_qos_mmp_order_level_e
 * @brief Qos MMP order level.
 */
/**
 * @public typedef jl_qos_mmp_order_level_t
 */
typedef enum jl_qos_mmp_order_level_e {
	QOS_MMP_ORDER_LV0 = 0, /**< 0 */
	QOS_MMP_ORDER_LV1, /**< 1*/
	QOS_MMP_ORDER_LV2, /**< 2 */
	QOS_MMP_ORDER_LV3, /**< 3 */
	QOS_MMP_ORDER_NUM /**< Invalid */
} jl_qos_mmp_order_level_t;

/**
 * @enum jl_qos_color_blind_e
 * @brief QoS color-blind mode type.
 */
/**
 * @public typedef jl_qos_color_blind_t
 */
typedef enum jl_qos_color_blind_e {
	QOS_COLOR_AWARE = 0, /**< 0 */
	QOS_COLOR_BLIND, /**< 1 */
	QOS_COLOR_MODE_NUM /**< Invalid */
} jl_qos_color_blind_t;

/**
 * @enum jl_qos_color_e
 * @brief QoS color classification type.
 */
/**
 * @public typedef jl_qos_color_t
 */
typedef enum jl_qos_color_e {
	QOS_COLOR_GREEN = 0, /**< 0 */
	QOS_COLOR_YELLOW, /**< 1 */
	QOS_COLOR_RED, /**< 2 */
	QOS_COLOR_NUM /**< Invalid */
} jl_qos_color_t;

/**
 * @enum jl_qos_queue_res_mode_e
 * @brief QoS egress queue resource management mode type.
 */
/**
 * @public typedef jl_qos_queue_res_mode_t
 */
typedef enum jl_qos_queue_res_mode_e {
	QOS_QUEUE_RES_LIMITER = 0, /**< 0 */
	QOS_QUEUE_RES_GUARANTEE, /**< 1 */
	QOS_QUEUE_RES_NUM /**< Invalid */
} jl_qos_queue_res_mode_t;

/**
 * @enum jl_qos_schedule_type_e
 * @brief QoS queue schedule type.
 */
/**
 *@public typedef jl_qos_schedule_type_t
 */
typedef enum jl_qos_schedule_type_e {
	QOS_SCHE_SP = 0, /**< 0 */
	QOS_SCHE_DWRR, /**< 1 */
} jl_qos_schedule_type_t;

/**
 * @struct jl_qos_ip_addr_s
 * @brief QoS IPv4 addr data structure.
 */
/**
 *@public typedef jl_qos_ip_addr_t
 */
typedef struct jl_qos_ip_addr_s {
	jl_uint8 addr[4]; /**< Address value */
	jl_uint8 mask[4]; /**< Address mask */
} jl_qos_ip_addr_t;

/**
 * @struct jl_qos_ip6_addr_s
 * @brief QoS IPv6 addr data structure.
 */
/**
 *@public typedef jl_qos_ip6_addr_t
 */
typedef struct jl_qos_ip6_addr_s {
	jl_uint16 addr[8]; /**< Address value */
	jl_uint16 mask[8]; /**< Address mask */
} jl_qos_ip6_addr_t;

/**
 * @struct jl_qos_pcp_data_s
 * @brief QoS PCP CFI/DEI data values for remapping.
 */
/**
 *@public typedef jl_qos_pcp_data_t
 */
typedef struct jl_qos_pcp_data_s {
	jl_uint8 valid; /**< Wether this entry is valid */
	jl_uint8 pcp; /**< PCP remapping val */
	jl_uint8 cfi_dei; /**< CFI/DEI remapping val */
} jl_qos_pcp_data_t;

/**
 * @struct jl_qos_vlan_data_s
 * @brief QoS VLAN datas for remapping.
 */
/**
 *@public typedef jl_qos_vlan_data_t
 */
typedef struct jl_qos_vlan_data_s {
	jl_uint8 valid; /**< Wether this entry is valid */
	jl_uint16 vid; /**< VID remapping val */
	jl_uint8 pcp; /**< PCP remapping val */
	jl_uint8 cfi_dei; /**< CFI/DEI remapping val */
} jl_qos_vlan_data_t;

/**
 * @struct jl_qos_remap_vlan_s
 * @brief QoS egress queue vlan header remapping configuration.
 */
/**
 *@public typedef jl_qos_remap_vlan_t
 */
typedef struct jl_qos_remap_vlan_s {
	jl_uint8 vlan_single_opt_valid; /**< Set vlan_single_opt valid */
	jl_qos_vlan_opt_t vlan_single_opt; /**< Egress port VLAN operation */
	jl_uint8 vid_sel_valid; /**< Set vid_sel valid */
	jl_qos_vlan_id_remap_sel_t vid_sel; /**< Selects which VID to use */
	jl_uint8 pcp_sel_valid; /**< Set pcp_sel valid */
	jl_qos_vlan_pcp_remap_sel_t pcp_sel; /**< Selects which PCP to use*/
	jl_uint8 cfi_dei_sel_valid; /**< Set cfi_dei_sel valid */
	jl_qos_vlan_pcp_remap_sel_t cfi_dei_sel; /**< Selects which CFI/DEI to use */
	jl_uint8 type_sel_valid; /**< Set type_sel valid */
	jl_qos_vlan_type_t type_sel; /**< Selects which TPID to use */
	jl_qos_vlan_data_t vlan_port; /**<Used VLAN data when selecting QOS_REMAP_VLAN_FROM_EPORT */
	jl_qos_pcp_data_t queue_mapping[QOS_QUEUE_NUM]; /**<Used PCP & CFI/DEI when selecting QOS_REMAP_VLAN_FROM_QUEUE_MAPPING */
} jl_qos_remap_vlan_t;

/**
 * @struct jl_qos_priority_s
 * @brief QoS queue priority structure.
 */
/**
 *@public typedef jl_qos_priority_t
 */
typedef struct jl_qos_priority_s {
	jl_uint8 valid;	/**< Priority valid */
	jl_uint8 priority; /**< Priority value */
} jl_qos_priority_t;

/**
 * @struct jl_qos_queue_priority_s
 * @brief QoS queue priority for all queues.
 */
/**
 *@public typedef jl_qos_queue_priority_t
 */
typedef struct jl_qos_queue_priority_s {
	jl_qos_priority_t queue[QOS_QUEUE_NUM]; /**< Queue priority abilities */
} jl_qos_queue_priority_t;

/**
 * @struct jl_qos_port_shaper_s
 * @brief QoS port shaper rate structure.
 */
/**
 *@public typedef jl_qos_port_shaper_t
 */
typedef struct jl_qos_port_shaper_s {
	jl_uint8 enable; /**< whether this shaper is enable, 0 = disable, 1 = enable */
	jl_qos_rate_unit_t unit; /**< Shaper rate unit */
	jl_uint32 rate;	/**< Shaper rate value */
} jl_qos_port_shaper_t;

/**
 * @struct jl_qos_shaper_rate_s
 * @brief QoS shaper rate structure.
 */
/**
 *@public typedef jl_qos_shaper_rate_t
 */
typedef struct jl_qos_shaper_rate_s {
	jl_uint8 valid; /**< whether this shaper config is valid */
	jl_uint8 enable; /**< whether this shaper is enable, 0 = disable, 1 = enable */
	jl_qos_rate_unit_t unit; /**< Shaper rate unit */
	jl_uint32 rate;	/**< Shaper rate value */
} jl_qos_shaper_rate_t;

/**
 * @struct jl_qos_queue_shaper_s
 * @brief QoS queue shaper for all queues.
 */
/**
 *@public typedef jl_qos_queue_shaper_t
 */
typedef struct jl_qos_queue_shaper_s {
	jl_qos_shaper_rate_t queue[QOS_QUEUE_NUM]; /**< Queue shaper abilities */
} jl_qos_queue_shaper_t;

/**
 * @struct jl_qos_priority_shaper_s
 * @brief QoS priority shaper for all priorities.
 */
/**
 *@public typedef jl_qos_priority_shaper_t
 */
typedef struct jl_qos_priority_shaper_s {
	jl_qos_shaper_rate_t priority[QOS_PRIORITY_NUM]; /**< Priority shaper abilities */
} jl_qos_priority_shaper_t;

/**
 * @struct jl_qos_weight_s
 * @brief QoS dwrr weight structure.
 */
/**
 *@public typedef jl_qos_weight_t
 */
typedef struct jl_qos_weight_s {
	jl_uint8 valid; /**< whether this queue weight is valid */
	jl_uint8 weight; /**< Queue weight */
} jl_qos_weight_t;

/**
 * @struct jl_qos_queue_dwrr_s
 * @brief QoS dwrr weight for all queues.
 */
/**
 *@public typedef jl_qos_queue_dwrr_t
 */
typedef struct jl_qos_queue_dwrr_s {
	jl_qos_weight_t queue[QOS_QUEUE_NUM]; /**< QoS queue dwrr weight abilities */
} jl_qos_queue_dwrr_t;

/**
 * @struct jl_qos_mac_s
 * @brief QoS MAC data structure.
 */
/**
 *@public typedef jl_qos_mac_t
 */
typedef struct jl_qos_mac_s {
	jl_mac_addr_t value; /**< MAC value */
	jl_mac_addr_t mask;	/**< MAC mask */
} jl_qos_mac_t;

/**
 * @struct jl_qos_mac_map_s
 * @brief QoS map MAC to queue structure.
 */
/**
 *@public typedef jl_qos_mac_map_t
 */
typedef struct jl_qos_mac_map_s {
	jl_qos_dir_type_t mac_type; /**< MAC type */
	jl_qos_mac_t mac; /**< MAC value */
	jl_uint8 queue;	/**< Mapped queue */
} jl_qos_mac_map_t;

/**
 * @struct jl_qos_vid_map_s
 * @brief QoS map VID to queue structure.
 */
/**
 *@public typedef jl_qos_vid_map_t
 */
typedef struct jl_qos_vid_map_s {
	jl_uint16 vid; /**< VID */
	jl_uint16 mask; /**< VID mask*/
	jl_uint8 inner_outer; /**< 0 = Outer, 1 = Inner */
	jl_uint8 cstag; /**< 0 = C-VID, 1 = S-VID */
	jl_uint8 queue;	/**< Mapped queue */
} jl_qos_vid_map_t;

/**
 * @struct jl_qos_ip_map_s
 * @brief QoS map MAC to queue structure.
 */
/**
 *@public typedef jl_qos_ip_map_t
 */
typedef struct jl_qos_ip_map_s {
	jl_qos_ip_type_t v4_v6; /**< 0 = IPv4, 1 = IPv6 */
	jl_qos_dir_type_t sa_da; /**< 0 = Source Address, 1 = Destination Address */
	union {
		jl_qos_ip_addr_t ipv4; /**< IPv4 address */
		jl_qos_ip6_addr_t ipv6; /**< IPv6 address  */
	} ip; /**< ip */
	jl_uint8 queue;	/**< Mapped queue */
} jl_qos_ip_map_t;


/**
 * @struct jl_qos_l4_port_range_map_s
 * @brief QoS map L4 port range to queue structure.
 */
/**
 *@public typedef jl_qos_l4_port_range_map_t
 */
typedef struct jl_qos_l4_port_range_map_s {
	jl_uint16 start_port; /**< Port range start */
	jl_uint16 end_port; /**< Port range end */
	jl_qos_dir_type_t source_dest; /**< Is source or destination */
	jl_qos_l4_protcol_t udp_tcp; /**< Is UDP or TCP */
	jl_uint8 queue;	/**< Mapped equeue */
} jl_qos_l4_port_range_map_t;

/**
 * @struct jl_qos_l4_protocol_map_s
 * @brief QoS map L4 protocol type to queue structure.
 */
/**
 *@public typedef jl_qos_l4_protocol_map_t
 */
typedef struct jl_qos_l4_protocol_map_s {
	jl_uint8 protocol; /**< L4 protocol */
	jl_uint8 queue;	/**< Mapped equeue */
} jl_qos_l4_protocol_map_t;

/**
 * @struct jl_qos_ethtype_map_s
 * @brief QoS map Ethernet type to queue structure.
 */
/**
 *@public typedef jl_qos_ethtype_map_t
 */
typedef struct jl_qos_ethtype_map_s {
	jl_uint16 eth_type; /**<  Ethernet type */
	jl_uint8 queue;	/**< Mapped equeue */
} jl_qos_ethtype_map_t;

/**
 * @struct jl_qos_mmp_single_rate_s
 * @brief QoS single rate three color marker rate structure.
 */
/**
 *@public typedef jl_qos_mmp_single_rate_t
 */
typedef struct jl_qos_mmp_single_rate_s {
	jl_uint32 CIR; /**< Committed Information Rate */
	jl_uint16 CBS; /**< Committed Burst Size */
	jl_uint16 EBS; /**< Excess Burst Size */
	jl_qos_rate_unit_t cir_unit; /**< CIR rate unit */
} jl_qos_mmp_single_rate_t;

/**
 * @struct jl_qos_mmp_single_rate_s
 * @brief QoS single rate three color marker rate structure.
 */
/**
 *@public typedef jl_qos_mmp_two_rate_t
 */
typedef struct jl_qos_mmp_two_rate_s {
	jl_uint32 CIR; /**< Committed Information Rate */
	jl_uint32 PIR; /**< Peak Information Rate */
	jl_uint16 CBS; /**< Committed Burst Size */
	jl_uint16 PBS; /**< Peak Burst Size */
	jl_qos_rate_unit_t cir_unit; /**< CIR rate unit */
	jl_qos_rate_unit_t pir_unit; /**< PIR rate unit */
} jl_qos_mmp_two_rate_t;

/**
 * @struct jl_qos_mmp_policy_s
 * @brief QoS MMP policy structure.
 */
/**
 *@public typedef jl_qos_mmp_policy_t
 */
typedef struct jl_qos_mmp_policy_s {
	jl_qos_mmp_mode_t bucket_mode; /**< Committed Information Rate */
	union {
		jl_qos_mmp_single_rate_t s_rate;  /**< srTCM cfg */
		jl_qos_mmp_two_rate_t t_rate; /**< trTCM cfg */
	} rate;/**< rate */
	jl_uint8 green_drop; /**< Wether drop green packets */
	jl_uint8 yellow_drop; /**< Wether drop yellow packets */
	jl_uint8 red_drop; /**< Wether drop red packets */
} jl_qos_mmp_policy_t;

/**
 * @struct jl_qos_color_remap_s
 * @brief QoS color remap configuration structure.
 */
/**
 *@public typedef jl_qos_color_remap_t
 */
typedef struct jl_qos_color_remap_t {
	jl_uint8 enable; /**< Wether remap action is enable, 0 = disable, 1 = enable */
	jl_qos_mmp_remap_mode_t remap_mode; /**< Color remap mode */
	jl_uint8 tos_mask; /**< Mask for updating the TOS/TC field */
	jl_uint8 green_tos; /**< New TOS/TC value for green packet */
	jl_uint8 yellow_tos; /**< New TOS/TC value for yellow packet */
	jl_uint8 red_tos; /**< New TOS/TC value for red packet */
	jl_uint8 green_dei; /**< New DEI value for packet green */
	jl_uint8 yellow_dei; /**< New DEI value for packet yellow */
	jl_uint8 red_dei; /**< New DEI value for packet red */
} jl_qos_color_remap_t;

/**
 * @struct jl_qos_mmp_cfg_s
 * @brief QoS MMP general configuration structure.
 */
/**
 *@public typedef jl_qos_mmp_cfg_t
 */
typedef struct jl_qos_mmp_cfg_s {
	jl_uint8 policy_valid; /**< Wether policy configuration is valid */
	jl_qos_mmp_policy_t policy; /**< MMP policy configuration */
	jl_uint8 color_blind_valid; /**< Wether color-blind mode setting is valid */
	jl_qos_color_blind_t color_blind; /**< MMP color-blind mode setting */
	jl_uint8 color_remap_valid; /**< Wether color-remap configuration is valid */
	jl_qos_color_remap_t color_remap; /**< MMP color-remap configuration */
	jl_uint8 max_len_valid; /**< Wether max-length setting is valid */
	jl_uint16 max_length; /**< MMP max-length setting */
	jl_uint8 correction_mode_valid; /**< Wether correction-mode setting is valid*/
	jl_qos_mmp_correction_mode_t correction_mode; /**< MMP correction-mode type */
	jl_uint8 byte_correction; /**< Extra bytes per packet to correct for IFG */
} jl_qos_mmp_cfg_t;

/**
 * @struct jl_qos_mmp_pointer_s
 * @brief QoS MMP pointer info in rule.
 * @typedef jl_qos_mmp_pointer_t
 */
/**
 *@public typedef jl_qos_mmp_pointer_t
 */
typedef struct jl_qos_mmp_pointer_s {
	jl_uint8 mmp_valid; /**< Wether this MMP pointer is valid */
	jl_uint8 mmp_pointer; /**< MMP pointer index */
	jl_uint8 mmp_order; /**< MMP pointer order, range [0,3] */
} jl_qos_mmp_pointer_t;

/**
 * @struct jl_qos_enqueue_state_s
 * @brief QoS enable enqueue state on port.
 */
/**
 *@public typedef jl_qos_enqueue_state_t
 */
typedef struct jl_qos_enqueue_state_s {
	jl_uint8 enable_mask; /**< Queue enable state mask value */
} jl_qos_enqueue_state_t;


/**
 * @struct jl_qos_mmp_used_info_s
 * @brief MMP used information for user debugging use
 */
/**
 *@public typedef jl_qos_mmp_used_info_t
 */
typedef struct jl_qos_mmp_used_info_s {
	jl_uint8 is_used; /**< Wether this MMP pointer is in use */
	jl_uint8 adm_used_num; /**< The number of times used in admission control */
	jl_uint8 acl_used_num; /**< The number of times used in ACL */
	jl_uint8 smac_used_num; /**< The number of times used in Source MAC rule */
	jl_uint8 dmac_used_num; /**< The number of times used in DEST MAC rule */
	jl_uint8 vlan_used_num; /**< The number of times used in VLAN processing */
} jl_qos_mmp_used_info_t;

/**
 * @struct jl_qos_schedule_s
 * @brief Queue schedule type (e.g. SP or DWRR)
 */
/**
 *@public typedef jl_qos_schedule_t
 */
typedef struct jl_qos_schedule_s {
	jl_qos_schedule_type_t type[QOS_QUEUE_NUM]; /**< Queue schedule type*/
} jl_qos_schedule_t;

/**
 * @brief Set the egress queue mapping configuration for non-VLAN packet
 * @details If a packet is non-VLAN tagged, force it  to specific egress queue for the specified egress port.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	queue				Queue
 * @return Set the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_nonvlan_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 queue);

/**
 * @brief Get the egress queue mapping configuration for non-VLAN packet
 * @details This function is used to get the specific egress queue for the packet which is non-VLAN tagged in the specified egress port
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	pqueue				Queue
 * @return Get the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_nonvlan_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_uint8 *pqueue);

/**
 * @brief Delete the egress queue mapping configuration for non-VLAN packet
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @return Delete the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_nonvlan_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport);

/**
 * @brief Set the egress queue mapping configuration for unknown packet
 * @details If a packet is unknown tagged, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	queue				Queue
 * @return Set the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_unknownl3_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 queue);

/**
 * @brief Get the egress queue mapping configuration for unknown packet
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	pqueue				Queue
 * @return Get the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_unknownl3_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_uint8 *pqueue);

/**
 * @brief Delete the egress queue mapping configuration for unknown packet
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @return Delete the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_unknownl3_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport);

/**
 * @brief Set the egress queue mapping configuration for packet with specific MAC address
 * @details If a packet is with specific MAC address, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				MAC to Queue mapping cfg
 * @return Set the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_mac_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_mac_map_t *pmap);

/**
 * @brief Get the egress queue mapping configuration for packet with specific MAC address
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				MAC to Queue mapping cfg
 * @return Get the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_mac_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_mac_map_t *pmap);

/**
 * @brief Delete the egress queue mapping configuration for packet with specific MAC address
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @return Delete the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_mac_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx);

/**
 * @brief Set the egress queue mapping configuration for packet with specific VID
 * @details If a packet is with specific VID, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				VID to Queue mapping cfg
 * @return Set the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_vid_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_vid_map_t *pmap);

/**
 * @brief Get the egress queue mapping configuration for packet with specific VID
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				VID to Queue mapping configuration
 * @return Get the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_vid_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_vid_map_t *pmap);

/**
 * @brief Delete the egress queue mapping configuration for packet with specific VID
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @return Delete the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_vid_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx);

/**
 * @brief Set the egress queue mapping configuration for packet with specific IP address
 * @details If a packet is with specific IP address, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				IP to Queue mapping cfg
 * @return Set the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_ip_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_ip_map_t *pmap);

/**
 * @brief Get the egress queue mapping configuration for packet with specific IP address
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				IP to Queue mapping cfg
 * @return Get the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_ip_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_ip_map_t *pmap);

/**
 * @brief Del the egress queue mapping configuration for packet with specific IP address
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @return Delete the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_ip_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx);

/**
 * @brief Set the egress queue mapping configuration for packet with specific EthType
 * @details If a packet is with specific EthType, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				EthType to Queue mapping cfg
 * @return Set the queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_ethtype_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_ethtype_map_t *pmap);

/**
 * @brief Get the egress queue mapping configuration for packet with specific EthType
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				EthType to Queue mapping cfg
 * @return Get the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_ethtype_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_ethtype_map_t *pmap);

/**
 * @brief Delete the egress queue mapping configuration for packet with specific EthType
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @return Del the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_ethtype_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx);

/**
 * @brief Set the egress queue mapping configuration for packet with specific TOS priority
 * @details If a packet is with specific TOS priority, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	tos					TOS value
 * @param	queue				Queue
 * @return Set the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_tos_2queue_set(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 tos,
        const jl_uint8 queue);

/**
 * @brief Get the egress queue mapping configuration for packet with specific TOS priority
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	tos					TOS value
 * @param	pqueue				Queue
 * @return Get the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_tos_2queue_get(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 tos,
        jl_uint8 *pqueue);

/**
 * @brief Delete the egress queue mapping configuration for packet with specific TOS priority
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	tos					TOS value
 * @return Del the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_tos_2queue_del(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 tos);

/**
 * @brief Set the egress queue mapping configuration for packet with specific MPLS EXP
 * @details If a packet is with specific MPLS EXP, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	exp					EXP value
 * @param	queue				Queue
 * @return Set the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_exp_2queue_set(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 exp,
        const jl_uint8 queue);

/**
 * @brief Get the egress queue mapping configuration for packet with specific MPLS EXP
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	exp					EXP value
 * @param	pqueue				Queue
 * @return Get the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_exp_2queue_get(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 exp,
        jl_uint8 *pqueue);

/**
 * @brief Delete the egress queue mapping configuration for packet with specific MPLS EXP
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	exp					EXP value
 * @return Delete the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_exp_2queue_del(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 exp);

/**
 * @brief Set the egress queue mapping configuration for packet with specific VLAN PCP
 * @details If a packet is with specific VLAN PCP, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	pcp					PCP value
 * @param	queue				Queue
 * @return Set the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_pcp_2queue_set(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 pcp,
        const jl_uint8 queue);

/**
 * @brief Get the egress queue mapping configuration for packet with specific VLAN PCP
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	pcp					PCP value
 * @param	pqueue				Queue
 * @return Get the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_pcp_2queue_get(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 pcp,
        jl_uint8 *pqueue);

/**
 * @brief Delete the egress queue mapping configuration for packet with specific VLAN PCP
 * @param	chip_id				Chip Id
 * @param	eport				Egress port Id
 * @param	pcp					PCP value
 * @return Delete the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_pcp_2queue_del(const jl_uint32 chip_id,
		const jl_uint8 eport,
        const jl_uint8 pcp);

/**
 * @brief Set the egress queue mapping configuration for packet with specific L4-port
 * @details If a packet is with specific L4-port, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport		    	Egress Port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				L4-port to Queue mapping cfg
 * @return Set the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_l4port_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_l4_port_range_map_t *pmap);

/**
 * @brief Get the egress queue mapping configuration for packet with specific L4-port
 * @param	chip_id				Chip Id
 * @param	eport		    	Egress Port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				L4-port to Queue mapping cfg
 * @return Get the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_l4port_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_l4_port_range_map_t *pmap);

/**
 * @brief Delete the egress queue mapping configuration for packet with specific L4-port
 * @param	chip_id				Chip Id
 * @param	eport		    	Egress Port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @return Delete the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_l4port_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx);

/**
 * @brief Set the egress queue mapping configuration for packet with specific L4-protocol type
 * @details If a packet is with specific L4-protocol type, force it to specific egress queue.
 * @param	chip_id				Chip Id
 * @param	eport		    	Egress Port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				L4-protocol to Queue mapping cfg
 * @return Set the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_l4protocol_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_l4_protocol_map_t *pmap);

/**
 * @brief Get the egress queue mapping configuration for packet with specific L4-protocol type
 * @param	chip_id				Chip Id
 * @param	eport		    	Egress Port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @param	pmap				L4-protocol to Queue mapping cfg
 * @return Get the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_l4protocol_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_l4_protocol_map_t *pmap);

/**
 * @brief Delete the egress queue mapping configuration for packet with specific L4-protocol type
 * @param	chip_id				Chip Id
 * @param	eport		    	Egress Port Id
 * @param	rule_idx			Rule index, range [0, 3]
 * @return Delete the egress queue mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT	    Port index Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_l4protocol_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx);

/**
 * @brief Set the color mapping configuration for non-VLAN packet
 * @details If a packet is non-VLAN tagged, force it to specific color.
 * @param	chip_id				Chip Id
 * @param	color				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @param	enable				Enable
 * @return Set the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_nonvlan_2color_set(const jl_uint32 chip_id,
        const jl_uint8 color,
        const jl_uint8 enable);

/**
 * @brief Get the color mapping configuration for non-VLAN packet
 * @param	chip_id				Chip Id
 * @param	pcolor				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @param	penable				Enable
 * @return Get the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_nonvlan_2color_get(const jl_uint32 chip_id,
        jl_uint8 *pcolor,
        jl_uint8 *penable);

/**
 * @brief Set the color mapping configuration for unknown packet
 * @details If a packet is unknown tagged, force it to specific color.
 * @param	chip_id				Chip Id
 * @param	color				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @param	enable				Enable
 * @return Set the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_unknownl3_2color_set(const jl_uint32 chip_id,
        const jl_uint8 color,
        const jl_uint8 enable);
/**
 * @brief Get the color mapping configuration for unknown packet
 * @param	chip_id				Chip Id
 * @param	pcolor				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @param	penable				Enable
 * @return Get the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_unknownl3_2color_get(const jl_uint32 chip_id,
        jl_uint8 *pcolor,
        jl_uint8 *penable);

/**
 * @brief Set the color mapping configuration for packet with specific TOS priority
 * @details If a packet is with specific TOS priority, force it to specific color.
 * @param	chip_id				Chip Id
 * @param	tos					TOS Value
 * @param	color				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @return Set the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_tos_2color_set(const jl_uint32 chip_id,
        const jl_uint8 tos,
        const jl_uint8 color);
/**
 * @brief Get the color mapping configuration for packet with specific TOS priority
 * @param	chip_id				Chip Id
 * @param	tos					TOS Value
 * @param	pcolor				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @return Get the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_tos_2color_get(const jl_uint32 chip_id,
        const jl_uint8 tos,
        jl_uint8 *pcolor);

/**
 * @brief Set the color mapping configuration for packet with specific MPLS EXP
 * @details If a packet is with specific MPLS EXP, force it to specific color.
 * @param	chip_id				Chip Id
 * @param	exp		   	 		EXP Value
 * @param	color				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @return Set the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_exp_2color_set(const jl_uint32 chip_id,
        const jl_uint8 exp,
        const jl_uint8 color);

/**
 * @brief Get the color mapping configuration for packet with specific MPLS EXP
 * @param	chip_id				Chip Id
 * @param	exp		   	 		EXP Value
 * @param	pcolor				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @return Get the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_exp_2color_get(const jl_uint32 chip_id,
        const jl_uint8 exp,
        jl_uint8 *pcolor);

/**
 * @brief Set the color mapping configuration for packet with specific PCP & DEI
 * @details If a packet is with specific PCP & DEI, force it to specific color.
 * @param	chip_id				Chip Id
 * @param	pcp		   	 		PCP Value
 * @param	dei		   	 		DEI Value
 * @param	color				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @return Set the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_pcpdei_2color_set(const jl_uint32 chip_id,
        const jl_uint8 pcp,
        const jl_uint8 dei,
        const jl_uint8 color);

/**
 * @brief Get the color mapping configuration for packet with specific PCP & DEI
 * @param	chip_id				Chip Id
 * @param	pcp		   	 		PCP Value
 * @param	dei		   	 		DEI Value
 * @param	pcolor				Color Mode,0 = GREEN, 1 = YELLOW, 2 = RED
 * @return Get the color mapping configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_pcpdei_2color_get(const jl_uint32 chip_id,
        const jl_uint8 pcp,
        const jl_uint8 dei,
        jl_uint8 *pcolor);

/**
 * @brief Set queue remap packet's VLAN priority configuration
 * @details Remap packet's PCP & CFI/DEI value on Egress Port
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pcfg		   	 	Remapping Cfg
 * @return Set queue remap configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT		Port Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_remap_vlan_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_remap_vlan_t *pcfg);

/**
 * @brief Get queue remap packet's VLAN priority configuration
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pcfg		   	 	Remapping Cfg
 * @return Get queue remap configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT		Port Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_remap_vlan_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_remap_vlan_t *pcfg);

/**
 * @brief Set color remap packet's TOS & DEI configuration
 * @details Remap packet's PCP & CFI/DEI value by color
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pcfg		   	 	Remapping Cfg
 * @return Set color remap configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT		Port Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_color_remap_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_color_remap_t *pcfg);

/**
 * @brief Get color remap packet's TOS & DEI configuration
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pcfg		   	 	Remapping Cfg
 * @return Get color remap configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT		Port Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_color_remap_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_color_remap_t *pcfg);

/**
 * @brief Set Meter-Maker-Policer configuration
 * @details Set ingress Meter-Maker-Policer configuration, such as rate policy and color remap.
 * @param	chip_id				Chip Id
 * @param	pointer		   		MMP Id
 * @param	pcfg		   	 	MMP Cfg
 * @return Set ingress Meter-Maker-Policer configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT		Port Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_mmp_cfg_set(const jl_uint32 chip_id,
                                       const jl_uint8 pointer,
                                       jl_qos_mmp_cfg_t *pcfg);

/**
 * @brief Get ingress Meter-Maker-Policer configuration
 * @param	chip_id				Chip Id
 * @param	pointer		   		MMP Id
 * @param	pcfg		   	 	MMP Cfg
 * @return Get ingress Meter-Maker-Policer cfg successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_mmp_cfg_get(const jl_uint32 chip_id,
                                       const jl_uint8 pointer,
                                       jl_qos_mmp_cfg_t *pcfg);

/**
 * @brief Get unused Meter-Maker-Policer index
 * @details Get an unused Meter-Maker-Policer index.
 * @param	chip_id				Chip Id
 * @param	ppointer		   	MMP Id
 * @return Get unused Meter-Maker-Policer index successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval	JL_ERR_UNAVAIL  Non Unused pointer
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API  jl_api_ret_t jl_qos_mmp_unused_pointer_get(const jl_uint32 chip_id,
        jl_uint8 *ppointer);

/**
 * @brief Get used Meter-Maker-Policer info structure
 * @details MMP pointer may be used by Port ACL and QoS, this function gathers the information which module is using them
 * @param	chip_id				Chip Id
 * @param	pointer		   		MMP Id
 * @param	ppointer_info		MMP used info
 * @return Get used Meter-Maker-Policer info successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API	jl_api_ret_t jl_qos_mmp_used_pointer_info_get(const jl_uint32 chip_id,
        const jl_uint8 pointer,
        jl_qos_mmp_used_info_t *ppointer_info);

/**
 * @brief Set QoS ingress admission control configuration
 * @details Enable QoS ingress admission control and select which mmp pointer to use
 * @param	chip_id				Chip Id
 * @param	inport		   		Ingress Port Id
 * @param	pcp					PCP
 * @param	ppointer			MMP Info
 * @return Set QoS ingress admission control cfg successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API	jl_api_ret_t jl_qos_ingress_admission_ctl_set(const jl_uint32 chip_id,
        const jl_uint8 inport,
        const jl_uint8 pcp,
        jl_qos_mmp_pointer_t *ppointer);

/**
 * @brief Get QoS ingress admission control configuration
 * @details Get QoS ingress admission control information
 * @param	chip_id				Chip Id
 * @param	inport		   		Ingress Port Id
 * @param	pcp					PCP
 * @param	ppointer			MMP Info
 * @return Get QoS ingress admission control info successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API	jl_api_ret_t jl_qos_ingress_admission_ctl_get(const jl_uint32 chip_id,
        const jl_uint8 inport,
        const jl_uint8 pcp,
        jl_qos_mmp_pointer_t *ppointer);

/**
 * @brief Set QoS map to queue trust L3
 * @details Egress queue will be selected from Layer 3 decoding if set trust L3 enabled, otherwise ignore L3 fields
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	enable				Enable, 1 trust, 0 ignore
 * @return Set QoS map to queue trust L3 successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API	jl_api_ret_t jl_qos_queue_map_trust_l3_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 enable);

/**
 * @brief Get QoS map to queue trust L3 info
 * @details Egress queue will be selected from Layer 3 decoding if trust L3, otherwise ignore L3 fields
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	penable				Enable, 1 trust, 0 ignore
 * @return Get QoS map to queue trust L3 info successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_queue_map_trust_l3_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_uint8 *penable);

/**
 * @brief Set QoS map to color trust L3
 * @details Initial color will be selected from Layer 3 decoding if set trust L3 enabled, otherwise ignore L3 fields
 * @param	chip_id				Chip Id
 * @param	inport		   		Ingress Port Id
 * @param	enable				Enable, 1 trust, 0 ignore
 * @return Set QoS map to color trust L3 info successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API	jl_api_ret_t jl_qos_color_map_trust_l3_set(const jl_uint32 chip_id,
        const jl_uint8 inport,
        const jl_uint8 enable);

/**
 * @brief Get QoS map to color trust L3 info
 * @details Initial color will be selected from Layer 3 decoding if trust L3, otherwise ignore L3 fields
 * @param	chip_id				Chip Id
 * @param	inport		   		Ingress Port Id
 * @param	penable				Enable, 1 trust, 0 ignore
 * @return Get QoS map to queue trust L3 info successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API	jl_api_ret_t jl_qos_color_map_trust_l3_get(const jl_uint32 chip_id,
        const jl_uint8 inport,
        jl_uint8 *penable);

/**
 * @brief Set QoS queue priority
 * @details Map from egress queue to egress priority
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pqueue_pri			Map Info
 * @return Set QoS queue priority successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @deprecated use \ref jl_qos_schedule_set().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_queue_priority_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_queue_priority_t *pqueue_pri);

/**
 * @brief Get QoS queue priority
 * @details Map from egress queue to egress priority
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pqueue_pri			Map Info
 * @return Get QoS queue priority info successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
* @deprecated use \ref jl_qos_schedule_get().
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_queue_priority_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_queue_priority_t *pqueue_pri);

/**
 * @brief Set QoS queue shaper
 * @details Set token bucket rate configuration for queue shaper
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pshaper				Shaper Info
 * @return Set QoS queue shaper rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval	JL_ERR_OUT_OF_RANGE Rate Out Of Range
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_queue_shaper_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_queue_shaper_t *pshaper);

/**
 * @brief Get QoS queue shaper
 * @details Get token bucket rate configuration for queue shaper
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pshaper				Shaper Info
 * @return Get QoS queue shaper rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_queue_shaper_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_queue_shaper_t *pshaper);

/**
 * @brief Set QoS priority shaper
 * @details Set token bucket rate configuration for priority shaper
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pshaper				Shaper Info
 * @return Set QoS priority shaper rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval	JL_ERR_OUT_OF_RANGE Rate Out Of Range
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_priority_shaper_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_priority_shaper_t *pshaper);

/**
 * @brief Get QoS priority shaper
 * @details Get token bucket rate configuration for priority shaper
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pshaper				Shaper Info
 * @return Get QoS priority shaper rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_priority_shaper_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_priority_shaper_t *pshaper);

/**
 * @brief Set QoS port shaper
 * @details Set token bucket rate configuration for the specified egress port
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pshaper				Shaper Info
 * @return Set QoS port shaper rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_port_shaper_set(const jl_uint32 chip_id,
                                        const jl_uint8 eport,
                                        jl_qos_port_shaper_t *pshaper);

/**
 * @brief Get QoS port shaper
 * @details Get token bucket rate configuration for the specified egress port
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pshaper				Shaper Info
 * @return Get QoS port shaper rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_port_shaper_get(const jl_uint32 chip_id,
                                        const jl_uint8 eport,
                                        jl_qos_port_shaper_t *pshaper);

/**
 * @brief Set DWRR weight configuration for queues
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pdwrr				DWRR Info
 * @return Set DWRR weight configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_dwrr_weight_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_queue_dwrr_t *pdwrr);

/**
 * @brief Get DWRR weight configuration for queues
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	pdwrr				DWRR Info
 * @return Get DWRR weight configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_dwrr_weight_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_queue_dwrr_t *pdwrr);

/**
 * @brief Set queues enable or not on port
 * @details Set queues enabled state on port, if disabled, all packets directed to this queue will be dropped
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	penqueue			Enable Mask Info
 * @return Set queues enable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @deprecated not used now
 */
attribute_deprecated
JL_API jl_api_ret_t jl_qos_enqueue_enable_set(const jl_uint32 chip_id,
                                       const jl_uint8 eport,
                                       jl_qos_enqueue_state_t *penqueue);

/**
 * @brief Get queues enable state on port
 * @details Get queues enabled state on port, if disabled, all packets directed to this queue will be dropped
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	penqueue			Enable Mask Info
 * @return Get queues enable state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_enqueue_enable_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_enqueue_state_t *penqueue);


/**
 * @brief Set each queue schedule type
 * @details Set queue schedule type , SP or DWRR
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	psche				Schedule Type Info
 * @return Set queue schedule type successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT		Port Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval	JL_ERR_CONFIG	Config Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_schedule_set(const jl_uint32 chip_id,
								const jl_uint8 eport,
								jl_qos_schedule_t *psche);

/**
 * @brief Get each queue schedule type
 * @details Get queue schedule type , SP or DWRR
 * @param	chip_id				Chip Id
 * @param	eport		   		Egress Port Id
 * @param	psche				Schedule Type Info
 * @return Get queue schedule type successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PORT		Port Error
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_schedule_get(const jl_uint32 chip_id,
								const jl_uint8 eport,
								jl_qos_schedule_t *psche);

/**
 * @brief Init resources for QoS module
 * @details Init resources for QoS module
 * @param	chip_id				Chip Id
 * @param	queue_num			Queue Number On Each Port
 * @return Init resources successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_res_init(const jl_uint32 chip_id, jl_uint8 queue_num);

/**
 * @brief Deinit resources for QoS module
 * @details Deinit resources for QoS module
 * @param	chip_id				Chip Id
 * @return Deinit resources successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_qos_res_deinit(const jl_uint32 chip_id);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_QOS_H__*/
/** @} QoS APIs*/
