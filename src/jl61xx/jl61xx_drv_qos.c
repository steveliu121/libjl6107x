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

#include "jl61xx/jl61xx_drv_qos.h"
#include "jl61xx/jl61xx_drv_switch.h"
#include "jl61xx/jl61xx_chip.h"

static jl_ret_t jl61xx_qos_drain_port(jl_device_t *dev, jl_uint32 port_mask);
static jl_ret_t jl61xx_qos_recover_port(jl_device_t *dev, jl_uint32 port_mask);
static jl_ret_t jl61xx_qos_check_port_empty(jl_device_t *dev, jl_uint32 port_mask);
static jl_ret_t jl61xx_qos_res_manage_mode_set(jl_device_t *dev, jl_uint8 index, jl_uint8 mode);

static jl_uint16 g_rsvd_limiter_xon[JL_MAX_CHIP_NUM][QOS_RESOURCE_MANAGEMENT_POINTER_NUM];
static jl_uint16 g_rsvd_limit_threshold[JL_MAX_CHIP_NUM][QOS_RESOURCE_MANAGEMENT_POINTER_NUM];

static jl61xx_qos_res_management_t *gp_res_mag_grt[JL_MAX_CHIP_NUM] = {0};

jl_ret_t jl61xx_qos_map_2queue_nonvlan_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 queue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;

	SWCORE_FORCE_NON_VLAN_PACKET_TO_SPECIFIC_EGRESS_QUEUE_t map_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_PORT(dev, eport);

	memset(&map_tbl, 0, sizeof(SWCORE_FORCE_NON_VLAN_PACKET_TO_SPECIFIC_EGRESS_QUEUE_t));

	if (queue != QOS_QUEUE_INVALID)
	{
		map_tbl.BF.force_queue = 1;
		map_tbl.BF.e_queue = queue;
	}

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_WRITE(dev, SWCORE,FORCE_NON_VLAN_PACKET_TO_SPECIFIC_EGRESS_QUEUE, map_tbl, cport, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_nonvlan_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 *pqueue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pqueue);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, FORCE_NON_VLAN_PACKET_TO_SPECIFIC_EGRESS_QUEUE, map_tbl, cport, 0);

	if (map_tbl.BF.force_queue)
		*pqueue = map_tbl.BF.e_queue;
	else
		*pqueue = QOS_QUEUE_INVALID;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_unknown_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 queue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;

	SWCORE_FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_EGRESS_QUEUE_t map_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_PORT(dev, eport);

	memset(&map_tbl, 0, sizeof(SWCORE_FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_EGRESS_QUEUE_t));

	if (queue != QOS_QUEUE_INVALID)
	{
		map_tbl.BF.force_queue = 1;
		map_tbl.BF.e_queue = queue;
	}

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_WRITE(dev, SWCORE,FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_EGRESS_QUEUE, map_tbl, cport, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_unknown_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 *pqueue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pqueue);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_EGRESS_QUEUE, map_tbl, cport, 0);

	if (map_tbl.BF.force_queue)
		*pqueue = map_tbl.BF.e_queue;
	else
		*pqueue = QOS_QUEUE_INVALID;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_mac_set(jl_device_t *dev,
                                       jl_uint8 eport,
                                       jl_uint8 rule_idx,
                                       jl_qos_mac_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_index = 0;
	jl_uint8 i = 0;
	jl_uint32 cport = 0;
	jl_uint64 val, mac_val = 0, mac_msk = 0;
	SWCORE_DA_OR_SA_MAC_TO_QUEUE_ASSIGNMENT_t  map_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		JL_CHECK_ENUM(pmap->queue, QOS_QUEUE_NUM);
	}

	memset(&map_tbl, 0, sizeof(SWCORE_DA_OR_SA_MAC_TO_QUEUE_ASSIGNMENT_t));

	cport = jlxx_port_l2c(dev, eport);

	tbl_index = cport * 4 + rule_idx;

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		for (i = 0; i < 6; i++)
		{
			val = pmap->mac.value.addr[i];
			val <<= ((5 - i) * 8);
			mac_val |= val;
			val = pmap->mac.mask.addr[i];
			val <<= ((5 - i) * 8);
			mac_msk |= val;
		}

		map_tbl.BF.mac_0_31 =  mac_val & 0xffffffff;

		map_tbl.BF.mac_32_47 = mac_val >> SWCORE_DA_OR_SA_MAC_TO_QUEUE_ASSIGNMENT_OFFSET_MAC_0_31_WIDTH;

		map_tbl.BF.mask_0_15 = mac_msk & 0xffff;

		map_tbl.BF.mask_16_47 = mac_msk >> SWCORE_DA_OR_SA_MAC_TO_QUEUE_ASSIGNMENT_OFFSET_MASK_0_15_WIDTH;

		map_tbl.BF.sa_or_da = (pmap->mac_type == QOS_DIR_SOURCE) ? 0 : 1;
		map_tbl.BF.force = 1;
		map_tbl.BF.queue = pmap->queue;
	}
	else
	{
		map_tbl.BF.mask_0_15 = 0xffff;
		map_tbl.BF.mask_16_47 = 0xffffffff;
	}

	REGISTER_WRITE(dev, SWCORE, DA_OR_SA_MAC_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_index, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_mac_get(jl_device_t *dev,
                                       jl_uint8 eport,
                                       jl_uint8 rule_idx,
                                       jl_qos_mac_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_index = 0;
	jl_uint32 cport = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	tbl_index = cport * 4 + rule_idx;

	REGISTER_READ(dev, SWCORE, DA_OR_SA_MAC_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_index, 0);

	pmap->mac.value.addr[0] = (map_tbl.BF.mac_32_47 >> 8) & 0xff;
	pmap->mac.value.addr[1] = (map_tbl.BF.mac_32_47 >> 0) & 0xff;
	pmap->mac.value.addr[2] = (map_tbl.BF.mac_0_31 >> 24) & 0xff;
	pmap->mac.value.addr[3] = (map_tbl.BF.mac_0_31 >> 16) & 0xff;
	pmap->mac.value.addr[4] = (map_tbl.BF.mac_0_31 >> 8) & 0xff;
	pmap->mac.value.addr[5] = (map_tbl.BF.mac_0_31 >> 0) & 0xff;

	pmap->mac.mask.addr[0] = (map_tbl.BF.mask_16_47 >> 24) & 0xff;
	pmap->mac.mask.addr[1] = (map_tbl.BF.mask_16_47 >> 16) & 0xff;
	pmap->mac.mask.addr[2] = (map_tbl.BF.mask_16_47 >> 8) & 0xff;
	pmap->mac.mask.addr[3] = (map_tbl.BF.mask_16_47 >> 0) & 0xff;
	pmap->mac.mask.addr[4] = (map_tbl.BF.mask_0_15 >> 8) & 0xff;
	pmap->mac.mask.addr[5] = (map_tbl.BF.mask_0_15 >> 0) & 0xff;

	pmap->mac_type = (map_tbl.BF.sa_or_da == 0) ? QOS_DIR_SOURCE : QOS_DIR_DEST;

	if (map_tbl.BF.force)
		pmap->queue = map_tbl.BF.queue;
	else
		pmap->queue = QOS_QUEUE_INVALID;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_vid_set(jl_device_t *dev,
							jl_uint8 eport,
							jl_uint8 rule_idx,
							jl_qos_vid_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_index = 0;
	jl_uint32 cport = 0;
	SWCORE_VID_TO_QUEUE_ASSIGNMENT_t  map_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		JL_CHECK_ENUM(pmap->queue, QOS_QUEUE_NUM);
	}

	memset(&map_tbl, 0, sizeof(SWCORE_VID_TO_QUEUE_ASSIGNMENT_t));

	if (pmap->vid > 0xfff ||
		pmap->inner_outer > 1 ||
		pmap->cstag > 1)
		return JL_ERR_PARAM;

	cport = jlxx_port_l2c(dev, eport);

	tbl_index = cport * 4 + rule_idx;

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		map_tbl.BF.vid = (pmap->vid & pmap->mask);
		map_tbl.BF.mask = pmap->mask;
		map_tbl.BF.inner_outer = (pmap->inner_outer == 0) ? 0 : 1;
		map_tbl.BF.cstag = (pmap->cstag == 0) ? 0 : 1;
		map_tbl.BF.force = 1;
		map_tbl.BF.queue = pmap->queue;
	}
	else
		map_tbl.BF.mask = 0xfff;
		
	REGISTER_WRITE(dev, SWCORE, VID_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_index, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_vid_get(jl_device_t *dev,
							jl_uint8 eport,
							jl_uint8 rule_idx,
							jl_qos_vid_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_index = 0;
	jl_uint32 cport = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	tbl_index = cport * 4 + rule_idx;

	REGISTER_READ(dev, SWCORE, VID_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_index, 0);

	pmap->vid = map_tbl.BF.vid;
	pmap->mask = map_tbl.BF.mask;
	pmap->inner_outer = map_tbl.BF.inner_outer;
	pmap->cstag = map_tbl.BF.cstag;
	
	if (map_tbl.BF.force)
		pmap->queue = map_tbl.BF.queue;
	else
		pmap->queue = QOS_QUEUE_INVALID;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_ip_set(jl_device_t *dev,
                                      jl_uint8 eport,
                                      jl_uint8 rule_idx,
                                      jl_qos_ip_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport = 0;
	jl_uint32 val, v4_addr = 0, v4_mask = 0;
	jl_uint8 tbl_index = 0;
	jl_uint8 i = 0;
	SWCORE_IP_ADDRESS_TO_QUEUE_ASSIGNMENT_t  map_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		JL_CHECK_ENUM(pmap->queue, QOS_QUEUE_NUM);
	}

	memset(&map_tbl, 0, sizeof(SWCORE_IP_ADDRESS_TO_QUEUE_ASSIGNMENT_t));

	if (pmap->v4_v6 > QOS_IP_V6 || pmap->sa_da > QOS_DIR_DEST)
		return JL_ERR_PARAM;

	cport = jlxx_port_l2c(dev, eport);

	tbl_index = cport * 4 + rule_idx;

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		map_tbl.BF.sa_or_da = (pmap->sa_da == QOS_DIR_SOURCE) ? 0 : 1;
		map_tbl.BF.ipv4_or_ipv6 = (pmap->v4_v6 == QOS_IP_V4) ? 0 : 1;

		if (map_tbl.BF.ipv4_or_ipv6 == QOS_IP_V4) {

			for (i = 0; i < 4; i++)
			{
				val = (jl_uint32)pmap->ip.ipv4.addr[i] << ((3 - i) * 8);
				v4_addr |= val;
				val = (jl_uint32)pmap->ip.ipv4.mask[i] << ((3 - i) * 8);
				v4_mask |= val;
			}

			map_tbl.BF.ip_0_31 = v4_addr;
			map_tbl.BF.ip_32_63 = 0;
			map_tbl.BF.ip_64_95 = 0;
			map_tbl.BF.ip_96_127 = 0;
			map_tbl.BF.mask_0_31 = v4_mask;
			map_tbl.BF.mask_32_63 = 0xffffffff;
			map_tbl.BF.mask_64_95 = 0xffffffff;
			map_tbl.BF.mask_96_127 = 0xffffffff;
		} else {
			map_tbl.BF.ip_0_31 = *(jl_uint32*)&pmap->ip.ipv6.addr[0];
			map_tbl.BF.ip_32_63 = *(jl_uint32*)&pmap->ip.ipv6.addr[2];
			map_tbl.BF.ip_64_95 = *(jl_uint32*)&pmap->ip.ipv6.addr[4];
			map_tbl.BF.ip_96_127 = *(jl_uint32*)&pmap->ip.ipv6.addr[6];

			map_tbl.BF.mask_0_31 = *(jl_uint32*)&pmap->ip.ipv6.mask[0];
			map_tbl.BF.mask_32_63 = *(jl_uint32*)&pmap->ip.ipv6.mask[2];
			map_tbl.BF.mask_64_95 = *(jl_uint32*)&pmap->ip.ipv6.mask[4];
			map_tbl.BF.mask_96_127 = *(jl_uint32*)&pmap->ip.ipv6.mask[6];
		}

		map_tbl.BF.force = 1;
		map_tbl.BF.queue = pmap->queue;
	}
	else
	{
		map_tbl.BF.mask_0_31 = 0xffffffff;
		map_tbl.BF.mask_32_63 = 0xffffffff;
		map_tbl.BF.mask_64_95 = 0xffffffff;
		map_tbl.BF.mask_96_127 = 0xffffffff;
	}

	REGISTER_WRITE(dev, SWCORE, IP_ADDRESS_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_index, 0);
	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_ip_get(jl_device_t *dev,
                                      jl_uint8 eport,
                                      jl_uint8 rule_idx,
                                      jl_qos_ip_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_index = 0;
	jl_uint8 i = 0;
	jl_uint32 cport;
	jl_uint32 val;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	memset(pmap, 0, sizeof(jl_qos_ip_map_t));

	cport = jlxx_port_l2c(dev, eport);

	tbl_index = cport * 4 + rule_idx;

	REGISTER_READ(dev, SWCORE, IP_ADDRESS_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_index, 0);

	pmap->sa_da = map_tbl.BF.sa_or_da;
	pmap->v4_v6 = map_tbl.BF.ipv4_or_ipv6;

	if (pmap->v4_v6 == 0) {
		for (i = 0; i < 4; i++)
		{
			val =  (map_tbl.BF.ip_0_31 >> ((3 - i) * 8));
			pmap->ip.ipv4.addr[i] = val & 0xff;

			val =  (map_tbl.BF.mask_0_31 >> ((3 - i) * 8));
			pmap->ip.ipv4.mask[i] = val & 0xff;
		}
	} else {
		*(jl_uint32*)&pmap->ip.ipv6.addr[0] = map_tbl.BF.ip_0_31;
		*(jl_uint32*)&pmap->ip.ipv6.addr[2] = map_tbl.BF.ip_32_63;
		*(jl_uint32*)&pmap->ip.ipv6.addr[4] = map_tbl.BF.ip_64_95;
		*(jl_uint32*)&pmap->ip.ipv6.addr[6] = map_tbl.BF.ip_96_127;

		*(jl_uint32*)&pmap->ip.ipv6.mask[0] = map_tbl.BF.mask_0_31;
		*(jl_uint32*)&pmap->ip.ipv6.mask[2] = map_tbl.BF.mask_32_63;
		*(jl_uint32*)&pmap->ip.ipv6.mask[4] = map_tbl.BF.mask_64_95;
		*(jl_uint32*)&pmap->ip.ipv6.mask[6] = map_tbl.BF.mask_96_127;
	}

	if (map_tbl.BF.force)
		pmap->queue = map_tbl.BF.queue;
	else
		pmap->queue = QOS_QUEUE_INVALID;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_ethtype_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_ethtype_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_index = 0;
	jl_uint32 cport;
	SWCORE_ETHERNET_TYPE_TO_QUEUE_ASSIGNMENT_t  map_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		JL_CHECK_ENUM(pmap->queue, QOS_QUEUE_NUM);
	}

	memset(&map_tbl, 0, sizeof(SWCORE_ETHERNET_TYPE_TO_QUEUE_ASSIGNMENT_t));

	cport = jlxx_port_l2c(dev, eport);

	tbl_index = cport * 4 + rule_idx;

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		map_tbl.BF.eth_type = pmap->eth_type;

		map_tbl.BF.force = 1;

		map_tbl.BF.queue = pmap->queue;
	}

	REGISTER_WRITE(dev, SWCORE, ETHERNET_TYPE_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_index, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_ethtype_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_ethtype_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_index = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	tbl_index = cport * 4 + rule_idx;

	REGISTER_READ(dev, SWCORE, ETHERNET_TYPE_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_index, 0);

	pmap->eth_type = map_tbl.BF.eth_type;

	if (map_tbl.BF.force)
		pmap->queue = map_tbl.BF.queue;
	else
		pmap->queue = QOS_QUEUE_INVALID;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_tos_set(jl_device_t *dev,
										jl_uint8 eport,
                                    	jl_uint16 tos,
                                    	jl_uint8 queue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_PORT(dev, eport);

	REGISTER_READ(dev, SWCORE, IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, tos, 0);

	cport = jlxx_port_l2c(dev, eport);

	switch (cport)
	{
		case 0:
			map_tbl.BF.p_queue_port_0 = queue;
			break;
		case 1:
			map_tbl.BF.p_queue_port_1 = queue;
			break;
		case 2:
			map_tbl.BF.p_queue_port_2 = queue;
			break;
		case 3:
			map_tbl.BF.p_queue_port_3 = queue;
			break;
		case 4:
			map_tbl.BF.p_queue_port_4 = queue;
			break;
		case 5:
			map_tbl.BF.p_queue_port_5 = queue;
			break;
		case 6:
			map_tbl.BF.p_queue_port_6 = queue;
			break;
		case 7:
			map_tbl.BF.p_queue_port_7 = queue;
			break;
		case 8:
			map_tbl.BF.p_queue_port_8 = queue;
			break;
		case 9:
			map_tbl.BF.p_queue_port_9 = queue;
			break;
		case 10:
			map_tbl.BF.p_queue_port_10_0_1 = queue & 0x3;
			map_tbl.BF.p_queue_port_10_2 = (queue >> 2) & 0x1;
			break;
		default:
			break;
	}

	REGISTER_WRITE(dev, SWCORE, IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, tos, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_tos_get(jl_device_t *dev,
										jl_uint8 eport,
                                    	jl_uint16 tos,
                                    	jl_uint8 *pqueue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pqueue);
	JL_CHECK_PORT(dev, eport);

	REGISTER_READ(dev, SWCORE, IP_TOS_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, tos, 0);

	cport = jlxx_port_l2c(dev, eport);

	switch (cport)
	{
		case 0:
			*pqueue = map_tbl.BF.p_queue_port_0;
			break;
		case 1:
			*pqueue = map_tbl.BF.p_queue_port_1;
			break;
		case 2:
			*pqueue = map_tbl.BF.p_queue_port_2;
			break;
		case 3:
			*pqueue = map_tbl.BF.p_queue_port_3;
			break;
		case 4:
			*pqueue = map_tbl.BF.p_queue_port_4;
			break;
		case 5:
			*pqueue = map_tbl.BF.p_queue_port_5;
			break;
		case 6:
			*pqueue = map_tbl.BF.p_queue_port_6;
			break;
		case 7:
			*pqueue = map_tbl.BF.p_queue_port_7;
			break;
		case 8:
			*pqueue = map_tbl.BF.p_queue_port_8;
			break;
		case 9:
			*pqueue = map_tbl.BF.p_queue_port_9;
			break;
		case 10:
			*pqueue = map_tbl.BF.p_queue_port_10_0_1 + \
							(map_tbl.BF.p_queue_port_10_2 << 2);
			break;
		default:
			break;
	}	

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_exp_set(jl_device_t *dev,
										jl_uint8 eport,
                                    	jl_uint8 exp,
                                    	jl_uint8 queue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_PORT(dev, eport);

	REGISTER_READ(dev, SWCORE, MPLS_EXP_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, exp, 0);

	cport = jlxx_port_l2c(dev, eport);

	switch (cport)
	{
		case 0:
			map_tbl.BF.p_queue_port_0 = queue;
			break;
		case 1:
			map_tbl.BF.p_queue_port_1 = queue;
			break;
		case 2:
			map_tbl.BF.p_queue_port_2 = queue;
			break;
		case 3:
			map_tbl.BF.p_queue_port_3 = queue;
			break;
		case 4:
			map_tbl.BF.p_queue_port_4 = queue;
			break;
		case 5:
			map_tbl.BF.p_queue_port_5 = queue;
			break;
		case 6:
			map_tbl.BF.p_queue_port_6 = queue;
			break;
		case 7:
			map_tbl.BF.p_queue_port_7 = queue;
			break;
		case 8:
			map_tbl.BF.p_queue_port_8 = queue;
			break;
		case 9:
			map_tbl.BF.p_queue_port_9 = queue;
			break;
		case 10:
			map_tbl.BF.p_queue_port_10_0_1 = queue & 0x3;
			map_tbl.BF.p_queue_port_10_2 = (queue >> 2) & 0x1;
			break;
		default:
			break;
	}

	REGISTER_WRITE(dev, SWCORE, MPLS_EXP_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, exp, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_exp_get(jl_device_t *dev,
										jl_uint8 eport,
                                    	jl_uint8 exp,
                                    	jl_uint8 *pqueue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pqueue);
	JL_CHECK_PORT(dev, eport);

	REGISTER_READ(dev, SWCORE, MPLS_EXP_FIELD_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, exp, 0);

	cport = jlxx_port_l2c(dev, eport);

	switch (cport)
	{
		case 0:
			*pqueue = map_tbl.BF.p_queue_port_0;
			break;
		case 1:
			*pqueue = map_tbl.BF.p_queue_port_1;
			break;
		case 2:
			*pqueue = map_tbl.BF.p_queue_port_2;
			break;
		case 3:
			*pqueue = map_tbl.BF.p_queue_port_3;
			break;
		case 4:
			*pqueue = map_tbl.BF.p_queue_port_4;
			break;
		case 5:
			*pqueue = map_tbl.BF.p_queue_port_5;
			break;
		case 6:
			*pqueue = map_tbl.BF.p_queue_port_6;
			break;
		case 7:
			*pqueue = map_tbl.BF.p_queue_port_7;
			break;
		case 8:
			*pqueue = map_tbl.BF.p_queue_port_8;
			break;
		case 9:
			*pqueue = map_tbl.BF.p_queue_port_9;
			break;
		case 10:
			*pqueue = map_tbl.BF.p_queue_port_10_0_1 + \
							(map_tbl.BF.p_queue_port_10_2 << 2);
			break;
		default:
			break;
	}	

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_pcp_set(jl_device_t *dev,
										jl_uint8 eport,
                                    	jl_uint8 pcp,
                                    	jl_uint8 queue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_PORT(dev, eport);

	REGISTER_READ(dev, SWCORE, VLAN_PCP_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, pcp, 0);

	cport = jlxx_port_l2c(dev, eport);

	switch (cport)
	{
		case 0:
			map_tbl.BF.p_queue_port_0 = queue;
			break;
		case 1:
			map_tbl.BF.p_queue_port_1 = queue;
			break;
		case 2:
			map_tbl.BF.p_queue_port_2 = queue;
			break;
		case 3:
			map_tbl.BF.p_queue_port_3 = queue;
			break;
		case 4:
			map_tbl.BF.p_queue_port_4 = queue;
			break;
		case 5:
			map_tbl.BF.p_queue_port_5 = queue;
			break;
		case 6:
			map_tbl.BF.p_queue_port_6 = queue;
			break;
		case 7:
			map_tbl.BF.p_queue_port_7 = queue;
			break;
		case 8:
			map_tbl.BF.p_queue_port_8 = queue;
			break;
		case 9:
			map_tbl.BF.p_queue_port_9 = queue;
			break;
		case 10:
			map_tbl.BF.p_queue_port_10_0_1 = queue & 0x3;
			map_tbl.BF.p_queue_port_10_2 = (queue >> 2) & 0x1;
			break;
		default:
			break;
	}

	REGISTER_WRITE(dev, SWCORE, VLAN_PCP_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, pcp, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_pcp_get(jl_device_t *dev,
										jl_uint8 eport,
                                    	jl_uint8 pcp,
                                    	jl_uint8 *pqueue)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pqueue);
	JL_CHECK_PORT(dev, eport);

	REGISTER_READ(dev, SWCORE, VLAN_PCP_TO_EGRESS_QUEUE_MAPPING_TABLE, map_tbl, pcp, 0);

	cport = jlxx_port_l2c(dev, eport);

	switch (cport)
	{
		case 0:
			*pqueue = map_tbl.BF.p_queue_port_0;
			break;
		case 1:
			*pqueue = map_tbl.BF.p_queue_port_1;
			break;
		case 2:
			*pqueue = map_tbl.BF.p_queue_port_2;
			break;
		case 3:
			*pqueue = map_tbl.BF.p_queue_port_3;
			break;
		case 4:
			*pqueue = map_tbl.BF.p_queue_port_4;
			break;
		case 5:
			*pqueue = map_tbl.BF.p_queue_port_5;
			break;
		case 6:
			*pqueue = map_tbl.BF.p_queue_port_6;
			break;
		case 7:
			*pqueue = map_tbl.BF.p_queue_port_7;
			break;
		case 8:
			*pqueue = map_tbl.BF.p_queue_port_8;
			break;
		case 9:
			*pqueue = map_tbl.BF.p_queue_port_9;
			break;
		case 10:
			*pqueue = map_tbl.BF.p_queue_port_10_0_1 + \
							(map_tbl.BF.p_queue_port_10_2 << 2);
			break;
		default:
			break;
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_l4port_range_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_l4_port_range_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_idx = 0;
	jl_uint32 cport = 0;
	SWCORE_L4_PORT_RANGE_TO_QUEUE_ASSIGNMENT_t  map_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		JL_CHECK_ENUM(pmap->queue, QOS_QUEUE_NUM);
	}

	memset(&map_tbl, 0, sizeof(SWCORE_L4_PORT_RANGE_TO_QUEUE_ASSIGNMENT_t));

	if (pmap->start_port > pmap->end_port)
		return JL_ERR_PARAM;

	cport = jlxx_port_l2c(dev, eport);

	tbl_idx = cport * 4 + rule_idx;

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		map_tbl.BF.start_port = pmap->start_port;

		map_tbl.BF.end_port = pmap->end_port;

		map_tbl.BF.sp_or_dp = (pmap->source_dest == QOS_DIR_SOURCE) ? 0 : 1;
		map_tbl.BF.udp_tcp = (pmap->udp_tcp == QOS_L4_UDP) ? 0 : 1;

		map_tbl.BF.force = 1;
		map_tbl.BF.queue = pmap->queue;
	}

	REGISTER_WRITE(dev, SWCORE, L4_PORT_RANGE_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_idx, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_l4port_range_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_l4_port_range_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_idx = 0;
	jl_uint32 cport = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	tbl_idx = cport * 4 + rule_idx;

	REGISTER_READ(dev, SWCORE, L4_PORT_RANGE_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_idx, 0);

	pmap->start_port = map_tbl.BF.start_port;
	pmap->end_port = map_tbl.BF.end_port;
	pmap->source_dest = map_tbl.BF.sp_or_dp;
	pmap->udp_tcp = map_tbl.BF.udp_tcp;

	if (map_tbl.BF.force)
		pmap->queue = map_tbl.BF.queue;
	else
		pmap->queue = QOS_QUEUE_INVALID;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_l4_protocol_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_l4_protocol_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_idx = 0;
	jl_uint32 cport = 0;
	SWCORE_L4_PROTOCOL_TO_QUEUE_ASSIGNMENT_t  map_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		JL_CHECK_ENUM(pmap->queue, QOS_QUEUE_NUM);
	}

	memset(&map_tbl, 0, sizeof(SWCORE_L4_PROTOCOL_TO_QUEUE_ASSIGNMENT_t));

	cport = jlxx_port_l2c(dev, eport);

	tbl_idx = cport * 4 + rule_idx;

	if (pmap->queue != QOS_QUEUE_INVALID)
	{
		map_tbl.BF.proto = pmap->protocol;
		map_tbl.BF.force = 1;
		map_tbl.BF.queue = pmap->queue;
	}

	REGISTER_WRITE(dev, SWCORE, L4_PROTOCOL_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_idx, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2queue_l4_protocol_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 rule_idx,
        jl_qos_l4_protocol_map_t *pmap)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_idx = 0;
	jl_uint32 cport = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pmap);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	tbl_idx = cport * 4 + rule_idx;

	REGISTER_READ(dev, SWCORE, L4_PROTOCOL_TO_QUEUE_ASSIGNMENT, map_tbl, tbl_idx, 0);

	pmap->protocol = map_tbl.BF.proto;

	if (map_tbl.BF.force)
		pmap->queue = map_tbl.BF.queue;
	else
		pmap->queue = QOS_QUEUE_INVALID;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_nonvlan_set(jl_device_t *dev,
        jl_uint8 color,
        jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_FORCE_NON_VLAN_PACKET_TO_SPECIFIC_COLOR_t color_tbl;

	JL_CHECK_POINTER(dev);

	memset(&color_tbl, 0, sizeof(SWCORE_FORCE_NON_VLAN_PACKET_TO_SPECIFIC_COLOR_t));

	color_tbl.BF.force_color = (enable == 0) ? 0 : 1;
	color_tbl.BF.color = color;

	REGISTER_WRITE(dev, SWCORE, FORCE_NON_VLAN_PACKET_TO_SPECIFIC_COLOR, color_tbl, 0, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_nonvlan_get(jl_device_t *dev,
        jl_uint8 *pcolor,
        jl_uint8 *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcolor);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(dev, SWCORE, FORCE_NON_VLAN_PACKET_TO_SPECIFIC_COLOR, color_tbl, 0, 0);

	*penable = color_tbl.BF.force_color;
	*pcolor = color_tbl.BF.color;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_unknown_set(jl_device_t *dev,
        jl_uint8 color,
        jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;
	SWCORE_FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_COLOR_t color_tbl;

	JL_CHECK_POINTER(dev);

	memset(&color_tbl, 0, sizeof(SWCORE_FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_COLOR_t));

	color_tbl.BF.force_color = (enable == 0) ? 0 : 1;

	color_tbl.BF.color = color;

	REGISTER_WRITE(dev, SWCORE, FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_COLOR, color_tbl, 0, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_unknown_get(jl_device_t *dev,
        jl_uint8 *pcolor,
        jl_uint8 *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcolor);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(dev, SWCORE, FORCE_UNKNOWN_L3_PACKET_TO_SPECIFIC_COLOR, color_tbl, 0, 0);

	*penable = color_tbl.BF.force_color;
	*pcolor = color_tbl.BF.color;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_tos_set(jl_device_t *dev,
                                       jl_uint8 tos,
                                       jl_uint8 color)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 tbl_idx = tos;

	SWCORE_IP_TOS_FIELD_TO_PACKET_COLOR_MAPPING_TABLE_t  color_tbl;

	JL_CHECK_POINTER(dev);

	memset(&color_tbl, 0, sizeof(SWCORE_IP_TOS_FIELD_TO_PACKET_COLOR_MAPPING_TABLE_t));

	color_tbl.BF.color = color;

	REGISTER_WRITE(dev, SWCORE, IP_TOS_FIELD_TO_PACKET_COLOR_MAPPING_TABLE, color_tbl, tbl_idx, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_tos_get(jl_device_t *dev,
                                       jl_uint8 tos,
                                       jl_uint8 *pcolor)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 tbl_idx = tos;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, IP_TOS_FIELD_TO_PACKET_COLOR_MAPPING_TABLE, color_tbl, tbl_idx, 0);

	*pcolor = color_tbl.BF.color;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_exp_set(jl_device_t *dev,
                                       jl_uint8 exp,
                                       jl_uint8 color)
{
	jl_ret_t ret = JL_ERR_OK;

	SWCORE_MPLS_EXP_FIELD_TO_PACKET_COLOR_MAPPING_TABLE_t  color_tbl;

	JL_CHECK_POINTER(dev);

	memset(&color_tbl, 0, sizeof(SWCORE_MPLS_EXP_FIELD_TO_PACKET_COLOR_MAPPING_TABLE_t));

	color_tbl.BF.color = color;

	REGISTER_WRITE(dev, SWCORE, MPLS_EXP_FIELD_TO_PACKET_COLOR_MAPPING_TABLE, color_tbl, exp, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_exp_get(jl_device_t *dev,
                                       jl_uint8 exp,
                                       jl_uint8 *pcolor)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, MPLS_EXP_FIELD_TO_PACKET_COLOR_MAPPING_TABLE, color_tbl, exp, 0);

	*pcolor = color_tbl.BF.color;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_pcpdei_set(jl_device_t *dev,
        jl_uint8 pcp,
        jl_uint8 dei,
        jl_uint8 color)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_idx = 0;

	SWCORE_VLAN_PCP_AND_DEI_TO_COLOR_MAPPING_TABLE_t color_tbl;

	JL_CHECK_POINTER(dev);

	memset(&color_tbl, 0, sizeof(SWCORE_VLAN_PCP_AND_DEI_TO_COLOR_MAPPING_TABLE_t));

	tbl_idx = pcp & BITS_WIDTH(3);
	tbl_idx |= ((dei & 0x1) << 3);

	color_tbl.BF.color = color;

	REGISTER_WRITE(dev, SWCORE, VLAN_PCP_AND_DEI_TO_COLOR_MAPPING_TABLE, color_tbl, tbl_idx, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_map_2color_pcpdei_get(jl_device_t *dev,
        jl_uint8 pcp,
        jl_uint8 dei,
        jl_uint8 *pcolor)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_idx = 0;

	JL_CHECK_POINTER(dev);

	tbl_idx = pcp & BITS_WIDTH(3);
	tbl_idx |= ((dei & 0x1) << 3);

	REGISTER_READ(dev, SWCORE, VLAN_PCP_AND_DEI_TO_COLOR_MAPPING_TABLE, color_tbl, tbl_idx, 0);

	*pcolor = color_tbl.BF.color;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_pcp_mapping_tbl_set(jl_device_t *dev,
        jl_uint8 pcp_sel,
        jl_uint8 dei_sel,
        jl_uint8 queue_idx,
        jl_qos_pcp_data_t *ppcp)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(ppcp);

	if ((pcp_sel != QOS_REMAP_PCP_FROM_QUEUE_MAPPING &&
	     dei_sel != QOS_REMAP_PCP_FROM_QUEUE_MAPPING) ||
	    ppcp->valid != TRUE)
		return JL_ERR_OK;

	REGISTER_READ(dev, SWCORE, EGRESS_QUEUE_TO_PCP_AND_CFI_DEI_MAPPING_TABLE, map_tbl, queue_idx, 0);

	if (dei_sel == QOS_REMAP_PCP_FROM_QUEUE_MAPPING)
	{
		if (ppcp->cfi_dei > 1)
			return JL_ERR_PARAM;
		map_tbl.BF.cfi_dei = ppcp->cfi_dei;
	}

	if (pcp_sel == QOS_REMAP_PCP_FROM_QUEUE_MAPPING)
	{
		if (ppcp->pcp > 8)
			return JL_ERR_PARAM;
		map_tbl.BF.pcp = ppcp->pcp;
	}


	REGISTER_WRITE(dev, SWCORE, EGRESS_QUEUE_TO_PCP_AND_CFI_DEI_MAPPING_TABLE, map_tbl, queue_idx, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_pcp_mapping_tbl_get(jl_device_t *dev,
        jl_uint8 queue_idx,
        jl_qos_pcp_data_t *ppcp)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(ppcp);

	REGISTER_READ(dev, SWCORE, EGRESS_QUEUE_TO_PCP_AND_CFI_DEI_MAPPING_TABLE, map_tbl, queue_idx, 0);

	ppcp->cfi_dei = map_tbl.BF.cfi_dei;
	ppcp->pcp = map_tbl.BF.pcp;
	ppcp->valid = TRUE;

	return 	JL_ERR_OK;
}

jl_ret_t jl61xx_qos_remap_vlan_set(jl_device_t *dev,
                                   jl_uint8 eport,
                                   jl_qos_remap_vlan_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 queue_idx = 0;
	jl_uint8 cfi_dei_sel = 0;
	jl_uint8 pcp_sel = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcfg);
	JL_CHECK_PORT(dev, eport);

	if (!pcfg->vlan_single_opt_valid &&
	    !pcfg->cfi_dei_sel_valid &&
	    !pcfg->pcp_sel_valid &&
	    !pcfg->vid_sel_valid &&
	    !pcfg->type_sel_valid)
		return JL_ERR_OK;

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, EGRESS_PORT_CONFIGURATION, eport_tbl, cport, 0);

	if (pcfg->vlan_single_opt_valid == TRUE)
	{
		JL_CHECK_MIN_MAX_RANGE(pcfg->vlan_single_opt, 0, 4);
		eport_tbl.BF.vlan_single_op = pcfg->vlan_single_opt;
	}

	if (pcfg->cfi_dei_sel_valid == TRUE) {
		JL_CHECK_MIN_MAX_RANGE(pcfg->cfi_dei_sel, 0, 2);
		eport_tbl.BF.cfi_dei_sel = pcfg->cfi_dei_sel;

		if (pcfg->cfi_dei_sel == QOS_REMAP_PCP_FROM_EPORT) {
			if (pcfg->vlan_port.valid)
				eport_tbl.BF.cfi_dei = pcfg->vlan_port.cfi_dei;
			cfi_dei_sel = QOS_REMAP_PCP_FROM_EPORT;
		} else if (pcfg->cfi_dei_sel == QOS_REMAP_PCP_FROM_QUEUE_MAPPING)
			cfi_dei_sel = QOS_REMAP_PCP_FROM_QUEUE_MAPPING;
	}

	if (pcfg->pcp_sel_valid == TRUE) {
		JL_CHECK_MIN_MAX_RANGE(pcfg->pcp_sel, 0, 2);
		eport_tbl.BF.pcp_sel = pcfg->pcp_sel;

		if (pcfg->pcp_sel ==  QOS_REMAP_PCP_FROM_EPORT) {
			if (pcfg->vlan_port.valid)
				eport_tbl.BF.pcp = pcfg->vlan_port.pcp;
			pcp_sel = QOS_REMAP_PCP_FROM_EPORT;
		} else if (pcfg->pcp_sel ==  QOS_REMAP_PCP_FROM_QUEUE_MAPPING)
			pcp_sel = QOS_REMAP_PCP_FROM_QUEUE_MAPPING;
	}

	if (pcfg->vid_sel_valid == TRUE) {
		JL_CHECK_MIN_MAX_RANGE(pcfg->vid_sel, 0, 2);
		eport_tbl.BF.vid_sel = pcfg->vid_sel;

		if (pcfg->vid_sel ==  QOS_REMAP_VID_FROM_EPORT) {
			if (pcfg->vlan_port.valid) {
				eport_tbl.BF.vid = pcfg->vlan_port.vid;
			}
		}
	}

	if (pcfg->type_sel_valid == TRUE) {
		JL_CHECK_MIN_MAX_RANGE(pcfg->type_sel, 0, 2);
		eport_tbl.BF.type_sel = pcfg->type_sel;
	}

	for (; queue_idx < QOS_QUEUE_NUM; queue_idx++) {
		ret = jl61xx_qos_pcp_mapping_tbl_set(dev, pcp_sel,
		                    cfi_dei_sel, queue_idx, &(pcfg->queue_mapping[queue_idx]));
		if (ret)
			return ret;
	}

	REGISTER_WRITE(dev, SWCORE, EGRESS_PORT_CONFIGURATION, eport_tbl, cport, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_remap_vlan_get(jl_device_t *dev,
                                   jl_uint8 eport,
                                   jl_qos_remap_vlan_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 queue_idx = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcfg);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, EGRESS_PORT_CONFIGURATION, eport_tbl, cport, 0);

	pcfg->vlan_single_opt_valid = TRUE;
	pcfg->vlan_single_opt = eport_tbl.BF.vlan_single_op;

	pcfg->cfi_dei_sel_valid = TRUE;
	pcfg->cfi_dei_sel = eport_tbl.BF.cfi_dei_sel;

	pcfg->pcp_sel_valid = TRUE;
	pcfg->pcp_sel = eport_tbl.BF.pcp_sel;

	pcfg->vid_sel_valid = TRUE;
	pcfg->vid_sel = eport_tbl.BF.vid_sel;

	pcfg->type_sel_valid = TRUE;
	pcfg->type_sel = eport_tbl.BF.type_sel;

	pcfg->vlan_port.valid = TRUE;
	pcfg->vlan_port.cfi_dei = eport_tbl.BF.cfi_dei;
	pcfg->vlan_port.pcp = eport_tbl.BF.pcp;
	pcfg->vlan_port.vid = eport_tbl.BF.vid;

	for (; queue_idx < QOS_QUEUE_NUM; queue_idx++) {
		ret = jl61xx_qos_pcp_mapping_tbl_get(dev, queue_idx, &(pcfg->queue_mapping[queue_idx]));
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_color_remap_set(jl_device_t *dev,
                                    jl_uint8 eport,
                                    jl_qos_color_remap_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcfg);
	JL_CHECK_PORT(dev, eport);

	SWCORE_COLOR_REMAP_FROM_EGRESS_PORT_t remap_tbl;
	memset(&remap_tbl, 0, sizeof(SWCORE_COLOR_REMAP_FROM_EGRESS_PORT_t));

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, EGRESS_PORT_CONFIGURATION, eport_tbl, cport, 0);
	eport_tbl.BF.color_remap = (pcfg->enable == 0) ? 0 : 1;

	JL_CHECK_ENUM(pcfg->remap_mode, QOS_MMP_REMAP_NUM);
	remap_tbl.BF.color_mode = pcfg->remap_mode;

	remap_tbl.BF.tos_mask_0_5 = pcfg->tos_mask;
	remap_tbl.BF.tos_mask_6_7 = (pcfg->tos_mask >>
									SWCORE_COLOR_REMAP_FROM_EGRESS_PORT_OFFSET_TOS_MASK_0_5_WIDTH);

	remap_tbl.BF.color2_tos = pcfg->green_tos;
	remap_tbl.BF.color2_tos |= ((jl_uint32)pcfg->yellow_tos << 8);
	remap_tbl.BF.color2_tos |= ((jl_uint32)pcfg->red_tos << 16);

	JL_CHECK_ENUM(pcfg->green_dei, 2);
	JL_CHECK_ENUM(pcfg->yellow_dei, 2);
	JL_CHECK_ENUM(pcfg->red_dei, 2);
	remap_tbl.BF.color2_dei = pcfg->green_dei & 0x1;
	remap_tbl.BF.color2_dei |= (pcfg->yellow_dei & 0x1) << 1;
	remap_tbl.BF.color2_dei |= (pcfg->red_dei & 0x1) << 2;

	REGISTER_WRITE(dev, SWCORE, COLOR_REMAP_FROM_EGRESS_PORT, remap_tbl, cport, 0);
	REGISTER_WRITE(dev, SWCORE, EGRESS_PORT_CONFIGURATION, eport_tbl, cport, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_color_remap_get(jl_device_t *dev,
                                    jl_uint8 eport,
                                    jl_qos_color_remap_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcfg);
	JL_CHECK_PORT(dev,eport);

	cport = jlxx_port_l2c(dev,eport);

	REGISTER_READ(dev, SWCORE, COLOR_REMAP_FROM_EGRESS_PORT, remap_tbl, cport, 0);
	REGISTER_READ(dev, SWCORE, EGRESS_PORT_CONFIGURATION, eport_tbl, cport, 0);

	pcfg->enable = eport_tbl.BF.color_remap;

	pcfg->remap_mode = remap_tbl.BF.color_mode;
	pcfg->tos_mask = remap_tbl.BF.tos_mask_0_5;
	pcfg->tos_mask |= (jl_uint32)remap_tbl.BF.tos_mask_6_7 <<
	                  SWCORE_COLOR_REMAP_FROM_EGRESS_PORT_OFFSET_TOS_MASK_0_5_WIDTH;

	pcfg->green_tos = remap_tbl.BF.color2_tos & 0xff;
	pcfg->yellow_tos = (remap_tbl.BF.color2_tos >> 8) & 0xff;
	pcfg->red_tos = (remap_tbl.BF.color2_tos >> 16) & 0xff;
	pcfg->green_dei = remap_tbl.BF.color2_dei & 0x1;
	pcfg->yellow_dei = (remap_tbl.BF.color2_dei >> 1) & 0x1;
	pcfg->red_dei = (remap_tbl.BF.color2_dei >> 2) & 0x1;

	return JL_ERR_OK;
}


static jl_ret_t jl61xx_qos_mmp_get_tick(jl_device_t *dev,
                                        jl_uint32 rate,
                                        jl_uint8 unit,
                                        jl_uint8 *tick,
                                        jl_uint32 *tokens)
{
	jl_uint8 idx = 0;
	jl_uint64 multiple = 0;
	jl_uint64 token_max = 0;
	jl_uint64 rate_temp = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(tick);
	JL_CHECK_POINTER(tokens);

	if (unit == QOS_RATE_UNIT_KBPS)
		multiple = 1000;
	else if (unit == QOS_RATE_UNIT_MBPS)
		multiple = 1000000;
	else
		return JL_ERR_PARAM;

	token_max = BITS_WIDTH(SWCORE_INGRESS_ADMISSION_CONTROL_TOKEN_BUCKET_CONFIGURATION_OFFSET_TOKENS_0_WIDTH);
	rate_temp = rate * multiple;

	rate_temp /= 8;

	for (idx = JL_SYSTIME_MAX_STEP; idx > 0 ; idx--) {
		if ((g_tick_freq_list[idx - 1] <= rate_temp) &&
		    ((g_tick_freq_list[idx - 1] * token_max) >= rate_temp)) {
			*tick = idx - 1;
			*tokens = rate_temp / g_tick_freq_list[idx - 1];
			break;
		}

		if (idx == 1) {
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Qos MMP rate %d out of range\n", rate);
			return JL_ERR_OUT_OF_RANGE;
		}
	}

	return JL_ERR_OK;
}



jl_ret_t jl61xx_qos_mmp_cfg_set(jl_device_t *dev,
                                jl_uint8 mmp_pointer,
                                jl_qos_mmp_cfg_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 tokens = 0;
	jl_uint8 drop_mask = 0;
	jl_uint8 tick = 0;
	jl_uint8 bucket_change = 0;

	SWCORE_INGRESS_ADMISSION_CONTROL_RESET_t rst_tbl;
	SWCORE_COLOR_REMAP_FROM_INGRESS_ADMISSION_CONTROL_t remap_tbl;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcfg);

	memset(&rst_tbl, 0, sizeof(SWCORE_INGRESS_ADMISSION_CONTROL_RESET_t));
	memset(&remap_tbl, 0, sizeof(SWCORE_COLOR_REMAP_FROM_INGRESS_ADMISSION_CONTROL_t));

	REGISTER_READ(dev, SWCORE, INGRESS_ADMISSION_CONTROL_TOKEN_BUCKET_CONFIGURATION, bucket_tbl, mmp_pointer, 0);

	if (pcfg->policy_valid) {
		bucket_change = 1;

		if (pcfg->policy.green_drop != 0)
			drop_mask |= 1;
		if (pcfg->policy.yellow_drop != 0)
			drop_mask |= (1 << 1);
		if (pcfg->policy.red_drop != 0)
			drop_mask |= (1 << 2);

		bucket_tbl.BF.drop_mask = drop_mask;

		JL_CHECK_ENUM(pcfg->policy.bucket_mode, QOS_MMP_MODE_NUM);
		bucket_tbl.BF.bucket_mode = (pcfg->policy.bucket_mode == 0) ? 0 : 1;

		/*update tickstep value */
		ret = jl61xx_calc_tick_freq(dev);
		if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
			return ret;

		if (pcfg->policy.bucket_mode == QOS_MMP_MODE_SR) {
			if (pcfg->policy.rate.s_rate.CBS < QOS_MIN_MMP_CAPACITY)
				return JL_ERR_PARAM;

			ret = jl61xx_qos_mmp_get_tick(dev, pcfg->policy.rate.s_rate.CIR,
			                              pcfg->policy.rate.s_rate.cir_unit, &tick, &tokens);
			if (ret)
				return ret;

			if (tokens < 0xfff && tokens > 0)
				tokens += 1;/*SS-70*/

			bucket_tbl.BF.tokens_0 = tokens;
			bucket_tbl.BF.tick_0 = tick;

			bucket_tbl.BF.bucket_capacity_0 = pcfg->policy.rate.s_rate.CBS;
			bucket_tbl.BF.bucket_capacity_1_0 = pcfg->policy.rate.s_rate.EBS;
			bucket_tbl.BF.bucket_capacity_1_1_15 =  (pcfg->policy.rate.s_rate.EBS >>
			                SWCORE_INGRESS_ADMISSION_CONTROL_TOKEN_BUCKET_CONFIGURATION_OFFSET_BUCKET_CAPACITY_1_0_WIDTH);
		} else {
			if (pcfg->policy.rate.t_rate.CBS < QOS_MIN_MMP_CAPACITY ||
			    pcfg->policy.rate.t_rate.PBS < QOS_MIN_MMP_CAPACITY)
				return JL_ERR_PARAM;

			ret = jl61xx_qos_mmp_get_tick(dev, pcfg->policy.rate.t_rate.CIR,
			                              pcfg->policy.rate.t_rate.cir_unit, &tick, &tokens);
			if (ret)
				return ret;

			if (tokens < 0xfff && tokens > 0)
				tokens += 1;/*SS-70*/

			bucket_tbl.BF.tokens_0 = tokens;
			bucket_tbl.BF.tick_0 = tick;

			bucket_tbl.BF.bucket_capacity_0 = pcfg->policy.rate.t_rate.CBS;

			ret = jl61xx_qos_mmp_get_tick(dev, pcfg->policy.rate.t_rate.PIR,
			                              pcfg->policy.rate.t_rate.pir_unit, &tick, &tokens);
			if (ret)
				return ret;

			if (tokens < 0xfff && tokens > 0)
				tokens += 1;/*SS-70*/

			bucket_tbl.BF.tokens_1 = tokens;
			bucket_tbl.BF.tick_1 = tick;

			bucket_tbl.BF.bucket_capacity_1_0 = pcfg->policy.rate.t_rate.PBS;
			bucket_tbl.BF.bucket_capacity_1_1_15 =  (pcfg->policy.rate.t_rate.PBS >>
			                SWCORE_INGRESS_ADMISSION_CONTROL_TOKEN_BUCKET_CONFIGURATION_OFFSET_BUCKET_CAPACITY_1_0_WIDTH);
		}
	}

	if (pcfg->color_blind_valid) {
		bucket_change = 1;
		bucket_tbl.BF.color_blind = (pcfg->color_blind == QOS_COLOR_AWARE) ? 0 : 1;
	}

	if (pcfg->max_len_valid) {
		bucket_change = 1;
		bucket_tbl.BF.max_length = pcfg->max_length;
	}

	if (pcfg->correction_mode_valid) {
		bucket_change = 1;
		bucket_tbl.BF.byte_correction_mode = pcfg->correction_mode;
		bucket_tbl.BF.byte_correction = pcfg->byte_correction;
	}

	if (bucket_change) {
		REGISTER_WRITE(dev, SWCORE, INGRESS_ADMISSION_CONTROL_TOKEN_BUCKET_CONFIGURATION, bucket_tbl, mmp_pointer, 0);
		rst_tbl.BF.bucket_reset = 1;
		REGISTER_WRITE(dev, SWCORE, INGRESS_ADMISSION_CONTROL_RESET, rst_tbl, mmp_pointer, 0);
	}

	if (pcfg->color_remap_valid) {
		remap_tbl.BF.enable = (pcfg->color_remap.enable == 0) ? 0 : 1;

		JL_CHECK_ENUM(pcfg->color_remap.remap_mode, QOS_MMP_REMAP_NUM);
		remap_tbl.BF.color_mode = pcfg->color_remap.remap_mode;
		remap_tbl.BF.tos_mask_0_4 = pcfg->color_remap.tos_mask;
		remap_tbl.BF.tos_mask_5_7 = (pcfg->color_remap.tos_mask >>
		                             SWCORE_COLOR_REMAP_FROM_INGRESS_ADMISSION_CONTROL_OFFSET_TOS_MASK_0_4_WIDTH);

		remap_tbl.BF.color2_tos = pcfg->color_remap.green_tos;
		remap_tbl.BF.color2_tos |= ((jl_uint32)pcfg->color_remap.yellow_tos << 8);
		remap_tbl.BF.color2_tos |= ((jl_uint32)pcfg->color_remap.red_tos << 16);

		JL_CHECK_ENUM(pcfg->color_remap.green_dei, 2);
		JL_CHECK_ENUM(pcfg->color_remap.yellow_dei, 2);
		JL_CHECK_ENUM(pcfg->color_remap.red_dei, 2);
		remap_tbl.BF.color2_dei = pcfg->color_remap.green_dei & 0x1;
		remap_tbl.BF.color2_dei |= (pcfg->color_remap.yellow_dei & 0x1) << 1;
		remap_tbl.BF.color2_dei |= (pcfg->color_remap.red_dei & 0x1) << 2;

		REGISTER_WRITE(dev, SWCORE, COLOR_REMAP_FROM_INGRESS_ADMISSION_CONTROL, remap_tbl, mmp_pointer, 0);
	}

	return JL_ERR_OK;
}


jl_ret_t jl61xx_qos_mmp_cfg_get(jl_device_t *dev,
                                jl_uint8 mmp_pointer,
                                jl_qos_mmp_cfg_t *pcfg)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 rate = 0;
	jl_uint32 tokens = 0;
	jl_uint8 tick = 0;
	jl_uint8 unit = 0;
	jl_uint8 drop_mask = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcfg);

	memset(pcfg, 0, sizeof(jl_qos_mmp_cfg_t));

	REGISTER_READ(dev, SWCORE, INGRESS_ADMISSION_CONTROL_TOKEN_BUCKET_CONFIGURATION, bucket_tbl, mmp_pointer, 0);
	REGISTER_READ(dev, SWCORE, COLOR_REMAP_FROM_INGRESS_ADMISSION_CONTROL, remap_tbl, mmp_pointer, 0);

	pcfg->color_blind = bucket_tbl.BF.color_blind;
	pcfg->color_blind_valid = TRUE;

	pcfg->max_length = bucket_tbl.BF.max_length;
	pcfg->max_len_valid = TRUE;

	pcfg->correction_mode = bucket_tbl.BF.byte_correction_mode;
	pcfg->byte_correction = bucket_tbl.BF.byte_correction;
	pcfg->correction_mode_valid = TRUE;

	/*update tickstep value */
	ret = jl61xx_calc_tick_freq(dev);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	pcfg->policy.bucket_mode = bucket_tbl.BF.bucket_mode;

	if (bucket_tbl.BF.bucket_mode == QOS_MMP_MODE_SR) {
		tick = bucket_tbl.BF.tick_0;
		tokens = bucket_tbl.BF.tokens_0;
		if (tokens != 0)
			tokens -= 1;/*SS-70*/

		rate = tokens * g_tick_freq_list[tick] * 8 / 1000;

		if ((tokens * g_tick_freq_list[tick] * 8) % 1000)
			rate += 1;

		unit = QOS_RATE_UNIT_KBPS;

		pcfg->policy.rate.s_rate.CIR = rate;
		pcfg->policy.rate.s_rate.cir_unit = unit;

		pcfg->policy.rate.s_rate.CBS = bucket_tbl.BF.bucket_capacity_0;
		pcfg->policy.rate.s_rate.EBS = bucket_tbl.BF.bucket_capacity_1_0;
		pcfg->policy.rate.s_rate.EBS |= ((jl_uint32)bucket_tbl.BF.bucket_capacity_1_1_15 <<
		                                 SWCORE_INGRESS_ADMISSION_CONTROL_TOKEN_BUCKET_CONFIGURATION_OFFSET_BUCKET_CAPACITY_1_0_WIDTH);
	} else {
		tick = bucket_tbl.BF.tick_0;
		tokens = bucket_tbl.BF.tokens_0;
		if (tokens != 0)
			tokens -= 1;/*SS-70*/

		rate = tokens * g_tick_freq_list[tick] * 8 / 1000;

		if ((tokens * g_tick_freq_list[tick] * 8) % 1000)
			rate += 1;

		unit = QOS_RATE_UNIT_KBPS;

		pcfg->policy.rate.t_rate.CIR = rate;
		pcfg->policy.rate.t_rate.cir_unit = unit;

		tick = bucket_tbl.BF.tick_1;
		tokens = bucket_tbl.BF.tokens_1;
		if (tokens != 0)
			tokens -= 1;/*SS-70*/

		rate = tokens * g_tick_freq_list[tick] * 8 / 1000;

		if ((tokens * g_tick_freq_list[tick] * 8) % 1000)
			rate += 1;

		pcfg->policy.rate.t_rate.PIR = rate;
		pcfg->policy.rate.t_rate.pir_unit = unit;

		pcfg->policy.rate.t_rate.CBS = bucket_tbl.BF.bucket_capacity_0;
		pcfg->policy.rate.t_rate.PBS = bucket_tbl.BF.bucket_capacity_1_0;
		pcfg->policy.rate.t_rate.PBS |= ((jl_uint32)bucket_tbl.BF.bucket_capacity_1_1_15 <<
		                                 SWCORE_INGRESS_ADMISSION_CONTROL_TOKEN_BUCKET_CONFIGURATION_OFFSET_BUCKET_CAPACITY_1_0_WIDTH);
	}

	drop_mask = bucket_tbl.BF.drop_mask;

	pcfg->policy.green_drop = (drop_mask & 0x1) ? 1 : 0;
	pcfg->policy.yellow_drop = (drop_mask & 0x2) ? 1 : 0;
	pcfg->policy.red_drop = (drop_mask & 0x4) ? 1 : 0;

	pcfg->policy_valid = TRUE;

	pcfg->color_remap.enable = remap_tbl.BF.enable;
	pcfg->color_remap.remap_mode = remap_tbl.BF.color_mode;
	pcfg->color_remap.tos_mask = remap_tbl.BF.tos_mask_0_4;
	pcfg->color_remap.tos_mask |= (jl_uint32)remap_tbl.BF.tos_mask_5_7 <<
	                              SWCORE_COLOR_REMAP_FROM_INGRESS_ADMISSION_CONTROL_OFFSET_TOS_MASK_0_4_WIDTH;

	pcfg->color_remap.green_tos = remap_tbl.BF.color2_tos & 0xff;
	pcfg->color_remap.yellow_tos = (remap_tbl.BF.color2_tos >> 8) & 0xff;
	pcfg->color_remap.red_tos = (remap_tbl.BF.color2_tos >> 16) & 0xff;

	pcfg->color_remap.green_dei = remap_tbl.BF.color2_dei & 0x1;
	pcfg->color_remap.yellow_dei = (remap_tbl.BF.color2_dei >> 1) & 0x1;
	pcfg->color_remap.red_dei = (remap_tbl.BF.color2_dei >> 2) & 0x1;

	pcfg->color_remap_valid = TRUE;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_mmp_unused_pointer_get(jl_device_t *dev, jl_uint8 *ppointer)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(ppointer);

	ret = jl61xx_get_unused_mmp_pointer(dev, ppointer);

	return ret;
}

jl_ret_t jl61xx_qos_mmp_used_pointer_info_get(jl_device_t *dev,
        jl_uint8 mmp_pointer,
        jl_qos_mmp_used_info_t *ppointer_info)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_mmp_used_info_t used_info;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(ppointer_info);

	ret = jl61xx_get_mmp_pointer_info(dev, mmp_pointer, &used_info);
	if (ret)
		return ret;

	memcpy((void*)ppointer_info, (void*)&used_info, sizeof(jl_qos_mmp_used_info_t));

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_ingress_initial_mmp_set(jl_device_t *dev,
        jl_uint8 inport,
        jl_uint8 pcp,
        jl_qos_mmp_pointer_t *ppointer)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_idx = 0;
	jl_uint8 origin_mmp_ptr;
	jl_uint8 origin_mmp_valid;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(ppointer);
	JL_CHECK_PORT(dev, inport);
	JL_CHECK_ENUM(ppointer->mmp_order, QOS_MMP_ORDER_NUM);
	JL_CHECK_ENUM(ppointer->mmp_pointer, QOS_MMP_POINTER_NUM);

	cport = jlxx_port_l2c(dev, inport);

	tbl_idx = cport & 0xf;
	tbl_idx |= ((pcp & 0x7) << 4);

	REGISTER_READ(dev, SWCORE, INGRESS_ADMISSION_CONTROL_INITIAL_POINTER, pointer_tbl, tbl_idx, 0);

	origin_mmp_valid = pointer_tbl.BF.mmp_valid;
	origin_mmp_ptr = pointer_tbl.BF.mmp_ptr;

	pointer_tbl.BF.mmp_valid = ppointer->mmp_valid;
	pointer_tbl.BF.mmp_ptr = ppointer->mmp_pointer;
	pointer_tbl.BF.mmp_order = ppointer->mmp_order;

	REGISTER_WRITE(dev, SWCORE, INGRESS_ADMISSION_CONTROL_INITIAL_POINTER, pointer_tbl, tbl_idx, 0);

	if (origin_mmp_valid) {
		ret = jl61xx_clear_mmp_pointer_info(dev, JL_MMP_MODULE_INITIAL_ADM, origin_mmp_ptr);
		if (ret)
			return ret;
	}

	if (ppointer->mmp_valid) {
		ret = jl61xx_set_mmp_pointer_info(dev, JL_MMP_MODULE_INITIAL_ADM, ppointer->mmp_pointer);
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}


jl_ret_t jl61xx_qos_ingress_initial_mmp_get(jl_device_t *dev,
        jl_uint8 inport,
        jl_uint8 pcp,
        jl_qos_mmp_pointer_t *ppointer)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tbl_idx = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(ppointer);
	JL_CHECK_PORT(dev, inport);

	cport = jlxx_port_l2c(dev, inport);

	tbl_idx = cport & 0xf;
	tbl_idx |= ((pcp & 0x7) << 4);

	REGISTER_READ(dev, SWCORE, INGRESS_ADMISSION_CONTROL_INITIAL_POINTER, pointer_tbl, tbl_idx, 0);

	ppointer->mmp_valid = pointer_tbl.BF.mmp_valid;
	ppointer->mmp_pointer = pointer_tbl.BF.mmp_ptr;
	ppointer->mmp_order = pointer_tbl.BF.mmp_order;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_queue_map_trust_l3_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	SWCORE_EGRESS_QUEUE_PRIORITY_SELECTION_t sel_tbl;

	sel_tbl.BF.prio_from_l3 = (enable == 0) ? 0 : 1;

	REGISTER_WRITE(dev, SWCORE, EGRESS_QUEUE_PRIORITY_SELECTION, sel_tbl, cport, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_queue_map_trust_l3_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 *penable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(penable);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, EGRESS_QUEUE_PRIORITY_SELECTION, sel_tbl, cport, 0);

	*penable = sel_tbl.BF.prio_from_l3;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_color_map_trust_l3_set(jl_device_t *dev,
        jl_uint8 inport,
        jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_PORT(dev, inport);

	cport = jlxx_port_l2c(dev, inport);

	REGISTER_READ(dev, SWCORE, SOURCE_PORT_TABLE, s_port_tbl, cport, 0);

	s_port_tbl.BF.color_from_l3 = (enable == 0) ? 0 : 1;

	REGISTER_WRITE(dev, SWCORE, SOURCE_PORT_TABLE, s_port_tbl, cport, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_color_map_trust_l3_get(jl_device_t *dev,
        jl_uint8 inport,
        jl_uint8 *penable)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(penable);
	JL_CHECK_PORT(dev, inport);

	cport = jlxx_port_l2c(dev, inport);

	REGISTER_READ(dev, SWCORE, SOURCE_PORT_TABLE, s_port_tbl, cport, 0);

	*penable = s_port_tbl.BF.color_from_l3;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_queue_priority_set(jl_device_t *dev,
                                       jl_uint8 eport,
                                       jl_qos_queue_priority_t *pqueue_pri)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 queue_idx = 0;
	jl_uint8 pri_val = 0;
	jl_uint8 need_guarantee = 0;
	jl_uint8 pri_changed = 0;
	jl_uint32 port_mask =0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pqueue_pri);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, MAP_QUEUE_TO_PRIORITY, prio_tbl, cport, 0);

	for (queue_idx = 0; queue_idx < QOS_QUEUE_NUM; queue_idx++) {
		if (pqueue_pri->queue[queue_idx].valid) {
			pri_val = pqueue_pri->queue[queue_idx].priority;

			JL_CHECK_ENUM(pri_val, QOS_PRIORITY_NUM);

			switch (queue_idx) {
			case 0:
				if (prio_tbl.BF.prio0 != pri_val)
					pri_changed = 1;
				prio_tbl.BF.prio0 = pri_val;
				break;
			case 1:
				if (prio_tbl.BF.prio1 != pri_val)
					pri_changed = 1;
				prio_tbl.BF.prio1 = pri_val;
				break;
			case 2:
				if (prio_tbl.BF.prio2 != pri_val)
					pri_changed = 1;
				prio_tbl.BF.prio2 = pri_val;
				break;
			case 3:
				if (prio_tbl.BF.prio3 != pri_val)
					pri_changed = 1;
				prio_tbl.BF.prio3 = pri_val;
				break;
			case 4:
				if (prio_tbl.BF.prio4 != pri_val)
					pri_changed = 1;
				prio_tbl.BF.prio4 = pri_val;
				break;
			case 5:
				if (prio_tbl.BF.prio5 != pri_val)
					pri_changed = 1;
				prio_tbl.BF.prio5 = pri_val;
				break;
			case 6:
				if (prio_tbl.BF.prio6 != pri_val)
					pri_changed = 1;
				prio_tbl.BF.prio6 = pri_val;
				break;
			case 7:
				if (prio_tbl.BF.prio7 != pri_val)
					pri_changed = 1;
				prio_tbl.BF.prio7 = pri_val;
				break;
			default:
				break;
			}
		}
	}

	if (!pri_changed)
		return JL_ERR_OK;

	/*wether any queues have the same priority*/
	pri_val = BIT(prio_tbl.BF.prio0) | BIT(prio_tbl.BF.prio1) | BIT(prio_tbl.BF.prio2) | BIT(prio_tbl.BF.prio3)
				| BIT(prio_tbl.BF.prio4) | BIT(prio_tbl.BF.prio5) | BIT(prio_tbl.BF.prio6) | BIT(prio_tbl.BF.prio7);

	if (pri_val != 0xff)
		need_guarantee = 1;

	SET_BIT(port_mask, eport);

	ret = jl61xx_qos_drain_port(dev, port_mask);
	if (ret)
		return ret;

	/*waiting for port empty*/

	ret = jl61xx_qos_check_port_empty(dev, port_mask);
	if (ret)
		return ret;

	ret = jl61xx_qos_res_manage_mode_set(dev, eport, need_guarantee);
	if (ret)
		return ret;

	REGISTER_WRITE(dev, SWCORE, MAP_QUEUE_TO_PRIORITY, prio_tbl, cport, 0);

	ret = jl61xx_qos_recover_port(dev, port_mask);
	if (ret)
		return ret;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_queue_priority_get(jl_device_t *dev,
                                       jl_uint8 eport,
                                       jl_qos_queue_priority_t *pqueue_pri)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pqueue_pri);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, MAP_QUEUE_TO_PRIORITY, prio_tbl, cport, 0);

	pqueue_pri->queue[0].valid = TRUE;
	pqueue_pri->queue[0].priority = prio_tbl.BF.prio0;
	pqueue_pri->queue[1].valid = TRUE;
	pqueue_pri->queue[1].priority = prio_tbl.BF.prio1;
	pqueue_pri->queue[2].valid = TRUE;
	pqueue_pri->queue[2].priority = prio_tbl.BF.prio2;
	pqueue_pri->queue[3].valid = TRUE;
	pqueue_pri->queue[3].priority = prio_tbl.BF.prio3;
	pqueue_pri->queue[4].valid = TRUE;
	pqueue_pri->queue[4].priority = prio_tbl.BF.prio4;
	pqueue_pri->queue[5].valid = TRUE;
	pqueue_pri->queue[5].priority = prio_tbl.BF.prio5;
	pqueue_pri->queue[6].valid = TRUE;
	pqueue_pri->queue[6].priority = prio_tbl.BF.prio6;
	pqueue_pri->queue[7].valid = TRUE;
	pqueue_pri->queue[7].priority = prio_tbl.BF.prio7;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_queue_shaper_bucket_set(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_uint8 shaper_mode,
        jl_uint16 tokens)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_QUEUE_SHAPER_BUCKET_THRESHOLD_CONFIGURATION_t thrs_tbl;
	SWCORE_QUEUE_SHAPER_BUCKET_CAPACITY_CONFIGURATION_t capt_tbl;

	memset(&thrs_tbl, 0, sizeof(SWCORE_QUEUE_SHAPER_BUCKET_THRESHOLD_CONFIGURATION_t));
	memset(&capt_tbl, 0, sizeof(SWCORE_QUEUE_SHAPER_BUCKET_CAPACITY_CONFIGURATION_t));

	if (shaper_mode == QOS_RATE_UNIT_KBPS ||
	    shaper_mode == QOS_RATE_UNIT_MBPS) {
		thrs_tbl.BF.threshold = (tokens * QOS_BUCKET_THRESHOLD_MULTIPLE_BPS);

		if (thrs_tbl.BF.threshold < QOS_BUCKET_MINDEPTH_VAL_BPS)
			thrs_tbl.BF.threshold = QOS_BUCKET_MINDEPTH_VAL_BPS;

		capt_tbl.BF.bucket_capacity =
		    (thrs_tbl.BF.threshold * QOS_BUCKET_BURSTSIZE_MULTIPLE_BPS);

	} else if (shaper_mode == QOS_RATE_UNIT_PPS) {
		thrs_tbl.BF.threshold = (tokens * QOS_BUCKET_THRESHOLD_MULTIPLE_PPS);
		capt_tbl.BF.bucket_capacity =
		    (thrs_tbl.BF.threshold * QOS_BUCKET_BURSTSIZE_MULTIPLE_PPS);
	} else
		return JL_ERR_PARAM;

	REGISTER_WRITE(dev, SWCORE, QUEUE_SHAPER_BUCKET_THRESHOLD_CONFIGURATION, thrs_tbl, shaper_idx, 0);
	REGISTER_WRITE(dev, SWCORE, QUEUE_SHAPER_BUCKET_CAPACITY_CONFIGURATION, capt_tbl, shaper_idx, 0);

	return JL_ERR_OK;
}


static jl_ret_t jl61xx_qos_queue_shaper_enable_set(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, QUEUE_SHAPER_ENABLE, en_tbl, 0, 0);

	if (shaper_idx >= 64) {
		shaper_idx %= 64;

		if (enable == DISABLED)
			CLR_BIT(en_tbl.BF.enable_64_87, shaper_idx);
		else
			SET_BIT(en_tbl.BF.enable_64_87, shaper_idx);
	} else if (shaper_idx >= 32) {
		shaper_idx %= 32;

		if (enable == DISABLED)
			CLR_BIT(en_tbl.BF.enable_32_63, shaper_idx);
		else
			SET_BIT(en_tbl.BF.enable_32_63, shaper_idx);
	} else {
		if (enable == DISABLED)
			CLR_BIT(en_tbl.BF.enable_0_31, shaper_idx);
		else
			SET_BIT(en_tbl.BF.enable_0_31, shaper_idx);
	}

	REGISTER_WRITE(dev, SWCORE, QUEUE_SHAPER_ENABLE, en_tbl, 0, 0);
	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_queue_shaper_enable_get(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_uint8 *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(dev, SWCORE, QUEUE_SHAPER_ENABLE, en_tbl, 0, 0);

	if (shaper_idx >= 64) {
		shaper_idx %= 64;
		*penable = GET_BIT(en_tbl.BF.enable_64_87, shaper_idx);
	} else if (shaper_idx >= 32) {
		shaper_idx %= 32;
		*penable = GET_BIT(en_tbl.BF.enable_32_63, shaper_idx);
	} else {
		*penable = GET_BIT(en_tbl.BF.enable_0_31, shaper_idx);
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_shaper_get_tick(jl_device_t *dev,
        jl_qos_shaper_rate_t *prate,
        jl_uint8 *tick)
{
	jl_uint8 idx = 0;
	jl_uint8 unit = 0;
	jl_uint64 token_max = 0;
	jl_uint64 rate = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(tick);
	JL_CHECK_POINTER(prate);

	rate = prate->rate;
	unit = prate->unit;

	token_max = BITS_WIDTH(SWCORE_QUEUE_SHAPER_RATE_CONFIGURATION_OFFSET_TOKENS_WIDTH);

	if (unit == QOS_RATE_UNIT_MBPS) {
		rate *= 1000;
		unit = QOS_RATE_UNIT_KBPS;
	}

	if (unit == QOS_RATE_UNIT_KBPS) {
		rate *= 1000;
		rate /= 8;
	}

	for (idx = JL_SYSTIME_MAX_STEP; idx > 0 ; idx--) {
		if ((g_tick_freq_list[idx - 1] <= rate) &&
		    ((g_tick_freq_list[idx - 1] * token_max) >= rate)) {
			*tick = idx - 1;
			break;
		}

		if (idx == 1) {
			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_WARNING, "Qos shaper rate %d unit %d out of range\n",
			           prate->rate, prate->unit);
			return JL_ERR_OUT_OF_RANGE;
		}
	}

	return JL_ERR_OK;
}


static jl_ret_t jl61xx_qos_queue_shaper_rate_set(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_qos_shaper_rate_t *prate)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tick = 0;
	jl_uint16 tokens = 0;
	jl_uint32 rate = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(prate);

	SWCORE_QUEUE_SHAPER_RATE_CONFIGURATION_t queue_shaper;
	memset(&queue_shaper, 0, sizeof(SWCORE_QUEUE_SHAPER_RATE_CONFIGURATION_t));

	if (prate->enable == DISABLED) {
		ret = jl61xx_qos_queue_shaper_enable_set(dev, shaper_idx, DISABLED);
		return ret;
	}

	rate = prate->rate;

	JL_CHECK_ENUM(prate->unit, QOS_RATE_UNIT_NUM);

	if (prate->unit == QOS_RATE_UNIT_MBPS) {
		rate *= 1000;
	}

	ret = jl61xx_qos_shaper_get_tick(dev, prate, &tick);
	if (ret)
		return ret;

	if (prate->unit == QOS_RATE_UNIT_KBPS ||
	    prate->unit == QOS_RATE_UNIT_MBPS) {
		queue_shaper.BF.packets_not_bytes = 0;
		tokens = (rate * 1000 / 8) / g_tick_freq_list[tick];

		if (tokens < 0xfff && tokens > 0)
			tokens += 1; /*SS-70*/

	} else if (prate->unit == QOS_RATE_UNIT_PPS) {
		queue_shaper.BF.packets_not_bytes = 1;
		tokens = rate / g_tick_freq_list[tick];
	} else/*if all drained*/
		return JL_ERR_PARAM;

	queue_shaper.BF.tick = tick;
	queue_shaper.BF.tokens = tokens;
	queue_shaper.BF.ifg_correction = (QOS_IFG_DEFAULT_LENGTH + QOS_FCS_DEFAULT_LENGTH);

	/*set bucket*/
	ret = jl61xx_qos_queue_shaper_bucket_set(dev, shaper_idx, prate->unit, queue_shaper.BF.tokens);
	if (ret)
		return ret;

	REGISTER_WRITE(dev, SWCORE, QUEUE_SHAPER_RATE_CONFIGURATION, queue_shaper, shaper_idx, 0);

	ret = jl61xx_qos_queue_shaper_enable_set(dev, shaper_idx, ENABLED);

	return ret;
}

static jl_ret_t jl61xx_qos_queue_shaper_rate_get(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_qos_shaper_rate_t *prate)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 enable = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(prate);

	REGISTER_READ(dev, SWCORE, QUEUE_SHAPER_RATE_CONFIGURATION, queue_shaper, shaper_idx, 0);

	if (queue_shaper.BF.packets_not_bytes == 0) {
		prate->unit = QOS_RATE_UNIT_KBPS;

		if (queue_shaper.BF.tokens != 0)
			queue_shaper.BF.tokens -= 1;
		prate->rate = queue_shaper.BF.tokens * g_tick_freq_list[queue_shaper.BF.tick] * 8 / 1000;

		if ((queue_shaper.BF.tokens * g_tick_freq_list[queue_shaper.BF.tick] * 8) % 1000)
			prate->rate += 1;

	} else {
		prate->unit = QOS_RATE_UNIT_PPS;
		prate->rate = queue_shaper.BF.tokens * g_tick_freq_list[queue_shaper.BF.tick];
	}

	ret = jl61xx_qos_queue_shaper_enable_get(dev, shaper_idx, &enable);
	if (ret)
		return ret;

	prate->enable = enable;

	return JL_ERR_OK;
}


jl_ret_t jl61xx_qos_queue_shaper_set(jl_device_t *dev,
                                     jl_uint8 eport,
                                     jl_qos_queue_shaper_t *pshaper)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 queue_idx = 0;
	jl_uint8 shaper_idx = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pshaper);
	JL_CHECK_PORT(dev, eport);

	/*update tickstep value */
	ret = jl61xx_calc_tick_freq(dev);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	cport = jlxx_port_l2c(dev, eport);

	for (; queue_idx < QOS_QUEUE_NUM; queue_idx++) {
		shaper_idx = cport * 8 + queue_idx;

		if (pshaper->queue[queue_idx].valid) {
			ret = jl61xx_qos_queue_shaper_rate_set(dev, shaper_idx, &(pshaper->queue[queue_idx]));
			if (ret)
				return ret;
		}
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_queue_shaper_get(jl_device_t *dev,
                                     jl_uint8 eport,
                                     jl_qos_queue_shaper_t *pshaper)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 queue_idx = 0;
	jl_uint8 shaper_idx = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pshaper);
	JL_CHECK_PORT(dev, eport);

	/*update tickstep value */
	ret = jl61xx_calc_tick_freq(dev);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	cport = jlxx_port_l2c(dev, eport);

	for (; queue_idx < QOS_QUEUE_NUM; queue_idx++) {
		shaper_idx = cport * 8 + queue_idx;

		ret = jl61xx_qos_queue_shaper_rate_get(dev, shaper_idx, &(pshaper->queue[queue_idx]));
		if (ret)
			return ret;

		pshaper->queue[queue_idx].valid = TRUE;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_port_shaper_enable_set(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, PORT_SHAPER_ENABLE, en_tbl, 0, 0);

	if (enable == DISABLED)
		CLR_BIT(en_tbl.BF.enable, eport);
	else
		SET_BIT(en_tbl.BF.enable, eport);

	REGISTER_WRITE(dev, SWCORE, PORT_SHAPER_ENABLE, en_tbl, 0, 0);

	REGISTER_READ(dev, TOP, FW_RESERVED14, rsvd14, 0, 0);
	if (enable == DISABLED) {
		/*enable shaper with fw work around*/
		CLR_BIT(rsvd14.BF.fw_reserved14, 18 + eport);
	} else {
		/*disable shaper with fw work around*/
		SET_BIT(rsvd14.BF.fw_reserved14, 18 + eport);
	}
	REGISTER_WRITE(dev, TOP, FW_RESERVED14, rsvd14, 0, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_port_shaper_enable_get(jl_device_t *dev,
        jl_uint8 eport,
        jl_uint8 *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(dev, SWCORE, PORT_SHAPER_ENABLE, en_tbl, 0, 0);

	*penable = GET_BIT(en_tbl.BF.enable, eport);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_port_shaper_bucket_set(jl_device_t *dev,
        jl_uint8 cport,
        jl_uint8 shaper_mode,
        jl_uint16 tokens)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_PORT_SHAPER_BUCKET_THRESHOLD_CONFIGURATION_t thrs_tbl;
	SWCORE_PORT_SHAPER_BUCKET_CAPACITY_CONFIGURATION_t capt_tbl;

	memset(&thrs_tbl, 0, sizeof(SWCORE_PORT_SHAPER_BUCKET_THRESHOLD_CONFIGURATION_t));
	memset(&capt_tbl, 0, sizeof(SWCORE_PORT_SHAPER_BUCKET_CAPACITY_CONFIGURATION_t));

	if (shaper_mode == QOS_RATE_UNIT_KBPS ||
	    shaper_mode == QOS_RATE_UNIT_MBPS) {
		thrs_tbl.BF.threshold = (tokens * QOS_BUCKET_THRESHOLD_MULTIPLE_BPS);

		if (thrs_tbl.BF.threshold < QOS_BUCKET_MINDEPTH_VAL_BPS)
			thrs_tbl.BF.threshold = QOS_BUCKET_MINDEPTH_VAL_BPS;

		capt_tbl.BF.bucket_capacity =
		    (thrs_tbl.BF.threshold * QOS_BUCKET_BURSTSIZE_MULTIPLE_BPS);

	} else if (shaper_mode == QOS_RATE_UNIT_PPS) {
		thrs_tbl.BF.threshold = (tokens * QOS_BUCKET_THRESHOLD_MULTIPLE_PPS);
		capt_tbl.BF.bucket_capacity =
		    (thrs_tbl.BF.threshold * QOS_BUCKET_BURSTSIZE_MULTIPLE_PPS);
	} else
		return JL_ERR_PARAM;

	REGISTER_WRITE(dev, SWCORE, PORT_SHAPER_BUCKET_THRESHOLD_CONFIGURATION, thrs_tbl, cport, 0);
	REGISTER_WRITE(dev, SWCORE, PORT_SHAPER_BUCKET_CAPACITY_CONFIGURATION, capt_tbl, cport, 0);

	return JL_ERR_OK;
}



jl_ret_t jl61xx_qos_port_shaper_set(jl_device_t *dev,
                                     jl_uint8 eport,
                                     jl_qos_port_shaper_t *pshaper)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;
	jl_uint8 tick = 0;
	jl_uint16 tokens = 0;
	jl_uint32 rate = 0;

	jl_qos_shaper_rate_t shaper_rate;
	SWCORE_PORT_SHAPER_RATE_CONFIGURATION_t port_shaper;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pshaper);
	JL_CHECK_PORT(dev, eport);

	memset(&port_shaper, 0, sizeof(SWCORE_PORT_SHAPER_RATE_CONFIGURATION_t));

	/*update tickstep value */
	ret = jl61xx_calc_tick_freq(dev);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	cport = jlxx_port_l2c(dev, eport);

	if (pshaper->enable == DISABLED) {
		ret = jl61xx_qos_port_shaper_enable_set(dev, cport, DISABLED);
		return ret;
	}

	rate = pshaper->rate;

	JL_CHECK_ENUM(pshaper->unit, QOS_RATE_UNIT_NUM);

	if (pshaper->unit == QOS_RATE_UNIT_MBPS) {
		rate *= 1000;
	}

	shaper_rate.valid = 1;
	shaper_rate.enable = 1;
	shaper_rate.rate = pshaper->rate;
	shaper_rate.unit = pshaper->unit;

	ret = jl61xx_qos_shaper_get_tick(dev, &shaper_rate, &tick);
	if (ret)
		return ret;

	if (pshaper->unit == QOS_RATE_UNIT_KBPS ||
	    pshaper->unit == QOS_RATE_UNIT_MBPS) {
		port_shaper.BF.packets_not_bytes = 0;
		tokens = (rate * 1000 / 8) / g_tick_freq_list[tick];
		if (tokens < 0xfff && tokens > 0)
			tokens += 1;
	} else if (pshaper->unit == QOS_RATE_UNIT_PPS) {
		port_shaper.BF.packets_not_bytes = 1;
		tokens = rate / g_tick_freq_list[tick];
	} else
		return JL_ERR_PARAM;

	port_shaper.BF.tick = tick;
	port_shaper.BF.tokens = tokens;
	port_shaper.BF.ifg_correction = (QOS_IFG_DEFAULT_LENGTH + QOS_FCS_DEFAULT_LENGTH);

	/*set bucket*/
	ret = jl61xx_qos_port_shaper_bucket_set(dev, cport, pshaper->unit, port_shaper.BF.tokens);
	if (ret)
		return ret;

	REGISTER_WRITE(dev, SWCORE, PORT_SHAPER_RATE_CONFIGURATION, port_shaper, cport, 0);

	ret = jl61xx_qos_port_shaper_enable_set(dev, cport, ENABLED);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_port_shaper_get(jl_device_t *dev,
                                     jl_uint8 eport,
                                     jl_qos_port_shaper_t *pshaper)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;
	jl_uint8 enable = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pshaper);
	JL_CHECK_PORT(dev, eport);

	/*update tickstep value */
	ret = jl61xx_calc_tick_freq(dev);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, PORT_SHAPER_RATE_CONFIGURATION, port_shaper, cport, 0);

	if (port_shaper.BF.packets_not_bytes == 0) {
		pshaper->unit = QOS_RATE_UNIT_KBPS;

		if (port_shaper.BF.tokens != 0)
			port_shaper.BF.tokens -= 1;
		pshaper->rate = port_shaper.BF.tokens * g_tick_freq_list[port_shaper.BF.tick] * 8 / 1000;

		if ((port_shaper.BF.tokens * g_tick_freq_list[port_shaper.BF.tick] * 8) % 1000)
			pshaper->rate += 1;

	} else {
		pshaper->unit = QOS_RATE_UNIT_PPS;
		pshaper->rate = port_shaper.BF.tokens * g_tick_freq_list[port_shaper.BF.tick];
	}

	ret = jl61xx_qos_port_shaper_enable_get(dev, cport, &enable);
	if (ret)
		return ret;

	pshaper->enable = enable;

	return JL_ERR_OK;
}


