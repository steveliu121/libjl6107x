#ifndef __KERNEL__
#include <stdint.h>
#endif
#include "jl_kit.h"
#include "driver/hal_smi.h"
#include "driver/jl_reg_io.h"

#define REG_NO_OPR_CTRL_0		16
#define REG_NO_OPR_CTRL_1		17
#define REG_NO_OPR_CTRL_2		18
#define REG_NO_OPR_DATA0_L		19
#define REG_NO_OPR_DATA6_L		16
#define APB_REG_BUSRT_SIZE_PG0_MAX	6
#define APB_REG_BUSRT_SIZE_MAX		8

jl_ret_t jl51xx_smi_read(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	/* access apb registers indirectly via SMI on domain "phy 0 & page 0" */
	jl_uint32 phy_0 = 0;
	/* page selection register */
	jl_uint32 phy_reg = 31;
	jl_uint32 page_0 = 0;
	jl_uint32 page_1 = 1;
	jl_uint32 reg_l_14b = 0;
	jl_uint32 reg_h_16b = 0;
	jl_uint16 try = 1000;
	jl_uint32 tmp = 0;
	jl_uint8 pg0_burst = 0;
	jl_uint8 pg1_burst = 0;
	jl_uint16 wide_en = 0;
	jl_uint16 burst_ctrl = 0;
	jl_int32 i, j;
	jl_api_ret_t ret = 0;

	if (size > APB_REG_BUSRT_SIZE_MAX) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Brust size overflow, max burst size is 8\n");

		return JL_ERR_PARAM;
	}

	/* set page 0 */
	io_desc->smi.ops.write(io_desc, phy_0, phy_reg, page_0);

	/* 1. set burst size to 0(1 word)
	 * '0' is same as '1', but would not enable the wide range
	 */
	wide_en = ((size > 1) ? 1 : 0);
	burst_ctrl = (size << 2) | (wide_en << 1);
	io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_CTRL_2, burst_ctrl);

	/* 2. set register address && issue read operation */
	reg_l_14b = ((addr << 2) & 0x0000fffc) | 1;
	reg_h_16b = (addr >> 14) & 0x000003fff;
	io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_CTRL_1, reg_h_16b);
	io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_CTRL_0, reg_l_14b);

	/* 4. check operation done & status */
	while (--try) {
		tmp = io_desc->smi.ops.read(io_desc, phy_0, REG_NO_OPR_CTRL_0);
		if (!(tmp & 1))
			break;
	}

	if (!try)
		goto timeout;

	tmp = io_desc->smi.ops.read(io_desc, phy_0, REG_NO_OPR_CTRL_2);
	if ((tmp & 1))
		goto error;

	/* 5. read data */
	pg0_burst = size;
	if (size > APB_REG_BUSRT_SIZE_PG0_MAX) {
		pg0_burst = APB_REG_BUSRT_SIZE_PG0_MAX;
		pg1_burst = size - APB_REG_BUSRT_SIZE_PG0_MAX;
	}
	for (i = 0; i < pg0_burst; i++) {
		buf[i] = io_desc->smi.ops.read(io_desc, phy_0, REG_NO_OPR_DATA0_L + (i * 2));
		tmp = io_desc->smi.ops.read(io_desc, phy_0, REG_NO_OPR_DATA0_L + (i * 2) + 1);
		buf[i] |= tmp << 16;
	}
	if (size > APB_REG_BUSRT_SIZE_PG0_MAX) {
		/* set page 1 */
		io_desc->smi.ops.write(io_desc, phy_0, phy_reg, page_1);
		for (j = 0; j < pg1_burst; j++) {
			buf[i + j] = io_desc->smi.ops.read(io_desc, phy_0, REG_NO_OPR_DATA6_L + (j * 2));
			tmp = io_desc->smi.ops.read(io_desc, phy_0, REG_NO_OPR_DATA6_L + (j * 2) + 1);
			buf[i + j] |= tmp << 16;
		}
	}

	return JL_ERR_OK;

