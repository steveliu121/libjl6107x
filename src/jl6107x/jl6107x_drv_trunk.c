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
#include"jl6107x/jl6107x_drv_trunk.h"
#include "jl6107x/jl6107x_chip.h"


static jl_ret_t _link_aggregation_hash_map_clear(jl_device_t *pdevice, jl_portmask_t la_cportmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 port_cnt = 0, i, hash_index;
	jl_port_t cport;
	jl_port_t port_member[JL_SWITCH_PORT_NUM];


	/* get the port members from la_portmask */
	JL_FOR_EACH_CPORT(pdevice, cport) {
		if (la_cportmask & (1 << cport)) {
			port_member[port_cnt] = cport;
			port_cnt++;
		}
	}
	for (i = 0; i < port_cnt; ++i) {
		for (hash_index = 0; hash_index < 256; hash_index++) {
			REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);

			CLR_BIT(la_weight.BF.ports, port_member[i]);

			REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);
		}
	}
	return ret;
}

static jl_ret_t _link_aggregation_hash_map_set(jl_device_t *pdevice, jl_portmask_t la_cportmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 port_cnt = 0, hash_cnt, i, j, hash_index;
	jl_port_t cport;
	jl_port_t port_member[JL_SWITCH_PORT_NUM];

	/* get the port members from la_portmask */
	JL_FOR_EACH_CPORT(pdevice, cport) {
		if (la_cportmask & (1 << cport)) {
			port_member[port_cnt] = cport;
			port_cnt++;
		}
	}
	if (port_cnt == 0)
		return ret;

	/* setup every port member of a link aggregation has equal weight*/
	hash_cnt = (jl_uint16)(256 / port_cnt);

	for (i = 0; i < port_cnt; ++i) {
		for (hash_index = hash_cnt * i; hash_index < hash_cnt * (i + 1); ++hash_index) {
			REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);

			la_weight.BF.ports |= 1 << port_member[i];
			for (j = 0; j < port_cnt; ++j) {
				if (i != j)
					CLR_BIT(la_weight.BF.ports, port_member[j]);
			}
			REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);
		}
	}

	if (256 - hash_index > 0) {
		i = 0;
		for (; hash_index < 256; hash_index++) {
			REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);

			la_weight.BF.ports |= 1 << port_member[i];
			for (j = 0; j < port_cnt; ++j) {
				if (i != j)
					CLR_BIT(la_weight.BF.ports, port_member[j]);
			}
			REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);
			i++;
		}
	}
	return ret;
}

static jl_ret_t _link_aggregation_clear_all(jl_device_t *pdevice)
{
	jl_uint32 i;
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport;
	SWCORE_LINK_AGGREGATION_CTRL_t la_ctrl;
	SWCORE_LINK_AGGREGATION_MEMBERSHIP_t sp2la;
	SWCORE_LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS_t la2phy;
	SWCORE_LINK_AGGREGATE_WEIGHT_t la_weight;

	/* clear the link aggregation ctrl table */
	memset(&la_ctrl, 0, sizeof(SWCORE_LINK_AGGREGATION_CTRL_t));
	REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_CTRL, la_ctrl, INDEX_ZERO, INDEX_ZERO);

	/* clear the link aggregation membership table and the link aggregation to physical ports members table*/
	sp2la.BF.la = 0;
	la2phy.BF.members = 0;
	JL_FOR_EACH_CPORT(pdevice, cport) {
		REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_MEMBERSHIP, sp2la, cport, INDEX_ZERO);
		REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cport, INDEX_ZERO);
	}

	/* clear the link aggregation weight table */
	la_weight.BF.ports = 0;
	for (i = 0; i < 256; ++i)
		REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, i, INDEX_ZERO);

	return ret;
}

static jl_ret_t _link_aggregation_one_port_one2one_set(jl_device_t *pdevice, jl_port_t cport)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 hash_index;

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_MEMBERSHIP, sp2la, cport, INDEX_ZERO);
	sp2la.BF.la = cport;
	REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_MEMBERSHIP, sp2la, cport, INDEX_ZERO);

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cport, INDEX_ZERO);
	la2phy.BF.members = 1 << cport;
	REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cport, INDEX_ZERO);

	for (hash_index = 0; hash_index < 256; ++hash_index) {
		REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);
		la_weight.BF.ports |= 1 << cport;
		REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);
	}

	return ret;
}

