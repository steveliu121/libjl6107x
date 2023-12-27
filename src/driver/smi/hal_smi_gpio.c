#include <stdint.h>
#include <stdio.h>

#include "jl_types.h"
#include "jl_error.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
#include "driver/hal_smi.h"

#define SMI_READ	2
#define SMI_WRITE	1

#define SMI_SETUP_TIME	    10
#define SMI_HOLD_TIME	    10

/* Minimum MDC period is 400 ns, plus some margin for error.  smi_DELAY
  * is done twice per period.
  */
#define SMI_DELAY 250

/* The PHY may take up to 300 ns to produce data, plus some margin
 * for error.
 */
#define SMI_READ_DELAY 350


static jl_ret_t init_smi(jl_io_desc_t *io_desc)
{
	jl_ret_t ret = JL_ERR_OK;

	ret = port_gpio_request(&io_desc->smi.gpio.mdc);
	if (ret)
		return ret;
	ret = port_gpio_request(&io_desc->smi.gpio.mdio);
	if (ret)
		return ret;

	port_gpio_set_dir(&io_desc->smi.gpio.mdc, 1);
	port_gpio_set_value(&io_desc->smi.gpio.mdc, 0);
	port_gpio_set_dir(&io_desc->smi.gpio.mdio, 0);

	return ret;
}

static jl_ret_t deinit_smi(jl_io_desc_t *io_desc)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_ret_t _ret = JL_ERR_OK;

	_ret = port_gpio_free(&io_desc->smi.gpio.mdc);
	if (_ret)
		ret = JL_ERR_FAIL;

	_ret = port_gpio_free(&io_desc->smi.gpio.mdio);
	if (_ret)
		ret = JL_ERR_FAIL;

	return ret;
}

static void set_mdc(jl_gpio_desc_t *gpio, jl_int32 val)
{
	port_gpio_set_value(gpio, val);
}

static void set_mdio_dir(jl_gpio_desc_t *gpio, jl_int32 dir)
{
	port_gpio_set_dir(gpio, dir);
}

static void set_mdio_data(jl_gpio_desc_t *gpio, jl_int32 val)
{
	port_gpio_set_value(gpio, val);
}

static jl_ret_t get_mdio_data(jl_gpio_desc_t *gpio)
{
	return port_gpio_get_value(gpio);
}

static void smibb_send_bit(jl_io_desc_t *io_desc, jl_int32 val)
{
	set_mdio_data(&io_desc->smi.gpio.mdio, val);
	port_ndelay(SMI_DELAY);
	set_mdc(&io_desc->smi.gpio.mdc, 1);
	port_ndelay(SMI_DELAY);
	set_mdc(&io_desc->smi.gpio.mdc, 0);
}

static jl_ret_t smibb_get_bit(jl_io_desc_t *io_desc)
{
	port_ndelay(SMI_DELAY);
	set_mdc(&io_desc->smi.gpio.mdc, 1);
	port_ndelay(SMI_READ_DELAY);
	set_mdc(&io_desc->smi.gpio.mdc, 0);

	return get_mdio_data(&io_desc->smi.gpio.mdio);
}

static void smibb_send_num(jl_io_desc_t *io_desc, jl_uint16 val, jl_int32 bits)
{
	jl_int32 i;

	for (i = bits - 1; i >= 0; i--)
		smibb_send_bit(io_desc, (val >> i) & 1);
}

static jl_uint16 smibb_get_num(jl_io_desc_t *io_desc, jl_int32 bits)
{
	jl_int32 i;
	jl_uint16 ret = 0;

	for (i = bits - 1; i >= 0; i--) {
		ret <<= 1;
		ret |= smibb_get_bit(io_desc);
	}

	return ret;
}

static void smibb_cmd(jl_io_desc_t *io_desc,
			jl_int32 op, jl_uint8 phy, jl_uint8 reg)
{
	jl_int32 i;

	set_mdio_dir(&io_desc->smi.gpio.mdio, 1);

	/*
	 * Send a 32 bit preamble ('1's) with an extra '1' bit for good
	 * measure.  The IEEE spec says this is a PHY optional
	 * requirement.  The AMD 79C874 requires one after power up and
	 * one after a MII communications error.  This means that we are
	 * doing more preambles than we need, but it is safer and will be
	 * much more robust.
	 */

	for (i = 0; i < 32; i++)
		smibb_send_bit(io_desc, 1);

	/* TODO Clause 45 */
	/* send the start bit (01) and the read opcode (10) or write (01).
	   Clause 45 operation uses 00 for the start and 11, 10 for
	   read/write */
	smibb_send_bit(io_desc, 0);
	smibb_send_bit(io_desc, 1);
	smibb_send_bit(io_desc, (op >> 1) & 1);
	smibb_send_bit(io_desc, (op >> 0) & 1);

	smibb_send_num(io_desc, phy, 5);
	smibb_send_num(io_desc, reg, 5);
}

jl_uint16 jl_smi_gpio_read(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg)
{
	jl_uint16 ret;

	smibb_cmd(io_desc, SMI_READ, phy, reg);

	set_mdio_dir(&io_desc->smi.gpio.mdio, 0);

	smibb_get_bit(io_desc);

	ret = smibb_get_num(io_desc, 16);

	smibb_get_bit(io_desc);

	return ret;
}

void jl_smi_gpio_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
	smibb_cmd(io_desc, SMI_WRITE, phy, reg);

	smibb_send_bit(io_desc, 1);
	smibb_send_bit(io_desc, 0);

	smibb_send_num(io_desc, val, 16);

	set_mdio_dir(&io_desc->smi.gpio.mdio, 0);
}

jl_api_ret_t jl_smi_gpio_init(jl_io_desc_t *io_desc)
{
	return init_smi(io_desc);
}

jl_api_ret_t jl_smi_gpio_deinit(jl_io_desc_t *io_desc)
{
	return deinit_smi(io_desc);
}
