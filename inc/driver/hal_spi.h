#ifndef __HAL_SPI_H__
#define __HAL_SPI_H__

#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_api_ret_t jl_spi_init(jl_io_desc_t *io_desc);
jl_api_ret_t jl_spi_deinit(jl_io_desc_t *io_desc);
void jl_spi_write(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val);
jl_uint16 jl_spi_read(jl_io_desc_t *io_desc, jl_uint16 reg);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_SPI_H__ */

