#include "jl_types.h"
#include "jl_error.h"
#include "jl_debug.h"
#include "jl_device.h"

jl_ret_t port_spi_init(jl_io_desc_t *io_desc)
{
	(void) io_desc;

	return JL_ERR_OK;
}

jl_ret_t port_spi_deinit(jl_io_desc_t *io_desc)
{
	(void) io_desc;

	return JL_ERR_OK;
}

jl_ret_t port_spi_write(jl_io_desc_t *io_desc,
			jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size)
{
	(void) io_desc;
	(void) tx_buf;
	(void) rx_buf;
	(void) size;

	return JL_ERR_OK;
}

jl_ret_t port_spi_read(jl_io_desc_t *io_desc,
			jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size)
{
	(void) io_desc;
	(void) tx_buf;
	(void) rx_buf;
	(void) size;

	return JL_ERR_OK;
}
