#include <stdint.h>
#include <stdio.h>

#include "jl_types.h"
#include "jl_error.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
#include "driver/hal_i2c.h"

#define I2C_WRITE	1
#define I2C_READ	2

#define I2C_SETUP_TIME	    10
#define I2C_HOLD_TIME	    10

#define HAL_I2C_DELAY_TIME_1US	    1
#define HAL_I2C_DELAY_TIME_2US	    2
#define HAL_I2C_DELAY_TIME_5US	    5
#define HAL_I2C_DELAY_TIME_10US	    10
#define HAL_I2C_DELAY_TIME_100US	100

enum i2c_state {
	I2C_NO_ACK = 0,
	I2C_ACK,
	I2C_ERROR,
};

#define I2C_WAIT_ACK_LOOP_TIME	100

#define I2C_HIGH		1
#define I2C_LOW		    0

#define I2C_OUT			1
#define I2C_IN		    0

#define UINT16_H(x)		(x >> 8)
#define UINT16_L(x)		(x & 0xff)
#define SWAP_BYTE(x)	((x >> 8) | ((x & 0xff) << 8))

static void set_dir(jl_gpio_desc_t *gpio, jl_int32 dir)
{	
	port_gpio_set_dir(gpio, dir);
}
static void set_dat(jl_gpio_desc_t *gpio, jl_int32 val)
{	
	port_gpio_set_value(gpio, val);
}
static jl_ret_t get_dat(jl_gpio_desc_t *gpio)
{	
	return port_gpio_get_value(gpio);
}

static jl_ret_t i2c_init(jl_io_desc_t *io_desc)
{
	jl_ret_t ret = JL_ERR_OK;

	ret = port_gpio_request(&io_desc->i2c.gpio.scl);
	if (ret) return ret;
	ret = port_gpio_request(&io_desc->i2c.gpio.sda);
	if (ret) return ret;

	set_dir(&io_desc->i2c.gpio.sda, I2C_IN);
	set_dir(&io_desc->i2c.gpio.scl, I2C_IN);

	return ret;
}

static jl_ret_t i2c_deinit(jl_io_desc_t *io_desc)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_ret_t _ret = JL_ERR_OK;

	_ret = port_gpio_free(&io_desc->i2c.gpio.scl);
	if (_ret)
		ret = JL_ERR_FAIL;

	_ret = port_gpio_free(&io_desc->i2c.gpio.sda);
	if (_ret)
		ret = JL_ERR_FAIL;

	return ret;
}

static inline void i2c_start(jl_io_desc_t *io_desc)
{
	set_dir(&io_desc->i2c.gpio.sda, I2C_OUT);	//SDA output
	set_dir(&io_desc->i2c.gpio.scl, I2C_OUT);	//SCL output

	set_dat(&io_desc->i2c.gpio.sda, I2C_HIGH);	//SDA=1
	set_dat(&io_desc->i2c.gpio.scl, I2C_HIGH);	//SCL=1
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	set_dat(&io_desc->i2c.gpio.sda, I2C_LOW);	//SDA=0
	port_udelay(HAL_I2C_DELAY_TIME_1US);
}

static inline void i2c_stop(jl_io_desc_t *io_desc)
{
	set_dir(&io_desc->i2c.gpio.sda, I2C_OUT);	//SDA output

	set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);	//SCL=0
	set_dat(&io_desc->i2c.gpio.sda, I2C_LOW);	//SDA=0
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	set_dat(&io_desc->i2c.gpio.scl, I2C_HIGH);	//SCL=1
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	set_dat(&io_desc->i2c.gpio.sda, I2C_HIGH);	//SDA=1
	port_udelay(HAL_I2C_DELAY_TIME_10US);

	set_dir(&io_desc->i2c.gpio.sda, I2C_IN);	//SDA input
	set_dir(&io_desc->i2c.gpio.scl, I2C_IN);	//SCL input
}

static  void i2c_ack(jl_io_desc_t *io_desc)
{
	set_dir(&io_desc->i2c.gpio.sda, I2C_OUT);	//SDA output

	set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);	//SCL=0
	set_dat(&io_desc->i2c.gpio.sda, I2C_LOW);	//SDA=0
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	set_dat(&io_desc->i2c.gpio.scl, I2C_HIGH);	//SCL=1
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);	//SCL=0
	port_udelay(HAL_I2C_DELAY_TIME_1US);
}

static inline void i2c_no_ack(jl_io_desc_t *io_desc)
{
	set_dir(&io_desc->i2c.gpio.sda, I2C_OUT);	//SDA output

	set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);	//SCL=0
	set_dat(&io_desc->i2c.gpio.sda, I2C_HIGH);	//SDA=1
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	set_dat(&io_desc->i2c.gpio.scl, I2C_HIGH);	//SCL=1
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);	//SCL=0
	port_udelay(HAL_I2C_DELAY_TIME_1US);
}

static inline void i2c_wait_ack(jl_io_desc_t *io_desc)
{
	jl_uint32 loop = 0;
	jl_uint8 ack = 0;

	set_dir(&io_desc->i2c.gpio.sda, I2C_IN);	//SDA input

	set_dat(&io_desc->i2c.gpio.scl, I2C_HIGH);	//SCL=1
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	ack = get_dat(&io_desc->i2c.gpio.sda);		//get SDA
	while (ack && (loop < I2C_WAIT_ACK_LOOP_TIME)) {
		ack = get_dat(&io_desc->i2c.gpio.sda);
		loop++;
	}

	port_udelay(HAL_I2C_DELAY_TIME_2US);
	set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);	//SCL=0
	port_udelay(HAL_I2C_DELAY_TIME_2US);
}

