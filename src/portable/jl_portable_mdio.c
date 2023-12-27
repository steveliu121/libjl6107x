#include "jl_types.h"
#include "jl_error.h"
#include "jl_device.h"

jl_ret_t port_mdio_init(jl_io_desc_t *io_desc)
{
	(void) io_desc;

	return JL_ERR_OK;
}

jl_ret_t port_mdio_deinit(jl_io_desc_t *io_desc)
{
	(void) io_desc;

	return JL_ERR_OK;
}

void port_mdio_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
	(void) io_desc;
	(void) phy;
	(void) reg;
	(void) val;
}

jl_uint16 port_mdio_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg)
{
	(void) io_desc;
	(void) phy;
	(void) reg;

	return 0;
}
