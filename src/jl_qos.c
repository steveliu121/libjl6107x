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


#ifndef __KERNEL__
#include <string.h>
#else
#include <linux/string.h>
#endif
#include "jl_ops.h"
#include "jl_qos.h"

jl_api_ret_t jl_qos_nonvlan_2queue_set(const jl_uint32 chip_id,
                                       const jl_uint8 eport,
                                       const jl_uint8 queue)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(queue, QOS_QUEUE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_nonvlan_map2q_set, chip_id, eport, queue);

	return ret;
}

jl_api_ret_t jl_qos_nonvlan_2queue_get(const jl_uint32 chip_id,
                                       const jl_uint8 eport,
                                       jl_uint8 *pqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pqueue);

	JL_DRV_SAFE_CALL(ret, qos_nonvlan_map2q_get, chip_id, eport, pqueue);

	return ret;
}

jl_api_ret_t jl_qos_nonvlan_2queue_del(const jl_uint32 chip_id,
                                       const jl_uint8 eport)
{
	jl_api_ret_t ret;
	jl_uint8 queue = QOS_QUEUE_INVALID;

	JL_DRV_SAFE_CALL(ret, qos_nonvlan_map2q_set, chip_id, eport, queue);

	return ret;
}

jl_api_ret_t jl_qos_unknownl3_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 queue)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(queue, QOS_QUEUE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_unknownl3_map2q_set, chip_id, eport, queue);

	return ret;
}

jl_api_ret_t jl_qos_unknownl3_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_uint8 *pqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pqueue);

	JL_DRV_SAFE_CALL(ret, qos_unknownl3_map2q_get, chip_id, eport, pqueue);

	return ret;
}

jl_api_ret_t jl_qos_unknownl3_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport)
{
	jl_api_ret_t ret;
	jl_uint8 queue = QOS_QUEUE_INVALID;

	JL_DRV_SAFE_CALL(ret, qos_unknownl3_map2q_set, chip_id, eport, queue);

	return ret;
}

jl_api_ret_t jl_qos_mac_2queue_set(const jl_uint32 chip_id,
                                   const jl_uint8 eport,
                                   const jl_uint8 rule_idx,
                                   jl_qos_mac_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_MAC_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_mac_map2q_set, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_mac_2queue_get(const jl_uint32 chip_id,
                                   const jl_uint8 eport,
                                   const jl_uint8 rule_idx,
                                   jl_qos_mac_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_MAC_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_mac_map2q_get, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_mac_2queue_del(const jl_uint32 chip_id,
                                   const jl_uint8 eport,
                                   const jl_uint8 rule_idx)
{
	jl_api_ret_t ret;
	jl_qos_mac_map_t map;

	JL_CHECK_ENUM(rule_idx, QOS_MAC_TO_QUEUE_RULE_NUM);

	memset(&map, 0, sizeof(jl_qos_mac_map_t));
	map.queue = QOS_QUEUE_INVALID;

	JL_DRV_SAFE_CALL(ret, qos_mac_map2q_set, chip_id, eport, rule_idx, &map);

	return ret;
}

jl_api_ret_t jl_qos_vid_2queue_set(const jl_uint32 chip_id,
										const jl_uint8 eport,
										const jl_uint8 rule_idx,
										jl_qos_vid_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_VID_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_vid_map2q_set, chip_id, eport, rule_idx, pmap);

	return ret;
}


jl_api_ret_t jl_qos_vid_2queue_get(const jl_uint32 chip_id,
										const jl_uint8 eport,
										const jl_uint8 rule_idx,
										jl_qos_vid_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_VID_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_vid_map2q_get, chip_id, eport, rule_idx, pmap);

	return ret;
}


jl_api_ret_t jl_qos_vid_2queue_del(const jl_uint32 chip_id,
										const jl_uint8 eport,
										const jl_uint8 rule_idx)
{
	jl_api_ret_t ret;
	jl_qos_vid_map_t map;

	JL_CHECK_ENUM(rule_idx, QOS_VID_TO_QUEUE_RULE_NUM);

	memset(&map, 0, sizeof(jl_qos_vid_map_t));
	map.queue = QOS_QUEUE_INVALID;

	JL_DRV_SAFE_CALL(ret, qos_vid_map2q_set, chip_id, eport, rule_idx, &map);

	return ret;
}

