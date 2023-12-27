#ifndef __KERNEL__
#include <stdint.h>
#endif
#include "jl_kit.h"
#include "driver/hal_jl61xx_smi.h"
#include "driver/jl_reg_ida.h"
jl_ret_t jl61xx_smi_read(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	return jl61xx_ida_read(io_desc, SMI, io_desc->smi.ops.read, io_desc->smi.ops.write, NULL, NULL, addr, buf, size);
}

jl_ret_t jl61xx_smi_write(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size)
{
	return jl61xx_ida_write(io_desc, SMI, io_desc->smi.ops.read, io_desc->smi.ops.write, NULL, NULL, addr, buf, size);
}
