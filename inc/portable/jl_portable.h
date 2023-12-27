#ifndef __JL_PORTABLE_H__
#define __JL_PORTABLE_H__

#ifndef __KERNEL__
#include <stdint.h>
#endif

#include "jl_error.h"
#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif
jl_ret_t port_gpio_request(jl_gpio_desc_t *gpio);
jl_ret_t port_gpio_free(jl_gpio_desc_t *gpio);
void port_gpio_set_dir(jl_gpio_desc_t *gpio, jl_int32 dir);
void port_gpio_set_value(jl_gpio_desc_t *gpio, jl_int32 val);
jl_int32 port_gpio_get_value(jl_gpio_desc_t *gpio);


jl_ret_t port_spi_init(jl_io_desc_t *io_desc);
jl_ret_t port_spi_deinit(jl_io_desc_t *io_desc);
jl_ret_t port_spi_read(jl_io_desc_t *io_desc,
			jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size);
jl_ret_t port_spi_write(jl_io_desc_t *io_desc,
			jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size);


jl_ret_t port_mdio_init(jl_io_desc_t *io_desc);
jl_ret_t port_mdio_deinit(jl_io_desc_t *io_desc);
void port_mdio_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val);
jl_uint16 port_mdio_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg);

/* FIXME */
void i2c_gpio_init(void);
void i2c_sda_set(jl_int8 val);
void i2c_scl_set(jl_int8 val);
void i2c_sda_get(void);
void i2c_sda_dir_in(void);
void i2c_sda_dir_out(void);

void *port_mutex_init(void);
void port_mutex_deinit(void *arg);
void port_mutex_lock(void *arg);
void port_mutex_unlock(void *arg);
void port_ndelay(jl_uint32 ns);
void port_udelay(jl_uint32 us);

void *port_mem_malloc(jl_uint32 size);
void port_mem_free(void *ptr);


#ifdef __cplusplus
}
#endif

#endif /* __JL_PORTABLE_H__ */
