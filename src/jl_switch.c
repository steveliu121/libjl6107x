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
#include "jl_base.h"
#include "jl_device.h"
#include "jl_switch.h"
#include "jl_ops.h"
#include "driver/jl_reg_io.h"

extern struct jl_module_s g_module;

static jl_ret_t __driver_probe(jl_device_t *device)
{
	jl_ret_t ret;

	JL_ASSERT(device != NULL);
	JL_ASSERT(device->driver != NULL);
	JL_ASSERT(device->driver->probe != NULL);

	ret = device->driver->probe(device);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"[%s]-[%s]: Driver probe failed!!!\n",
				device->driver->name, device->name);

	return ret;

}

static jl_ret_t __driver_remove(jl_device_t *device)
{
	jl_ret_t ret;

	JL_ASSERT(device != NULL);
	JL_ASSERT(device->driver != NULL);
	JL_ASSERT(device->driver->remove != NULL);

	ret = device->driver->remove(device);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"[%s]-[%s]: Driver remove failed!!!\n",
				device->driver->name, device->name);

	return ret;
}

static jl_ret_t __device_create(jl_dev_t *device)
{
	jl_ret_t ret;
	jl_uint32 i;
	jl_device_t *_device;

	JL_ASSERT(device != NULL);

	if (device->id >= JL_MAX_CHIP_NUM) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Error: chip id out of range!!!\n");
		return JL_ERR_BADID;
	}

	for (i = 0; i < g_module.driver_num; i++) {
		if ((g_module.drivers[i]->compat_id & JL_CHIP_SERIES_MASK)
				!= (device->compat_id & JL_CHIP_SERIES_MASK))
			continue;

		if (g_module.devices[device->id] != NULL) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
					"Error: device[%d] already created!!!\n",
					device->id);
			return JL_ERR_EXISTS;
		}

		device->io.chip = g_module.drivers[i]->compat_id;

		g_module.devices[device->id] = &g_module.device_pool[device->id];
		_device = g_module.devices[device->id];
		memcpy(_device, device, sizeof(jl_dev_t));

		_device->driver = g_module.drivers[i];

		ret = __driver_probe(_device);
		if (!ret)
			goto exit;
		else {
			memset(_device, 0, sizeof(jl_device_t));
			g_module.devices[device->id] = NULL;

			return ret;
		}
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
			"Error: no compatible driver for device[%s]!!!\n",
			device->name);

	return JL_ERR_FAIL;

exit:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
			"Device[%d] create done!!!\n", device->id);

	return JL_ERR_OK;
}

static jl_ret_t __device_delete(jl_uint32 chip_id)
{
	jl_ret_t ret;

	if (g_module.devices[chip_id] == NULL) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Error: device[%d] not exist!!!\n",
				chip_id);

		return JL_ERR_EXISTS;
	}

	ret = __driver_remove(g_module.devices[chip_id]);
	if (ret)
		return ret;

	memset(g_module.devices[chip_id], 0, sizeof(jl_device_t));
	g_module.devices[chip_id] = NULL;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
			"Device[%d] delete done!!!\n", chip_id);

	return JL_ERR_OK;
}

static jl_ret_t __device_open(jl_uint32 chip_id)
{
	jl_device_t *device;

	if (g_module.devices[chip_id] == NULL) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Error: device[%d] not exist!!!\n",
				chip_id);

		return JL_ERR_EXISTS;
	}

	device = g_module.devices[chip_id];

	return device->io_desc.ops.init(&device->io_desc);
}

static jl_ret_t __device_close(jl_uint32 chip_id)
{
	jl_device_t *device;

	if (g_module.devices[chip_id] == NULL) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Error: device[%d] not exist!!!\n",
				chip_id);

		return JL_ERR_EXISTS;
	}

	device = g_module.devices[chip_id];

	return device->io_desc.ops.deinit(&device->io_desc);
}

