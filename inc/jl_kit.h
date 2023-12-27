/*
 * Copyright (c) 2014-2023 JLSemi Limited
 * All Rights Reserved
 *
 * THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE of JLSemi Limited
 * The copyright notice above does not evidence any actual or intended
 * publication of such source code.
 *
 * No part of this code may be reproduced, stored in a retrieval system,
 * or transmitted, in any form or by any means, electronic, mechanical,
 * photocopying, recording, or otherwise, without the prior written
 * permission of JLSemi Limited
 */

#ifndef __JL_KIT_H__
#define __JL_KIT_H__

#include "jl_error.h"
#include "jl_types.h"
#include "jl_debug.h"
#include "portable/jl_portable.h"

#define JL_MAX(x, y)			((x) >= (y) ? (x) : (y))
#define JL_MIN(x, y)			((x) <= (y) ? (x) : (y))
#define JL_ARRAY_SIZE(x)		(sizeof(x)/sizeof((x)[0]))

#define JL_CHECK_POINTER(x)			\
	do {					\
		if (x == NULL)			\
			return JL_ERR_PARAM;	\
	} while (0)

#define JL_ASSERT(expr) \
	do { \
		if (!(expr)) \
			return JL_ERR_ASSERT;	\
	} while (0)

#define JL_CHECK_BOOL(value) \
do { \
	if (((value) != 0) && ((value) != 1)) { \
		JL_PRINT("value is %d", value);\
		return JL_ERR_PARAM;				\
		} \
} while (0)

#define JL_CHECK_ENUM(expr, value) \
do {							 \
	if ((expr) >= (value)) \
		return JL_ERR_PARAM;	\
} while (0)

#define JL_CHECK_MIN_MAX_RANGE(value, min, max) \
do { \
	if (((value) > (max)) || \
		((value) < (min))) { \
		return JL_ERR_PARAM; \
	} \
} while (0)

#define __MUTEX_INIT(mutex) \
	do { \
		mutex = port_mutex_init(); \
		if (mutex == NULL) \
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_ERROR, "mutex init fail\n"); \
	} while (0)

#define __MUTEX_DEINIT(mutex) \
	do { \
		if (mutex != NULL) { \
			port_mutex_deinit(mutex); \
			mutex = NULL; \
		} \
	} while (0)

#define __MUTEX_LOCK(mutex) \
	do { \
		if (mutex != NULL) \
			port_mutex_lock(mutex); \
	} while (0)

#define __MUTEX_UNLOCK(mutex) \
	do { \
		if (mutex != NULL) \
			port_mutex_unlock(mutex); \
	} while (0)

#ifdef CONFIG_JL_API_MUTEX
#define JL_API_MUTEX_INIT(mutex)	__MUTEX_INIT(mutex)
#define JL_API_MUTEX_DEINIT(mutex)	__MUTEX_DEINIT(mutex)
#define JL_API_MUTEX_LOCK(mutex)	__MUTEX_LOCK(mutex)
#define JL_API_MUTEX_UNLOCK(mutex)	__MUTEX_UNLOCK(mutex)
#else
#define JL_API_MUTEX_INIT(mutex)
#define JL_API_MUTEX_DEINIT(mutex)
#define JL_API_MUTEX_LOCK(mutex)
#define JL_API_MUTEX_UNLOCK(mutex)
#endif				/* JL_API_MUTEX */

#ifdef CONFIG_JL_IO_MUTEX
#define JL_IO_MUTEX_INIT(mutex)		__MUTEX_INIT(mutex)
#define JL_IO_MUTEX_DEINIT(mutex)	__MUTEX_DEINIT(mutex)
#define JL_IO_MUTEX_LOCK(mutex)		__MUTEX_LOCK(mutex)
#define JL_IO_MUTEX_UNLOCK(mutex)	__MUTEX_UNLOCK(mutex)
#else
#define JL_IO_MUTEX_INIT(mutex)
#define JL_IO_MUTEX_DEINIT(mutex)
#define JL_IO_MUTEX_LOCK(mutex)
#define JL_IO_MUTEX_UNLOCK(mutex)
#endif				/* JL_IO_MUTEX */

#define JL_STAT_INIT(stat)	(stat = STAT_READY)
#define JL_STAT_DEINIT(stat)	(stat = STAT_UNINIT)
#define JL_STAT_IS_READY(stat)	(stat == STAT_READY)
#define JL_STAT_IS_UNINIT(stat)	(stat == STAT_UNINIT)
#define JL_CHECK_STAT(stat)					\
	do {						\
		if (stat != STAT_READY)	\
			return JL_ERR_INIT;		\
	} while (0)

