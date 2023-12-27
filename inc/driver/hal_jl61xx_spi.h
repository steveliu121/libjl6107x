#ifndef __HAL_JL61XX_SPI_H__
#define __HAL_JL61XX_SPI_H__

#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl61xx_spi_read(jl_io_desc_t *io_desc,
			 jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
jl_ret_t jl61xx_spi_write(jl_io_desc_t *io_desc,
			  jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_JL61XX_SPI_H__ */