static jl_ret_t __device_reorder(jl_module_t *module)
{
	jl_device_t **devices = NULL;
	jl_uint32 num = 0;
	jl_uint32 max_id = 0;
	jl_device_t *_devices[JL_MAX_CHIP_NUM] = {NULL};
	jl_uint32 id = JL_MAX_CHIP_NUM;
	jl_uint32 not_inorder = 0;
	jl_uint32 i;

	JL_ASSERT(module != NULL);

	devices = module->devices;
	/* num == JL_MAX_CHIP_NUM by default, correct it in this function */
	num = module->device_num;

	for (i = 0; i < num; i++) {
		if (devices[i] == NULL)
			continue;

		id = devices[i]->id;
		if (id >= JL_MAX_CHIP_NUM) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
					"Error: chip id out of range!!!\n");
			return JL_ERR_BADID;
		}
		if (_devices[id] != NULL) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
					"Error: duplicated chip id!!!\n");
			return JL_ERR_BADID;
		}

		_devices[id] = devices[i];

		if (id != i)
			not_inorder = 1;
		max_id = JL_MAX(id, max_id);
	}

	/* maybe there's null device before device with the max id */
	module->device_num = (id == JL_MAX_CHIP_NUM) ? 0 : (max_id + 1);

	if (!not_inorder)
		return JL_ERR_OK;

	for (i = 0; i < num; i++)
		devices[i] = _devices[i];

	return JL_ERR_OK;
}

static jl_ret_t __driver_match_device(jl_module_t *module)
{
	jl_ret_t ret;
	jl_uint32 driver_num;
	jl_uint32 device_num;
	jl_uint32 i, j;

	JL_ASSERT(module != NULL);

	ret = __device_reorder(module);
	if (ret)
		return ret;

	driver_num = module->driver_num;
	device_num = module->device_num;

	for (i = 0; i < driver_num; i++) {
		for (j = 0; j < device_num; j++) {
			if (module->devices[j] == NULL)
				continue;

			if ((module->devices[j]->compat_id & JL_CHIP_SERIES_MASK)
					== (module->drivers[i]->compat_id & JL_CHIP_SERIES_MASK)) {
				module->devices[j]->io_desc.chip = module->drivers[i]->compat_id;
				module->devices[j]->driver = module->drivers[i];

				ret = __driver_probe(module->devices[j]);
				if (ret)
					return ret;
			}
		}
	}

	return JL_ERR_OK;
}

static jl_ret_t __module_init(jl_module_t *module)
{
	jl_ret_t ret;

	ret = __driver_match_device(module);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR,
				"Driver match failed!!!\n");
		return ret;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
			"Module initialize done!!!\n");

	return JL_ERR_OK;
}

static jl_ret_t __module_exit(jl_module_t *module)
{
	jl_uint32 i;
	jl_ret_t ret = JL_ERR_OK;

	JL_ASSERT(module != NULL);

	for (i = 0; i < JL_MAX_CHIP_NUM; i++) {
		if (module->devices[i] == NULL)
			continue;

		ret |= __driver_remove(module->devices[i]);

		memset(module->devices[i], 0, sizeof(jl_device_t));
		module->devices[i] = NULL;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
				"Device[%d] delete done!!!\n", i);
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
			"JLSemi switch module exit!!!\n");

	return ((ret == JL_ERR_OK) ? JL_ERR_OK : JL_ERR_FAIL);
}

jl_api_ret_t jl_switch_init(void)
{
	jl_api_ret_t ret = JL_ERR_OK;

	if (JL_STAT_IS_READY(g_module.stat))
		return JL_ERR_OK;

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_dump_enable();
	jl_dump_add_comment("======jl switch init======");
#endif

	ret = __module_init(&g_module);
	if (ret)
		return ret;

	JL_STAT_INIT(g_module.stat);

	return JL_ERR_OK;
}

