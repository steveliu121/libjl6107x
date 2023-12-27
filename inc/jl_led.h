/**
 * @file jl_led.h
 * @brief LED
 */

/**
 * @defgroup led_group LED
 * @{
 */

/**
 * @example example_led.c
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

#ifndef __JL_API_LED_H__
#define __JL_API_LED_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum jl_led_group_e
 * @brief Led group.
 */
enum jl_led_group_e {
	LED_GROUP0 = 0,/**< 0 */
	LED_GROUP1,/**< 1 */
	LED_GROUP2,/**< 2 */
	LED_GROUP_END,/**< Invalid */
};

/**
 * @public typedef jl_led_group_t
 */
typedef enum jl_led_group_e jl_led_group_t;

/**
 * @enum jl_led_blink_rate_e
 * @brief Led blink rate.
 */
enum jl_led_blink_rate_e {
	LED_BLINKRATE_32MS = 4,/**< 4 */
	LED_BLINKRATE_48MS = 6,/**< 6 */
	LED_BLINKRATE_64MS = 8,/**< 8 */
	LED_BLINKRATE_96MS = 12,/**< 12 */
	LED_BLINKRATE_128MS = 16,/**< 16 */
	LED_BLINKRATE_256MS = 32,/**< 32 */
	LED_BLINKRATE_512MS = 64,/**< 64 */
	LED_BLINKRATE_1024MS = 128,/**< 128 */
	LED_BLINKRATE_END,/**< Invalid */
};

/**
 * @public typedef jl_led_blink_rate_t
 */
typedef enum jl_led_blink_rate_e jl_led_blink_rate_t;

/**
 * @enum jl_led_config_e
 * @brief Led config.
 */
enum jl_led_config_e {
	LED_CONFIG_LINK = 0,/**< 0 */
	LED_CONFIG_ACT,/**< 1 */
	LED_CONFIG_LINK_ACT,/**< 2 */
	LED_CONFIG_SPD10,/**< 3 */
	LED_CONFIG_SPD100,/**< 4 */
	LED_CONFIG_SPD1000,/**< 5 */
	LED_CONFIG_SPD10_ACT,/**< 6 */
	LED_CONFIG_SPD100_ACT,/**< 7 */
	LED_CONFIG_SPD1000_ACT,/**< 8 */
	LED_CONFIG_SPD100_10_ACT,/**< 9 */
	LED_CONFIG_DUPLEX,/**< 10 */
	LED_CONFIG_COL,/**< 11 */
	LED_CONFIG_DUP_COL,/**< 12 */
	LED_CONFIG_LOOP,/**< 13 */
	LED_CONFIG_LINK_RX_ACT,/**< 14 */
	LED_CONFIG_LINK_TX_ACT,/**< 15 */
	LED_CONFIG_MASTER,/**< 16 */
	LED_CONFIG_OFF,/**< 17 */
	LED_CONFIG_END,/**< Invalid */
};

/**
 * @public typedef jl_led_config_t
 */
typedef enum jl_led_config_e jl_led_config_t;

/**
 * @enum jl_led_active_e
 * @brief Led active mode.
 */
enum jl_led_active_e {
	LED_ACTIVE_HIGH = 0,/**< 0 */
	LED_ACTIVE_LOW,/**< 1 */
	LED_ACTIVE_END,/**< Invalid */
};

/**
 * @public typedef jl_led_active_t
 */
typedef enum jl_led_active_e jl_led_active_t;

/**
 * @brief Set LED enable configuration
 * @details This function can be used to enable LED per port per group.\n Portmask indicates an
 * entry of 10-bit correspond to 10 ports. If set to 1, it means the specified led of
 * this port will be enabled. If set to 0, it means the specified led of this port will be disabled.
 * @param[in] chip_id   Chip ID
 * @param[in] group	LED group index
 * @param[in] portmask	PHY portmask
 * @return Set LED enable successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_led_enable_set(jl_uint32 chip_id, jl_led_group_t group, jl_portmask_t portmask);

/**
 * @brief Get LED enable status
 * @param[in]  chip_id   Chip ID
 * @param[in]	group		LED group index
 * @param[out]	pportmask	PHY portmask
 * @return Get LED enable status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_led_enable_get(jl_uint32 chip_id, jl_led_group_t group, jl_portmask_t *pportmask);

/**
 * @brief Set LED configuration
 * @details This function can be used to set the specified led group behavior.
 * @param[in] chip_id   Chip ID
 * @param[in] group	LED group index
 * @param[in] config	led behavior configuration
 * @return Set LED configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_led_group_config_set(jl_uint32 chip_id, jl_led_group_t group, jl_led_config_t config);

/**
 * @brief Get LED configuration
 * @details This function can be used to get the specified led group behavior.
 * @param[in]  chip_id   Chip ID
 * @param[in]	group	LED group index
 * @param[out]	pconfig	LED behavior configuration
 * @return Get LED configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_led_group_config_get(jl_uint32 chip_id, jl_led_group_t group, jl_led_config_t *pconfig);

/**
 * @brief Set LED blink rate in millisecond
 * @details This function can be used to set the LED blink rate of all the LED groups.
 * @param[in]  chip_id   Chip ID
 * @param[in]	blink_rate	LED blink rate
 * @return Set LED blink rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_led_blink_rate_set(jl_uint32 chip_id, jl_led_blink_rate_t blink_rate);

/**
 * @brief Get LED blink rate in millisecond
 * @details This function can be used to get the blink rate of all the LED groups.
 * @param[in]  chip_id   Chip ID
 * @param[out]	pblink_rate	LED blink rate
 * @return Get LED blink rate successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_led_blink_rate_get(jl_uint32 chip_id, jl_led_blink_rate_t *pblink_rate);

/**
 * @brief Set LED polarity as active high
 * @details This function can be used to set the specified LED group of the specified portmask as
 * active high triggered.\n Portmask indicates an entry of 10-bit correspond to 10 ports.
 * If set to 1, it means the specified led of this port will be active high triggered.
 * If set to 0, it means the specified led of this port will be active low triggered.
 * @param[in]   chip_id   Chip ID
 * @param[in]	group		LED group index
 * @param[in]	portmask	PHY portmask
 * @return Set LED active high successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_led_group_active_high_set(jl_uint32 chip_id, jl_led_group_t group, jl_portmask_t portmask);

/**
 * @brief Get LED active high ports
 * @details This function can be used to get the active high triggered ports of the specified LED group.
 * @param[in]  chip_id   Chip ID
 * @param[in]	group		LED group index
 * @param[out]	pportmask	PHY portmask
 * @return Get LED active high successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 */
JL_API jl_api_ret_t jl_led_group_active_high_get(jl_uint32 chip_id, jl_led_group_t group, jl_portmask_t *pportmask);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_LED_H__*/
/** @} LED APIs*/
