/**
 * @file jl_acl.h
 * @brief ACL
 */

/**
 * @defgroup acl_group ACL
 * @{
 */

/**
 * @example example_acl.c
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

#ifndef __JL_API_ACL_H__
#define __JL_API_ACL_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
* ACL_RULE_FIELD_NUM
*/
#define ACL_RULE_FIELD_NUM (6)

/**
 * ACL_TEMPLATE_NUM
 */
#define ACL_TEMPLATE_NUM (5)

/**
 * ACL_MAC_RULE_NUM
 */
#define ACL_MAC_RULE_NUM (4)

/**
 * ACL_TBL_RULE_NUM
 */
#define ACL_TBL_RULE_NUM (128)
/**
 * ACL_MATCH_COUNTER
 */
#define ACL_MATCH_COUNTER (32)
/**
 * ACL_QUEUE_NUM
 */
#define ACL_QUEUE_NUM (8)
/**
 * ACL_MMP_POINTER_NUM
 */
#define ACL_MMP_POINTER_NUM (16)

/**
 * @enum jl_acl_vlan_tag_e
 * @brief ACL VLAN tag type.
 */
/**
 *@public typedef jl_acl_vlan_tag_t
 */
typedef enum jl_acl_vlan_tag_e {
	ACL_VLAN_CTAG = 0, /**< 0, Customer VLAN tag */
	ACL_VLAN_STAG, /**< 1, Service VLAN tag */
} jl_acl_vlan_tag_t;

/**
 * @enum jl_acl_l3_type_e
 * @brief ACL L3 tag type.
 */
/**
 *@public typedef jl_acl_l3_type_t
 */
typedef enum jl_acl_l3_type_e {
	ACL_L3_IPV4 = 0, /**< 0, IPv4 */
	ACL_L3_IPV6, /**< 1, IPv6 */
	ACL_L3_MPLS, /**< 2, MPLS */
	ACL_L3_UNKNOWN, /**< 3, Unknown */
} jl_acl_l3_type_t;

/**
 * @enum jl_acl_l4_type_e
 * @brief ACL L4 tag type.
 */
/**
 * @public typedef jl_acl_l4_type_t
 */
typedef enum jl_acl_l4_type_e {
	ACL_L4_UNKNOWN = 0, /**< 0, Unknown */
	ACL_L4_V4V6, /**< 1, IPv4 or IPv6 */
	ACL_L4_UDP, /**< 2, UDP */
	ACL_L4_TCP, /**< 3, TCP */
	ACL_L4_IGMP, /**< 4, IGMP */
	ACL_L4_ICMP, /**< 5, ICMP */
	ACL_L4_ICMPV6, /**< 6, ICMPV6 */
	ACL_L4_MLD, /**< 7, MLD */
} jl_acl_l4_type_t;

/**
 * @enum jl_acl_field_type_e
 * @brief ACL rule template field type.
 */
/**
 * @public typedef jl_acl_field_type_t
 */
typedef enum jl_acl_field_type_e {
	ACL_FIELD_NO_FIELD = 0, /**< 0 */
	ACL_FIELD_SOURCE_PORT, /**< 1 */
	ACL_FIELD_MAC_DA, /**< 2 */
	ACL_FIELD_MAC_SA, /**< 3 */
	ACL_FIELD_OUTER_VID, /**< 4 */
	ACL_FIELD_HAS_VLAN, /**< 5 */
	ACL_FIELD_OUTER_ETH_TYPE, /**< 6 */
	ACL_FIELD_INNER_ETH_TYPE, /**< 7 */
	ACL_FIELD_OUTER_PCP, /**< 8 */
	ACL_FIELD_OUTER_DEI, /**< 9 */
	ACL_FIELD_INNER_VID, /**< 10 */
	ACL_FIELD_INNER_PCP, /**< 11 */
	ACL_FIELD_INNER_DEI, /**< 12 */
	ACL_FIELD_ETH_TYPE, /**< 13 */
	ACL_FIELD_L3_TYPE, /**< 14 */
	ACL_FIELD_IP_SA, /**< 15 */
	ACL_FIELD_IP_DA, /**< 16 */
	ACL_FIELD_IP6_SA, /**< 17 */
	ACL_FIELD_IP6_DA, /**< 18 */
	ACL_FIELD_MPLS, /**< 19 */
	ACL_FIELD_TOS, /**< 20 */
	ACL_FIELD_TTL, /**< 21 */
	ACL_FIELD_L4_PROTOCOL, /**< 22 */
	ACL_FIELD_L4_TYPE, /**< 23 */
	ACL_FIELD_L4_SOURCE_PORT, /**< 24 */
	ACL_FIELD_L4_DEST_PORT, /**< 25 */
	ACL_FIELD_MLD_ADDR, /**< 26 */
	ACL_FIELD_ICMP_TYPE, /**< 27 */
	ACL_FIELD_ICMP_CODE, /**< 28 */
	ACL_FIELD_ICMP6_TYPE, /**< 29 */
	ACL_FIELD_ICMP6_CODE, /**< 30 */
	ACL_FIELD_IGMP_TYPE, /**< 31 */
	ACL_FIELD_IGMP_GROUP_ADDR, /**< 32 */
	ACL_FIELD_RULE_POINTER, /**< 33 */
	ACL_FIELD_NUM,/**< 34 */
} jl_acl_field_type_t;


