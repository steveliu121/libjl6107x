#ifndef __HAL_JL61XX_I2C_H__
#define __HAL_JL61XX_I2C_H__

#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_DEVICE_ADDR    	0x5c

jl_ret_t jl61xx_i2c_read(jl_io_desc_t *io_desc,
			 jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
jl_ret_t jl61xx_i2c_write(jl_io_desc_t *io_desc,
			  jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_JL61XX_I2C_H__ */