jl_api_ret_t jl_qos_ip_2queue_set(const jl_uint32 chip_id,
                                  const jl_uint8 eport,
                                  const jl_uint8 rule_idx,
                                  jl_qos_ip_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_IP_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_ip_map2q_set, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_ip_2queue_get(const jl_uint32 chip_id,
                                  const jl_uint8 eport,
                                  const jl_uint8 rule_idx,
                                  jl_qos_ip_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_IP_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_ip_map2q_get, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_ip_2queue_del(const jl_uint32 chip_id,
                                  const jl_uint8 eport,
                                  const jl_uint8 rule_idx)
{
	jl_api_ret_t ret;
	jl_qos_ip_map_t map;

	JL_CHECK_ENUM(rule_idx, QOS_IP_TO_QUEUE_RULE_NUM);

	memset(&map, 0, sizeof(jl_qos_ip_map_t));
	map.queue = QOS_QUEUE_INVALID;

	JL_DRV_SAFE_CALL(ret, qos_ip_map2q_set, chip_id, eport, rule_idx, &map);

	return ret;
}

jl_api_ret_t jl_qos_ethtype_2queue_set(const jl_uint32 chip_id,
                                       const jl_uint8 eport,
                                       const jl_uint8 rule_idx,
                                       jl_qos_ethtype_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_ETHTYPE_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_ethtype_map2q_set, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_ethtype_2queue_get(const jl_uint32 chip_id,
                                       const jl_uint8 eport,
                                       const jl_uint8 rule_idx,
                                       jl_qos_ethtype_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_ETHTYPE_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_ethtype_map2q_get, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_ethtype_2queue_del(const jl_uint32 chip_id,
                                       const jl_uint8 eport,
                                       const jl_uint8 rule_idx)
{
	jl_api_ret_t ret;
	jl_qos_ethtype_map_t map;

	JL_CHECK_ENUM(rule_idx, QOS_ETHTYPE_TO_QUEUE_RULE_NUM);
	map.queue = QOS_QUEUE_INVALID;

	JL_DRV_SAFE_CALL(ret, qos_ethtype_map2q_set, chip_id, eport, rule_idx, &map);

	return ret;
}

jl_api_ret_t jl_qos_tos_2queue_set(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 tos,
                                	const jl_uint8 queue)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(queue, QOS_QUEUE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_tos_map2q_set, chip_id, eport, tos, queue);

	return ret;
}

jl_api_ret_t jl_qos_tos_2queue_get(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 tos,
                                	jl_uint8 *pqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pqueue);

	JL_DRV_SAFE_CALL(ret, qos_tos_map2q_get, chip_id, eport, tos, pqueue);

	return ret;
}

jl_api_ret_t jl_qos_tos_2queue_del(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 tos)
{
	jl_api_ret_t ret;
	jl_uint8 queue = QOS_QUEUE_1;

	JL_DRV_SAFE_CALL(ret, qos_tos_map2q_set, chip_id, eport, tos, queue);

	return ret;
}

jl_api_ret_t jl_qos_exp_2queue_set(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 exp,
                                	const jl_uint8 queue)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(exp, QOS_PRIORITY_NUM);
	JL_CHECK_ENUM(queue, QOS_QUEUE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_exp_map2q_set, chip_id, eport, exp, queue);

	return ret;
}

jl_api_ret_t jl_qos_exp_2queue_get(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 exp,
                                	jl_uint8 *pqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pqueue);
	JL_CHECK_ENUM(exp, QOS_PRIORITY_NUM);

	JL_DRV_SAFE_CALL(ret, qos_exp_map2q_get, chip_id, eport, exp, pqueue);

	return ret;
}

jl_api_ret_t jl_qos_exp_2queue_del(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 exp)
{
	jl_api_ret_t ret;
	jl_uint8 queue = QOS_QUEUE_1;

	JL_CHECK_ENUM(exp, QOS_PRIORITY_NUM);

	JL_DRV_SAFE_CALL(ret, qos_exp_map2q_set, chip_id, eport, exp, queue);

	return ret;
}

jl_api_ret_t jl_qos_pcp_2queue_set(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 pcp,
                                	const jl_uint8 queue)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(pcp, QOS_PRIORITY_NUM);
	JL_CHECK_ENUM(queue, QOS_QUEUE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_pcp_map2q_set, chip_id, eport, pcp, queue);

	return ret;
}

