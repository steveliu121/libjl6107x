/**
 * @file jl_types.h
 * @brief common data types
 */

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

#ifndef __JL_TYPES_H__
#define __JL_TYPES_H__
#ifndef __KERNEL__
#include <stdint.h>
#else
#include <linux/types.h>
#endif
#if defined(WIN32)
#if defined(CONFIG_JL_DLL_EXPORT_SYMBOL)
#define JL_API          __declspec(dllexport)
#elif defined(CONFIG_JL_STATIC_LINK)
#define JL_API
#else
#define JL_API          __declspec(dllimport)
#endif
#else				/* WIN32 */
#define JL_API
#endif

#define attribute_deprecated	__attribute__((deprecated))

#ifndef NULL
#define NULL 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#if defined(__GNUC__)
#ifndef likely
#define likely(x)       __builtin_expect(!!(x), 1)
#endif

#ifndef unlikely
#define unlikely(x)     __builtin_expect(!!(x), 0)
#endif
#else
#ifndef likely
#define likely(x)       (x)
#endif

#ifndef unlikely
#define unlikely(x)     (x)
#endif

#endif

typedef uint64_t jl_uint64;
typedef int64_t jl_int64;
typedef uint32_t jl_uint32;
typedef int32_t jl_int32;
typedef uint16_t jl_uint16;
typedef int16_t jl_int16;
typedef uint8_t jl_uint8;
typedef int8_t jl_int8;
typedef uint8_t jl_bool;
typedef uintptr_t jl_uintptr;
typedef jl_int32 jl_api_ret_t;
typedef jl_int32 jl_ret_t;
typedef jl_uint64 jl_u_long_t;

#define CONST_T     const

/**
 * @enum jl_enable_e
 * @brief enable status.
 */
/**
 * @public typedef jl_enable_t
 */
typedef enum jl_enable_e {
	DISABLED = 0,/**< 0 */
	ENABLED,/**< 1 */
	JL_ENABLE_END/**< Invalid */
} jl_enable_t;

#ifndef ETHER_ADDR_LEN
#define ETHER_ADDR_LEN      6
#endif
/**
 * @union mac_addr
 * @brief Initialization mac_addr.
 * @typedef jl_mac_addr_t
 */
typedef union mac_addr {
	struct{
		jl_uint8 addr[ETHER_ADDR_LEN];/**< Indicates MAC Address */
		jl_uint16 unuse;
	};
	jl_uint64 val;/**< Indicates 48 bit MAC Address */
	//jl_uint64 val:48;/**< Indicates 48 bit MAC Address */
} jl_mac_addr_t;

/**
 * @public typedef jl_portmask_t
 */
typedef jl_uint32 jl_portmask_t;

/**
 * @enum jl_chip_e
 * @brief switch chip series identifier.
 */
/**
 * @public typedef jl_chip_t
 */
typedef enum jl_chip_e {
	JL_CHIP_51XX = 0xffU,/**< 0xff */
	JL_CHIP_61XX = 0x1ffU,/**< 0x1ff */
	JL_CHIP_6107X = 0x2ffU,/**< 0x2ff */
	JL_CHIP_SERIES_MASK = 0xffffff00U/**< 0xffffff00 */
} jl_chip_t;

/**
 * @enum jl_chip_name_e
 * @brief switch chip name.
 */
/**
 * @public typedef jl_chip_name_t
 */
typedef enum jl_chip_name_e {
	JL_CHIP_5104 = 0x0U,/**< 0x0 */
	JL_CHIP_5105 = 0x1U,/**< 0x1 */
	JL_CHIP_5106 = 0x2U,/**< 0x2 */
	JL_CHIP_5108 = 0x3U,/**< 0x3 */
	JL_CHIP_5109 = 0x4U,/**< 0x4 */
	JL_CHIP_5110 = 0x5U,/**< 0x5 */
	JL_CHIP_6108 = 0x100U,/**< 0x100 */
	JL_CHIP_6110 = 0x101U,/**< 0x101 */
	JL_CHIP_6105 = 0x200U,/**< 0x200 */
	JL_CHIP_6107 = 0x201U,/**< 0x201 */
	JL_CHIP_6107S = 0x202U,/**< 0x202 */
	JL_CHIP_6107SC = 0x203U/**< 0x203 */
} jl_chip_name_t;

/**
 * @enum jl_port_e
 * @brief Initialization jl_port_e.
 */
/**
 * @public typedef jl_port_t
 */
typedef enum jl_port_e {
	UTP_PORT0 = 0,/**< 0 */
	UTP_PORT1,/**< 1 */
	UTP_PORT2,/**< 2 */
	UTP_PORT3,/**< 3 */
	UTP_PORT4,/**< 4 */
	UTP_PORT5,/**< 5 */
	UTP_PORT6,/**< 6 */
	UTP_PORT7,/**< 7 */
	UTP_PORT8,/**< 8 */
	UTP_PORT9,/**< 9 */
	UTP_PORT10,/**< 10 */
	UTP_PORT11,/**< 11 */
	UTP_PORT12,/**< 12 */
	UTP_PORT13,/**< 13 */
	UTP_PORT14,/**< 14 */
	UTP_PORT15,/**< 15 */
	UTP_PORT16,/**< 16 */
	UTP_PORT17,/**< 17 */
	UTP_PORT18,/**< 18 */
	UTP_PORT19,/**< 19 */
	UTP_PORT20,/**< 20 */
	UTP_PORT21,/**< 21 */
	UTP_PORT22,/**< 22 */
	UTP_PORT23,/**< 23 */
	UTP_PORT24,/**< 24 */
	UTP_PORT25,/**< 25 */

	EXT_PORT0,/**< 26 */
	EXT_PORT1,/**< 27 */

	CPU_PORT0,/**< 28 */

	JL_PORT_MAX = 29,/**< 29 */
	UNDEF_PORT = 0xffffffff/**< Invalid port */
} jl_port_t;