static jl_ret_t _link_aggregation_group_clear(jl_device_t *pdevice, jl_la_group_t cla_id)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport;
	SWCORE_LINK_AGGREGATION_MEMBERSHIP_t sp2la;
	jl_portmask_t la_cportmask;

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);
	la_cportmask = la2phy.BF.members;

	/*clear the link aggregation membership table*/
	la2phy.BF.members = 0;
	REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);

	/* clear the link aggregation to physical ports members table*/
	sp2la.BF.la = 0;
	JL_FOR_EACH_CPORT(pdevice, cport) {
		if (la_cportmask & (1 << cport)) {
			REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_MEMBERSHIP, sp2la, cport, INDEX_ZERO);
			/* set the deleted la port member one_to_one mapping */
			_link_aggregation_one_port_one2one_set(pdevice, cport);
		}
	}

	return ret;
}

static jl_ret_t _link_aggregation_portmask_set(jl_device_t *pdevice, jl_la_group_t cla_id, jl_portmask_t la_cportmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport;
	SWCORE_LINK_AGGREGATION_MEMBERSHIP_t sp2la;
	SWCORE_LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS_t la2phy;

	JL_FOR_EACH_CPORT(pdevice, cport) {
		if (la_cportmask & (1 << cport)) {
			sp2la.BF.la = cla_id;
			REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_MEMBERSHIP, sp2la, cport, INDEX_ZERO);
		}
	}
	la2phy.BF.members = la_cportmask;
	REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);

	_link_aggregation_hash_map_set(pdevice, la_cportmask);

	return ret;
}

static jl_ret_t _link_aggregation_port_del(jl_device_t *pdevice, jl_la_group_t cla_id, jl_port_t cport_id)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t cport;
	SWCORE_LINK_AGGREGATION_MEMBERSHIP_t sp2la;

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);
	if ((1 << cport_id) != (la2phy.BF.members & (1 << cport_id)))
		return JL_ERR_PORT;

	CLR_BIT(la2phy.BF.members, cport_id);

	if (cport_id == (jl_port_t)cla_id) {
		//clear la_id group
		_link_aggregation_group_clear(pdevice, cla_id);

		//set la group with the rest portmembers in la
		if (la2phy.BF.members != 0) {
			JL_FOR_EACH_CPORT(pdevice, cport)
				if (la2phy.BF.members & (1 << cport)) {
					break;
				}
			_link_aggregation_portmask_set(pdevice, (jl_la_group_t)cport, la2phy.BF.members);
		}
	} else {
		/* delete la for cport_id and delete port for cla_id */
		sp2la.BF.la = 0;
		REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_MEMBERSHIP, sp2la, cport_id, INDEX_ZERO);
		REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);
		/* reset the link aggregation weight table after del port */
		_link_aggregation_hash_map_set(pdevice, la2phy.BF.members);
		/* set the deleted port one_to_one mapping */
		_link_aggregation_one_port_one2one_set(pdevice, cport_id);
	}

	return ret;
}


/*
 * Set the rest ports which haven't being added to a link aggregation group  to the link aggregation.
 * one port to one link aggregation, the link aggregation number is the port number.
 */
static jl_ret_t _link_aggregation_one2one_set(jl_device_t *pdevice)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_la_group_t la_id;
	jl_port_t cport;
	jl_port_t port_status[((struct jl_switch_dev_s *)((pdevice)->switch_dev))->max_cport + 1];

	memset(port_status, 0, sizeof(port_status));

	/* get all the port members in all link aggregation */
	for (la_id = 0; la_id <= ((struct jl_switch_dev_s *)((pdevice)->switch_dev))->max_cport; ++la_id) {

		REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, la_id, INDEX_ZERO);

		/* get the port members in a link aggregation */
		if (la2phy.BF.members != 0)
			JL_FOR_EACH_CPORT(pdevice, cport) {
				if (la2phy.BF.members & (1 << cport))
					port_status[cport] = 1;
			}
	}

	/* set the ports which are not in link aggregation cportmask and weight, one to one mapping */
	JL_FOR_EACH_CPORT(pdevice, cport) {
		if (port_status[cport] == 0) {
			_link_aggregation_one_port_one2one_set(pdevice, cport);
		}
	}
	return ret;
}

