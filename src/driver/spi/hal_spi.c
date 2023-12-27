#ifndef __KERNEL__
#include <stdint.h>
#include <stdio.h>
#endif
#include "jl_types.h"
#include "jl_error.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
#include "driver/hal_spi.h"

jl_api_ret_t jl_spi_init(jl_io_desc_t *io_desc)
{
	port_spi_init(io_desc);

	return JL_ERR_OK;
}

jl_api_ret_t jl_spi_deinit(jl_io_desc_t *io_desc)
{
	port_spi_deinit(io_desc);

	return JL_ERR_OK;
}

jl_uint16 jl_spi_read(jl_io_desc_t *io_desc, jl_uint16 reg)
{
	jl_uint8 tx_buf[5] = {0};
	jl_uint8 rx_buf[5] = {0};
	uint16_t result = 0xffff;
	
	tx_buf[0] = 3;/* opcode(write:02, read:03) */
	tx_buf[1] = (jl_uint8)(reg>>8);
	tx_buf[2] = (jl_uint8)reg;

	port_spi_read(io_desc, tx_buf, rx_buf, 5);
	result = 0;
	result |= rx_buf[3]<<8;
	result |= rx_buf[4];
	return ((jl_uint16) (rx_buf[3] << 8 | rx_buf[4]));
}
void jl_spi_write(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val)
{
	jl_uint8 tx_buf[5] = {0};
	jl_uint8 rx_buf[5] = {0};

	tx_buf[0] = 2;/* opcode(write:02, read:03) */
	tx_buf[1] = (jl_uint8)(reg>>8);
	tx_buf[2] = (jl_uint8)reg;
	tx_buf[3] = (jl_uint8)(val>>8);
	tx_buf[4] = (jl_uint8)val;
	port_spi_write(io_desc, tx_buf, rx_buf, 5);
}
