#ifndef __KERNEL__
#include <stdint.h>
#endif
#include "jl_kit.h"
#if ((defined CONFIG_JL_IO_CPU) && (defined CONFIG_JL_CHIP_JL61XX))
#include "driver/hal_jl61xx_cpu.h"
#endif
#if ((defined CONFIG_JL_IO_SMI) && (defined CONFIG_JL_CHIP_JL61XX))
#include "driver/hal_jl61xx_smi.h"
#endif
#if ((defined CONFIG_JL_IO_I2C) && (defined CONFIG_JL_CHIP_JL61XX))
#include "driver/hal_jl61xx_i2c.h"
#endif
#if ((defined CONFIG_JL_IO_SPI) && (defined CONFIG_JL_CHIP_JL61XX))
#include "driver/hal_jl61xx_spi.h"
#endif
#include "driver/hal_smi.h"
#include "driver/hal_i2c.h"
#include "driver/hal_spi.h"
#include "driver/jl_reg_io.h"

#ifdef CONFIG_JL_IO_SMI
static jl_ret_t __smi_ops_regist(jl_io_desc_t *io_desc)
{
	switch (io_desc->type) {
#if ((defined CONFIG_JL_SMI_OVER_MDIO) || (defined CONFIG_JL_SMI_OVER_WINUSB))
	case JL_IO_SMI:
		io_desc->smi.ops.init = jl_smi_mdio_init;
		io_desc->smi.ops.deinit = jl_smi_mdio_deinit;
		io_desc->smi.ops.read = jl_smi_mdio_read;
		io_desc->smi.ops.write = jl_smi_mdio_write;

		break;
#endif
#ifdef CONFIG_JL_SMI_OVER_GPIO
	case JL_IO_SMI_GPIO:
		io_desc->smi.ops.init = jl_smi_gpio_init;
		io_desc->smi.ops.deinit = jl_smi_gpio_deinit;
		io_desc->smi.ops.read = jl_smi_gpio_read;
		io_desc->smi.ops.write = jl_smi_gpio_write;

		break;
#endif
#if ((defined CONFIG_JL_SMI_OVER_SPI) || (defined CONFIG_JL_SMI_OVER_FTDI))
	case JL_IO_SMI_SPI:
	case JL_IO_SMI_SPI_DEMO:
		io_desc->smi.ops.init = jl_smi_spi_init;
		io_desc->smi.ops.deinit = jl_smi_spi_deinit;
		io_desc->smi.ops.read = jl_smi_spi_read;
		io_desc->smi.ops.write = jl_smi_spi_write;

		break;
#endif
#ifdef CONFIG_JL_SMI_OVER_HID
	case JL_IO_SMI_HID_DEMO:
		io_desc->smi.ops.init = jl_smi_hid_init;
		io_desc->smi.ops.deinit = jl_smi_hid_deinit;
		io_desc->smi.ops.read = jl_smi_hid_read;
		io_desc->smi.ops.write = jl_smi_hid_write;

		break;
#endif
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown SMI device type!!!\n");
		return JL_ERR_NOXIO;
	}

	return JL_ERR_OK;
}
#endif

#ifdef CONFIG_JL_IO_I2C
static jl_ret_t __i2c_ops_regist(jl_io_desc_t *io_desc)
{
	switch (io_desc->type) {
#ifdef CONFIG_JL_I2C_OVER_GPIO
	case JL_IO_I2C_GPIO:
		io_desc->i2c.ops.init = jl_i2c_gpio_init;
		io_desc->i2c.ops.deinit = jl_i2c_gpio_deinit;
		io_desc->i2c.ops.read = jl_i2c_gpio_read;
		io_desc->i2c.ops.write = jl_i2c_gpio_write;

		break;
#endif
#ifdef CONFIG_JL_I2C_RAW
	case JL_IO_I2C:
		io_desc->i2c.ops.init = jl_i2c_raw_init;
		io_desc->i2c.ops.deinit = jl_i2c_raw_deinit;
		io_desc->i2c.ops.read = jl_i2c_raw_read;
		io_desc->i2c.ops.write = jl_i2c_raw_write;

		break;
#endif
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown I2C device type!!!\n");
		return JL_ERR_NOXIO;
	}

	return JL_ERR_OK;
}
#endif