jl_ret_t jl6107x_link_aggregation_enable_set(jl_device_t *pdevice, jl_enable_t enable)
{
	jl_ret_t ret = JL_ERR_OK;

	if (enable >= JL_ENABLE_END)
		return JL_ERR_PARAM;

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_CTRL, la_ctrl, INDEX_ZERO, INDEX_ZERO);

	la_ctrl.BF.enable = enable;

	REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_CTRL, la_ctrl, INDEX_ZERO, INDEX_ZERO);

	if (enable == DISABLED)
		_link_aggregation_clear_all(pdevice);
	else if (enable == ENABLED)
		_link_aggregation_one2one_set(pdevice);

	return ret;
}

jl_ret_t jl6107x_link_aggregation_enable_get(jl_device_t *pdevice, jl_enable_t *penable)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(penable);

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_CTRL, la_ctrl, INDEX_ZERO, INDEX_ZERO);

	*penable = la_ctrl.BF.enable;

	return ret;
}

jl_ret_t jl6107x_link_aggregation_portmask_set(jl_device_t *pdevice, jl_la_group_t la_id, jl_portmask_t la_portmask)
{
	jl_ret_t ret = JL_ERR_OK;

	jl_portmask_t la_cportmask = 0, old_cportmask;
	jl_la_group_t cla_id = 0;
	jl_port_t cport;

	JL_CHECK_LA_ID(pdevice, la_id);
	JL_CHECK_PORTMASK(pdevice, la_portmask);
	if (!(la_portmask & (0x1UL << la_id)))
		return JL_ERR_PARAM;

	jlxx_portmask_l2c(pdevice, la_portmask, &la_cportmask);
	cla_id = (jl_la_group_t)jlxx_port_l2c(pdevice, (jl_port_t)la_id);

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);
	old_cportmask = la2phy.BF.members;
	JL_FOR_EACH_CPORT(pdevice, cport) {
		/*delete port*/
		if ((old_cportmask & (1 << cport)) && !(la_cportmask & (1 << cport))) {
			_link_aggregation_one_port_one2one_set(pdevice, cport);
		}
		/* add port */
		else if (!(old_cportmask & (1 << cport)) && (la_cportmask & (1 << cport))) {
			la2phy.BF.members = 0;
			REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cport, INDEX_ZERO);
		}
	}
	_link_aggregation_portmask_set(pdevice, cla_id, la_cportmask);

	return ret;
}

jl_ret_t jl6107x_link_aggregation_portmask_get(jl_device_t *pdevice, jl_la_group_t la_id, jl_portmask_t *pla_portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_la_group_t cla_id = 0;

	JL_CHECK_LA_ID(pdevice, la_id);
	JL_CHECK_POINTER(pla_portmask);

	cla_id = (jl_la_group_t)jlxx_port_l2c(pdevice, (jl_port_t)la_id);

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);
	jlxx_portmask_c2l(pdevice, la2phy.BF.members, pla_portmask);

	return ret;
}

jl_ret_t jl6107x_link_aggregation_port_del(jl_device_t *pdevice, jl_la_group_t la_id, jl_port_t port_id)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_la_group_t cla_id = 0;
	jl_port_t cport_id;

	JL_CHECK_PORT(pdevice, port_id);
	JL_CHECK_LA_ID(pdevice, la_id);

	cport_id = jlxx_port_l2c(pdevice, port_id);
	cla_id = (jl_la_group_t)jlxx_port_l2c(pdevice, (jl_port_t)la_id);

	_link_aggregation_port_del(pdevice, cla_id, cport_id);

	return ret;
}

jl_ret_t jl6107x_link_aggregation_group_del(jl_device_t *pdevice, jl_la_group_t la_id)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_la_group_t cla_id = 0;

	JL_CHECK_LA_ID(pdevice, la_id);
	cla_id = (jl_la_group_t)jlxx_port_l2c(pdevice, (jl_port_t)la_id);

	_link_aggregation_group_clear(pdevice, cla_id);

	return ret;
}

