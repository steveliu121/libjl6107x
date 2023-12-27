#ifndef __KERNEL__
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#endif
#include "jl_error.h"
#include "jl_types.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
#include "driver/hal_smi.h"


struct bit_buffer_s {
	jl_uint8 *p_data;
	jl_uint8  i_mask;
	jl_int32 i_size;
	jl_int32 i_data;
};

static void write_bits(struct bit_buffer_s *buffer, jl_int32 count, jl_uint64 bits)
{
	while (count > 0) {
		count--;
		if ((bits >> count)&0x01)
			buffer->p_data[buffer->i_data] |= buffer->i_mask;
		else
			buffer->p_data[buffer->i_data] &= ~buffer->i_mask;
		buffer->i_mask >>= 1;
		if (buffer->i_mask == 0) {
			buffer->i_data++;
			buffer->i_mask = 0x80;
		}
	}
}
#if 0
static void read_bits(struct bit_buffer_s *buffer, jl_int32 count, jl_uint64 *bits)
{
	*bits = 0;
	while (count > 0) {
		count--;
		*bits += buffer->p_data[buffer->i_data] & buffer->i_mask;
		buffer->i_mask >>= 1;
		*bits <<= 1;
		if (buffer->i_mask == 0) {
			buffer->i_data++;
			buffer->i_mask = 0x80;
		}
	}
}
#endif
/* wr:0,read;1:write*/
static void __pack_spi_frame(jl_int32 wr, jl_uint8 *buf, jl_uint8 phy, jl_uint8 reg, jl_uint16 data)
{
	struct bit_buffer_s bitbuf;

	bitbuf.i_size = 8;			/* a clause-22 frame size in bytes */
	bitbuf.i_mask = 0x80;
	bitbuf.p_data = buf;
	bitbuf.i_data = 0;

	write_bits(&bitbuf, 32, 0xffffffff);	/* 32 bits preambles:32-1 */
	write_bits(&bitbuf, 2, 1);		/* 2 bits start code:01 */
	if (wr)
		write_bits(&bitbuf, 2, 1);		/* 2 bits opcode(write:01, read:10) */
	else
		write_bits(&bitbuf, 2, 2);
	write_bits(&bitbuf, 5, phy);		/* 5 bits phy id */
	write_bits(&bitbuf, 5, reg);		/* 5 bits register address */
	write_bits(&bitbuf, 2, 2);		/* 2 bits turn around(a trick way) */
	write_bits(&bitbuf, 16, data);		/* 16 bits data, if spi read, tx data is 0xff(MISO/MOSI wired-AND) */
}

jl_uint16 jl_smi_spi_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg)
{
	jl_uint8 tx_buf[8] = {0};
	jl_uint8 rx_buf[8] = {0};

	__pack_spi_frame(0, tx_buf, phy, reg, 0xffff);
	port_spi_read(io_desc, tx_buf, rx_buf, 8);

	return ((jl_uint16 ) (rx_buf[6] << 8 | rx_buf[7]));
}

void jl_smi_spi_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val)
{
	jl_uint8 tx_buf[8] = {0};
	jl_uint8 rx_buf[8] = {0};

	__pack_spi_frame(1, tx_buf, phy, reg, val);
	port_spi_write(io_desc, tx_buf, rx_buf, 8);
}

jl_api_ret_t jl_smi_spi_init(jl_io_desc_t *io_desc)
{
	return port_spi_init(io_desc);
}

jl_api_ret_t jl_smi_spi_deinit(jl_io_desc_t *io_desc)
{
	return port_spi_deinit(io_desc);
}
