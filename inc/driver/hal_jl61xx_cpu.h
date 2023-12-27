#ifndef __HAL_JL61XX_CPU_H__
#define __HAL_JL61XX_CPU_H__

#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif

#define AHB_SLV0_MEM_BASE            (0x40000000UL)
#define APB_SLV0_MEM_BASE            (0x60000000UL)
#define REG_ADDR(addr)               ((addr >= 0x200000) ? \
				     (APB_SLV0_MEM_BASE + (addr & 0xFFFFF)) : \
				     (AHB_SLV0_MEM_BASE + (addr & 0xFFFFF)))

jl_ret_t jl61xx_cpu_read(jl_io_desc_t *io_desc,
		jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
jl_ret_t jl61xx_cpu_write(jl_io_desc_t *io_desc,
		jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);

jl_api_ret_t jl_cpu_init(jl_io_desc_t *io_desc);
jl_api_ret_t jl_cpu_deinit(jl_io_desc_t *io_desc);

#ifdef __cplusplus
}
#endif

#endif /* __HAL_JL61XX_CPU_H__ */