static jl_ret_t jl61xx_qos_prio_shaper_bucket_set(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_uint8 shaper_mode,
        jl_uint16 tokens)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	SWCORE_PRIO_SHAPER_BUCKET_THRESHOLD_CONFIGURATION_t thrs_tbl;
	SWCORE_PRIO_SHAPER_BUCKET_CAPACITY_CONFIGURATION_t capt_tbl;

	memset(&thrs_tbl, 0, sizeof(SWCORE_PRIO_SHAPER_BUCKET_THRESHOLD_CONFIGURATION_t));
	memset(&capt_tbl, 0, sizeof(SWCORE_PRIO_SHAPER_BUCKET_CAPACITY_CONFIGURATION_t));

	if (shaper_mode == QOS_RATE_UNIT_KBPS ||
	    shaper_mode == QOS_RATE_UNIT_MBPS) {
		thrs_tbl.BF.threshold = (tokens * QOS_BUCKET_THRESHOLD_MULTIPLE_BPS);

		if (thrs_tbl.BF.threshold < QOS_BUCKET_MINDEPTH_VAL_BPS)
			thrs_tbl.BF.threshold = QOS_BUCKET_MINDEPTH_VAL_BPS;

		capt_tbl.BF.bucket_capacity =
		    (thrs_tbl.BF.threshold * QOS_BUCKET_BURSTSIZE_MULTIPLE_BPS);

	} else if (shaper_mode == QOS_RATE_UNIT_PPS) {
		thrs_tbl.BF.threshold = (tokens * QOS_BUCKET_THRESHOLD_MULTIPLE_PPS);
		capt_tbl.BF.bucket_capacity =
		    (thrs_tbl.BF.threshold * QOS_BUCKET_BURSTSIZE_MULTIPLE_PPS);
	} else
		return JL_ERR_PARAM;

	REGISTER_WRITE(dev, SWCORE, PRIO_SHAPER_BUCKET_THRESHOLD_CONFIGURATION, thrs_tbl, shaper_idx, 0);
	REGISTER_WRITE(dev, SWCORE, PRIO_SHAPER_BUCKET_CAPACITY_CONFIGURATION, capt_tbl, shaper_idx, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_prio_shaper_enable_set(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_uint8 enable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	REGISTER_READ(dev, SWCORE, PRIO_SHAPER_ENABLE, en_tbl, 0, 0);

	if (shaper_idx >= 64) {
		shaper_idx %= 64;

		if (enable == DISABLED)
			CLR_BIT(en_tbl.BF.enable_64_87, shaper_idx);
		else
			SET_BIT(en_tbl.BF.enable_64_87, shaper_idx);
	} else if (shaper_idx >= 32) {
		shaper_idx %= 32;

		if (enable == DISABLED)
			CLR_BIT(en_tbl.BF.enable_32_63, shaper_idx);
		else
			SET_BIT(en_tbl.BF.enable_32_63, shaper_idx);
	} else {
		if (enable == DISABLED)
			CLR_BIT(en_tbl.BF.enable_0_31, shaper_idx);
		else
			SET_BIT(en_tbl.BF.enable_0_31, shaper_idx);
	}

	REGISTER_WRITE(dev, SWCORE, PRIO_SHAPER_ENABLE, en_tbl, 0, 0);
	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_prio_shaper_enable_get(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_uint8 *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(penable);

	REGISTER_READ(dev, SWCORE, PRIO_SHAPER_ENABLE, en_tbl, 0, 0);

	if (shaper_idx >= 64) {
		shaper_idx %= 64;
		*penable = GET_BIT(en_tbl.BF.enable_64_87, shaper_idx);
	} else if (shaper_idx >= 32) {
		shaper_idx %= 32;
		*penable = GET_BIT(en_tbl.BF.enable_32_63, shaper_idx);
	} else {
		*penable = GET_BIT(en_tbl.BF.enable_0_31, shaper_idx);
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_prio_shaper_rate_set(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_qos_shaper_rate_t *prate)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 tick = 0;
	jl_uint16 tokens = 0;
	jl_uint32 rate = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(prate);

	SWCORE_PRIO_SHAPER_RATE_CONFIGURATION_t prio_shaper;
	memset(&prio_shaper, 0, sizeof(SWCORE_PRIO_SHAPER_RATE_CONFIGURATION_t));

	if (prate->enable == DISABLED) {
		ret = jl61xx_qos_prio_shaper_enable_set(dev, shaper_idx, DISABLED);
		return ret;
	}

	rate = prate->rate;

	if (prate->unit == QOS_RATE_UNIT_MBPS)
		rate *= 1000;

	ret = jl61xx_qos_shaper_get_tick(dev, prate, &tick);
	if (ret)
		return ret;

	if (prate->unit == QOS_RATE_UNIT_KBPS ||
	    prate->unit == QOS_RATE_UNIT_MBPS) {
		prio_shaper.BF.packets_not_bytes = 0;
		tokens = (rate * 1000 / 8) / g_tick_freq_list[tick];
		if (tokens < 0xfff && tokens > 0)
			tokens += 1;/*SS-70*/
	} else if (prate->unit == QOS_RATE_UNIT_PPS) {
		prio_shaper.BF.packets_not_bytes = 1;
		tokens = rate / g_tick_freq_list[tick];
	} else
		return JL_ERR_PARAM;

	prio_shaper.BF.tick = tick;
	prio_shaper.BF.tokens = tokens;
	prio_shaper.BF.ifg_correction = (QOS_IFG_DEFAULT_LENGTH + QOS_FCS_DEFAULT_LENGTH);

	/*set bucket*/
	ret = jl61xx_qos_prio_shaper_bucket_set(dev, shaper_idx, prate->unit, prio_shaper.BF.tokens);
	if (ret)
		return ret;

	REGISTER_WRITE(dev, SWCORE, PRIO_SHAPER_RATE_CONFIGURATION, prio_shaper, shaper_idx, 0);

	ret = jl61xx_qos_prio_shaper_enable_set(dev, shaper_idx, ENABLED);

	return ret;
}

static jl_ret_t jl61xx_qos_prio_shaper_rate_get(jl_device_t *dev,
        jl_uint8 shaper_idx,
        jl_qos_shaper_rate_t *prate)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 enable = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(prate);

	REGISTER_READ(dev, SWCORE, PRIO_SHAPER_RATE_CONFIGURATION, prio_shaper, shaper_idx, 0);

	if (prio_shaper.BF.packets_not_bytes == 0) {
		prate->unit = QOS_RATE_UNIT_KBPS;

		if (prio_shaper.BF.tokens != 0)
			prio_shaper.BF.tokens -= 1;
		prate->rate = prio_shaper.BF.tokens * g_tick_freq_list[prio_shaper.BF.tick] * 8 / 1000;

		if ((prio_shaper.BF.tokens * g_tick_freq_list[prio_shaper.BF.tick] * 8) % 1000)
			prate->rate += 1;
	} else {
		prate->unit = QOS_RATE_UNIT_PPS;
		prate->rate = prio_shaper.BF.tokens * g_tick_freq_list[prio_shaper.BF.tick];
	}

	ret = jl61xx_qos_prio_shaper_enable_get(dev, shaper_idx, &enable);
	if (ret)
		return ret;

	prate->enable = enable;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_prio_shaper_set(jl_device_t *dev,
                                    jl_uint8 eport,
                                    jl_qos_priority_shaper_t *pshaper)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 prio_idx = 0;
	jl_uint8 shaper_idx = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pshaper);
	JL_CHECK_PORT(dev, eport);

	/*update tickstep value */
	ret = jl61xx_calc_tick_freq(dev);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	cport = jlxx_port_l2c(dev, eport);

	for (; prio_idx < QOS_PRIORITY_NUM; prio_idx++) {
		shaper_idx = cport * 8 + prio_idx;

		if (pshaper->priority[prio_idx].valid) {
			ret = jl61xx_qos_prio_shaper_rate_set(dev, shaper_idx, &(pshaper->priority[prio_idx]));
			if (ret)
				return ret;
		}
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_prio_shaper_get(jl_device_t *dev,
                                    jl_uint8 eport,
                                    jl_qos_priority_shaper_t *pshaper)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 prio_idx = 0;
	jl_uint8 shaper_idx = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pshaper);
	JL_CHECK_PORT(dev, eport);

	/*update tickstep value */
	ret = jl61xx_calc_tick_freq(dev);
	if ((ret != JL_ERR_EXISTS) && (ret != JL_ERR_OK))
		return ret;

	cport = jlxx_port_l2c(dev, eport);

	for (; prio_idx < QOS_QUEUE_NUM; prio_idx++) {
		shaper_idx = cport * 8 + prio_idx;

		ret = jl61xx_qos_prio_shaper_rate_get(dev, shaper_idx, &(pshaper->priority[prio_idx]));
		if (ret)
			return ret;

		pshaper->priority[prio_idx].valid = TRUE;
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_sche_weight_set(jl_device_t *dev,
        jl_uint8 sche_idx,
        jl_qos_weight_t *pweight)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pweight);

	SWCORE_DWRR_WEIGHT_CONFIGURATION_t weight_tbl;
	memset(&weight_tbl, 0, sizeof(SWCORE_DWRR_WEIGHT_CONFIGURATION_t));

	weight_tbl.BF.weight = pweight->weight;

	REGISTER_WRITE(dev, SWCORE, DWRR_WEIGHT_CONFIGURATION, weight_tbl, sche_idx, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_sche_weight_get(jl_device_t *dev,
        jl_uint8 sche_idx,
        jl_qos_weight_t *pweight)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pweight);

	REGISTER_READ(dev, SWCORE, DWRR_WEIGHT_CONFIGURATION, weight_tbl, sche_idx, 0);

	pweight->weight = weight_tbl.BF.weight;
	pweight->valid = TRUE;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_queue_dwrr_set(jl_device_t *dev,
                                   jl_uint8 eport,
                                   jl_qos_queue_dwrr_t *pdwrr)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 queue_idx = 0;
	jl_uint8 sche_idx = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pdwrr);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	for (; queue_idx < QOS_QUEUE_NUM; queue_idx++) {
		sche_idx = cport * 8 + queue_idx;

		if (pdwrr->queue[queue_idx].valid) {
			ret = jl61xx_qos_sche_weight_set(dev, sche_idx, &(pdwrr->queue[queue_idx]));
			if (ret)
				return ret;
		}
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_queue_dwrr_get(jl_device_t *dev,
                                   jl_uint8 eport,
                                   jl_qos_queue_dwrr_t *pdwrr)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 queue_idx = 0;
	jl_uint8 sche_idx = 0;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pdwrr);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	for (; queue_idx < QOS_QUEUE_NUM; queue_idx++) {
		sche_idx = cport * 8 + queue_idx;

		ret = jl61xx_qos_sche_weight_get(dev, sche_idx, &(pdwrr->queue[queue_idx]));
		if (ret)
			return ret;
	}

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_enqueue_enable_set(jl_device_t *dev,
                                       jl_uint8 eport,
                                       jl_qos_enqueue_state_t *pstate)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pstate);
	JL_CHECK_PORT(dev, eport);

	SWCORE_ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_t enqueue_tbl;
	memset(&enqueue_tbl, 0, sizeof(SWCORE_ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_t));

	cport = jlxx_port_l2c(dev, eport);

	enqueue_tbl.BF.q_on = pstate->enable_mask;

	REGISTER_WRITE(dev, SWCORE, ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES, enqueue_tbl, cport, 0);

	gp_res_mag_grt[dev->id]->enqueue_mask[cport] = pstate->enable_mask;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_enqueue_enable_get(jl_device_t *dev,
                                       jl_uint8 eport,
                                       jl_qos_enqueue_state_t *pstate)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pstate);
	JL_CHECK_PORT(dev, eport);

	cport = jlxx_port_l2c(dev, eport);

	REGISTER_READ(dev, SWCORE, ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES, enqueue_tbl, cport, 0);

	pstate->enable_mask = enqueue_tbl.BF.q_on;

	return JL_ERR_OK;
}

