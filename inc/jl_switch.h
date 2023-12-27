/**
 * @file jl_switch.h
 * @brief Initialization
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

#ifndef __JL_SWITCH_H__
#define __JL_SWITCH_H__

#include "jl_types.h"
#include "jl_debug.h"
#include "jl_error.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup initialization_group Initialization
 * @{
 */


/**
 * @struct jl_switch_info_s
 * @brief Initialization switch info.
 */
/**
 * @public typedef jl_switch_info_t
 */
typedef struct jl_switch_info_s {
	jl_uint32 compat_id;/**< compatible id(switch chip name) \ref jl_chip_name_t */
	jl_uint32 sdk_ver;/**< Indicates SDK version */
} jl_switch_info_t;

/**
 * @struct jl_io_s
 * @brief register access io descriptor.
 */
/**
 * @public typedef jl_io_t
 */
typedef struct jl_io_s {
	jl_chip_t chip;/**< switch chip identifier \ref jl_chip_t, only for internal usage*/
	jl_io_type_t type;/**< register access io type */
	union {
		struct {
			void *priv;/**< private pointer for user usage */
		} cpu;
		struct {
			union {
				struct {
					jl_uint32 bus_id;/**< bit24-31 is for mido phy address,bit0-23 is for mdio bus id*/
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
		} i2c;
		struct {
			union{
				struct{
					void *priv;/**< private pointer for user usage */
				} raw;
			};
		} spi;
	};
} jl_io_t;

/**
 * @struct jl_dev_s
 * @brief switch device information.
 */
/**
 * @public typedef jl_dev_t
 */
typedef struct jl_dev_s {
	jl_uint32 compat_id;/**< compatible id(switch chip name) \ref jl_chip_name_t */
	char name[32];/**< switch device name */
	jl_uint32 id;/**< device id range [0, \ref JL_MAX_CHIP_NUM -1] */
	jl_io_t io;/**< device io information */
} jl_dev_t;


/**
 * @brief Initialize the switch system
 * @details This function initialize the switch system.
 * And switch devices created statically will be initialized.
 * @note Switch device could be created statically in file jl_module.c.
 * Refer to "sdk programming guide" for more details.
 * @return the switch system is initialized or not
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_FAIL		Fail
 */
JL_API jl_api_ret_t jl_switch_init(void);

/**
 * @brief Deinitialize the switch system
 * @details This function de-initialize the switch system.
 * And switch devices which created statically or dynamically will be deinitialized and deleted.
 * @return the switch system is deinitialized or not
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_FAIL		Fail
 */
JL_API jl_api_ret_t jl_switch_deinit(void);

/**
 * @brief Get chip's version info
 * @details This function is used to get the version information
 * @param chip_id	User defined chip index number
 * @param pinfo		Chip information
 * @return version info is sucessfully get or not
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_FAIL		Fail
 */
JL_API jl_api_ret_t jl_switch_info(jl_uint32 chip_id, jl_switch_info_t *pinfo);

/**
 * @brief Set state to the switch
 * @details This function indicates set uninit or ready state to the switch .
 * @param state		The state of switch
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_PARAM		Fail
 */
JL_API jl_api_ret_t jl_switch_init_state_set(jl_stat_t state);

/**
 * @brief Get state from the switch
 * @details This function indicates get uninit or ready state from the switch.
 * @param pstate	The pstate of switch
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval JL_ERR_PARAM		Fail
 */
JL_API jl_api_ret_t jl_switch_init_state_get(jl_stat_t *pstate);

/**
 * @brief Create switch device
 * @details This function create a switch device dynamically.
 * The switch device io is opened at the same time.
 * @note \ref jl_switch_device_create and \ref jl_switch_device_delete should be called in pairs.
 * @param device	The switch device description
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_switch_device_create(jl_dev_t *device);

/**
 * @brief Delete switch device
 * @details This function delete a switch device dynamically.
 * The switch device io is closed at the same time.
 * @note \ref jl_switch_device_create and \ref jl_switch_device_delete should be called in pairs.
 * @param chip_id	The switch device id
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_switch_device_delete(jl_uint32 chip_id);

/**
 * @brief Open switch device
 * @details This function open the switch device io.
 * @note \ref jl_switch_device_open and \ref jl_switch_device_close should be called in pairs.
 * @param chip_id	The switch device id
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_switch_device_open(jl_uint32 chip_id);

/**
 * @brief Close switch device
 * @details This function close the switch device io.
 * @note \ref jl_switch_device_open and \ref jl_switch_device_close should be called in pairs.
 * @param chip_id	The switch device id
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_switch_device_close(jl_uint32 chip_id);

/**
 * @brief Check logical port ID.
 * @details This function check logical port ID.
 * @param chip_id	The switch device id
 * @param lport		Logical port ID
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_lport_check(jl_uint32 chip_id, jl_port_t lport);

/**
 * @brief Check is logical port a UTP port.
 * @details This function check is logical port a UTP port.
 * @param chip_id	The switch device id
 * @param lport		Logical port ID
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_utpport_check(jl_uint32 chip_id, jl_port_t lport);

/**
 * @brief Check is logical port a Extension port.
 * @details This function check is logical port a Extension port.
 * @param chip_id	The switch device id
 * @param lport		Logical port ID
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_extport_check(jl_uint32 chip_id, jl_port_t lport);

/**
 * @brief Check is logical port a CPU port.
 * @details This function check is logical port a CPU port.
 * @param chip_id	The switch device id
 * @param lport		Logical port ID
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_cpuport_check(jl_uint32 chip_id, jl_port_t lport);

/**
 * @brief Get switch core port ID.
 * @details This function get switch core port ID.
 * @param chip_id	The switch device id
 * @param lport		Logical port ID
 * @return switch core port ID
 * @retval Not UNDEF_PORT	Normal port id
 * @retval UNDEF_PORT		Error Port id
 */
JL_API jl_uint32 jl_port_l2c_get(jl_uint32 chip_id, jl_port_t lport);

/**
 * @brief Get logical port ID.
 * @details This function get logical port ID.
 * @param chip_id	The switch device id
 * @param cport		Switch core port ID
 * @return logical port ID
 * @retval Not UNDEF_PORT	Normal port id
 * @retval UNDEF_PORT		Error Port id
 */
JL_API jl_port_t jl_port_c2l_get(jl_uint32 chip_id, jl_port_t cport);

/**
 * @brief Get valid logical portmask.
 * @details This function get valid logical portmask.
 * @param[in]	chip_id		The switch device id
 * @param[out]	lportmask	Logical port portmask
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_lportmask_get(jl_uint32 chip_id, jl_portmask_t *lportmask);

/**
 * @brief Get valid switch core portmask.
 * @details This function get valid switch core portmask.
 * @param[in]	chip_id		The switch device id
 * @param[out]	cportmask	Switch core port portmask
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_cportmask_get(jl_uint32 chip_id, jl_portmask_t *cportmask);

/**
 * @brief Get switch core portmask from logical portmask.
 * @details This function get switch core portmask from logical portmask.
 * @param[in]	chip_id		The switch device id
 * @param[in]	lportmask	Logical port ID
 * @param[out]	cportmask	Switch core port ID
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_portmask_l2c_get(jl_uint32 chip_id, 	jl_portmask_t lportmask, jl_portmask_t *cportmask);

/**
 * @brief Get logical portmask from switch core portmask.
 * @details This function get logical portmask from switch core portmask.
 * @param[in]	chip_id		The switch device id
 * @param[in]	cportmask	Switch core port ID
 * @param[out]	lportmask	Logical port ID
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_portmask_c2l_get(jl_uint32 chip_id, 	jl_portmask_t cportmask, jl_portmask_t *lportmask);


/**
 * @brief Show switch port info with chip info.
 * @details This function show switch port info with init chip info.
 * @param[in]	chip_id		the switch device id
 * @return function result
 * @retval JL_ERR_OK		Ok
 * @retval minus_value		Fail
 */
JL_API jl_api_ret_t jl_show_switchport_info(jl_uint32 chip_id);

#ifdef __cplusplus
}
#endif
#endif /* __JL_SWITCH_H__ */
/** @} Initialization APIs*/