static inline void i2c_write_byte(jl_io_desc_t *io_desc, jl_uint8 byte)
{
	jl_uint8 i = 8;
	jl_uint8 data = 0;
	jl_uint8 bit = 0;

	set_dir(&io_desc->i2c.gpio.sda, I2C_OUT);	//SDA output
	set_dir(&io_desc->i2c.gpio.scl, I2C_OUT);	//SCL output

	data = byte;
	while (i--) {
		bit = !!(data & 0x80);
		set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);	//SCL=0
		port_udelay(HAL_I2C_DELAY_TIME_1US);
		set_dat(&io_desc->i2c.gpio.sda, bit);
		set_dat(&io_desc->i2c.gpio.scl, I2C_HIGH);	//SCL=1
		port_udelay(HAL_I2C_DELAY_TIME_1US);
		data<<=1;
	}
	set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);		//SCL=0
	port_udelay(HAL_I2C_DELAY_TIME_1US);
}

static inline uint8_t i2c_read_byte(jl_io_desc_t *io_desc, jl_uint8 ack)
{
	jl_uint8 data = 0;
	jl_uint8 i = 8;

	port_udelay(HAL_I2C_DELAY_TIME_1US);
	set_dir(&io_desc->i2c.gpio.sda, I2C_IN);	//SDA input
	port_udelay(HAL_I2C_DELAY_TIME_1US);
	while (i--) {
		data <<= 1;
		set_dat(&io_desc->i2c.gpio.scl, I2C_HIGH);	//SCL=1
		port_udelay(HAL_I2C_DELAY_TIME_1US);
		data += get_dat(&io_desc->i2c.gpio.sda);	//get SDA
		port_udelay(HAL_I2C_DELAY_TIME_1US);
		set_dat(&io_desc->i2c.gpio.scl, I2C_LOW);	//SCL=0
		port_udelay(HAL_I2C_DELAY_TIME_1US);
	}

	if (I2C_ACK == ack){
		i2c_ack(io_desc);
	} else {
		i2c_no_ack(io_desc);
	}
	return data;
}

jl_api_ret_t jl_i2c_gpio_init(jl_io_desc_t *io_desc)
{
	return i2c_init(io_desc);
}

jl_api_ret_t jl_i2c_gpio_deinit(jl_io_desc_t *io_desc)
{
	return i2c_deinit(io_desc);
}

void jl_i2c_gpio_write(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val)
{
	if (io_desc->i2c.mode == I2C_MODE_LSB) {
		val = SWAP_BYTE(val);
	} else if (io_desc->i2c.mode == I2C_MODE_LITE) {
		reg = SWAP_BYTE(reg);
		val = SWAP_BYTE(val);
	}

	i2c_start(io_desc);
	//I2C dev_addr
	i2c_write_byte(io_desc, io_desc->i2c.dev_addr << 1);
	i2c_wait_ack(io_desc);

	//write addr
	i2c_write_byte(io_desc, reg>>8);
	i2c_wait_ack(io_desc);
	i2c_write_byte(io_desc, reg&0xff);
	i2c_wait_ack(io_desc);

	//write data
	i2c_write_byte(io_desc, val>>8);
	i2c_wait_ack(io_desc);
	i2c_write_byte(io_desc, val&0xff);
	i2c_wait_ack(io_desc);
	port_udelay(HAL_I2C_DELAY_TIME_100US);
	i2c_stop(io_desc);
}

jl_uint16  jl_i2c_gpio_read(jl_io_desc_t *io_desc, jl_uint16 reg)
{
	jl_uint8 return_hi = 0;
	jl_uint8 return_lo = 0;
	jl_uint8 dev_addr = io_desc->i2c.dev_addr << 1;
	jl_uint16 value = 0;

	if (io_desc->i2c.mode == I2C_MODE_LITE) {
		reg = SWAP_BYTE(reg);
		dev_addr |= 0x01;
	}

	i2c_start(io_desc);
	i2c_write_byte(io_desc, dev_addr);
	i2c_wait_ack(io_desc);

	//write addr
	i2c_write_byte(io_desc, reg>>8);
	i2c_wait_ack(io_desc);
	i2c_write_byte(io_desc, reg & 0xff);
	i2c_wait_ack(io_desc);

	if (io_desc->i2c.mode != I2C_MODE_LITE) {
		i2c_stop(io_desc);
		port_udelay(HAL_I2C_DELAY_TIME_5US);
		i2c_start(io_desc);
		i2c_write_byte(io_desc, 0x1|dev_addr);
		i2c_wait_ack(io_desc);
	}
	//read data
	return_hi =	i2c_read_byte(io_desc, I2C_ACK);
	return_lo = i2c_read_byte(io_desc, I2C_NO_ACK);
	i2c_stop(io_desc);

	if (io_desc->i2c.mode == I2C_MODE_MSB)
		value = (return_hi<<8)|(return_lo);
	else
		value = (return_lo<<8)|(return_hi);

	return value;
}