/**
 * @enum jl_acl_mac_type_e
 * @brief ACL rule dest and source mac type.
 */
/**
 * @public typedef jl_acl_mac_type_t
 */
typedef enum jl_acl_mac_type_e {
	ACL_MAC_DEST = 0, /**< 0, Destination MAC */
	ACL_MAC_SOURCE, /**< 1, Source MAC */
	ACL_MAC_TYPE_NUM /**< 2 */
} jl_acl_mac_type_t;

/**
 * @enum jl_acl_tbl_e
 * @brief ACL rule hash and tcam table type.
 */
/**
 * @public typedef jl_acl_tbl_t
 */
typedef enum jl_acl_tbl_e {
	ACL_TBL_UNKNOWN = 0, /**< 0, ACL unknown table*/
	ACL_TBL_TCAM , /**< 1, ACL tcam table*/
	ACL_TBL_LARGE, /**< 2, ACL large hash table */
	ACL_TBL_SMALL, /**< 3, ACL small hash table */
} jl_acl_tbl_t;

/**
 * @enum jl_acl_tpid_e
 * @brief ACL EthType action type.
 */
/**
 * @public typedef jl_acl_tpid_t
 */
typedef enum jl_acl_tpid_e {
	ACL_TPID_0X8100 = 0, /**< 0, C-VLAN */
	ACL_TPID_0X88A8, /**< 1, S-VLAN */
	ACL_TPID_USER_DEFINED, /**< 2, User defined VLAN */
	ACL_TPID_RESERVED, /**< 3 */
} jl_acl_tpid_t;

/**
 * @enum jl_acl_color_e
 * @brief ACL stream color type.
 */
/**
 * @public typedef jl_acl_color_t
 */
typedef enum jl_acl_color_e {
	ACL_COLOR_GREEN = 0, /**< 0, Color green */
	ACL_COLOR_YELLOW, /**< 1, Color yellow */
	ACL_COLOR_RED, /**< 2, Color red */
} jl_acl_color_t;


/**
 * @enum jl_acl_data_type_e
 * @brief ACL data type.
 */
/**
 * @public typedef jl_acl_data_type_t
 */
typedef enum jl_acl_data_type_e {
	ACL_DATA_SPECIFIC_VALUE = 0, /**< 0, Specific Value */
	ACL_DATA_RANGE_VALUE, /**< 1, Range Value */
} jl_acl_data_type_t;

/**
 * @struct jl_acl_port_s
 * @brief ACL template configuration on ingress port.
 */
/**
 * @public typedef jl_acl_port_t
 */
typedef struct jl_acl_port_s {
	jl_uint8 valid; /**< Entry valid or not */
	jl_uint8 enable; /**< Use acl or not */
	jl_uint8 acl_tpl_type; /**< Which common ACL template to use */
	jl_uint8 icmp_tpl_type; /**< Which ICMP ACL template to use */
	jl_uint8 icmpv6_tpl_type; /**< Which ICMPV6 ACL template to use */
	jl_uint8 igmp_tpl_type; /**< Which IGMP ACL template to use */
} jl_acl_port_t;

/**
 * @struct jl_acl_field_s
 * @brief ACL field type structure.
 */
/**
 * @public typedef jl_acl_field_t
 */
typedef struct jl_acl_field_s {
	jl_uint8 field_type; /**< ACL filed type */
	jl_uint8 field_size; /**< ACL filed size */
} jl_acl_field_t;