#if 0
static jl_ret_t jl61xx_qos_res_guarantee_set(jl_device_t *dev,
        jl_uint8 pointer,
        jl_uint16 cells)
{
	jl_ret_t ret = JL_ERR_OK;
	JL_CHECK_POINTER(dev);

	SWCORE_EGRESS_QUEUE_GUARANTEE_SET_t grt_tbl;

	if (pointer >= QOS_RESOURCE_MANAGEMENT_POINTER_NUM)
		return JL_ERR_PARAM;

	grt_tbl.BF.guarantee = cells;

	REGISTER_WRITE(dev, SWCORE, EGRESS_QUEUE_GUARANTEE_SET, grt_tbl, pointer, 0);

	gp_res_mag_grt[dev->id]->guarantee[pointer] = grt_tbl.BF.guarantee;

	return JL_ERR_OK;
}
#endif


static jl_ret_t jl61xx_qos_res_guarantee_get(jl_device_t *dev,
        jl_uint8 pointer,
        jl_uint16 *pcells)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pcells);

	if (pointer >= QOS_RESOURCE_MANAGEMENT_POINTER_NUM)
		return JL_ERR_PARAM;

	REGISTER_READ(dev, SWCORE, EGRESS_QUEUE_GUARANTEE_SET, grt_tbl, pointer, 0);

	*pcells = grt_tbl.BF.guarantee;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_res_limiter_set(jl_device_t *dev,
        jl_uint8 pointer,
        jl_uint16 xon,
		jl_uint16 threshold)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);

	if (pointer >= QOS_RESOURCE_MANAGEMENT_POINTER_NUM)
		return JL_ERR_PARAM;

	SWCORE_RESOURCE_LIMITER_SET_t limit_tbl;

	limit_tbl.BF.xon = xon & BITS_WIDTH(SWCORE_RESOURCE_LIMITER_SET_OFFSET_XON_WIDTH);
	limit_tbl.BF.threshold = threshold & BITS_WIDTH(SWCORE_RESOURCE_LIMITER_SET_OFFSET_THRESHOLD_WIDTH);

	REGISTER_WRITE(dev, SWCORE, RESOURCE_LIMITER_SET, limit_tbl, pointer, 0);

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_res_limiter_get(jl_device_t *dev,
        jl_uint8 pointer,
        jl_uint16 *pxon,
		jl_uint16 *pthreshold)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pxon);
	JL_CHECK_POINTER(pthreshold);

	if (pointer >= QOS_RESOURCE_MANAGEMENT_POINTER_NUM)
		return JL_ERR_PARAM;

	REGISTER_READ(dev, SWCORE, RESOURCE_LIMITER_SET, limit_tbl, pointer, 0);

	*pxon = limit_tbl.BF.xon;
	*pthreshold = limit_tbl.BF.threshold;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_res_manage_info_get(jl_device_t *dev,
        jl_uint8 index,
        jl61xx_qos_res_info_t *pinfo)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pinfo);

	if (index != QOS_MULTICAST_RES_MANAGEMENT_ID)
		index = jlxx_port_l2c(dev, index);

	REGISTER_READ(dev, SWCORE, EGRESS_RESOURCE_MANAGEMENT_MODE, mode_tbl, index, 0);

	pinfo->mode = mode_tbl.BF.mode;
	pinfo->eql_ptr = mode_tbl.BF.eql_ptr;
	pinfo->eqg_ptr = mode_tbl.BF.eqg_ptr;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_guarantee_res_chk(jl_device_t *dev,
        jl_uint8 index,
        jl_uint8 *ptr,
        jl_uint8 *pcells)
{
	jl_uint16 now_res = 0;
	jl_uint16 need_res = 0;
	jl_uint16 guarantee = 0;
	jl_uint16 margin_res = 0;
	jl_uint8 pointer = 0;
	jl_uint8 valid_pointer = 0xff;
	jl_uint32 lport;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(ptr);
	JL_CHECK_POINTER(pcells);

	valid_pointer = 0;

	if (index == QOS_MULTICAST_RES_MANAGEMENT_ID)
		valid_pointer = QOS_RESOURCE_MANAGEMENT_POINTER_NUM - 1;  /* index 3 is for multicast*/

	margin_res = gp_res_mag_grt[dev->id]->global_margin + gp_res_mag_grt[dev->id]->ffa_margin;

	JL_FOR_EACH_PORT(dev, lport) {
		cport = jlxx_port_l2c(dev, lport);
		if ((cport < QOS_PORT_MAX) &&
			(gp_res_mag_grt[dev->id]->resource[cport].mode == QOS_QUEUE_RES_GUARANTEE)) {
			pointer = gp_res_mag_grt[dev->id]->resource[cport].eqg_ptr;
			guarantee = gp_res_mag_grt[dev->id]->guarantee[pointer];
			now_res += (QOS_QUEUE_NUM * guarantee);
		}
	}

	if (gp_res_mag_grt[dev->id]->resource[QOS_MULTICAST_RES_MANAGEMENT_ID].mode == QOS_QUEUE_RES_GUARANTEE) {
		pointer = gp_res_mag_grt[dev->id]->resource[QOS_MULTICAST_RES_MANAGEMENT_ID].eqg_ptr;
		guarantee = gp_res_mag_grt[dev->id]->guarantee[pointer];
		now_res += guarantee;
	}

	guarantee =  gp_res_mag_grt[dev->id]->guarantee[valid_pointer];

	if (index != QOS_MULTICAST_RES_MANAGEMENT_ID)
		need_res += (QOS_QUEUE_NUM * guarantee);
	else
		need_res += guarantee;

	if ((now_res + need_res) > (QOS_RESOURCE_TOTAL_CELLS - margin_res))
		return JL_ERR_RESOURCE;

	*ptr = valid_pointer;
	*pcells = guarantee;

	return JL_ERR_OK;
}