jl_api_ret_t jl_switch_deinit(void)
{
	jl_api_ret_t ret = JL_ERR_OK;

	if (JL_STAT_IS_UNINIT(g_module.stat))
		return JL_ERR_OK;

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_dump_add_comment("======jl switch deinit======");
#endif

	ret = __module_exit(&g_module);
	if (ret)
		return ret;

	JL_STAT_DEINIT(g_module.stat);

	return ret;
}

jl_api_ret_t jl_switch_info(jl_uint32 chip_id, jl_switch_info_t *pinfo)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, switch_info, chip_id, pinfo);

	return ret;
}

jl_api_ret_t jl_switch_init_state_set(jl_stat_t state)
{
	if (state >= STAT_END)
		return JL_ERR_PARAM;

	g_module.stat = state;

	return JL_ERR_OK;
}

jl_api_ret_t jl_switch_init_state_get(jl_stat_t *pstate)
{
	JL_CHECK_POINTER(pstate);

	*pstate = g_module.stat;

	return JL_ERR_OK;
}

jl_api_ret_t jl_switch_device_create(jl_dev_t *device)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_CHECK_STAT(g_module.stat);

	ret = __device_create(device);

	return ret;
}

jl_api_ret_t jl_switch_device_delete(jl_uint32 chip_id)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_CHECK_STAT(g_module.stat);

	ret = __device_delete(chip_id);

	return ret;
}

jl_api_ret_t jl_switch_device_open(jl_uint32 chip_id)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_CHECK_STAT(g_module.stat);

	ret = __device_open(chip_id);

	return ret;
}

jl_api_ret_t jl_switch_device_close(jl_uint32 chip_id)
{
	jl_api_ret_t ret = JL_ERR_OK;

	JL_CHECK_STAT(g_module.stat);

	ret = __device_close(chip_id);

	return ret;
}

jl_api_ret_t jl_lport_check(jl_uint32 chip_id, jl_port_t lport)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lport_check, chip_id, lport);

	return ret;
}

jl_api_ret_t jl_utpport_check(jl_uint32 chip_id, jl_port_t lport)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, utpport_check, chip_id, lport);

	return ret;
}

jl_api_ret_t jl_extport_check(jl_uint32 chip_id, jl_port_t lport)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, extport_check, chip_id, lport);

	return ret;
}

jl_api_ret_t jl_cpuport_check(jl_uint32 chip_id, jl_port_t lport)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, cpuport_check, chip_id, lport);

	return ret;
}

jl_uint32 jl_port_l2c_get(jl_uint32 chip_id, jl_port_t lport)
{
	jl_uint32 ret;

	JL_DRV_PORT_SAFE_CALL(ret, port_l2c_get, chip_id, lport);

	return ret;
}

jl_port_t jl_port_c2l_get(jl_uint32 chip_id, jl_port_t cport)
{
	jl_port_t ret;

	JL_DRV_PORT_SAFE_CALL(ret, port_c2l_get, chip_id, cport);

	return ret;
}

jl_api_ret_t jl_lportmask_get(jl_uint32 chip_id, jl_portmask_t *lportmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, lportmask_get, chip_id, lportmask);

	return ret;
}

jl_api_ret_t jl_cportmask_get(jl_uint32 chip_id, jl_portmask_t *cportmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, cportmask_get, chip_id, cportmask);

	return ret;
}

jl_api_ret_t jl_portmask_l2c_get(jl_uint32 chip_id, jl_portmask_t lportmask, jl_portmask_t *cportmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, portmask_l2c_get, chip_id, lportmask, cportmask);

	return ret;
}

jl_api_ret_t jl_portmask_c2l_get(jl_uint32 chip_id, jl_portmask_t cportmask, jl_portmask_t *lportmask)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, portmask_c2l_get, chip_id, cportmask, lportmask);

	return ret;
}

jl_api_ret_t jl_show_switchport_info(jl_uint32 chip_id)
{
	jl_api_ret_t ret;

	JL_DRV_SAFE_CALL(ret, show_switchport_info, chip_id);

	return ret;
}

