#ifndef __KERNEL__
#include <stdint.h>
#endif
#include "jl_kit.h"
#include "driver/jl_reg_ida.h"
#include "driver/jl_reg_io.h"

jl_ret_t jl61xx_ida_read(jl_io_desc_t *io_desc, jl_access_type_t type,
		jl_uint16 (*phy_read)(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg),
		void (*phy_write)(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg, jl_uint16 val),
		jl_uint16 (*common_read)(jl_io_desc_t *io_desc, jl_uint16 reg),
		void (*common_write)(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val),
		jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	jl_uint16 burst_size = 0;
	jl_uint16 burst_ctrl = 0;
	jl_uint16 try = 5;
	jl_uint32 tmp = 0;
	jl_int32 i = 0;
	jl_api_ret_t ret = 0;
	jl_uint32 ida_phy_id = 0;

	if ((size == 0) || (size > IDA_BURST_SIZE_MAX)) {
		JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR,
				"Wrong burst size, brust size range:[1, %d]\n",
				IDA_BURST_SIZE_MAX);

		return JL_ERR_PARAM;
	}
	JL_CHECK_POINTER(io_desc);

	if(type == SMI){
		/* set IDA page id */
		ida_phy_id = (jl_uint32)GET_BITS(io_desc->smi.mdio.bus_id, 24, 31);
		phy_write(io_desc, ida_phy_id, 31, IDA_PAGE_ID);

		/* 1. set burst control: read_indication & burst_size */
		burst_size = size - 1;
		burst_ctrl = burst_size << 1;
		phy_write(io_desc, ida_phy_id, IDA_SMI_REG_INFOR, burst_ctrl);

		/* 2. set register address */
		phy_write(io_desc, ida_phy_id, IDA_SMI_REG_ADDR_L, (addr & 0xffff));
		phy_write(io_desc, ida_phy_id, IDA_SMI_REG_ADDR_H, ((addr >> 16) & 0xff));

		/* 3. set ida read request */
		phy_write(io_desc, ida_phy_id, IDA_SMI_REG_CMD, 1);

		/* 4. check operation done & status */
		while (--try) {
			tmp = phy_read(io_desc, ida_phy_id, IDA_SMI_REG_CMD);
			if (!(tmp & 1))
				break;
		}

		if (!try)
			goto timeout;

		/* 5. read data */
		for (i = 0; i < size; i++) {
			buf[i] = phy_read(io_desc, ida_phy_id, IDA_SMI_REG_RD_L_SI0);
			tmp = phy_read(io_desc, ida_phy_id, IDA_SMI_REG_RD_H);
			buf[i] |= tmp << 16;
		}

		return JL_ERR_OK;

	}else if ((type == I2C) || (type == SPI)){
		/* 1. set burst control: read_indication & burst_size */
		burst_size = size - 1;
		burst_ctrl = burst_size << 1;
		common_write(io_desc, IDA_REG_INFOR, burst_ctrl);

		/* 2. set register address */
		common_write(io_desc, IDA_REG_ADDR_L, (addr & 0xffff));
		common_write(io_desc, IDA_REG_ADDR_H, ((addr >> 16) & 0xff));

		/* 3. set ida read request */
		common_write(io_desc, IDA_REG_CMD, 1);

		/* 4. check operation done & status */
		while (--try) {
			tmp = common_read(io_desc, IDA_REG_CMD);
			if (!(tmp & 1))
				break;
		}

		if (!try)
			goto timeout;

		/* 5. read data */
		for (i = 0; i < size; i++) {
			buf[i] = common_read(io_desc, IDA_REG_RD_L_SI0);
			tmp = common_read(io_desc, IDA_REG_RD_H);
			buf[i] |= tmp << 16;
		}

		return JL_ERR_OK;

	timeout:
		JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR, "IDA read register timeout\n");
		ret = JL_ERR_TIMEOUT;
	}
	return ret;
}