jl_api_ret_t jl_qos_pcp_2queue_get(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 pcp,
                                	jl_uint8 *pqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pqueue);
	JL_CHECK_ENUM(pcp, QOS_PRIORITY_NUM);

	JL_DRV_SAFE_CALL(ret, qos_pcp_map2q_get, chip_id, eport, pcp, pqueue);

	return ret;
}

jl_api_ret_t jl_qos_pcp_2queue_del(const jl_uint32 chip_id,
									const jl_uint8 eport,
                                	const jl_uint8 pcp)
{
	jl_api_ret_t ret;
	jl_uint8 queue = QOS_QUEUE_1;

	JL_CHECK_ENUM(pcp, QOS_PRIORITY_NUM);

	JL_DRV_SAFE_CALL(ret, qos_pcp_map2q_set, chip_id, eport, pcp, queue);

	return ret;
}

jl_api_ret_t jl_qos_l4port_2queue_set(const jl_uint32 chip_id,
                                      const jl_uint8 eport,
                                      const jl_uint8 rule_idx,
                                      jl_qos_l4_port_range_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_L4_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_l4port_map2q_set, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_l4port_2queue_get(const jl_uint32 chip_id,
                                      const jl_uint8 eport,
                                      const jl_uint8 rule_idx,
                                      jl_qos_l4_port_range_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_L4_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_l4port_map2q_get, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_l4port_2queue_del(const jl_uint32 chip_id,
                                      const jl_uint8 eport,
                                      const jl_uint8 rule_idx)
{
	jl_api_ret_t ret;
	jl_qos_l4_port_range_map_t map;

	JL_CHECK_ENUM(rule_idx, QOS_L4_TO_QUEUE_RULE_NUM);

	memset(&map, 0, sizeof(jl_qos_l4_port_range_map_t));
	map.queue = QOS_QUEUE_INVALID;

	JL_DRV_SAFE_CALL(ret, qos_l4port_map2q_set, chip_id, eport, rule_idx, &map);

	return ret;
}

jl_api_ret_t jl_qos_l4protocol_2queue_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_l4_protocol_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_L4_TO_QUEUE_RULE_NUM);

	JL_DRV_SAFE_CALL(ret, qos_l4protocol_map2q_set, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_l4protocol_2queue_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx,
        jl_qos_l4_protocol_map_t *pmap)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pmap);
	JL_CHECK_ENUM(rule_idx, QOS_L4_TO_QUEUE_RULE_NUM);
	JL_DRV_SAFE_CALL(ret, qos_l4protocol_map2q_get, chip_id, eport, rule_idx, pmap);

	return ret;
}

jl_api_ret_t jl_qos_l4protocol_2queue_del(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 rule_idx)
{
	jl_api_ret_t ret;
	jl_qos_l4_protocol_map_t map;

	JL_CHECK_ENUM(rule_idx, QOS_L4_TO_QUEUE_RULE_NUM);
	memset(&map, 0, sizeof(jl_qos_l4_protocol_map_t));
	map.queue = QOS_QUEUE_INVALID;

	JL_DRV_SAFE_CALL(ret, qos_l4protocol_map2q_set, chip_id, eport, rule_idx, &map);

	return ret;
}

jl_api_ret_t jl_qos_nonvlan_2color_set(const jl_uint32 chip_id,
                                       const jl_uint8 color,
                                       const jl_uint8 enable)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(color, QOS_COLOR_NUM);

	JL_DRV_SAFE_CALL(ret, qos_nonvlan_map2color_set, chip_id, color, enable);

	return ret;
}

jl_api_ret_t jl_qos_nonvlan_2color_get(const jl_uint32 chip_id,
                                       jl_uint8 *pcolor,
                                       jl_uint8 *penable)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcolor);
	JL_CHECK_POINTER(penable);

	JL_DRV_SAFE_CALL(ret, qos_nonvlan_map2color_get, chip_id, pcolor, penable);

	return ret;
}

jl_api_ret_t jl_qos_unknownl3_2color_set(const jl_uint32 chip_id,
        const jl_uint8 color,
        const jl_uint8 enable)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(color, QOS_COLOR_NUM);

	JL_DRV_SAFE_CALL(ret, qos_unknownl3_map2color_set, chip_id, color, enable);

	return ret;
}

