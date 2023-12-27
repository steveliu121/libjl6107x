/**
 * @file jl_device.h
 * @brief switch device
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

#ifndef __JL_DEVICE_H__
#define __JL_DEVICE_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

struct jl_io_desc_s;
struct jl_driver_s;
struct jl_device_s;
typedef struct jl_io_desc_s jl_io_desc_t;
typedef struct jl_driver_s jl_driver_t;
typedef struct jl_device_s jl_device_t;

/**
 * JL_MAX_CHIP_NUM
 */
#define JL_MAX_CHIP_NUM			8

/* XXX keep consistent with jl_io_s */

typedef struct jl_io_desc_s {
	jl_chip_t chip;/**< switch chip identifier */
	jl_io_type_t type;/**< register access io type */
	union {
		struct {
			void *priv;/**< private pointer for user usage */
			struct {
				jl_api_ret_t (*init)(jl_io_desc_t *io_desc);
				jl_api_ret_t (*deinit)(jl_io_desc_t *io_desc);
				jl_uint32 (*read)(jl_io_desc_t *io_desc, jl_uint32 reg);
				void (*write)(jl_io_desc_t *io_desc, jl_uint32 reg, jl_uint32 val);
			} ops;/**< internal usage */
		} cpu;
		struct {
			union {
				struct {
					jl_uint32 bus_id;/**< mdio bus id */
					void *priv;/**< private pointer for user usage */
				} mdio;
				struct {
					jl_gpio_desc_t mdc;/**< mdc gpio descriptor */
					jl_gpio_desc_t mdio;/**< mdio gpio descriptor */
					void *priv;/**< private pointer for user usage */
				} gpio;
				struct {
					jl_uint32 bus_id;/**< spi bus id */
					void *priv;/**< private pointer for user usage */
				} spi;
				struct {
					jl_uint32 bus_id;/**< hid bus id */
					void *priv;/**< private pointer for user usage */
				} hid;
			};
			struct {
				jl_api_ret_t (*init)(jl_io_desc_t *io_desc);
				jl_api_ret_t (*deinit)(jl_io_desc_t *io_desc);
				jl_uint16 (*read)(jl_io_desc_t *io_desc,
						jl_uint8 phy, jl_uint8 reg);
				void (*write)(jl_io_desc_t *io_desc,
						jl_uint8 phy, jl_uint8 reg,
						jl_uint16 val);
			} ops;/**< internal usage */
		} smi;
		struct {
			jl_uint8 dev_addr;/**< i2c address */
			jl_uint8 mode;/**< i2c mode:0--MSB 1--LSB 2--Lite */
			union{
				struct{
					void *priv;/**< private pointer for user usage */
				} raw;
				struct {
					jl_gpio_desc_t scl;/**< scl gpio descriptor */
					jl_gpio_desc_t sda;/**< sda gpio descriptor */
					void *priv;/**< private pointer for user usage */
				} gpio;
			};
			struct {
				jl_api_ret_t (*init)(jl_io_desc_t *io_desc);
				jl_api_ret_t (*deinit)(jl_io_desc_t *io_desc);
				jl_uint16 (*read)(jl_io_desc_t *io_desc, jl_uint16 reg);
				void (*write)(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val);
			} ops;/**< internal usage */
		} i2c;
		struct {
			union{
				struct{
					void *priv;/**< private pointer for user usage */
				} raw;
			};
			struct {
				jl_api_ret_t (*init)(jl_io_desc_t *io_desc);
				jl_api_ret_t (*deinit)(jl_io_desc_t *io_desc);
				jl_uint16 (*read)(jl_io_desc_t *io_desc, jl_uint16 reg);
				void (*write)(jl_io_desc_t *io_desc, jl_uint16 reg, jl_uint16 val);
			} ops;/**< internal usage */
		} spi;
	};
	jl_stat_t stat;/**< internal usage */
	void *mutex;/**< internal usage */
	struct {
		jl_ret_t (*init)(jl_io_desc_t *io_desc);
		jl_ret_t (*deinit)(jl_io_desc_t *io_desc);
		jl_ret_t (*read)(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
		jl_ret_t (*write)(jl_io_desc_t *io_desc,
				jl_uint32 addr, jl_uint32 *buf, jl_uint8 size);
	} ops;/**< internal usage */
	jl_uint32 reserved;/**< internal usage */
} jl_io_desc_t;

/* XXX keep consistent with jl_dev_s */
typedef struct jl_device_s {
	jl_uint32 compat_id;/**< compatible id(switch chip name) \ref jl_chip_name_t */
	char name[32];/**< switch device name */
	jl_uint32 id;/**< device id range [0, \ref JL_MAX_CHIP_NUM - 1] */
	jl_io_desc_t io_desc;/**< device io information */
	jl_driver_t *driver;/**< internal usage */
	const void *switch_dev;/**< internal usage */
	void *mutex;/**< internal usage */
	void *priv;/**< internal usage */
} jl_device_t;

typedef struct jl_driver_s {
	jl_uint32 compat_id;
	char name[32];
	jl_ret_t (*probe)(jl_device_t *device);
	jl_ret_t (*remove)(jl_device_t *device);
	jl_uint32 ref_count;
	void *ops;
	void *drv_data;
} jl_driver_t;

typedef struct jl_module_s {
	jl_driver_t **drivers;
	jl_device_t **devices;
	/* for device created dynamically */
	jl_device_t device_pool[JL_MAX_CHIP_NUM];
	jl_uint32 driver_num;
	jl_uint32 device_num;
	jl_stat_t stat;
} jl_module_t;

#ifdef __cplusplus
}
#endif

#endif /* __JL_DEVICE_H__ */