/**
 * @struct jl_acl_template_s
 * @brief ACL template ability.
 */
/**
 * @public typedef jl_acl_template_t
 */
typedef struct jl_acl_template_s {
	jl_acl_field_t field[ACL_RULE_FIELD_NUM]; /**< ACL fileds array */
} jl_acl_template_t;

/**
 * @struct jl_acl_ip_addr_s
 * @brief ACL ipv4 addr structure.
 */
/**
 * @public typedef jl_acl_ip_addr_t
 */
typedef struct jl_acl_ip_addr_s {
	jl_uint8 addr[4]; /**< ACL IPv4 address array */
} jl_acl_ip_addr_t;

/**
 * @struct jl_acl_ip6_addr_s
 * @brief ACL ipv6 addr structure.
 */
/**
 * @public typedef jl_acl_ip6_addr_t
 */
typedef struct jl_acl_ip6_addr_s {
	jl_uint16 addr[8]; /**< ACL IPv6 address array */
} jl_acl_ip6_addr_t;

/**
 * @struct jl_acl_val_s
 * @brief ACL common value structure.
 */
/**
 * @public typedef jl_acl_val_t
 */
typedef struct jl_acl_val_s {
	jl_acl_data_type_t type; /**< Value Type */
	jl_uint32 value; /**< Specific Value */
	jl_uint32 mask; /**< Specific mask */
    jl_uint32 range_s; /**< Range Start */
    jl_uint32 range_e; /**< Range End */
} jl_acl_val_t;

/**
 * @struct jl_acl_flag_s
 * @brief ACL flag value structure.
 */
/**
 * @public typedef jl_acl_flag_t
 */
typedef struct jl_acl_flag_s {
	jl_uint32 value; /**< Flag value */
} jl_acl_flag_t;

/**
 * @struct jl_acl_mac_s
 * @brief ACL mac value structure.
 */
/**
 * @public typedef jl_acl_mac_t
 */
typedef struct jl_acl_mac_s {
	jl_mac_addr_t value; /**< MAC value */
	jl_mac_addr_t mask; /**< MAC mask */
} jl_acl_mac_t;

/**
 * @struct jl_acl_mac_range_s
 * @brief ACL mac value structure.
 */
/**
 * @public typedef jl_acl_mac_range_t
 */
typedef struct jl_acl_mac_range_s {
	jl_acl_mac_type_t mac_type; /**< Source or destination MAC*/
	jl_mac_addr_t mac_s; /**< MAC range start */
	jl_mac_addr_t mac_e; /**< MAC range end */
} jl_acl_mac_range_t;

/**
 * @struct jl_acl_ip_s
 * @brief ACL ipv4 addr structure.
 */
/**
 * @public typedef jl_acl_ip_t
 */
typedef struct jl_acl_ip_s {
	jl_acl_ip_addr_t value; /**< IPv4 value */
	jl_acl_ip_addr_t mask; /**< IPv4 mask */
} jl_acl_ip_t;

/**
 * @struct jl_acl_ip6_s
 * @brief ACL ipv6 addr structure.
 */
/**
 * @public typedef jl_acl_ip6_t
 */
typedef struct jl_acl_ip6_s {
	jl_acl_ip6_addr_t value; /**< IPv6 value */
	jl_acl_ip6_addr_t mask; /**< IPv6 mask */
} jl_acl_ip6_t;

/**
 * @struct jl_acl_tag_s
 * @brief ACL vlan tag value structure.
 */
/**
 * @public typedef jl_acl_tag_t
 */
typedef struct jl_acl_tag_s {
	jl_acl_vlan_tag_t value; /**< VLAN tag */
} jl_acl_tag_t;

/**
 * @struct jl_acl_l3_s
 * @brief ACL l3 type value structure.
 */
/**
 * @public typedef jl_acl_l3_t
 */
typedef struct jl_acl_l3_s {
	jl_acl_l3_type_t value; /**< L3 type */
} jl_acl_l3_t;

/**
 * @struct jl_acl_l4_s
 * @brief ACL l4 type value structure.
 */
/**
 * @public typedef jl_acl_l4_t
 */
typedef struct jl_acl_l4_s {
	jl_acl_l4_type_t value; /**< L4 type */
} jl_acl_l4_t;

/**
 * @struct jl_acl_field_data_s
 * @brief ACL field data strucure.
 */