/**
 * @enum jl_la_group_e
 *.@brief Link aggregation group
 */
/**
 * @public typedef jl_la_group_t
 */
typedef enum jl_la_group_e {
	LA_GROUP_0 = 0, /**< 0 */
	LA_GROUP_1,/**< 1 */
	LA_GROUP_2,/**< 2 */
	LA_GROUP_3,/**< 3 */
	LA_GROUP_4,/**< 4 */
	LA_GROUP_5,/**< 5 */
	LA_GROUP_6,/**< 6 */
	LA_GROUP_7,/**< 7 */
	LA_GROUP_8,/**< 8 */
	LA_GROUP_9,/**< 9 */
	LA_GROUP_10,/**< 10 */
	LA_GROUP_11,/**< 11 */
	LA_GROUP_12,/**< 12 */
	LA_GROUP_13,/**< 13 */
	LA_GROUP_14,/**< 14 */
	LA_GROUP_15,/**< 15 */
	LA_GROUP_16,/**< 16 */
	LA_GROUP_17,/**< 17 */
	LA_GROUP_18,/**< 18 */
	LA_GROUP_19,/**< 19 */
	LA_GROUP_20,/**< 20 */
	LA_GROUP_21,/**< 21 */
	LA_GROUP_22,/**< 22 */
	LA_GROUP_23,/**< 23 */
	LA_GROUP_24,/**< 24 */
	LA_GROUP_25,/**< 25 */
	LA_GROUP_26,/**< 26 */
	LA_GROUP_27,/**< 27 */
	LA_GROUP_28,/**< 28 */
	LA_GROUP_END/**< Invalid */
} jl_la_group_t;

/**
 * @enum jl_port_type_e
 * @brief Initialization port_type_e.
 */
/**
 * @public typedef jl_port_type_t
 */
typedef enum jl_port_type_e {
	PORT_TYPE_UTP = 0,/**< UTP Port */
	PORT_TYPE_EXT,/**< External Port */
	PORT_TYPE_CPU,/**< Inner CPU Port */
	PORT_TYPE_UNDEF,/**< Undefined Port */
	PORT_TYPE_END/**< Invalid Type */
} jl_port_type_t;

/**
 * @enum jl_port_dir_e
 * @brief port direction
 */
/**
 * @public typedef jl_port_dir_t
 */
typedef enum jl_port_dir_e {
	PORT_DIR_INGRESS = 0,/**< ingress direction */
	PORT_DIR_EGRESS = 1/**< egress direction */
} jl_port_dir_t;

/**
 * @enum jl_stat_e
 * @brief Initialization stat_e.
 */
/**
 * @public typedef jl_stat_t
 */
typedef enum jl_stat_e {
	STAT_UNINIT = 0,/**< Uninitialized State */
	STAT_READY,/**< Ready State */
	STAT_END/**< Invalid State */
} jl_stat_t;

/**
 * @struct jl_gpio_desc_s
 * @brief gpio descriptor.
 * @typedef jl_gpio_desc_t
 */
typedef struct jl_gpio_desc_s {
	jl_int32 pin;/**< gpio pin number */
	char name[16];/**< gpio pin name */
	void *priv;/**< private pointer for user usage */
} jl_gpio_desc_t;

/**
 * @enum jl_io_type_e
 * @brief register access io type.
 */
/**
 * @public typedef jl_io_type_t
 */

typedef enum jl_io_type_e {
	JL_IO_CPU = 0,/**< switch chip internal cpu */
	JL_IO_SMI,/**< smi interface */
	JL_IO_SMI_GPIO,/**< smi over gpio */
	JL_IO_SMI_SPI,/**< smi over spi */
	JL_IO_SMI_SPI_DEMO,/**< smi over ftdi */
	JL_IO_SMI_HID_DEMO,/**< smi over hid */
	JL_IO_I2C,/**< i2c interface */
	JL_IO_I2C_GPIO,/**< i2c over gpio */
	JL_IO_SPI/**< spi interface */
} jl_io_type_t;

/**
 * @enum jl_mdio_phy_address_e
 * @brief define mdio phy address value
 */
/**
 * @public typedef jl_mdio_phy_address_t
 */
typedef enum jl_mdio_phy_address_e {
	MDIO_DEVID_0 = 0,/**< mii management MDC/MDIO device id 0 */
	MDIO_DEVID_29 = 29/**< mii management MDC/MDIO device id 29  */
} jl_mdio_phy_address_t;

#endif				/* __JL_TYPES_H__ */
