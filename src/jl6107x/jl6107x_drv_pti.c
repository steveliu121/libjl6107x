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

#include "jl6107x/jl6107x_drv_pti.h"
#include "jl6107x/jl6107x_drv_switch.h"
#include "jl6107x/jl6107x_chip.h"

static jl_portmask_t pti_group_port[PTI_GROUP_END] = {0};

jl_ret_t jl6107x_port_isolation_dst_set(jl_device_t *device, jl_port_t dstport, jl_portmask_t portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_portmask_t cportmask = 0;

	/* Check Port Valid */
	JL_CHECK_PORT(device, dstport);

	JL_CHECK_PORTMASK(device, portmask);

	ret = jlxx_portmask_l2c(device, portmask, &cportmask);
	if (ret != JL_ERR_OK)
		return ret;

	REGISTER_READ(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, dstport), INDEX_ZERO);
	ptfilter.BF.src_port_filter = cportmask;
	REGISTER_WRITE(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, dstport), INDEX_ZERO);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: ptfilter: 0x%x \n", ptfilter.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_isolation_dst_get(jl_device_t *device, jl_port_t dstport, jl_portmask_t *pportmask)
{
	jl_ret_t ret = JL_ERR_OK;

	/* Check Port Valid */
	JL_CHECK_PORT(device, dstport);

	JL_CHECK_POINTER(pportmask);

	*pportmask = 0;

	REGISTER_READ(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, dstport), INDEX_ZERO);

	ret = jlxx_portmask_c2l(device, (jl_portmask_t)ptfilter.BF.src_port_filter, pportmask);

	JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: ptfilter: 0x%x \n", \
					ptfilter.val[0]);

	return ret;
}

jl_ret_t jl6107x_port_isolation_src_set(jl_device_t *device, jl_port_t srcport, jl_portmask_t portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t port = 0;
	jl_uint32 cport = 0;

	/* Check Port Valid */
	JL_CHECK_PORT(device, srcport);

	JL_CHECK_PORTMASK(device, portmask);

	cport = jlxx_port_l2c(device, srcport);
	if (cport == UNDEF_PORT)
		return JL_ERR_PORT;

	JL_FOR_EACH_PORT(device, port) {
		REGISTER_READ(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, port), INDEX_ZERO);
		if (GET_BIT(portmask, port))
			SET_BIT(ptfilter.BF.src_port_filter, cport);
		else
			CLR_BIT(ptfilter.BF.src_port_filter, cport);
		REGISTER_WRITE(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, port), INDEX_ZERO);

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, ptfilter: 0x%x \n", \
						jlxx_port_l2c(device, port), ptfilter.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_port_isolation_src_get(jl_device_t *device, jl_port_t srcport, jl_portmask_t *pportmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t port = 0;
	jl_uint32 cport = 0;

	/* Check Port Valid */
	JL_CHECK_PORT(device, srcport);

	JL_CHECK_POINTER(pportmask);

	cport = jlxx_port_l2c(device, srcport);
	if (cport == UNDEF_PORT)
		return JL_ERR_PORT;

	*pportmask = 0;

	JL_FOR_EACH_PORT(device, port) {
		REGISTER_READ(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, port), INDEX_ZERO);
		if (GET_BIT(ptfilter.BF.src_port_filter, cport))
			SET_BIT(*pportmask, port);

		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, ptfilter: 0x%x \n", \
						jlxx_port_l2c(device, port), ptfilter.val[0]);
	}

	return ret;
}

jl_ret_t jl6107x_port_isolation_group_set(jl_device_t *device, jl_pti_group_t group_id, jl_portmask_t portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t port = 0;
	jl_portmask_t tmpportmask = 0;
	jl_portmask_t cportmask = 0;

	JL_PTI_CHECK_GRP_ID(group_id);

	JL_CHECK_PORTMASK(device, portmask);

	if (pti_group_port[group_id] == portmask) {
		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "SWCORE: set the portmask from the group id is the same, no operation is required \n");
		return ret;
	}

	ret = jl6107x_port_isolation_group_clear(device, group_id);
	if (ret != JL_ERR_OK)
		return ret;

	pti_group_port[group_id] = portmask;
	JL_FOR_EACH_PORT(device, port) {
		if (GET_BIT(portmask, port)) {
			REGISTER_READ(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, port), INDEX_ZERO);
			tmpportmask = portmask;
			CLR_BIT(tmpportmask, port);
			ret = jlxx_portmask_l2c(device, tmpportmask, &cportmask);
			if (ret != JL_ERR_OK)
				return ret;
			BITMASK_SET(ptfilter.BF.src_port_filter, cportmask);
			REGISTER_WRITE(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, port), INDEX_ZERO);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, ptfilter: 0x%x \n", \
							jlxx_port_l2c(device, port), ptfilter.val[0]);
		}
	}

	return ret;
}

jl_ret_t jl6107x_port_isolation_group_get(jl_device_t *device, jl_pti_group_t group_id, jl_portmask_t *pportmask)
{
	jl_ret_t ret = JL_ERR_OK;

	if (device == NULL)
		return JL_ERR_PARAM;

	JL_PTI_CHECK_GRP_ID(group_id);

	JL_CHECK_POINTER(pportmask);

	*pportmask = pti_group_port[group_id];

	return ret;
}

jl_ret_t jl6107x_port_isolation_group_clear(jl_device_t *device, jl_pti_group_t group_id)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_port_t port = 0;
	jl_portmask_t tmpportmask = 0;
	jl_portmask_t cportmask = 0;

	JL_PTI_CHECK_GRP_ID(group_id);

	if (pti_group_port[group_id] == 0) {
		JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_INFO, "SWCORE: the portmask from the group id is zero, no operation is required \n");
		return ret;
	}

	JL_FOR_EACH_PORT(device, port) {
		if (GET_BIT(pti_group_port[group_id], port)) {
			REGISTER_READ(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, port), INDEX_ZERO);
			tmpportmask = pti_group_port[group_id];
			CLR_BIT(tmpportmask, port);
			ret = jlxx_portmask_l2c(device, tmpportmask, &cportmask);
			if (ret != JL_ERR_OK)
				return ret;
			BITMASK_CLR(ptfilter.BF.src_port_filter, cportmask);
			REGISTER_WRITE(device, SWCORE, INGRESS_EGRESS_PORT_PACKET_TYPE_FILTER, ptfilter, jlxx_port_l2c(device, port), INDEX_ZERO);

			JL_DBG_MSG(JL_FLAG_SWCORE, _DBG_ON, "SWCORE: cport: %u, ptfilter: 0x%x \n", \
							jlxx_port_l2c(device, port), ptfilter.val[0]);
		}
	}
	pti_group_port[group_id] = 0;

	return ret;
}