/**
 * @public typedef jl_acl_field_data_t
 */
typedef struct jl_acl_field_data_s {
	jl_acl_field_type_t field_type; /**< Input parameter, field type */
	union {
		jl_acl_val_t source_port; /**< Packet's source port */
		jl_acl_mac_t s_mac;	/**< Packet's source MAC address */
		jl_acl_mac_t d_mac;	/**< Packet's destination MAC address */
		jl_acl_val_t outer_vid; /**< Whether the packet have any VLAN tags, 0 = no, 1 = has */
		jl_acl_flag_t has_vlans; /**< What Ethernet Type is in this VLAN, 0 = C-VLAN, 1 = S-VLAN */
		jl_acl_tag_t outer_cstag; /**< What Ethernet Type is in this outer VLAN, 0 = C-VLAN, 1 = S-VLAN */
		jl_acl_tag_t inner_cstag; /**< What Ethernet Type is in this inner VLAN, 0 = C-VLAN, 1 = S-VLAN */
		jl_acl_val_t outer_pcp; /**< The packets outermost VLAN PCP field */
		jl_acl_val_t outer_dei; /**< The packets outermost VLAN DEI field */
		jl_acl_val_t inner_vid; /**< The packes innermost VLAN Identifier */
		jl_acl_val_t inner_pcp; /**< The packes innermost VLAN PCP field */
		jl_acl_val_t inner_dei; /**< The packes innermost VLAN DEI field */
		jl_acl_val_t eth_type; /**< The packets Ethernet Type after VLANs */
		jl_acl_l3_t l3_type; /**< The type of an L3 packet, see jl_acl_l3_type_e */
		jl_acl_ip_t s_ip; /**< IPv4 Source Address */
		jl_acl_ip_t d_ip; /**< IPv4 Destination Address */
		jl_acl_ip6_t s_ip6; /**< IPv6 Source Address */
		jl_acl_ip6_t d_ip6; /**< IPv6 Destination Address */
		jl_acl_val_t mpls; /**< Outermost MPLS label */
		jl_acl_val_t tos; /**< IPv4 or IPv6 TOS byte */
		jl_acl_val_t ttl; /**< IPv4, IPv6 or MPLS TTL byte */
		jl_acl_val_t l4_pro; /**< IPv4, IPv6 L4 protocol type byte */
		jl_acl_l4_t l4_type; /**< The type of an L4 packet, see jl_acl_l4_type_e */
		jl_acl_val_t l4_s_port; /**< L4 TCP or UDP packets source port */
		jl_acl_val_t l4_d_port; /**< L4 TCP or UDP packets destination port */
		jl_acl_ip6_t mld_addr; /**< The MLD headers Multicast Address field */
		jl_acl_val_t icmp_type; /**< ICMP Type */
		jl_acl_val_t icmp_code; /**< ICMP Code */
		jl_acl_val_t icmp6_type; /**< ICMPv6 Type */
		jl_acl_val_t icmp6_code; /**< ICMPv6 Code */
		jl_acl_val_t igmp_type; /**< IGMP type */
		jl_acl_ip_t igmp_group; /**< IGMP Group Address */
		jl_acl_flag_t rule_pointer; /**< Used to avoid hash collision, shoud be equal to the templ_type*/
		jl_acl_mac_t mac_data; /**< Union jl_acl_mac_t type data */
		jl_acl_ip_t ip_data; /**< Union jl_acl_ip_t type data */
		jl_acl_ip6_t ip6_data; /**< Union jl_acl_ip6_t type data */
		jl_acl_val_t val_data; /**< Union jl_acl_val_t type data */
	} field; /**< Input parameter, rule data*/
	jl_uint8 field_idx; /**< not for user*/
	jl_uint8 reg_offset; /**< not for user*/
	jl_uint8 tcam_flag; /**< not for user*/
	struct jl_acl_field_data_s *next;/**< not for user*/
} jl_acl_field_data_t;

/**
 * @struct jl_acl_rule_s
 * @brief ACL rule info strucure.
 */
/**
 * @public typedef jl_acl_rule_t
 */
typedef struct jl_acl_rule_s {
	jl_uint8 templ_idx; /**< Input parameter, template index this rule used */
	jl_uint8 field_num; /**< Output parameter, number of fileds in this rule */
	jl_uint8 field_mask; /**< Output parameter, mask of fileds in this rule */
	jl_uint8 with_pointer; /**< Output parameter, wether the template is with RULE_POINTER */
	jl_acl_field_data_t* head; /**< Pointer to link list with field data*/
} jl_acl_rule_t;

