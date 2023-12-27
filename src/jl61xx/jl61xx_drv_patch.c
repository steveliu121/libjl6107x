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

#include "jl61xx/jl61xx_patch.h"
#include "jl61xx/jl61xx_drv_switch.h"
#include <driver/hal_jl61xx_smi.h>
#include <driver/hal_smi.h>
#include <driver/hal_jl61xx_spi.h>
#include <driver/hal_spi.h>

static jl_uint32 g_mdio_devid = MDIO_DEVID_0;
static jl_bool __is_mid29_load_patch(jl_device_t *pdevice)
{
	jl_bool ret = FALSE;
#if defined(CONFIG_JL_IO_SMI)
	if ((pdevice->io_desc.type == JL_IO_SMI) && (g_mdio_devid == MDIO_DEVID_29))
		ret = TRUE;
	return ret;
#else
	(void) pdevice;
	return ret;
#endif
}

static void __exchange_mid(jl_device_t *pdevice, jl_mdio_phy_address_t new_mid)
{
#if defined(CONFIG_JL_IO_SMI)
	if (pdevice->io_desc.type == JL_IO_SMI) {
		jl_uint32 smi_busid = 0;
		jl_uint32 old_mid = 0;

		smi_busid = pdevice->io_desc.smi.mdio.bus_id & 0xffffff;
		old_mid = pdevice->io_desc.smi.mdio.bus_id>>24;
		if (old_mid == MDIO_DEVID_29) {
			/* skip ida status check(skip a read operation) */
			pdevice->io_desc.reserved = 1;
		} else {
			/* do ida status check */
			pdevice->io_desc.reserved = 0;
		}
		pdevice->io_desc.smi.mdio.bus_id = (jl_uint32)((new_mid<<24)+smi_busid);
	}
#else
	(void) pdevice;
	(void) new_mid;
#endif
}

static jl_ret_t __patch_select(jl_device_t *pdevice, jl_uint8 **patch, jl_uint32 *patch_size)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 i;
	jl_uint32 eco_ver;
	struct jl_switch_dev_s *switch_dev =
		(struct jl_switch_dev_s *)pdevice->switch_dev;

	/* TOP FW_RESERVED5 Register setting */
	REGISTER_READ(pdevice, TOP, FW_RESERVED5, version, INDEX_ZERO, INDEX_ZERO);

	eco_ver = GET_BITS(version.BF.fw_reserved5, 20, 23);

	for (i = 0; g_61xx_eco_patch_tbl[i].pkg_ver != 0xff; i++) {
		if ((switch_dev->pkg == g_61xx_eco_patch_tbl[i].pkg_ver)
			&& (eco_ver == g_61xx_eco_patch_tbl[i].eco_ver)) {
			*patch = g_61xx_eco_patch_tbl[i].peco_patch;
			*patch_size = g_61xx_eco_patch_tbl[i].eco_patch_size;

			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "patch size[%d]bytes\n",
					g_61xx_eco_patch_tbl[i].eco_patch_size);

			return JL_ERR_OK;
		}
	}

	return JL_ERR_UNAVAIL;
}

static void __exchange_io(jl_device_t *pdevice, jl_io_type_t io_type)
{
#if defined(CONFIG_JL_IO_SPI) && defined(CONFIG_JL_IO_SMI)
	if (io_type == JL_IO_SMI_SPI) {
		/* change io from spi ---> smi_over_spi */
		pdevice->io_desc.ops.read = jl61xx_smi_read;
		pdevice->io_desc.ops.write = jl61xx_smi_write;
		pdevice->io_desc.smi.ops.read = jl_smi_spi_read;
		pdevice->io_desc.smi.ops.write = jl_smi_spi_write;

		/* skip ida status check(skip a read operation) */
		pdevice->io_desc.reserved = 1;
	} else {
		/* do ida status check */
		pdevice->io_desc.reserved = 0;

		/* change io from smi_over_spi ---> spi */
		pdevice->io_desc.ops.read = jl61xx_spi_read;
		pdevice->io_desc.ops.write = jl61xx_spi_write;
		pdevice->io_desc.spi.ops.read = jl_spi_read;
		pdevice->io_desc.spi.ops.write = jl_spi_write;
	}
#else
	(void) pdevice;
	(void) io_type;
#endif
}

