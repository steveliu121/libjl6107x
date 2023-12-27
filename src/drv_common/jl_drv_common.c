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

#include "jl_base.h"
#include "jl_device.h"
#include "drv_common/jl_drv_common.h"

jl_ret_t jlxx_lport_check(jl_device_t *pdevice, jl_port_t lport)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	JL_ASSERT(pdevice);
	JL_ASSERT(pdevice->switch_dev);

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	if (lport > pswitch_dev->max_lport)
		return JL_ERR_PORT;

	if (pswitch_dev->lport_type[lport] == PORT_TYPE_UNDEF)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jlxx_cport_check(jl_device_t *pdevice, jl_port_t cport)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	JL_ASSERT(pdevice);
	JL_ASSERT(pdevice->switch_dev);

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	if (cport > pswitch_dev->max_cport)
		return JL_ERR_PORT;

	if (pswitch_dev->cport_type[cport] == PORT_TYPE_UNDEF)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jlxx_utpport_check(jl_device_t *pdevice, jl_port_t lport)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	JL_ASSERT(pdevice);
	JL_ASSERT(pdevice->switch_dev);

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	if (lport > pswitch_dev->max_lport)
		return JL_ERR_PORT;

	if (pswitch_dev->lport_type[lport] != PORT_TYPE_UTP)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jlxx_extport_check(jl_device_t *pdevice, jl_port_t lport)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	JL_ASSERT(pdevice);
	JL_ASSERT(pdevice->switch_dev);

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	if (lport > pswitch_dev->max_lport)
		return JL_ERR_PORT;

	if (pswitch_dev->lport_type[lport] != PORT_TYPE_EXT)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_ret_t jlxx_cpuport_check(jl_device_t *pdevice, jl_port_t lport)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	JL_ASSERT(pdevice);
	JL_ASSERT(pdevice->switch_dev);

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	if (lport > pswitch_dev->max_lport)
		return JL_ERR_PORT;

	if (pswitch_dev->lport_type[lport] != PORT_TYPE_CPU)
		return JL_ERR_PORT;

	return JL_ERR_OK;
}

jl_uint32 jlxx_port_l2c(jl_device_t *pdevice, jl_port_t lport)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	if ((pdevice == NULL) || (pdevice->switch_dev == NULL))
		return UNDEF_PORT;

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	if (lport > pswitch_dev->max_lport)
		return UNDEF_PORT;

	return pswitch_dev->l2c_port[lport];
}

jl_uint32 jlxx_port_l2p(jl_device_t *pdevice, jl_port_t lport)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	if ((pdevice == NULL) || (pdevice->switch_dev == NULL))
		return UNDEF_PORT;

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	if (lport > pswitch_dev->max_lport)
		return UNDEF_PORT;

	return pswitch_dev->l2p_port[lport];
}

jl_port_t jlxx_port_c2l(jl_device_t *pdevice, jl_port_t cport)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	if ((pdevice == NULL) || (pdevice->switch_dev == NULL))
		return UNDEF_PORT;

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	if (cport > pswitch_dev->max_cport)
		return UNDEF_PORT;

	return pswitch_dev->c2l_port[cport];
}

jl_api_ret_t jlxx_lportmask_get(jl_device_t *pdevice, jl_portmask_t *lportmask)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	if ((pdevice == NULL) || (pdevice->switch_dev == NULL))
		return JL_ERR_PORT;

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	*lportmask = pswitch_dev->lportmask;

	return JL_ERR_OK;
}


jl_api_ret_t jlxx_cportmask_get(jl_device_t *pdevice, jl_portmask_t *cportmask)
{
	struct jl_switch_dev_s *pswitch_dev = NULL;

	if ((pdevice == NULL) || (pdevice->switch_dev == NULL))
		return JL_ERR_PORT;

	pswitch_dev = (struct jl_switch_dev_s *)(pdevice->switch_dev);

	*cportmask = pswitch_dev->cportmask;

	return JL_ERR_OK;
}

jl_ret_t jlxx_portmask_l2c(jl_device_t *pdevice, jl_portmask_t lportmask, jl_portmask_t *cportmask)
{
	jl_port_t lport;
	jl_port_t cport;

	JL_ASSERT(pdevice);

	*cportmask = 0;

	JL_FOR_EACH_PORT(pdevice, lport) {
		if (lportmask & (0x1UL << lport)) {
			cport = jlxx_port_l2c(pdevice, lport);
			if (cport == UNDEF_PORT)
				return JL_ERR_PORT;
			*cportmask |= 0x1UL << cport;
		}
	}

	return JL_ERR_OK;
}

jl_ret_t jlxx_portmask_c2l(jl_device_t *pdevice, jl_portmask_t cportmask, jl_portmask_t *lportmask)
{
	jl_port_t lport;
	jl_port_t cport;

	JL_ASSERT(pdevice);

	*lportmask = 0;

	JL_FOR_EACH_CPORT(pdevice, cport) {
		if (cportmask & (0x1UL << cport)) {
			lport = jlxx_port_c2l(pdevice, cport);
			if (lport == UNDEF_PORT)
				return JL_ERR_PORT;
			*lportmask |= 0x1UL << lport;
		}
	}

	return JL_ERR_OK;
}
