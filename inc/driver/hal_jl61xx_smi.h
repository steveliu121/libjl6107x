#ifndef __JL_JL61XX_SMI_H__
#define __JL_JL61XX_SMI_H__

#include "jl_types.h"
#include "jl_device.h"

#ifdef __cplusplus
extern "C" {
#endif

jl_ret_t jl61xx_smi_read(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
jl_ret_t jl61xx_smi_write(jl_io_desc_t *io_desc,
			jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);

#ifdef __cplusplus
}
#endif

#endif /* __JL_JL61XX_SMI_H__ */
