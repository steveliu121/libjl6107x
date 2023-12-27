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

#ifndef __JL_DRV_COMMON_H__
#define __JL_DRV_COMMON_H__

#include "jl_error.h"
#include "jl_device.h"
#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JL_SWITCH_PORT_NUM		JL_PORT_MAX

/* check logical port */
#define JL_CHECK_PORT(pdevice, x) \
do { \
	if (jlxx_lport_check(pdevice, x) != JL_ERR_OK) \
		return JL_ERR_PORT; \
} while (0)

/* check logical utp port */
#define JL_CHECK_UTP_PORT(pdevice, x) \
do { \
	if (jlxx_utpport_check(pdevice, x) != JL_ERR_OK) \
		return JL_ERR_PORT; \
} while (0)

/* check logical ext port */
#define JL_CHECK_EXT_PORT(pdevice, x) \
do { \
	if (jlxx_extport_check(pdevice, x) != JL_ERR_OK) \
		return JL_ERR_PORT; \
} while (0)

/* check logical utp_ext port */
#define JL_CHECK_UTP_EXT_PORT(pdevice, x) \
do { \
	if ((jlxx_utpport_check(pdevice, x) != JL_ERR_OK) \
		&& (jlxx_extport_check(pdevice, x) != JL_ERR_OK)) \
		return JL_ERR_PORT;	    \
} while (0)

/* check logical cpu port */
#define JL_CHECK_CPU_PORT(pdevice, x) \
do { \
	if (jlxx_cpuport_check(pdevice, x) != JL_ERR_OK) \
		return JL_ERR_PORT; \
} while (0)

/* check logical portmask */
#define JL_CHECK_PORTMASK(pdevice, x) \
do { \
	struct jl_switch_dev_s *pswitch_dev = NULL; \
	JL_ASSERT(pdevice); \
	JL_ASSERT((pdevice)->switch_dev); \
	pswitch_dev = (struct jl_switch_dev_s *)((pdevice)->switch_dev); \
	if ((x) != (pswitch_dev->lportmask & (x))) \
		return JL_ERR_PORT; \
} while (0)

/* check logical utp portmask */
#define JL_CHECK_UTP_PORTMASK(pdevice, x) \
do { \
	struct jl_switch_dev_s *pswitch_dev = NULL; \
	JL_ASSERT(pdevice); \
	JL_ASSERT((pdevice)->switch_dev); \
	pswitch_dev = (struct jl_switch_dev_s *)((pdevice)->switch_dev); \
	if ((x) != (pswitch_dev->utp_portmask & (x))) \
		return JL_ERR_PORT; \
} while (0)

/* check logical ext portmask */
#define JL_CHECK_EXT_PORTMASK(pdevice, x) \
do { \
	struct jl_switch_dev_s *pswitch_dev = NULL; \
	JL_ASSERT(pdevice); \
	JL_ASSERT((pdevice)->switch_dev); \
	pswitch_dev = (struct jl_switch_dev_s *)((pdevice)->switch_dev); \
	if ((x) != (pswitch_dev->ext_portmask & (x))) \
		return JL_ERR_PORT; \
} while (0)

/* check logical cpu portmask */
#define JL_CHECK_CPU_PORTMASK(pdevice, x) \
do { \
	struct jl_switch_dev_s *pswitch_dev = NULL; \
	JL_ASSERT(pdevice); \
	JL_ASSERT((pdevice)->switch_dev); \
	pswitch_dev = (struct jl_switch_dev_s *)((pdevice)->switch_dev); \
	if ((x) != (pswitch_dev->cpu_portmask & (x))) \
		return JL_ERR_PORT; \
} while (0)

/* check logical la id */
#define JL_CHECK_LA_ID(pdevice, x) \
	do { \
	if (jlxx_lport_check(pdevice, (jl_port_t)x) != JL_ERR_OK) \
		return JL_ERR_PARAM; \
	} while (0)


#define JL_PORT_L2C_MACRO(pdevice, frontp, macp) \
do { \
	struct jl_switch_dev_s *pswitch_dev = NULL; \
	JL_ASSERT(pdevice); \
	JL_ASSERT((pdevice)->switch_dev); \
	pswitch_dev = (struct jl_switch_dev_s *)((pdevice)->switch_dev); \
	if ((frontp) > pswitch_dev->max_lport) \
		return JL_ERR_PORT; \
	(macp) = pswitch_dev->l2c_port[frontp]; \
} while (0)

#define JL_PORT_C2L_MACRO(pdevice, macp, frontp) \
do { \
	struct jl_switch_dev_s *pswitch_dev = NULL; \
	JL_ASSERT(pdevice); \
	JL_ASSERT((pdevice)->switch_dev); \
	pswitch_dev = (struct jl_switch_dev_s *)((pdevice)->switch_dev); \
	if ((macp) > pswitch_dev->max_cport) \
		return JL_ERR_PORT; \
	(frontp) = pswitch_dev->c2l_port[macp]; \
} while (0)