jl_ret_t jl6107x_link_aggregation_hashfactor_set(jl_device_t *pdevice, jl_uint32 la_hashfactor_bitmask)
{
	jl_ret_t ret = JL_ERR_OK;

	if (la_hashfactor_bitmask >= 64)
		return JL_ERR_PARAM;

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_CTRL, la_ctrl, INDEX_ZERO, INDEX_ZERO);
	CLR_BITS(*(la_ctrl.val), SWCORE_LINK_AGGREGATION_CTRL_OFFSET_ENABLE_WIDTH,
		SWCORE_LINK_AGGREGATION_CTRL_OFFSET_RESERVED - SWCORE_LINK_AGGREGATION_CTRL_OFFSET_ENABLE_WIDTH);

	*(la_ctrl.val) |= BITS_OFFSET_L(la_hashfactor_bitmask, SWCORE_LINK_AGGREGATION_CTRL_OFFSET_ENABLE_WIDTH,
		SWCORE_LINK_AGGREGATION_CTRL_OFFSET_RESERVED - SWCORE_LINK_AGGREGATION_CTRL_OFFSET_ENABLE_WIDTH);

	REGISTER_WRITE(pdevice, SWCORE, LINK_AGGREGATION_CTRL, la_ctrl, INDEX_ZERO, INDEX_ZERO);

	return ret;
}

jl_ret_t jl6107x_link_aggregation_hashfactor_get(jl_device_t *pdevice, jl_uint32 *pla_hashfactor_bitmask)
{
	jl_ret_t ret = JL_ERR_OK;

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_CTRL, la_ctrl, INDEX_ZERO, INDEX_ZERO);

	*pla_hashfactor_bitmask = GET_BITS(*(la_ctrl.val), SWCORE_LINK_AGGREGATION_CTRL_OFFSET_ENABLE_WIDTH,
		SWCORE_LINK_AGGREGATION_CTRL_OFFSET_RESERVED - SWCORE_LINK_AGGREGATION_CTRL_OFFSET_ENABLE_WIDTH);

	return ret;
}

jl_ret_t jl6107x_link_aggregation_hash_map_set(jl_device_t *pdevice, jl_la_group_t la_id, jl_portmask_t la_portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_portmask_t la_cportmask = 0;
	jl_la_group_t cla_id;

	JL_CHECK_PORTMASK(pdevice, la_portmask);
	JL_CHECK_LA_ID(pdevice, la_id);

	cla_id = (jl_la_group_t)jlxx_port_l2c(pdevice, (jl_port_t)la_id);
	jlxx_portmask_l2c(pdevice, la_portmask, &la_cportmask);

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);
	if (la_cportmask != (la2phy.BF.members & la_cportmask))
		return JL_ERR_PARAM;

	_link_aggregation_hash_map_set(pdevice, la_cportmask);

	return ret;
}

jl_ret_t jl6107x_link_aggregation_hash_map_get(jl_device_t *pdevice, jl_la_hash2ports_t *pla_hash2ports)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 hash_index;
	jl_portmask_t portmask = 0;

	for (hash_index = 0; hash_index < 256; ++hash_index) {
		REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATE_WEIGHT, la_weight, hash_index, INDEX_ZERO);

		portmask = 0;

		jlxx_portmask_c2l(pdevice, la_weight.BF.ports, &portmask);
		pla_hash2ports->value[hash_index] = portmask;
	}

	return ret;
}

jl_ret_t jl6107x_link_aggregation_hash_map_clear(jl_device_t *pdevice, jl_la_group_t la_id, jl_portmask_t la_portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_portmask_t la_cportmask = 0;

	jl_la_group_t cla_id;

	JL_CHECK_PORTMASK(pdevice, la_portmask);
	JL_CHECK_LA_ID(pdevice, la_id);

	cla_id = (jl_la_group_t)jlxx_port_l2c(pdevice, (jl_port_t)la_id);
	jlxx_portmask_l2c(pdevice, la_portmask, &la_cportmask);

	REGISTER_READ(pdevice, SWCORE, LINK_AGGREGATION_TO_PHYSICAL_PORTS_MEMBERS, la2phy, cla_id, INDEX_ZERO);
	if (la_cportmask != (la2phy.BF.members & la_cportmask))
		return JL_ERR_PARAM;

	_link_aggregation_hash_map_clear(pdevice, la_cportmask);
	return ret;
}