/**
 * @struct jl_acl_action_s
 * @brief ACL action info strucure.
 */
/**
 * @public typedef jl_acl_action_t
 */
typedef struct jl_acl_action_s {
	jl_uint8 drop_enable; /**< Drop all packets */
	jl_uint8 send2cpu_enable; /**< Send the packet to CPU port */
	jl_uint8 send2port_enable; /**< Send the packet to a specific port, 0 = disable, 1 = enable */
	jl_port_t dest_port; /**< The port send to */
	jl_uint8 force_vid_enable; /**< Override the Ingress VID, 0 = disable, 1 = enable */
	jl_uint16 force_vid; /**< The forced ingress VID */
	jl_uint8 force_queue_enable; /**< Force to a specific egress queue, 0 = disable, 1 = enable*/
	jl_uint8 e_queue; /**< Forced egress queue, range 0 - 7 */
	jl_uint8 force_color_enable; /**< Wether the packet shall have a forced color, 0 = disable, 1 = enable */
	jl_uint8 color; /**< Forced color */
	jl_uint8 mmp_valid; /**< If set, this entry contains a valid MMP pointer */
	jl_uint8 mmp_ptr; /**< Ingress MMP pointer */
	jl_uint8 mmp_order; /**< Ingress MMP pointer order */
	jl_uint8 update_counter_enable; /**< Use selected statistics counter, 0 = disable, 1 = enable */
	jl_uint8 counter; /**< Selected statistics counter index, range 0 - 31 */
	jl_uint8 no_learning_enable; /**< If set this packets MAC SA will not be learned */
	jl_uint8 update_cfi_dei_enable; /**< Wether update the CFI/DEI value of the packets outermost VLAN, 0 = disable, 1 = enable */
	jl_uint8 new_cfi_dei; /**< New CFI/DEI value */
	jl_uint8 update_pcp_enable; /**< Wether update the PCP value of the packets outermost VLAN, 0 = disable, 1 = enable */
	jl_uint8 new_pcp; /**< New PCP value */
	jl_uint8 update_vid_enable; /**< Wether update the VID value of the packets outermost VLAN, 0 = disable, 1 = enable */
	jl_uint16 new_vid; /**< New VID value */
	jl_uint8 update_etype_enable; /**< Wether update the e VLANs TPID type, 0 = disable, 1 = enable */
	jl_acl_tpid_t new_etype; /**< New TPID value, see jl_acl_tpid_e */
	jl_uint8 input_mirror_enable; /**< Enable input mirror for this rule, 0 = disable, 1 = enable */
	jl_port_t dest_input_mirror; /**< Destination physical port for input mirroring */
} jl_acl_action_t;

/**
 * @struct jl_l2_mac_action_s
 * @brief ACL mac action info strucure.
 */
/**
 * @public typedef jl_acl_mac_action_t
 */
typedef struct jl_l2_mac_action_s {
	jl_uint8 drop_enable; /**< Drop all packets */
	jl_uint8 send2cpu_enable; /**< Send the packet to CPU port */
	jl_uint8 force_queue_enable; /**< Force to a specific egress queue, 0 = disable, 1 = enable*/
	jl_uint8 e_queue; /**< Forced egress queue, range 0 - 7 */
	jl_uint8 force_color_enable; /**< Wether the packet shall have a forced color, 0 = disable, 1 = enable */
	jl_uint8 color; /**< Forced color */
	jl_uint8 mmp_valid; /**< If set, this entry contains a valid MMP pointer */
	jl_uint8 mmp_ptr; /**< Ingress MMP pointer */
	jl_uint8 mmp_order; /**< Ingress MMP pointer order */
	jl_portmask_t port_mask; /**< Ports that this filter rule applies to, one bit means a port */
} jl_acl_mac_action_t;