#define JL_PORT_LMAP_TO_CMAP(pdevice, frntpmap, macpmap) \
do { \
	jl_uint8 fp, mp; \
	macpmap = 0; \
	JL_FOR_EACH_PORT((pdevice), fp) \
	{ \
		if ((frntpmap) & (BIT(fp))) { \
			JL_PORT_L2C_MACRO((pdevice), fp, mp); \
			macpmap |= BIT(mp); \
		} \
	} \
} while (0)

#define JL_PORT_CMAP_TO_LMAP(pdevice, macpmap, frntpmap) \
do {\
	jl_uint8 fp, mp; \
	frntpmap = 0; \
	JL_FOR_EACH_CPORT((pdevice), mp) \
	{ \
		if (macpmap & (BIT(mp))) { \
			JL_PORT_C2L_MACRO((pdevice), mp, fp); \
			frntpmap |= (BIT(fp)); \
		} \
	} \
} while (0)

#define JL_FOR_EACH_PORT(pdevice, x) \
	for (x = 0; ((pdevice) != NULL) && ((pdevice)->switch_dev != NULL)  && \
		(x <= ((struct jl_switch_dev_s *)((pdevice)->switch_dev))->max_lport); x++) \
		if (jlxx_lport_check(pdevice, x) == JL_ERR_OK)

#define JL_FOR_EACH_CPORT(pdevice, x) \
	for (x = 0; ((pdevice) != NULL) && ((pdevice)->switch_dev != NULL)  && \
		(x <= ((struct jl_switch_dev_s *)((pdevice)->switch_dev))->max_cport); x++) \
		if (jlxx_cport_check(pdevice, x) == JL_ERR_OK)

#define JL_FOR_EACH_UTP_PORT(pdevice, x) \
	for (x = 0; ((pdevice) != NULL) && ((pdevice)->switch_dev != NULL)  && \
		(x <= ((struct jl_switch_dev_s *)((pdevice)->switch_dev))->max_lport); x++) \
		if (jlxx_utpport_check(pdevice, x) == JL_ERR_OK)

#define JL_FOR_EACH_EXT_PORT(pdevice, x) \
	for (x = 0; ((pdevice) != NULL) && ((pdevice)->switch_dev != NULL)  && \
		(x <= ((struct jl_switch_dev_s *)((pdevice)->switch_dev))->max_lport); x++) \
		if (jlxx_extport_check(pdevice, x) == JL_ERR_OK)


struct jl_switch_dev_s {
	jl_uint32 pkg;
	jl_uint32 core_ver;
	/* logical port to switch core port */
	jl_uint32 l2c_port[JL_SWITCH_PORT_NUM];
	/* logical port to phy/mac port */
	jl_uint32 l2p_port[JL_SWITCH_PORT_NUM];
	/* switch core port to logical port */
	jl_uint32 c2l_port[JL_SWITCH_PORT_NUM];
	/* logical port type */
	jl_port_type_t lport_type[JL_SWITCH_PORT_NUM];
	/* switch core port type */
	jl_port_type_t cport_type[JL_SWITCH_PORT_NUM];
	/* max logical port */
	jl_uint32 max_lport;
	/* max switch core port */
	jl_uint32 max_cport;
	/* logical portmask */
	jl_portmask_t lportmask;
	/* logical utp portmask */
	jl_portmask_t utp_portmask;
	/* logical ext portmask */
	jl_portmask_t ext_portmask;
	/* logical cpu portmask */
	jl_portmask_t cpu_portmask;
	/* switch core portmask */
	jl_portmask_t cportmask;
};


jl_ret_t jlxx_lport_check(jl_device_t *pdevice, jl_port_t lport);
jl_ret_t jlxx_cport_check(jl_device_t *pdevice, jl_port_t cport);
jl_ret_t jlxx_utpport_check(jl_device_t *pdevice, jl_port_t lport);
jl_ret_t jlxx_extport_check(jl_device_t *pdevice, jl_port_t lport);
jl_ret_t jlxx_cpuport_check(jl_device_t *pdevice, jl_port_t lport);
jl_uint32 jlxx_port_l2c(jl_device_t *pdevice, jl_port_t lport);
jl_uint32 jlxx_port_l2p(jl_device_t *pdevice, jl_port_t lport);
jl_port_t jlxx_port_c2l(jl_device_t *pdevice, jl_port_t cport);
jl_api_ret_t jlxx_lportmask_get(jl_device_t *pdevice, jl_portmask_t *lportmask);
jl_api_ret_t jlxx_cportmask_get(jl_device_t *pdevice, jl_portmask_t *cportmask);
jl_ret_t jlxx_portmask_l2c(jl_device_t *pdevice,
		jl_portmask_t lportmask, jl_portmask_t *cportmask);
jl_ret_t jlxx_portmask_c2l(jl_device_t *pdevice,
		jl_portmask_t cportmask, jl_portmask_t *lportmask);

#ifdef __cplusplus
}
#endif

#endif				/* __JL_DRV_COMMON_H__ */
