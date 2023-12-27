#ifndef __JL_REGISTER_H__
#define __JL_REGISTER_H__

#ifndef __KERNEL__
#include <stdio.h>
#include <string.h>
#else
#include <linux/string.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define INDEX_ZERO                      (0)
#define REG_BASE(group, name)           group##_##name
#define REG(group, name)                group##_##name##_t
#define REG_SIZE(group, name)           group##_##name##_ENTRY_SIZE
#define REG_NUM(group, name)            group##_##name##_NUM
#define REG_SHIFT(var)                  ((var) << 2)
#define GROUP_BLOCK_SIZE(group)         group##_BLOCK_SIZE
#define REG_OFFSET(group, name, tblidx, blkidx) \
		((blkidx)*GROUP_BLOCK_SIZE(group)+REG_SHIFT((tblidx)*REG_SIZE(group, name)))

#define BURST_READ(pdevice, reg, pval, group, name)  \
		jl_reg_burst_read(&pdevice->io_desc, reg, pval, REG_SIZE(group, name))
#define BURST_WRITE(pdevice, reg, pval, group, name) \
		jl_reg_burst_write(&pdevice->io_desc, reg, pval, REG_SIZE(group, name))

#define REG_BURST_READ(pdevice, group, name, tblidx, blkidx, pval)	\
	do {\
		ret = BURST_READ(pdevice, REG_BASE(group, name) + \
		REG_OFFSET(group, name, tblidx, blkidx), pval, group, name);\
		if (ret) \
			return ret;\
	} while (0)
#define REG_BURST_WRITE(pdevice, group, name, tblidx, blkidx, pval) \
	do {\
		ret = BURST_WRITE(pdevice, REG_BASE(group, name) + \
		REG_OFFSET(group, name, tblidx, blkidx), pval, group, name);\
		if (ret) \
			return ret;\
	} while (0)
#define REGISTER_READ(device, group, name, var, tblidx, blkidx) \
	if ((tblidx >= REG_NUM(group, name)) || (blkidx >= JL_PORT_MAX)) \
		return JL_ERR_PARAM; \
	REG(group, name) var; \
	memset(&var, 0, sizeof(REG(group, name))); \
	ret = BURST_READ(device, REG_BASE(group, name)+REG_OFFSET(group, name, tblidx, blkidx), var.val, group, name); \
	if (ret != JL_ERR_OK) \
		return ret
#define REGISTER_WRITE(device, group, name, var, tblidx, blkidx) \
do { \
	if ((tblidx >= REG_NUM(group, name)) || (blkidx >= JL_PORT_MAX)) \
		return JL_ERR_PARAM; \
	REG(group, name) *_x; \
	typeof(&var) _y; \
	(void)(&_x == &_y); \
	ret = BURST_WRITE(device, REG_BASE(group, name)+REG_OFFSET(group, name, tblidx, blkidx), var.val, group, name); \
	if (ret != JL_ERR_OK) \
		return ret; \
} while (0)

#define REGISTER_WRITE_BITS(X, Y, Z)	jl_write_bits((jl_uint8 *)X, (jl_uint64)Y, Z, Z##_WIDTH)
#define REGISTER_READ_BITS(X, Y, Z)	jl_read_bits((jl_uint8 *)X, (jl_uint64 *)Y, Z, Z##_WIDTH)

#define REG_BURST_READ_OFFSET(device, group, name, offset, blkidx, burst, pval) \
do { \
	if (blkidx >= JL_PORT_MAX) \
		return JL_ERR_PARAM; \
	ret = jl_reg_burst_read(&device->io_desc, REG_BASE(group, name)+REG_SHIFT(offset)+blkidx*GROUP_BLOCK_SIZE(group), pval, burst); \
	if (ret != JL_ERR_OK) \
		return ret; \
} while (0)
#define REG_BURST_WRITE_OFFSET(device, group, name, offset, blkidx, burst, pval) \
do { \
	if (blkidx >= JL_PORT_MAX) \
		return JL_ERR_PARAM; \
	ret = jl_reg_burst_write(&device->io_desc, REG_BASE(group, name)+REG_SHIFT(offset)+blkidx*GROUP_BLOCK_SIZE(group), pval, burst); \
	if (ret != JL_ERR_OK) \
		return ret; \
} while (0)

#ifdef __cplusplus
}
#endif
#endif				/* __JL_REGISTER_H__ */