/**
 * @brief Set ACL filter rule templates
 * @details This function sets the ACL filter rule templates for packet decoder. There are 5 rule templates,
 * each template contains 6 fields, these templates can be modified by user
 * @param	chip_id				Index of chip
 * @param	index				Index of template, range [0,4]
 * @param	ptempl				Pointer to the array with field types
 * @return Set ACL filter rule templates successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_CONFIG	Config Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_acl_template_set(const jl_uint32 chip_id,
									const jl_uint8 index,
									jl_acl_template_t *ptempl);

/**
 * @brief Get ACL filter rule templates
 * @details This function gets the ACL filter rule templates from register. There are 5 rule templates,
 * each template contains 6 fields
 * @param	chip_id				Index of chip
 * @param	index				Index of template, range [0,4]
 * @param	ptempl				Pointer to the array with field types
 * @return Get ACL filter rule templates successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_acl_template_get(const jl_uint32 chip_id,
									const jl_uint8 index,
									jl_acl_template_t *ptempl);

/**
 * @brief Insert field rules to ACL filter cfg structure
 * @details This function set the ACL filter cfg structure with a linked list of field rules
 * @param	chip_id				Index of chip
 * @param	pfield				Pointer to field type structure
 * @param	prule				Pointer to acl rule structure
 * @return Insert field to rule successfully or not
 *		@retval JL_ERR_OK		Ok
		@retval JL_ERR_INIT		Check Switch State Fail
 		@retval JL_ERR_PARAM	Param Error
		@retval JL_ERR_CONFIG	Config Error
		@retval JL_ERR_MEMORY	Memory Resource Fail
		@retval JL_ERR_FULL		Field Entrys Full Fail
 */
JL_API jl_api_ret_t jl_acl_field_insert(const jl_uint32 chip_id,
									jl_acl_field_data_t *pfield,
									jl_acl_rule_t *prule);

/**
 * @brief Clear field rules from ACL rule
 * @details This function clear all field rule buffers malloced in acl rule
 * @param	chip_id				Index of chip
 * @param	prule				Pointer to acl rule structure
 * @return Clear field rules successfully or not
 *		@retval JL_ERR_OK		Ok
		@retval JL_ERR_INIT		Check Switch State Fail
 		@retval JL_ERR_PARAM	Param Error
 */

JL_API jl_api_ret_t jl_acl_field_clear(const jl_uint32 chip_id, jl_acl_rule_t *prule);

/**
 * @brief Set ACL filter rules and actions to ACL table
 * @details This function sets the ACL filter rules and the correspond actions into the ACL table.
 * There are 64 large hash entrys 32 small hash entrys and 32 tcam entrys.Each ACL rule entry contains several compare
 * fields, and each ACL action entry also contains several actions.
 * @param	chip_id				Index of chip
 * @param	index				Index of rule entry, used by user. range [0,127]
 * @param	prule				Pointer to acl rule structure
 * @param	paction				Pointer to acl action structure
 * @return set the ACL filter rules and actions successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_CONFIG	Config Error
 *		@retval JL_ERR_RESOURCE	Table Resource Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_acl_rule_set(const jl_uint32 chip_id,
									const jl_uint8 index,
									jl_acl_rule_t *prule,
									jl_acl_action_t *paction);

/**
 * @brief Get ACL filter rules and actions from ACL table
 * @param	chip_id				Index of chip
 * @param	index				Index of rule entry, used by user. range [0,127]
 * @param	prule				Pointer to acl rule structure
 * @param	paction				Pointer to acl action structure
 * @return get the ACL filter rules and actions successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 		@retval	JL_ERR_EMPTY	None Rule Exists
		@retval JL_ERR_MEMORY	Memory Resource Fail
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_rule_get(const jl_uint32 chip_id,
									const jl_uint8 index,
									jl_acl_rule_t *prule,
									jl_acl_action_t *paction);

/**
 * @brief Del ACL filter rules and actions in ACL table
 * @param	chip_id				Index of chip
 * @param	index				Index of rule entry, used by user. range [0,127]
 * @return del the ACL filter rules and actions successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_rule_del(const jl_uint32 chip_id, const jl_uint8 index);

/**
 * @brief Del all ACL filter rules and actions in ACL table
 * @param	chip_id				Index of chip
 * @return del the ACL filter rules and actions successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_rule_del_all(const jl_uint32 chip_id);

/**
 * @brief Set which result shall be selected when multiple tables hav been hit
 * @param	chip_id				Index of chip
 * @param	selection			Selection value.
 * bit0: 0 means tcam have priority, bit1: 0 means small hash table have priority
 * @return set the selection successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_multi_hits_selection_set(const jl_uint32 chip_id, const jl_uint8 selection);

/**
 * @brief Get which result shall be selected when multiple tables have been hit
 * @param	chip_id				Index of chip
 * @param	pselection			Selection value out
 * bit0: 0 means tcam have priority, bit1: 0 means small hash table have priority
 * @return get the selection successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_multi_hits_selection_get(const jl_uint32 chip_id, jl_uint8 *pselection);

/**
 * @brief Get unused ACL match counter index
 * @details This function is used to get unused counter index for user to use when setting acl rules
 * @param	chip_id				Index of chip
 * @param	pcounter			Pointer to counter index out, range [0, 31]
 * @return get the counter index successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_RESOURCE	None Aailable Counter Fail
 */