static jl_ret_t jl61xx_qos_check_port_empty(jl_device_t *dev, jl_uint32 l_mask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 c_mask;
	jl_uint8 i;

	JL_CHECK_POINTER(dev);

	ret = jlxx_portmask_l2c(dev, l_mask, &c_mask);
	if (ret)
		return ret;

	for (i = 0; i < QOS_PORT_MAX; i++) {
		REGISTER_READ(dev, SWCORE, PACKET_BUFFER_STATUS, stat, 0, 0);

		if (((~stat.BF.empty) & c_mask) == 0) {
			return JL_ERR_OK;
		}
		port_udelay(50000);
	}

	return JL_ERR_BUSY;
}


static jl_ret_t jl61xx_qos_drain_port(jl_device_t *dev, jl_uint32 l_mask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 c_mask = 0;
	jl_uint32 lport, cport;
	SWCORE_DRAIN_PORT_t drain_mask;
	SWCORE_ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_t enqueue_tbl;

	JL_CHECK_POINTER(dev);
	//disable all queues

	memset(&drain_mask, 0, sizeof(SWCORE_DRAIN_PORT_t));
	memset(&enqueue_tbl, 0, sizeof(SWCORE_ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES_t));

	enqueue_tbl.BF.q_on = 0;
	JL_FOR_EACH_PORT(dev, lport) {
		if (CHECK_BIT(l_mask, lport))
		{
			cport = jlxx_port_l2c(dev, lport);
			REGISTER_WRITE(dev, SWCORE, ENABLE_ENQUEUE_TO_PORTS_AND_QUEUES, enqueue_tbl, cport, 0);
		}
	}

	//drain port
	ret = jlxx_portmask_l2c(dev, l_mask, &c_mask);
	if (ret)
		return ret;

	drain_mask.BF.drain_mask = c_mask;
	REGISTER_WRITE(dev, SWCORE, DRAIN_PORT, drain_mask, 0, 0);

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_recover_port(jl_device_t *dev, jl_uint32 l_mask)
{
	jl_ret_t ret = JL_ERR_OK;
	SWCORE_DRAIN_PORT_t drain_mask;
	jl_qos_enqueue_state_t queue;
	jl_uint32 lport;
	jl_uint32 cport;

	JL_CHECK_POINTER(dev);

	drain_mask.BF.drain_mask = 0;
	REGISTER_WRITE(dev, SWCORE, DRAIN_PORT, drain_mask, 0, 0);

	//enable queues
	JL_FOR_EACH_PORT(dev, lport) {
		if (CHECK_BIT(l_mask, lport))
		{
			cport = jlxx_port_l2c(dev, lport);
			if (cport < QOS_PORT_MAX) {
				queue.enable_mask = gp_res_mag_grt[dev->id]->enqueue_mask[cport];
				ret = jl61xx_qos_enqueue_enable_set(dev, lport, &queue);
				if (ret)
					return ret;
			}
		}
	}

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_res_manage_mode_set(jl_device_t *dev,
                                        jl_uint8 index,
                                        jl_uint8 mode)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 eqg_ptr = 0;
	jl_uint8 cells = 0;
	jl_uint32 cport, lport;

	JL_CHECK_POINTER(dev);
	JL_CHECK_ENUM(mode, QOS_QUEUE_RES_NUM);

	lport = index;
	//must drain port first before this setting
	if (index != QOS_MULTICAST_RES_MANAGEMENT_ID)
	{
		cport = jlxx_port_l2c(dev, index);

		if (cport == UNDEF_PORT)
			return JL_ERR_PORT;

		index = cport;
	}

	REGISTER_READ(dev, SWCORE, EGRESS_RESOURCE_MANAGEMENT_MODE, mode_tbl, index, 0);

	/*has been set already*/
	if (mode == mode_tbl.BF.mode)
		return JL_ERR_OK;

	mode_tbl.BF.mode = (mode == QOS_QUEUE_RES_LIMITER) ? 0 : 1;

	/*set guaranteed*/
	if (mode_tbl.BF.mode) {
		ret = jl61xx_qos_guarantee_res_chk(dev, lport, &eqg_ptr, &cells);
		if (ret)
			return ret;
#if 0
		guarantee has been set by fw wr already in this patch
		if (eqg_ptr != (QOS_RESOURCE_MANAGEMENT_POINTER_NUM- 1)) { //pointer 3 is used for multicast
			ret = jl61xx_qos_res_guarantee_set(dev, eqg_ptr, cells);
			if (ret)
				return ret;
		}
#endif
	}

	mode_tbl.BF.eqg_ptr = eqg_ptr;
	REGISTER_WRITE(dev, SWCORE, EGRESS_RESOURCE_MANAGEMENT_MODE, mode_tbl, index, 0);

	gp_res_mag_grt[dev->id]->resource[index].mode = mode_tbl.BF.mode;
	gp_res_mag_grt[dev->id]->resource[index].eqg_ptr = mode_tbl.BF.eqg_ptr;
	gp_res_mag_grt[dev->id]->resource[index].eql_ptr = mode_tbl.BF.eql_ptr;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_res_manage_margin_get(jl_device_t *dev,
        jl_uint16 *pglobal,
        jl_uint16 *pffa)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(pglobal);
	JL_CHECK_POINTER(pffa);

	REGISTER_READ(dev, SWCORE, EGRESS_RESOURCE_MANAGEMENT_MARGIN, margin, 0, 0);

	*pglobal = margin.BF.global_margin;
	*pffa = margin.BF.ffa_margin;

	return JL_ERR_OK;
}

static jl_ret_t jl61xx_qos_collect_resource_info(jl_device_t *dev)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 cells = 0;
	jl_uint16 cells_ffa;
	jl_uint32 lport, cport;
	jl_uint8 idx = 0;
	jl61xx_qos_res_info_t res_info;

	JL_CHECK_POINTER(dev);

	for (idx = 0; idx < QOS_RESOURCE_MANAGEMENT_POINTER_NUM; idx++) {
		ret = jl61xx_qos_res_guarantee_get(dev, idx, &cells);
		if (ret)
			return ret;
		gp_res_mag_grt[dev->id]->guarantee[idx] = cells;
	}

	JL_FOR_EACH_PORT(dev, lport) {
		ret = jl61xx_qos_res_manage_info_get(dev, lport, &res_info);
		if (ret)
			return ret;
		cport = jlxx_port_l2c(dev, lport);
		gp_res_mag_grt[dev->id]->resource[cport] = res_info;
	}

	ret = jl61xx_qos_res_manage_info_get(dev, QOS_MULTICAST_RES_MANAGEMENT_ID, &res_info);
	if (ret)
		return ret;
	gp_res_mag_grt[dev->id]->resource[QOS_MULTICAST_RES_MANAGEMENT_ID] = res_info;

	ret = jl61xx_qos_res_manage_margin_get(dev, &cells, &cells_ffa);
	if (ret)
		return ret;

	gp_res_mag_grt[dev->id]->global_margin = cells;
	gp_res_mag_grt[dev->id]->ffa_margin = cells_ffa;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_schedule_set(jl_device_t *dev, jl_uint8 eport, jl_qos_schedule_t *psche)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 i, dwrr_num = 0, q_prio = 0;
	jl_qos_queue_priority_t  t_prio;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(psche);
	JL_CHECK_PORT(dev, eport);

	if (!gp_res_mag_grt[dev->id]->res_init)
		return JL_ERR_INIT;

	for (i = 0; i < QOS_QUEUE_NUM; i++)
	{
		if (psche->type[i] > QOS_SCHE_DWRR)
			return JL_ERR_PARAM;

		if (psche->type[i] == QOS_SCHE_DWRR)
		{
			t_prio.queue[i].priority = 7;
			dwrr_num++;
		}
		else
			t_prio.queue[i].priority = q_prio++;

		t_prio.queue[i].valid = TRUE;
	}

	if (dwrr_num == 1)
		return JL_ERR_CONFIG;

	ret = jl61xx_qos_queue_priority_set(dev, eport, &t_prio);

	return ret;
}


