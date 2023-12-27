#include <stdint.h>

#include "jl_kit.h"
#include "driver/hal_jl61xx_i2c.h"
#include "driver/jl_reg_ida.h"

jl_ret_t jl61xx_i2c_read(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	io_desc->i2c.dev_addr = I2C_DEVICE_ADDR;
	return jl61xx_ida_read(io_desc, I2C, NULL, NULL, io_desc->i2c.ops.read, io_desc->i2c.ops.write, addr, buf, size);
}

jl_ret_t jl61xx_i2c_write(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	io_desc->i2c.dev_addr = I2C_DEVICE_ADDR;
	return jl61xx_ida_write(io_desc, I2C, NULL, NULL, io_desc->i2c.ops.read, io_desc->i2c.ops.write, addr, buf, size);
}

