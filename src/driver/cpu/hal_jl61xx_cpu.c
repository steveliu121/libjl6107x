#include <stdint.h>

#include "jl_kit.h"
#include "driver/hal_jl61xx_cpu.h"
#include "driver/jl_reg_io.h"

jl_ret_t jl61xx_cpu_read(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	jl_int32 i;
	jl_uint32 reg_addr;
	(void) io_desc;
	for (i = 0; i < size; i++) {
		reg_addr = addr + i*4;
		buf[i] = REG32(REG_ADDR(reg_addr));
	}
	return JL_ERR_OK;
}

jl_ret_t jl61xx_cpu_write(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	jl_int32 i;
	jl_uint32 reg_addr;
	(void) io_desc;
	for (i = 0; i < size; i++) {
		reg_addr = addr + i*4;
		REG32(REG_ADDR(reg_addr)) = buf[i];
	}
	return JL_ERR_OK;
}