#ifdef CONFIG_JL_IO_SPI
static jl_ret_t __spi_ops_regist(jl_io_desc_t *io_desc)
{
	switch (io_desc->type) {
#ifdef CONFIG_JL_SPI_RAW
	case JL_IO_SPI:
		io_desc->spi.ops.init = jl_spi_init;
		io_desc->spi.ops.deinit = jl_spi_deinit;
		io_desc->spi.ops.read = jl_spi_read;
		io_desc->spi.ops.write = jl_spi_write;

		break;
#endif
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown SPI device type!!!\n");
		return JL_ERR_NOXIO;
	}

	return JL_ERR_OK;
}
#endif
static jl_ret_t __io_device_init_ex(jl_io_desc_t *io_desc, jl_uint8 init)
{
	jl_ret_t ret = JL_ERR_OK;

	switch (io_desc->type) {
#ifdef CONFIG_JL_IO_CPU
	case JL_IO_CPU:
		(void)init;
		io_desc->ops.init = jl_reg_io_init;
		io_desc->ops.deinit = jl_reg_io_deinit;

		switch (io_desc->chip) {
#ifdef CONFIG_JL_CHIP_JL61XX
		case JL_CHIP_61XX:
		case JL_CHIP_6107X:
			io_desc->ops.read = jl61xx_cpu_read;
			io_desc->ops.write = jl61xx_cpu_write;
			break;
#endif
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown IO chip series [0x%x]!!!\n", io_desc->chip);
			return JL_ERR_NOXIO;
		}

		break;
#endif
#ifdef CONFIG_JL_IO_SMI
	case JL_IO_SMI:
	case JL_IO_SMI_GPIO:
	case JL_IO_SMI_SPI:
	case JL_IO_SMI_SPI_DEMO:
	case JL_IO_SMI_HID_DEMO:
		if (!init) {
			ret = io_desc->smi.ops.deinit(io_desc);
			break;
		}

		io_desc->ops.init = jl_reg_io_init;
		io_desc->ops.deinit = jl_reg_io_deinit;

		switch (io_desc->chip) {
#ifdef CONFIG_JL_CHIP_JL51XX
		case JL_CHIP_51XX:
			io_desc->ops.read = jl51xx_smi_read;
			io_desc->ops.write = jl51xx_smi_write;
			break;
#endif
#ifdef CONFIG_JL_CHIP_JL61XX
		case JL_CHIP_61XX:
		case JL_CHIP_6107X:
			io_desc->ops.read = jl61xx_smi_read;
			io_desc->ops.write = jl61xx_smi_write;
			break;
#endif
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown IO chip series [0x%x]!!!\n", io_desc->chip);
			return JL_ERR_NOXIO;
		}

		ret = __smi_ops_regist(io_desc);
		if (ret)
			return ret;

		ret = io_desc->smi.ops.init(io_desc);

		break;
#endif
#ifdef CONFIG_JL_IO_I2C
	case JL_IO_I2C:
	case JL_IO_I2C_GPIO:
		if (!init) {
			ret = io_desc->i2c.ops.deinit(io_desc);
			break;
		}

		io_desc->ops.init = jl_reg_io_init;
		io_desc->ops.deinit = jl_reg_io_deinit;

		switch (io_desc->chip) {
#ifdef CONFIG_JL_CHIP_JL61XX
		case JL_CHIP_61XX:
		case JL_CHIP_6107X:
			io_desc->ops.read = jl61xx_i2c_read;
			io_desc->ops.write = jl61xx_i2c_write;
			break;
#endif
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown IO chip series [0x%x]!!!\n", io_desc->chip);
			return JL_ERR_NOXIO;
		}

		ret = __i2c_ops_regist(io_desc);
		if (ret)
			return ret;

		ret = io_desc->i2c.ops.init(io_desc);
		break;
#endif
#ifdef CONFIG_JL_IO_SPI
	case JL_IO_SPI:
		if (!init) {
			ret = io_desc->spi.ops.deinit(io_desc);
			break;
		}

		io_desc->ops.init = jl_reg_io_init;
		io_desc->ops.deinit = jl_reg_io_deinit;

		switch (io_desc->chip) {
#ifdef CONFIG_JL_CHIP_JL61XX
		case JL_CHIP_61XX:
		case JL_CHIP_6107X:
			io_desc->ops.read = jl61xx_spi_read;
			io_desc->ops.write = jl61xx_spi_write;
			break;
#endif
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown IO chip series [0x%x]!!!\n", io_desc->chip);
			return JL_ERR_NOXIO;
		}

		ret = __spi_ops_regist(io_desc);
		if (ret)
			return ret;

		ret = io_desc->spi.ops.init(io_desc);
		break;

#endif
	default:
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "Unknown IO device type [%d]!!!\n", io_desc->type);
		return JL_ERR_NOXIO;
	}

	return ret;
}

static jl_ret_t __io_device_init(jl_io_desc_t *io_desc)
{
	return __io_device_init_ex(io_desc, 1);
}

static jl_ret_t __io_device_deinit(jl_io_desc_t *io_desc)
{
	return __io_device_init_ex(io_desc, 0);
}

void jl_read_bits(jl_uint8 *bit_buf, jl_uint64 *bits, jl_uint8 start, jl_uint8 count)
{
	jl_uint64 i_bits = 0;
	jl_uint8 i_count = 0;
	jl_uint32 i_index = (start) / 8;
	jl_uint8 i_mask = 1 << ((start) % 8);

	while (i_count < count) {
		if (bit_buf[i_index] & i_mask)
			i_bits |= (1ULL << i_count);
		if (i_mask == 0x80) {
			i_index++;
			i_mask = 0x01;
		} else
			i_mask <<= 1;
		i_count++;
	}
	*bits = i_bits;
};

