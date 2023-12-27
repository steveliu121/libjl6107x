#ifndef __HAL_IDA_H__
#define __HAL_IDA_H__

#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum jl_access_type {
	SMI = 0,
	I2C,
	SPI
} jl_access_type_t;

//#define IDA_PHY_ID		    0
#define IDA_PAGE_ID		    60
#define IDA_BURST_SIZE_MAX	16

#define IDA_SMI_REG_CMD		0
#define IDA_SMI_REG_INFOR		1
#define IDA_SMI_REG_ADDR_L		2
#define IDA_SMI_REG_ADDR_H		3
#define IDA_SMI_REG_WD_L		4
#define IDA_SMI_REG_WD_H_SI0	13
#define IDA_SMI_REG_RD_L_SI0	14
#define IDA_SMI_REG_RD_H		7

#define IDA_BASE 0x780
#define IDA_REG_CMD         (0 + IDA_BASE)
#define IDA_REG_INFOR		(1 + IDA_BASE)
#define IDA_REG_ADDR_L		(2 + IDA_BASE)
#define IDA_REG_ADDR_H		(3 + IDA_BASE)
#define IDA_REG_WD_L		(4 + IDA_BASE)
#define IDA_REG_WD_H_SI0	(13 + IDA_BASE)
#define IDA_REG_RD_L_SI0	(14 + IDA_BASE)
#define IDA_REG_RD_H		(7 + IDA_BASE)

#define SPI_READ_INSTRUCTION	0x3
#define SPI_WRITE_INSTRUCTION	0x2

jl_ret_t jl61xx_ida_read(jl_io_desc_t *io_desc, jl_access_type_t type,
		jl_uint16 (*phy_read)(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg),
		void (*phy_write)(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg, jl_uint16 val),
		jl_uint16 (*common_read)(jl_io_desc_t *io_desc, jl_uint16 reg),
		void (*common_write)(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val),
		jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);

jl_ret_t jl61xx_ida_write(jl_io_desc_t *io_desc, jl_access_type_t type,
		jl_uint16 (*phy_read)(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg),
		void (*phy_write)(jl_io_desc_t *io_desc, jl_uint8 phy, jl_uint8 reg, jl_uint16 val),
		jl_uint16 (*common_read)(jl_io_desc_t *io_desc, jl_uint16 reg),
		void (*common_write)(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val),
		jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);




#ifdef __cplusplus
}
#endif

#endif /* __HAL_IDA_H__ */