jl_ret_t jl61xx_ida_write(jl_io_desc_t *io_desc, jl_access_type_t type,
		jl_uint16 (*phy_read)(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg),
		void (*phy_write)(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg, jl_uint16 val),
		jl_uint16 (*common_read)(jl_io_desc_t *io_desc, jl_uint16 reg),
		void (*common_write)(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val),
		jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	jl_uint16 burst_size = 0;
	jl_uint16 burst_ctrl = 0;
	jl_uint16 try = 5;
	jl_uint16 tmp = 0;
	jl_int32 i = 0;
	jl_api_ret_t ret = 0;
	jl_uint32 ida_phy_id = 0;

	if ((size == 0) || (size > IDA_BURST_SIZE_MAX)) {
		JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR,
				"Wrong burst size, brust size range:[1, %d]\n",
				IDA_BURST_SIZE_MAX);

		return JL_ERR_PARAM;
	}
	JL_CHECK_POINTER(io_desc);

#ifdef CONFIG_JL_DUMP_REG_WRITE
	if (size == 1) {
		ret = jl_dump_indirect_write(addr, *buf);
		if (ret)
			JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR, "Dump smi write fail!!!\n");
	} else {
		ret = jl_dump_indirect_burst_write(addr, size, buf);
		if (ret)
			JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR, "Dump smi write fail!!!\n");
	}
#endif
	if (type == SMI){
		ida_phy_id = (jl_uint32)GET_BITS(io_desc->smi.mdio.bus_id, 24, 31);
		/* set IDA page id */
		phy_write(io_desc, ida_phy_id, 31, IDA_PAGE_ID);

		/* 1. write data */
		for (i = 0; i < size; i++) {
			phy_write(io_desc, ida_phy_id, IDA_SMI_REG_WD_L, (buf[i] & 0x0000ffff));
			phy_write(io_desc, ida_phy_id, IDA_SMI_REG_WD_H_SI0,
						((buf[i] >> 16) & 0x0000ffff));
		}

		/* 2. set burst control: write_indication & burst_size */
		burst_size = size - 1;
		burst_ctrl = burst_size << 1 | 1;
		phy_write(io_desc, ida_phy_id, IDA_SMI_REG_INFOR, burst_ctrl);

		/* 3. set register address */
		phy_write(io_desc, ida_phy_id, IDA_SMI_REG_ADDR_L, (addr & 0xffff));
		phy_write(io_desc, ida_phy_id, IDA_SMI_REG_ADDR_H, ((addr >> 16) & 0xff));

		/* 3. issue write operation */
		phy_write(io_desc, ida_phy_id, IDA_SMI_REG_CMD, 1);

		/* 4. check operation done & status */
		/*XXX skip ida status check */
		if (io_desc->reserved)
			return JL_ERR_OK;
		while (--try) {
			tmp = phy_read(io_desc, ida_phy_id, IDA_SMI_REG_CMD);
			if (!(tmp & 1))
				break;
		}

		if (!try)
			goto timeout;

		return JL_ERR_OK;
	}else if ((type == I2C) || (type == SPI))
	{
		/* 1. write data */
		for (i = 0; i < size; i++) {
			common_write(io_desc, IDA_REG_WD_L, (buf[i] & 0x0000ffff));
			common_write(io_desc, IDA_REG_WD_H_SI0,
						((buf[i] >> 16) & 0x0000ffff));
		}

		/* 2. set burst control: write_indication & burst_size */
		burst_size = size - 1;
		burst_ctrl = burst_size << 1 | 1;
		common_write(io_desc, IDA_REG_INFOR, burst_ctrl);

		/* 3. set register address */
		common_write(io_desc, IDA_REG_ADDR_L, (addr & 0xffff));
		common_write(io_desc, IDA_REG_ADDR_H, ((addr >> 16) & 0xff));

		/* 3. issue write operation */
		common_write(io_desc, IDA_REG_CMD, 1);

		/* 4. check operation done & status */
		/*XXX skip ida status check */
		if (io_desc->reserved)
			return JL_ERR_OK;
		while (--try) {
			tmp = common_read(io_desc, IDA_REG_CMD);
			if (!(tmp & 1))
				break;
		}

		if (!try)
			goto timeout;
	}

	return JL_ERR_OK;

timeout:
	JL_DBG_MSG(JL_FLAG_IO, _DBG_ERROR, "IDA write register timeout\n");
	ret = JL_ERR_TIMEOUT;

	return ret;
}