/* Macros for memory access operations */
#define _REG8P(p, i)           ((uint8_t *) ((uintptr_t)((p) + (i))))
#define _REG16P(p, i)          ((uint16_t *) ((uintptr_t)((p) + (i))))
#define _REG32P(p, i)          ((uint32_t *) ((uintptr_t)((p) + (i))))

#define _REG8(p, i)                         (*(_REG8P(p, i)))
#define _REG16(p, i)                        (*(_REG16P(p, i)))
#define _REG32(p, i)                        (*(_REG32P(p, i)))
#define REG8(addr)                          _REG8((addr), 0)
#define REG16(addr)                         _REG16((addr), 0)
#define REG32(addr)                         _REG32((addr), 0)

/* Macros for address type convert and access operations */
#define ADDR16(addr)                        ((jl_uint32)(jl_uintptr)(addr))
#define ADDR32(addr)                        ((jl_uint32)(jl_uintptr)(addr))
#define ADDR8P(addr)                        ((jl_uint8 *)(jl_uintptr)(addr))
#define ADDR16P(addr)                       ((jl_uint16 *)(jl_uintptr)(addr))
#define ADDR32P(addr)                       ((jl_uint32 *)(jl_uintptr)(addr))

/* Macros for Bit Operations */
#define BITMASK_MAX                         0xFFFFFFFFUL
#define BITOFS_MAX                          31

// For 32 bit OS, Bit Operation support mask 32 bit wide
#ifndef BIT
#define BIT(bitofs)                         (0x1UL << (bitofs))
#endif
#define BITS(start, end)                    (~(BIT(start) - 1) & ((BIT(end) - 1) | BIT(end)))
#define BITS_WIDTH(width)                   (BITMASK_MAX >> (BITOFS_MAX - (width - 1)))
#define GET_BIT(regval, bitofs)             (((regval) >> (bitofs)) & 0x1)
#define SET_BIT(regval, bitofs)             ((regval) |= BIT(bitofs))
#define CLR_BIT(regval, bitofs)             ((regval) &= (~BIT(bitofs)))
#define FLIP_BIT(regval, bitofs)            ((regval) ^= BIT(bitofs))
#define WRITE_BIT(regval, bitofs, val)      do { CLR_BIT(regval, bitofs); ((regval) |= ((val) << bitofs) & BIT(bitofs)); } while (0)
#define CHECK_BIT(regval, bitofs)           (!!((regval) & (0x1UL<<(bitofs))))
#define GET_BITS(regval, start, end)        (((regval) & BITS((start), (end))) >> (start))
#define SET_BITS(regval, start, end)        ((regval) |= BITS((start), (end)))
#define CLR_BITS(regval, start, end)        ((regval) &= (~BITS((start), (end))))
#define FLIP_BITS(regval, start, end)       ((regval) ^= BITS((start), (end)))
#define WRITE_BITS(regval, start, end, val) do { CLR_BITS(regval, start, end); ((regval) |= ((val) << start) & BITS((start), (end))); } while (0)
#define CHECK_BITS_ALL(regval, start, end)  (!((~(regval)) & BITS((start), (end))))
#define CHECK_BITS_ANY(regval, start, end)  ((regval) & BITS((start), (end)))

#define BITMASK_SET(regval, mask)           ((regval) |= (mask))
#define BITMASK_CLR(regval, mask)           ((regval) &= (~(mask)))
#define BITMASK_FLIP(regval, mask)          ((regval) ^= (mask))
#define BITMASK_CHECK_ALL(regval, mask)     (!((~(regval)) & (mask)))
#define BITMASK_CHECK_ANY(regval, mask)     ((regval) & (mask))

// eg: BITS_RANGE(8,4) = 0xf00
#ifndef BITS_RANGE
#define BITS_RANGE(offset, range)           BITS((offset), ((offset) + (range) - 1))
#endif

// eg: BITS_OFFSET_R(0x2345, 8 , 4) = 0x3
#ifndef BITS_OFFSET_R
#define BITS_OFFSET_R(val, offset, range) (((val) >> (offset)) & (BITS(0, (range) - 1)))
#endif

// eg: BITS_OFFSET_L(0x2345, 8 , 4) = 0x500
#ifndef BITS_OFFSET_L
#define BITS_OFFSET_L(val, offset, range) (((val) & (BITS(0, (range) - 1))) << (offset))
#endif
#endif				/* __JL_KIT_H__ */
