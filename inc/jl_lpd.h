/**
 * @file jl_lpd.h
 * @brief Loop Detection
 */

/**
 * @defgroup loop_detection_group Loop Detection
 * @{
 */

/**
 * @example example_lpd.c
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

#ifndef __JL_API_LPD_H__
#define __JL_API_LPD_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct jl_lpd_config_s
 * @brief Loop detection configuration.
 * @note "src_mac" should be distinct for each switch using JLSemi Loop Detection Function \n
 * "eth_type" "op_code" should be set to the same value for switches using JLSemi Loop Detection Function
 */
/**
 * @public typedef jl_lpd_config_t
 */
typedef struct jl_lpd_config_s {
	jl_uint16 eth_type; /**< ethernet type (a given value) */
	jl_uint16 ttl; /**< loop detection frame ttl:0001h is minimum value, 0000h is maximum(to avoid broadcast storm) */
	jl_uint16 timescale; /**< loop frame insert timescale, 2’b00:us 2’b01:ms 2’b10:s. default:2s */
	jl_uint16 trig_time; /**< loop frame insert time value at timescale. default:2s */
	jl_mac_addr_t src_mac; /**< loop frame src mac address (a random address) */
} jl_lpd_config_t;

/**
 * @enum jl_lpd_port_status_e
 * @brief Port loop detect status.
 */
/**
 * @public typedef jl_lpd_port_status_t
 */
typedef enum jl_lpd_port_status_e {
	PORT_NO_LOOP_STATUS = 0,/**< 0 */
	PORT_LOOP_STATUS,/**< 1 */
	PORT_LOOP_STATUS_END/**< Invalid */
} jl_lpd_port_status_t;

/**
 * @brief Get "Loop Detection" configuration
 * @param[in]	chip_id		Chip Id
 * @param[out] lpd_config	Loop detection configuration
 * @return Get loop detection configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_lpd_config_get(jl_uint32 chip_id, jl_lpd_config_t *lpd_config);

/**
 * @brief Set "Loop Detection" configuration
 * @param[in]	chip_id		Chip Id
 * @param[in]	lpd_config	Loop detection configuration
 * @return	Set loop detection configuration successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_lpd_config_set(jl_uint32 chip_id, jl_lpd_config_t *lpd_config);

/**
 * @brief Enable/Disable "Loop Detection"
 * @param[in]	chip_id		Chip Id
 * @param[in]	enable		Enable/Disable the loop detection function
 * @return	Set the loop detection function successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_lpd_enable_set(jl_uint32 chip_id, jl_enable_t enable);

/**
 * @brief Get "Loop Detection"status
 * @param[in]	chip_id		Chip Id
 * @param[out]	penable		Status of the loop detection function
 * @return	Get the loop detection function successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_lpd_enable_get(jl_uint32 chip_id, jl_enable_t *penable);

/**
 * @brief Set status Loop Detection of the specified port
 * @param[in]	chip_id		Chip Id
 * @param[in]	port		Port index
 * @param[in]	enable		Enable/Disable port supports loop detection
 * @return	Set whether the specified port supports loop detection successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_lpd_port_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable);

/**
 * @brief get status Loop Detection of the specified port
 * @param[in]	chip_id		Chip Id
 * @param[in]	port		Port index
 * @param[out]	penable		Check whether port supports loop detection
 * @return	Get whether the specified port supports loop detection successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_lpd_port_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable);

/**
 * @brief Get port "Loop Detection" status
 * @param[in]	chip_id		Chip Id
 * @param[in]	port		Port index
 * @param[out]	pstatus		Get loop detection current status of the each port
 * @return	Get the current loop detection status of the specified port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_lpd_port_status_get(jl_uint32 chip_id, jl_port_t port, jl_lpd_port_status_t *pstatus);

/**
 * @brief Get portmask "Loop Detection" history status
 * @param[in]	chip_id		Chip Id
 * @param[out]	pmaskstatus	Loop detection history status of the portmask
 * @return	Get loop detection history status of the portmask successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_lpd_status_history_get(jl_uint32 chip_id, jl_portmask_t *pmaskstatus);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_LPD */
/** @} Loop Detection APIs*/

