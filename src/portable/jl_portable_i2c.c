#include "jl_types.h"
#include "jl_error.h"
#include "jl_device.h"

#include "portable/jl_portable.h"

jl_api_ret_t jl_i2c_raw_init(jl_io_desc_t *io_desc)
{
	(void) io_desc;

	return JL_ERR_OK;
}

jl_api_ret_t jl_i2c_raw_deinit(jl_io_desc_t *io_desc)
{
	(void) io_desc;

	return JL_ERR_OK;
}


jl_uint16 jl_i2c_raw_read(jl_io_desc_t *io_desc, jl_uint16 reg)
{
	(void) io_desc;
	(void) reg;
	
	return 0;
}

void jl_i2c_raw_write(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val)
{
	(void) io_desc;
	(void) reg;
	(void) val;
}