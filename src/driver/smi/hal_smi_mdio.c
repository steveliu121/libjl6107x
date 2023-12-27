#ifndef __KERNEL__
#include <stdint.h>
#include <stdio.h>
#endif
#include "jl_error.h"
#include "jl_types.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
#include "driver/hal_smi.h"


jl_uint16 jl_smi_mdio_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg)
{
	jl_uint16 val;

	val = port_mdio_read(io_desc, phy, reg);

	return val;
}

void jl_smi_mdio_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
	port_mdio_write(io_desc, phy, reg, val);
}

jl_api_ret_t jl_smi_mdio_init(jl_io_desc_t *io_desc)
{
	return port_mdio_init(io_desc);
}

jl_api_ret_t jl_smi_mdio_deinit(jl_io_desc_t *io_desc)
{
	return port_mdio_deinit(io_desc);
}