jl_api_ret_t jl_qos_unknownl3_2color_get(const jl_uint32 chip_id,
        jl_uint8 *pcolor,
        jl_uint8 *penable)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcolor);
	JL_CHECK_POINTER(penable);

	JL_DRV_SAFE_CALL(ret, qos_unknownl3_map2color_get, chip_id, pcolor, penable);

	return ret;
}

jl_api_ret_t jl_qos_tos_2color_set(const jl_uint32 chip_id,
                                   const jl_uint8 tos,
                                   const jl_uint8 color)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(color, QOS_COLOR_NUM);

	JL_DRV_SAFE_CALL(ret, qos_tos_map2color_set, chip_id, tos, color);

	return ret;
}

jl_api_ret_t jl_qos_tos_2color_get(const jl_uint32 chip_id,
                                   const jl_uint8 tos,
                                   jl_uint8 *pcolor)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcolor);

	JL_DRV_SAFE_CALL(ret, qos_tos_map2color_get, chip_id, tos, pcolor);

	return ret;
}

jl_api_ret_t jl_qos_exp_2color_set(const jl_uint32 chip_id,
                                   const jl_uint8 exp,
                                   const jl_uint8 color)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(exp, QOS_PRIORITY_NUM);
	JL_CHECK_ENUM(color, QOS_COLOR_NUM);

	JL_DRV_SAFE_CALL(ret, qos_exp_map2color_set, chip_id, exp, color);

	return ret;
}

jl_api_ret_t jl_qos_exp_2color_get(const jl_uint32 chip_id,
                                   const jl_uint8 exp,
                                   jl_uint8 *pcolor)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcolor);
	JL_CHECK_ENUM(exp, QOS_PRIORITY_NUM);

	JL_DRV_SAFE_CALL(ret, qos_exp_map2color_get, chip_id, exp, pcolor);

	return ret;
}

jl_api_ret_t jl_qos_pcpdei_2color_set(const jl_uint32 chip_id,
                                      const jl_uint8 pcp,
                                      const jl_uint8 dei,
                                      const jl_uint8 color)
{
	jl_api_ret_t ret;

	JL_CHECK_ENUM(pcp, QOS_PRIORITY_NUM);
	JL_CHECK_ENUM(color, QOS_COLOR_NUM);

	if (dei > 1)
		return JL_ERR_PARAM;

	JL_DRV_SAFE_CALL(ret, qos_pcpdei_map2color_set, chip_id, pcp, dei, color);

	return ret;
}

jl_api_ret_t jl_qos_pcpdei_2color_get(const jl_uint32 chip_id,
                                      const jl_uint8 pcp,
                                      const jl_uint8 dei,
                                      jl_uint8 *pcolor)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcolor);
	JL_CHECK_ENUM(pcp, QOS_PRIORITY_NUM);

	if (dei > 1)
		return JL_ERR_PARAM;

	JL_DRV_SAFE_CALL(ret, qos_pcpdei_map2color_get, chip_id, pcp, dei, pcolor);

	return ret;
}

jl_api_ret_t jl_qos_remap_vlan_set(const jl_uint32 chip_id,
                                   const jl_uint8 eport,
                                   jl_qos_remap_vlan_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);

	JL_DRV_SAFE_CALL(ret, qos_remap_vlan_set, chip_id, eport, pcfg);

	return ret;
}

jl_api_ret_t jl_qos_remap_vlan_get(const jl_uint32 chip_id,
                                   const jl_uint8 eport,
                                   jl_qos_remap_vlan_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);

	JL_DRV_SAFE_CALL(ret, qos_remap_vlan_get, chip_id, eport, pcfg);

	return ret;
}

jl_api_ret_t jl_qos_color_remap_set(const jl_uint32 chip_id,
                                    const jl_uint8 eport,
                                    jl_qos_color_remap_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);

	JL_DRV_SAFE_CALL(ret, qos_color_remap_set, chip_id, eport, pcfg);

	return ret;
}

jl_api_ret_t jl_qos_color_remap_get(const jl_uint32 chip_id,
                                    const jl_uint8 eport,
                                    jl_qos_color_remap_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);

	JL_DRV_SAFE_CALL(ret, qos_color_remap_get, chip_id, eport, pcfg);

	return ret;
}

