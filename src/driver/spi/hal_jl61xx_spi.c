#ifndef __KERNEL__
#include <stdint.h>
#endif
#include "jl_kit.h"
#include "driver/hal_jl61xx_spi.h"
#include "driver/jl_reg_ida.h"

jl_ret_t jl61xx_spi_read(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	return jl61xx_ida_read(io_desc, SPI, NULL, NULL, io_desc->spi.ops.read, io_desc->spi.ops.write, addr, buf, size);
}

jl_ret_t jl61xx_spi_write(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	return jl61xx_ida_write(io_desc, SPI, NULL, NULL, io_desc->spi.ops.read, io_desc->spi.ops.write,  addr, buf, size);
}