void jl_write_bits(jl_uint8 *bit_buf, jl_uint64 bits, jl_uint8 start, jl_uint8 count)
{
	jl_uint64 i_bits = bits;
	jl_uint8 i_count = 0;
	jl_uint32 i_index = (start) / 8;
	jl_uint8 i_mask = 1 << ((start) % 8);

	while (i_count < count) {
		if ((i_bits >> i_count) & 0x01)
			bit_buf[i_index] |= i_mask;
		else
			bit_buf[i_index] &= ~i_mask;
		if (i_mask == 0x80) {
			i_index++;
			i_mask = 0x01;
		} else
			i_mask <<= 1;
		i_count++;
	}
};

jl_uint16 jl_phy_reg_read_ext(jl_io_desc_t *io_desc, jl_uint32 phy, jl_uint32 page, jl_uint32 reg)
{
	jl_uint16 ret;

	JL_CHECK_POINTER(io_desc);
	JL_CHECK_STAT(io_desc->stat);

	if (io_desc->chip != JL_CHIP_51XX) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Only JL51XX support this phy io api !!!\n");
		return 0;
	}

	JL_IO_MUTEX_LOCK(io_desc->mutex);
	io_desc->smi.ops.write(io_desc, phy, 0x1f, page);
	ret = io_desc->smi.ops.read(io_desc, phy, reg);
	JL_IO_MUTEX_UNLOCK(io_desc->mutex);

	return ret;
}

void jl_phy_reg_write_ext(jl_io_desc_t *io_desc, jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint16 val)
{
	if (io_desc == NULL) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Error: NULL param!!!\n");
		return;
	}
	if (JL_STAT_IS_UNINIT(io_desc->stat)) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Error: stat!!!\n");
		return;
	}

	if (io_desc->chip != JL_CHIP_51XX) {
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Only JL51XX support this phy io api !!!\n");
		return;
	}

	JL_IO_MUTEX_LOCK(io_desc->mutex);
	io_desc->smi.ops.write(io_desc, phy, 0x1f, page);
	io_desc->smi.ops.write(io_desc, phy, reg, val);
	JL_IO_MUTEX_UNLOCK(io_desc->mutex);

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_ret_t ret;

	ret = jl_dump_direct_write(phy, page, reg, val);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SMI, _DBG_ERROR, "Dump smi write fail!!!\n");
#endif
}

jl_api_ret_t jl_reg_read(jl_io_desc_t *io_desc, jl_uint32 addr, jl_uint32 *val)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(io_desc);
	JL_CHECK_STAT(io_desc->stat);

	JL_IO_MUTEX_LOCK(io_desc->mutex);
	ret = io_desc->ops.read(io_desc, addr, val, 1);
	JL_IO_MUTEX_UNLOCK(io_desc->mutex);

	return ret;
}

jl_api_ret_t jl_reg_burst_read(jl_io_desc_t *io_desc, jl_uint32 addr, jl_uint32 *buf, jl_uint32 size)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(io_desc);
	JL_CHECK_STAT(io_desc->stat);

	JL_IO_MUTEX_LOCK(io_desc->mutex);
	ret = io_desc->ops.read(io_desc, addr, buf, size);
	JL_IO_MUTEX_UNLOCK(io_desc->mutex);

	return ret;
}

jl_api_ret_t jl_reg_write(jl_io_desc_t *io_desc, jl_uint32 addr, jl_uint32 val)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(io_desc);
	JL_CHECK_STAT(io_desc->stat);

	JL_IO_MUTEX_LOCK(io_desc->mutex);
	ret = io_desc->ops.write(io_desc, addr, &val, 1);
	JL_IO_MUTEX_UNLOCK(io_desc->mutex);

	return ret;
}

jl_api_ret_t jl_reg_burst_write(jl_io_desc_t *io_desc, jl_uint32 addr, jl_uint32 *buf, jl_uint32 size)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(io_desc);
	JL_CHECK_STAT(io_desc->stat);

	JL_IO_MUTEX_LOCK(io_desc->mutex);
	ret = io_desc->ops.write(io_desc, addr, buf, size);
	JL_IO_MUTEX_UNLOCK(io_desc->mutex);

	return ret;
}

jl_api_ret_t jl_reg_io_init(jl_io_desc_t *io_desc)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(io_desc);

	if (JL_STAT_IS_READY(io_desc->stat))
		return JL_ERR_OK;

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_dump_file_open();
#endif

	ret = __io_device_init(io_desc);

	JL_IO_MUTEX_INIT(io_desc->mutex);
	JL_STAT_INIT(io_desc->stat);

	return ret;
}

jl_api_ret_t jl_reg_io_deinit(jl_io_desc_t *io_desc)
{
	jl_api_ret_t ret;

	JL_CHECK_POINTER(io_desc);

	if (JL_STAT_IS_UNINIT(io_desc->stat))
		return JL_ERR_OK;

#ifdef CONFIG_JL_DUMP_REG_WRITE
	jl_dump_file_close();
#endif

	ret = __io_device_deinit(io_desc);

	JL_IO_MUTEX_DEINIT(io_desc->mutex);
	JL_STAT_DEINIT(io_desc->stat);

	return ret;
}
