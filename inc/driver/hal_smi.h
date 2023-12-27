#ifndef __JL_SMI_H__
#define __JL_SMI_H__

#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl51xx_smi_read(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
jl_ret_t jl51xx_smi_write(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);

jl_api_ret_t jl_smi_mdio_init(jl_io_desc_t *io_desc);
jl_api_ret_t jl_smi_mdio_deinit(jl_io_desc_t *io_desc);
void jl_smi_mdio_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val);
jl_uint16 jl_smi_mdio_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg);

jl_api_ret_t jl_smi_gpio_init(jl_io_desc_t *io_desc);
jl_api_ret_t jl_smi_gpio_deinit(jl_io_desc_t *io_desc);
void jl_smi_gpio_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val);
jl_uint16 jl_smi_gpio_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg);

jl_api_ret_t jl_smi_spi_init(jl_io_desc_t *io_desc);
jl_api_ret_t jl_smi_spi_deinit(jl_io_desc_t *io_desc);
void jl_smi_spi_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val);
jl_uint16 jl_smi_spi_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg);

jl_api_ret_t jl_smi_hid_init(jl_io_desc_t *io_desc);
jl_api_ret_t jl_smi_hid_deinit(jl_io_desc_t *io_desc);
void jl_smi_hid_write(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg, jl_uint16 val);
jl_uint16 jl_smi_hid_read(jl_io_desc_t *io_desc,
			jl_uint8 phy, jl_uint8 reg);

#ifdef __cplusplus
}
#endif

#endif /* __JL_SMI_H__ */