jl_ret_t jl61xx_qos_schedule_get(jl_device_t *dev, jl_uint8 eport, jl_qos_schedule_t *psche)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_qos_queue_priority_t  t_prio;
	jl_uint8 i = 0, dwrr_num = 0;

	JL_CHECK_POINTER(dev);
	JL_CHECK_POINTER(psche);
	JL_CHECK_PORT(dev, eport);

	memset(&t_prio, 0, sizeof(jl_qos_queue_priority_t));

	ret = jl61xx_qos_queue_priority_get(dev, eport, &t_prio);
	if (ret)
		return ret;

	for (i = 0; i < QOS_QUEUE_NUM; i++)
	{
		psche->type[i] = QOS_SCHE_SP;

		if (t_prio.queue[i].priority == 7)
		{
			psche->type[i] = QOS_SCHE_DWRR;
			dwrr_num++;

			if ((i == 7) && (dwrr_num == 1))
				psche->type[i] = QOS_SCHE_SP;
		}
	}
	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_init(jl_device_t *dev)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 lport, cport;
	jl_uint8 idx;
	jl_qos_enqueue_state_t state;

	JL_CHECK_POINTER(dev);

	if (gp_res_mag_grt[dev->id] == NULL)
	{
		gp_res_mag_grt[dev->id] = (jl61xx_qos_res_management_t*)port_mem_malloc(sizeof(jl61xx_qos_res_management_t));

		if (!gp_res_mag_grt[dev->id])
			return JL_ERR_MEMORY;

		memset((void*)gp_res_mag_grt[dev->id], 0, sizeof(jl61xx_qos_res_management_t));

		ret = jl61xx_qos_collect_resource_info(dev);
		if (ret)
			goto err;

		JL_FOR_EACH_PORT(dev, lport) {
			ret = jl61xx_qos_enqueue_enable_get(dev, lport, &state);
			if (ret)
				goto err;

			cport = jlxx_port_l2c(dev, lport);
			gp_res_mag_grt[dev->id]->enqueue_mask[cport] = state.enable_mask;
		}
	}

	for (idx = 0; idx < QOS_RESOURCE_MANAGEMENT_POINTER_NUM; idx++)
		g_rsvd_limiter_xon[dev->id][idx] = 0xffff;

	return JL_ERR_OK;

