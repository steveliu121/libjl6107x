#ifndef __JL_REG_IO_H__
#define __JL_REG_IO_H__

#include "jl_types.h"
#include "jl_device.h"
#include "jl_error.h"
#include "jl_debug.h"

#ifdef __cplusplus
extern "C" {
#endif

/* TODO only for Little Endian, bits count should not bigger than 64 */
JL_API void jl_read_bits(jl_uint8 *buffer, jl_uint64 *bits,
					jl_uint8 start, jl_uint8 count);
JL_API void jl_write_bits(jl_uint8 *buffer, jl_uint64 bits,
					jl_uint8 start, jl_uint8 count);

/* TODO rm phy APIs */
JL_API jl_uint16 jl_phy_reg_read_ext(jl_io_desc_t *io_desc,
		jl_uint32 phy, jl_uint32 page, jl_uint32 reg);
JL_API void jl_phy_reg_write_ext(jl_io_desc_t *io_desc,
		jl_uint32 phy, jl_uint32 page, jl_uint32 reg, jl_uint16 val);
JL_API jl_api_ret_t jl_reg_read(jl_io_desc_t *io_desc,
		jl_uint32 addr, jl_uint32 *val);
JL_API jl_api_ret_t jl_reg_burst_read(jl_io_desc_t *io_desc,
		jl_uint32 addr, jl_uint32 *buf, jl_uint32 size);
JL_API jl_api_ret_t jl_reg_write(jl_io_desc_t *io_desc,
		jl_uint32 addr, jl_uint32 val);
JL_API jl_api_ret_t jl_reg_burst_write(jl_io_desc_t *io_desc,
		jl_uint32 addr, jl_uint32 *buf, jl_uint32 size);
JL_API jl_api_ret_t jl_reg_io_init(jl_io_desc_t *io_desc);
JL_API jl_api_ret_t jl_reg_io_deinit(jl_io_desc_t *io_desc);

#ifdef CONFIG_JL_DUMP_REG_WRITE
JL_API jl_api_ret_t jl_dump_file_open(void);
JL_API void jl_dump_file_close(void);
JL_API jl_api_ret_t jl_dump_direct_write(jl_uint32 phy_id, jl_uint32 page_id,
					jl_uint32 reg_id, jl_uint16 val);
JL_API jl_api_ret_t jl_dump_indirect_write(jl_uint32 addr, jl_uint32 val);
JL_API jl_api_ret_t jl_dump_indirect_burst_write(jl_uint32 addr,
					jl_uint32 size, jl_uint32 *pval);
JL_API jl_api_ret_t jl_dump_add_delay(jl_uint32 us);
JL_API jl_api_ret_t jl_dump_add_comment(char *comment);
JL_API void jl_dump_enable(void);
JL_API void jl_dump_disable(void);
#endif


#ifdef __cplusplus
}
#endif

#endif /* __JL_REG_IO_H__ */