static jl_ret_t __ccs_reset_for_spi(jl_device_t *pdevice)
{
	TOP_FW_RESERVED14_t rsv14;
	TOP_FW_RESERVED19_t csr;
	TOP_FW_RESERVED20_t data0;
	CRG_CCS_RST_t ccs;
	jl_ret_t ret = JL_ERR_OK;

	/* Force sram patch switch to SPI */
	ret = jl_reg_burst_read(&pdevice->io_desc, TOP_FW_RESERVED14, rsv14.val, 1);
	if (ret)
		goto err;
	SET_BIT(rsv14.BF.fw_reserved14, 31);
	ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED14, rsv14.val, 1);
	if (ret)
		goto err;

	/*clear control&status register reserved19, fill data0 register reserved20 with '0xed0c'*/
	csr.val[0] = 0;
	ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED19, csr.val, 1);
	if (ret)
		goto err;

	data0.val[0] = 0xed0c;
	ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED20, data0.val, 1);
	 if (ret)
		 goto err;

	 /* skip ida status check(skip a read operation) */
	pdevice->io_desc.reserved = 1;
	 /* reset CCS */
	ccs.val[0] = 1;
	ret = jl_reg_burst_write(&pdevice->io_desc, CRG_CCS_RST, ccs.val, 1);
	if (ret)
		goto err;

	port_udelay(10);

	__exchange_io(pdevice, JL_IO_SMI_SPI);

	return ret;
err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "ccs reset reg read/write fail[%d]!!!\n", ret);

	return ret;
}

static jl_ret_t __ccs_reset(jl_device_t *pdevice)
{
	jl_uint16 try = 1000;
	TOP_FW_RESERVED19_t csr;
	TOP_FW_RESERVED20_t data0;
	CRG_CCS_RST_t ccs;
	jl_ret_t ret = JL_ERR_OK;

	/*clear control&status register reserved19, fill data0 register reserved20 with '0xed0c'*/
	csr.val[0] = 0;
	ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED19, csr.val, 1);
	if (ret)
		goto err;
	data0.val[0] = 0xed0c;
	ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED20, data0.val, 1);
	if (ret)
		goto err;

	if (__is_mid29_load_patch(pdevice) == TRUE)
		pdevice->io_desc.reserved = 1;

	 /* reset CCS */
	ccs.val[0] = 1;
	ret = jl_reg_burst_write(&pdevice->io_desc, CRG_CCS_RST, ccs.val, 1);
	if (ret)
		goto err;

	port_udelay(100);
	if (__is_mid29_load_patch(pdevice) == TRUE) {
		/*when mid29 load patch, skip status check,change mid from 29 to 0 */
		__exchange_mid(pdevice, MDIO_DEVID_0);
		return ret;
	}

	while (--try) {
		ret = jl_reg_burst_read(&pdevice->io_desc, TOP_FW_RESERVED20, data0.val, 1);
		if (ret)
			goto err;
		if (data0.BF.fw_reserved20 == 0xc0de)
			break;
	}

	if (!try) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Timeout get data0 failed !!!\n");
		return JL_ERR_TIMEOUT;
	}

	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "cpu reset reg read/write fail[%d]!!!\n", ret);

	return ret;
}

static jl_ret_t __patch_load(jl_device_t *pdevice,
		jl_uint8 *patch_data, jl_uint32 patch_size)
{
	jl_uint8 try = 100;
	jl_uint16 index = 0;
	jl_uint16 i;
	TOP_FW_RESERVED19_t csr = {0};
	jl_uint32 data[4] = {0};
	jl_ret_t ret = JL_ERR_OK;

	while (index != patch_size) {
		/* format data */
		for (i = 0; i < 4; i++) {
			data[i] = (((jl_uint32)patch_data[index+3]) << 24)
					| (((jl_uint32)patch_data[index+2]) << 16)
					| (((jl_uint32)patch_data[index+1]) << 8)
					| ((jl_uint32)patch_data[index]);
			index += 4;
		}

		if ((pdevice->io_desc.type == JL_IO_SPI) || (__is_mid29_load_patch(pdevice) == TRUE)) {
			port_udelay(100);
		} else {
			/* check if ready to receive image data */
			try = 100;
			while (--try) {
				ret = jl_reg_burst_read(&pdevice->io_desc, TOP_FW_RESERVED19, csr.val, 1);
				if (ret)
					goto err;
				if (GET_BIT(csr.BF.fw_reserved19, 8))
					return JL_ERR_FAIL;
				if (!GET_BIT(csr.BF.fw_reserved19, 15))
					break;
			}

			if (!try) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Timeout check receive data failed !!!\n");
				return JL_ERR_TIMEOUT;
			}
		}
		/* download image data */
		ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED20, data, sizeof(data)/sizeof(jl_uint32));
		if (ret)
			goto err;
		SET_BIT(csr.BF.fw_reserved19, 15);
		ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED19, csr.val, 1);
		if (ret)
			goto err;
	}
	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "patch load reg read/write fail[%d]!!!\n", ret);

	return ret;
}