timeout:
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "read register timeout\n");
	ret = JL_ERR_TIMEOUT;
error:
	ret = JL_ERR_FAIL;
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "read register fail\n");

	return ret;
}

jl_ret_t jl51xx_smi_write(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	/* access apb registers indirectly via SMI on domain "phy 0 & page 0" */
	jl_uint32 phy_0 = 0;
	/* page selection register */
	jl_uint32 phy_reg = 31;
	jl_uint32 page_0 = 0;
	jl_uint32 page_1 = 1;
	jl_uint32 reg_l_14b = 0;
	jl_uint32 reg_h_16b = 0;
	jl_uint16 try = 1000;
	jl_uint16 tmp = 0;
	jl_uint8 pg0_burst = 0;
	jl_uint8 pg1_burst = 0;
	jl_uint16 wide_en = 0;
	jl_uint16 burst_ctrl = 0;
	jl_int32 i, j;
	jl_ret_t ret = 0;

	if (size > APB_REG_BUSRT_SIZE_MAX) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Brust size overflow, max burst size is 8\n");

		return JL_ERR_PARAM;
	}

#ifdef CONFIG_JL_DUMP_REG_WRITE
	if (size == 1) {
		ret = jl_dump_indirect_write(addr, *buf);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Dump smi write fail!!!\n");
	} else {
		ret = jl_dump_indirect_burst_write(addr, size, buf);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Dump smi write fail!!!\n");
	}
#endif

	/* set page 0 */
	io_desc->smi.ops.write(io_desc, phy_0, phy_reg, page_0);

	/* 1. write data */
	pg0_burst = size;
	if (size > APB_REG_BUSRT_SIZE_PG0_MAX) {
		pg0_burst = APB_REG_BUSRT_SIZE_PG0_MAX;
		pg1_burst = size - APB_REG_BUSRT_SIZE_PG0_MAX;
	}
	for (i = 0; i < pg0_burst; i++) {
		io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_DATA0_L + (i * 2), (buf[i] & 0x0000ffff));
		io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_DATA0_L + (i * 2 + 1), ((buf[i] >> 16) & 0x0000ffff));
	}
	if (size > APB_REG_BUSRT_SIZE_PG0_MAX) {
		/* set page 1 */
		io_desc->smi.ops.write(io_desc, phy_0, phy_reg, page_1);
		for (j = 0; j < pg1_burst; j++) {
			io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_DATA6_L + (j * 2), (buf[i + j] & 0x0000ffff));
			io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_DATA6_L + (j * 2 + 1), ((buf[i + j] >> 16) & 0x0000ffff));
		}
		/* set page 0 */
		io_desc->smi.ops.write(io_desc, phy_0, phy_reg, page_0);
	}

	/* 2. set burst size to 0(1 word)
	 * '0' is same as '1', but would not enable the wide range
	 */
	wide_en = ((size > 1) ? 1 : 0);
	burst_ctrl = (size << 2) | (wide_en << 1);
	io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_CTRL_2, burst_ctrl);

	/* 3. set register address && issue read operation */
	reg_l_14b = ((addr << 2) & 0x0000fffc) | 3;
	reg_h_16b = (addr >> 14) & 0x000003fff;
	io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_CTRL_1, reg_h_16b);
	io_desc->smi.ops.write(io_desc, phy_0, REG_NO_OPR_CTRL_0, reg_l_14b);

	/* 4. check operation done & status */
	while (--try) {
		tmp = io_desc->smi.ops.read(io_desc, phy_0, REG_NO_OPR_CTRL_0);
		if (!(tmp & 1))
			break;
	}

	if (!try)
		goto timeout;

	tmp = io_desc->smi.ops.read(io_desc, phy_0, REG_NO_OPR_CTRL_2);
	if ((tmp & 1))
		goto error;

	return JL_ERR_OK;

timeout:
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "write register timeout\n");
	ret = JL_ERR_TIMEOUT;
error:
	ret = JL_ERR_FAIL;
	JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "write register fail\n");

	return ret;
}