jl_api_ret_t jl_qos_mmp_cfg_set(const jl_uint32 chip_id,
                                const jl_uint8 pointer,
                                jl_qos_mmp_cfg_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_ENUM(pointer, QOS_MMP_POINTER_NUM);

	JL_DRV_SAFE_CALL(ret, qos_mmp_cfg_set, chip_id, pointer, pcfg);

	return ret;
}

jl_api_ret_t jl_qos_mmp_cfg_get(const jl_uint32 chip_id,
                                const jl_uint8 pointer,
                                jl_qos_mmp_cfg_t *pcfg)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pcfg);
	JL_CHECK_ENUM(pointer, QOS_MMP_POINTER_NUM);

	JL_DRV_SAFE_CALL(ret, qos_mmp_cfg_get, chip_id, pointer, pcfg);

	return ret;
}


jl_api_ret_t jl_qos_mmp_unused_pointer_get(const jl_uint32 chip_id,
        jl_uint8 *ppointer)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(ppointer);

	JL_DRV_SAFE_CALL(ret, qos_mmp_unused_pointer_get, chip_id, ppointer);

	return ret;
}

jl_api_ret_t jl_qos_mmp_used_pointer_info_get(const jl_uint32 chip_id,
        const jl_uint8 pointer,
        jl_qos_mmp_used_info_t *ppointer_info)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(ppointer_info);
	JL_CHECK_ENUM(pointer, QOS_MMP_POINTER_NUM);

	JL_DRV_SAFE_CALL(ret, qos_mmp_used_pointer_info_get, chip_id, pointer, ppointer_info);

	return ret;
}

jl_api_ret_t jl_qos_ingress_admission_ctl_set(const jl_uint32 chip_id,
        const jl_uint8 inport,
        const jl_uint8 pcp,
        jl_qos_mmp_pointer_t *ppointer)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(ppointer);
	JL_CHECK_ENUM(pcp, QOS_PRIORITY_NUM);

	JL_DRV_SAFE_CALL(ret, qos_ingress_initial_mmp_set, chip_id, inport, pcp, ppointer);

	return ret;
}

jl_api_ret_t jl_qos_ingress_admission_ctl_get(const jl_uint32 chip_id,
        const jl_uint8 inport,
        const jl_uint8 pcp,
        jl_qos_mmp_pointer_t *ppointer)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(ppointer);
	JL_CHECK_ENUM(pcp, QOS_PRIORITY_NUM);

	JL_DRV_SAFE_CALL(ret, qos_ingress_initial_mmp_get, chip_id, inport, pcp, ppointer);

	return ret;
}

jl_api_ret_t jl_qos_queue_map_trust_l3_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        const jl_uint8 enable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, qos_queue_map_priority_set, chip_id, eport, enable);

	return ret;
}

jl_api_ret_t jl_qos_queue_map_trust_l3_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_uint8 *penable)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(penable);

	JL_DRV_SAFE_CALL(ret, qos_queue_map_priority_get, chip_id, eport, penable);

	return ret;
}

jl_api_ret_t jl_qos_color_map_trust_l3_set(const jl_uint32 chip_id,
        const jl_uint8 inport,
        const jl_uint8 enable)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, qos_color_map_priority_set, chip_id, inport, enable);

	return ret;
}

jl_api_ret_t jl_qos_color_map_trust_l3_get(const jl_uint32 chip_id,
        const jl_uint8 inport,
        jl_uint8 *penable)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(penable);

	JL_DRV_SAFE_CALL(ret, qos_color_map_priority_get, chip_id, inport, penable);

	return ret;
}

jl_api_ret_t jl_qos_queue_priority_set(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_queue_priority_t *pqueue_pri)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pqueue_pri);

	JL_DRV_SAFE_CALL(ret, qos_priority_to_queue_set, chip_id, eport, pqueue_pri);

	return ret;
}

jl_api_ret_t jl_qos_queue_priority_get(const jl_uint32 chip_id,
        const jl_uint8 eport,
        jl_qos_queue_priority_t *pqueue_pri)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pqueue_pri);

	JL_DRV_SAFE_CALL(ret, qos_priority_to_queue_get, chip_id, eport, pqueue_pri);

	return ret;
}