JL_API jl_api_ret_t jl_acl_stat_counter_get(const jl_uint32 chip_id, jl_uint8 *pcounter);

/**
 * @brief Enable ACL rule match statistics
 * @param	chip_id				Index of chip
 * @param	index				ACL rule index, range [0, 127]
 * @param	counter				Counter index, range [0, 31]
 * @return Set ACL rule match statistics enable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_stat_enable(const jl_uint32 chip_id,
										const jl_uint8 index,
										const jl_uint8 counter);

/**
 * @brief Disable ACL rule match statistics
 * @param	chip_id				Index of chip
 * @param	index				ACL rule index, range [0, 127]
 * @return Set ACL rule match statistics disable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_stat_disable(const jl_uint32 chip_id, const jl_uint8 index);

/**
 * @brief Read ACL rule match statistics
 * @param	chip_id				Index of chip
 * @param	counter				Counter index, range [0, 31]
 * @param	ppackets			Pointer to packet statistics
 * @return Read ACL rule match statistics successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_stat_read(const jl_uint32 chip_id,
									const jl_uint8 counter,
									jl_uint32 *ppackets);

/**
 * @brief Clear ACL rule match statistics
 * @param	chip_id				Index of chip
 * @param	counter				Counter index, range [0, 31]
 * @return Clear ACL rule match statistics successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_stat_clear(const jl_uint32 chip_id, const jl_uint8 counter);

/**
 * @brief Set which ACL rule template be used in the specified source port
 * @param	chip_id				Index of chip
 * @param	inport				Ingress port Id
 * @param	pport				Pointer to used ACL templates struct
 * @return Set ACL rule template to the the specified source port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_port_set(const jl_uint32 chip_id,
										const jl_uint8 inport,
										jl_acl_port_t *pport);

/**
 * @brief Get which ACL rule template be used in the specified source port
 * @param	chip_id				Index of chip
 * @param	inport				Ingress port Id
 * @param	pport				Pointer to used ACL templates struct
 * @return Get ACL rule template for the specified source port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_port_get(const jl_uint32 chip_id,
										const jl_uint8 inport,
										jl_acl_port_t *pport);

/**
 * @brief Set reserved mac range filter actions
 * @param	chip_id				Index of chip
 * @param	index				Rule index, range [0, 3]
 * @param	pmac_range			Mac address range
 * @param	paction				Actions
 * @return Set reserved mac range filter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_macrange_action_set(const jl_uint32 chip_id,
										const jl_uint8 index,
										jl_acl_mac_range_t *pmac_range,
										jl_acl_mac_action_t *paction);
/**
 * @brief Get reserved mac range filter actions
 * @param	chip_id				Index of chip
 * @param	index				Rule index, range [0, 3]
 * @param	mac_type			Mac Type, 0 = Destination MAC, 1 =  Source MAC
 * @param	pmac_range			Mac address range
 * @param	paction				Pointer to actions
 * @return Get reserved mac range filter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_macrange_action_get(const jl_uint32 chip_id,
										const jl_uint8 index,
										const jl_uint8 mac_type,
										jl_acl_mac_range_t *pmac_range,
										jl_acl_mac_action_t *paction);

/**
 * @brief Delete reserved mac range filter actions
 * @param	chip_id				Index of chip
 * @param	index				Rule index, range [0, 3]
 * @param	mac_type			Mac Type, 0 = Destination MAC, 1 =  Source MAC
 * @return Delete reserved mac range filter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	Param Error
 *		@retval JL_ERR_TIMEOUT	Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_acl_macrange_action_del(const jl_uint32 chip_id,
										const jl_uint8 index,
										const jl_uint8 mac_type);
#ifdef __cplusplus
}
#endif

#endif /* __JL_API_ACL_H__*/
/** @} ACL APIs*/