err:
	if (gp_res_mag_grt[dev->id])
		port_mem_free(gp_res_mag_grt[dev->id]);

	gp_res_mag_grt[dev->id] = NULL;

	return ret;
}

jl_ret_t jl61xx_qos_deinit(jl_device_t *dev)
{
	JL_CHECK_POINTER(dev);

	if (gp_res_mag_grt[dev->id])
		port_mem_free(gp_res_mag_grt[dev->id]);

	gp_res_mag_grt[dev->id] = NULL;

	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_resouce_init(jl_device_t *dev, jl_uint8 queue_num)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 idx = 0;
	jl_uint16 limit_xon = 0;
	jl_uint16 limit_threshold = 0;

	JL_CHECK_POINTER(dev);

	if (gp_res_mag_grt[dev->id]->res_init)
		return JL_ERR_OK;
	else
		gp_res_mag_grt[dev->id]->res_init = 1;

	for (idx = 0; idx < QOS_RESOURCE_MANAGEMENT_POINTER_NUM; idx++) {
		ret = jl61xx_qos_res_limiter_get(dev, idx, &limit_xon, &limit_threshold);
		if (ret)
			return ret;

		if (limit_xon > QOS_RES_LIMITER_XON)
			g_rsvd_limiter_xon[dev->id][idx] = limit_xon;
		else
		{
			g_rsvd_limiter_xon[dev->id][idx] = 0xffff;
			continue;
		}
		g_rsvd_limit_threshold[dev->id][idx] = limit_threshold;
	}

	for (idx = 0; idx < QOS_RESOURCE_MANAGEMENT_POINTER_NUM; idx++) {
		if (g_rsvd_limiter_xon[dev->id][idx] != 0xffff) {
			ret = jl61xx_qos_res_limiter_set(dev, idx, QOS_RES_LIMITER_XON,
											 g_rsvd_limit_threshold[dev->id][idx]);
			if (ret)
				return ret;
		}
	}

	gp_res_mag_grt[dev->id]->queue_num = queue_num;
	
	return JL_ERR_OK;
}

jl_ret_t jl61xx_qos_resouce_deinit(jl_device_t *dev)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 idx = 0;

	JL_CHECK_POINTER(dev);

	if (!gp_res_mag_grt[dev->id]->res_init)
		return JL_ERR_OK;
	else
		gp_res_mag_grt[dev->id]->res_init = 0;

	for (idx = 0; idx < QOS_RESOURCE_MANAGEMENT_POINTER_NUM; idx++) {
		if (g_rsvd_limiter_xon[dev->id][idx] != 0xffff)
		{
			ret = jl61xx_qos_res_limiter_set(dev, idx, g_rsvd_limiter_xon[dev->id][idx], g_rsvd_limit_threshold[dev->id][idx]);
			if (ret)
				return ret;
			g_rsvd_limiter_xon[dev->id][idx] = 0xffff;
		}
	}

	return JL_ERR_OK;
}