jl_api_ret_t jl_qos_queue_shaper_set(const jl_uint32 chip_id,
                                     const jl_uint8 eport,
                                     jl_qos_queue_shaper_t *pshaper)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pshaper);

	JL_DRV_SAFE_CALL(ret, qos_queue_shaper_set, chip_id, eport, pshaper);

	return ret;
}

jl_api_ret_t jl_qos_queue_shaper_get(const jl_uint32 chip_id,
                                     const jl_uint8 eport,
                                     jl_qos_queue_shaper_t *pshaper)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pshaper);

	JL_DRV_SAFE_CALL(ret, qos_queue_shaper_get, chip_id, eport, pshaper);

	return ret;
}

jl_api_ret_t jl_qos_priority_shaper_set(const jl_uint32 chip_id,
                                        const jl_uint8 eport,
                                        jl_qos_priority_shaper_t *pshaper)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pshaper);

	JL_DRV_SAFE_CALL(ret, qos_priority_shaper_set, chip_id, eport, pshaper);

	return ret;
}

jl_api_ret_t jl_qos_priority_shaper_get(const jl_uint32 chip_id,
                                        const jl_uint8 eport,
                                        jl_qos_priority_shaper_t *pshaper)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pshaper);

	JL_DRV_SAFE_CALL(ret, qos_priority_shaper_get, chip_id, eport, pshaper);

	return ret;
}

jl_api_ret_t jl_qos_port_shaper_set(const jl_uint32 chip_id,
                                        const jl_uint8 eport,
                                        jl_qos_port_shaper_t *pshaper)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pshaper);

	JL_DRV_SAFE_CALL(ret, qos_port_shaper_set, chip_id, eport, pshaper);

	return ret;
}

jl_api_ret_t jl_qos_port_shaper_get(const jl_uint32 chip_id,
                                        const jl_uint8 eport,
                                        jl_qos_port_shaper_t *pshaper)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pshaper);

	JL_DRV_SAFE_CALL(ret, qos_port_shaper_get, chip_id, eport, pshaper);

	return ret;
}

jl_api_ret_t jl_qos_dwrr_weight_set(const jl_uint32 chip_id,
                                   const jl_uint8 eport,
                                   jl_qos_queue_dwrr_t *pdwrr)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pdwrr);

	JL_DRV_SAFE_CALL(ret, qos_queue_dwrr_set, chip_id, eport, pdwrr);

	return ret;
}

jl_api_ret_t jl_qos_dwrr_weight_get(const jl_uint32 chip_id,
                                   const jl_uint8 eport,
                                   jl_qos_queue_dwrr_t *pdwrr)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(pdwrr);

	JL_DRV_SAFE_CALL(ret, qos_queue_dwrr_get, chip_id, eport, pdwrr);

	return ret;
}

jl_api_ret_t jl_qos_enqueue_enable_set(const jl_uint32 chip_id,
                                       const jl_uint8 eport,
                                       jl_qos_enqueue_state_t *penqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(penqueue);

	JL_DRV_SAFE_CALL(ret, qos_enqueue_enable_set, chip_id, eport, penqueue);

	return ret;
}

jl_api_ret_t jl_qos_enqueue_enable_get(const jl_uint32 chip_id,
                                       const jl_uint8 eport,
                                       jl_qos_enqueue_state_t *penqueue)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(penqueue);

	JL_DRV_SAFE_CALL(ret, qos_enqueue_enable_get, chip_id, eport, penqueue);

	return ret;
}

jl_api_ret_t jl_qos_schedule_set(const jl_uint32 chip_id,
								const jl_uint8 eport,
								jl_qos_schedule_t *psche)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, qos_schedule_set, chip_id, eport, psche);

	return ret;
}

jl_api_ret_t jl_qos_schedule_get(const jl_uint32 chip_id,
								const jl_uint8 eport,
								jl_qos_schedule_t *psche)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, qos_schedule_get, chip_id, eport, psche);

	return ret;
}

jl_api_ret_t jl_qos_res_init(const jl_uint32 chip_id, const jl_uint8 queue_num)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, qos_res_init, chip_id, queue_num);

	return ret;
}

jl_api_ret_t jl_qos_res_deinit(const jl_uint32 chip_id)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, qos_res_deinit, chip_id);

	return ret;
}
