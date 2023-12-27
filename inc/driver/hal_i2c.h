#ifndef __HAL_I2C_H__
#define __HAL_I2C_H__

#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define I2C_MODE_MSB		0
#define I2C_MODE_LSB		1
#define I2C_MODE_LITE		2

jl_api_ret_t jl_i2c_gpio_init(jl_io_desc_t *io_desc);
jl_api_ret_t jl_i2c_gpio_deinit(jl_io_desc_t *io_desc);
void jl_i2c_gpio_write(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val);
jl_uint16 jl_i2c_gpio_read(jl_io_desc_t *io_desc, jl_uint16 reg);

jl_api_ret_t jl_i2c_raw_init(jl_io_desc_t *io_desc);
jl_api_ret_t jl_i2c_raw_deinit(jl_io_desc_t *io_desc);
void jl_i2c_raw_write(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val);
jl_uint16 jl_i2c_raw_read(jl_io_desc_t *io_desc, jl_uint16 reg);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_I2C_H__ */
