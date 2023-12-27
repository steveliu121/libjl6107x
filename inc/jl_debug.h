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

#ifndef __JL_DEBUG_H__
#define __JL_DEBUG_H__

#ifdef __KERNEL__
#include <linux/kernel.h>
#define JL_PRINT		printk
#else
#include <stdio.h>
#define JL_PRINT		printf
#endif

#ifdef CONFIG_JL_DEBUG
#define JL_BIT(n)				((0x1U)<<n)
#define JL_FLAG_SYS				JL_BIT(1)
#define JL_FLAG_PHY				JL_BIT(2)
#define JL_FLAG_SMI				JL_BIT(3)
#define JL_FLAG_MAC_UCTRL			JL_BIT(4)
#define JL_FLAG_EMAC				JL_BIT(5)
#define JL_FLAG_SWCORE				JL_BIT(6)
#define JL_FLAG_LED				JL_BIT(7)
#define JL_FLAG_IO				JL_BIT(8)
#define JL_FLAG_ALL				(0xFFFFFFFFU)


enum {
	_DBG_OFF = 0,
	_DBG_ERROR,
	_DBG_WARNING,
	_DBG_INFO,
	_DBG_ON,
};

#define _JL_DBG_TOKEN { \
	"Null", \
	"Err", \
	"Warn", \
	"Info", \
	"Debug" \
}

JL_API extern const char *_jl_dbg_token[];

#define JL_DBG_TOKEN(r)          \
	_jl_dbg_token[(((jl_int32)r) > 0 && ((int)r) <= _DBG_ON) ? (r) : _DBG_OFF]


#define JL_DBG_FLAG_MASK		JL_FLAG_ALL

#define JL_DBG_MSG(flag, level, fmt, ...) \
	do { \
	_Pragma("GCC diagnostic push") \
	_Pragma("GCC diagnostic ignored \"-Wformat\"") \
		if (((flag) & JL_DBG_FLAG_MASK) && (level <= JL_DBG_LEVEL)) { \
			JL_PRINT("[%s][%s:%d] "fmt, JL_DBG_TOKEN(level), __func__, __LINE__, ##__VA_ARGS__); \
		} \
	_Pragma("GCC diagnostic pop") \
	} while (0)
#else
#define JL_DBG_MSG(flag, level, ...) \
		do { \
			} while (0)
#endif

#endif				/* __JL_DEBUG_H__ */