static jl_ret_t __patch_boot(jl_device_t *pdevice)
{
	TOP_FW_RESERVED19_t csr;
	TOP_FW_RESERVED26_t resv26;
	jl_uint16 try = 1000;
	jl_ret_t ret = JL_ERR_OK;

	/*if pre-delay need*/
	/*deliver recover mid instruction by fw_reserved26 registor*/
	resv26.BF.fw_reserved26 = g_mdio_devid; //notice fw set strap mid
	ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED26, resv26.val, 1);
	if (ret)
		goto err;

	/*deliver execute patch instruction*/
	csr.BF.fw_reserved19 = 0;
	SET_BIT(csr.BF.fw_reserved19, 14);
	ret = jl_reg_burst_write(&pdevice->io_desc, TOP_FW_RESERVED19, csr.val, 1);
	if (ret)
		goto err;

	if (pdevice->io_desc.type == JL_IO_SPI) {
		port_udelay(1000);
		__exchange_io(pdevice, JL_IO_SPI);
	} else {
		/*check patch done running*/
		if (__is_mid29_load_patch(pdevice) == TRUE) {
			__exchange_mid(pdevice, g_mdio_devid); /*recover io_desc mid 29 */
		}
		while (--try) {
			ret = jl_reg_burst_read(&pdevice->io_desc, TOP_FW_RESERVED19, csr.val, 1);
			if (ret)
				goto err;
			if (GET_BITS(csr.BF.fw_reserved19, 5, 7) == 0x7)
				break;
			port_udelay(1000);
		}
		if (!try) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Timeout check patch done running flag[%lx] failed !!!\n", \
				GET_BITS(csr.BF.fw_reserved19, 5, 7));
			return JL_ERR_TIMEOUT;
		}

		/*Verify patch integrity*/
		ret = jl_reg_burst_read(&pdevice->io_desc, TOP_FW_RESERVED26, resv26.val, 1);
		if (ret)
			goto err;

		TOP_FW_RESERVED26_t resv;

		ret = jl_reg_burst_read(&pdevice->io_desc, TOP_FW_RESERVED26, resv.val, 1);
		if (ret)
			goto err;
		if (resv26.BF.fw_reserved26 == resv.BF.fw_reserved26)
			goto err;
	}
	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "patch load reg read/write fail[%d]!!!\n", ret);

	return ret;
}

static jl_ret_t __pinmux_read(jl_device_t *device, jl_uint32 *pmx_load)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 i = 0;
	jl_uint32 pmx_val[10] = {0};

	ret = jl_reg_burst_read(&device->io_desc, PMX_PIN_MUX_0, pmx_val, 10);
	if (ret)
		goto err;
	memcpy(pmx_load, pmx_val, sizeof(pmx_val));
	for (i = 0; i < sizeof(pmx_val); i++)
		JL_DBG_MSG(JL_FLAG_IO, _DBG_ON, "IO: pmx_load[%d]: 0x%x \n", i, pmx_load[i]);

	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "pinmux read fail[%d]!!!\n", ret);

	return ret;
}

static jl_ret_t __pinmux_write(jl_device_t *device, jl_uint32 *pmx_load)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint16 i = 0;
	jl_uint32 pmx_val[10] = {0};

	memcpy(pmx_val, pmx_load, sizeof(pmx_val));
	ret = jl_reg_burst_write(&device->io_desc, PMX_PIN_MUX_0, pmx_val, 10);
	if (ret)
		goto err;
	for (i = 0; i < sizeof(pmx_val); i++)
		JL_DBG_MSG(JL_FLAG_IO, _DBG_ON, "IO: pmx_load[%d]: 0x%x \n", i, pmx_val[i]);

	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "pinmux write fail[%d]!!!\n", ret);

	return ret;
}

jl_ret_t jl61xx_load_patch(jl_device_t *device)
{
	if (device->io_desc.type == JL_IO_CPU ||
		device->io_desc.type == JL_IO_I2C ||
		device->io_desc.type == JL_IO_I2C_GPIO) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "IO_TYPE: %d, Skip load patch\n", device->io_desc.type);
		return JL_ERR_OK;
	}

	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 *patch_data = NULL;
	jl_uint32 patch_size = 0;
	jl_uint32 pmx_load[10] = {0};

	ret = __patch_select(device, &patch_data, &patch_size);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Could not find patch file, skip load patch!!!\n");
		return JL_ERR_OK;
	}

	if (patch_size == 0)
		return JL_ERR_OK;

	ret = __pinmux_read(device, pmx_load);
	if (ret)
		goto err;

	if (device->io_desc.type == JL_IO_SMI)
		g_mdio_devid = device->io_desc.smi.mdio.bus_id>>24;
	else
		g_mdio_devid = 0;

	if (device->io_desc.type == JL_IO_SPI)
		ret = __ccs_reset_for_spi(device);
	else
		ret = __ccs_reset(device);
	if (ret)
		goto err;

	ret = __patch_load(device, patch_data, patch_size);
	if (ret)
		goto err;

	ret = __patch_boot(device);
	if (ret)
		goto err;

	ret = __pinmux_write(device, pmx_load);
	if (ret)
		goto err;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Switch load patch ready.\n");

	return ret;

err:
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Switch load patch failed[%d]!!!\n", ret);

	return ret;
}
