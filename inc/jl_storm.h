/**
 * @file jl_storm.h
 * @brief Multicast Broadcast Storm Control reference
 */

/**
 * @defgroup storm_group Storm
 * @{
 */

/**
 * @example example_storm.c
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

#ifndef __JL_STORM_H__
#define __JL_STORM_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum JL_STORM_TYPE_E
 * @brief storm ctrl frame type.
 */
/**
 * @public typedef jl_storm_type_t
 */
	typedef enum JL_STORM_TYPE_E {
		JL_STORM_TYPE_BROADCAST, /**< 0 */
		JL_STORM_TYPE_MULTICAST, /**< 1 */
		JL_STORM_TYPE_FLOODING, /**< 2 */
		JL_STORM_TYPE_END, /**< 3 */
	} jl_storm_type_t;

/**
 * @enum JL_STORM_RATE_MODE_E
 * @brief storm ctrl rate mode.
 */
/**
 * @public typedef jl_storm_rate_mode_t
 */
	typedef enum JL_STORM_RATE_MODE_E {
		JL_STORM_RATE_MODE_KBPS, /**< 0 */
		JL_STORM_RATE_MODE_PPS, /**< 1 */
		JL_STORM_RATE_MODE_END, /**< 2 */
	} jl_storm_rate_mode_t;

/**
 * @brief Set storm control enable configuration
 * @details This function can be used to enable storm control per egress port per traffic type.
 * @param[in]	chip_id     The switch support chip id
 * @param[in]	egress_port	The specified egress port
 * @param[in]	type		The traffic type to be set
 * @param[in]	enable		The enable status to be set
 * @return Set storm control enable successfully or not
 * @retval JL_ERR_OK		Ok
 * @retval Other	Fail
 */
JL_API jl_api_ret_t jl_storm_control_enable_set(const jl_uint32 chip_id,
						const jl_port_t egress_port,
						const jl_storm_type_t type, const jl_bool enable);

/**
 * @brief Get storm control enable configuration
 * @param[in]	chip_id     The switch support chip id
 * @details This function can be used to get the specified traffic type enable status of the specified egress port
 * @param[in]	egress_port	The specified egress port
 * @param[in]	type		The traffic type
 * @param[out]	penable		The returned enable status
 * @return Get storm control enable successfully or not
 * @retval JL_ERR_OK		Ok
 * @retval Other	Fail
 */
JL_API jl_api_ret_t jl_storm_control_enable_get(const jl_uint32 chip_id,
						const jl_port_t egress_port,
						const jl_storm_type_t type, jl_bool *penable);

/**
 * @brief Set storm control rate configuration
 * @details This function can set storm control rate for each egress port each traffic type.
 * - Support two mode: Kbps/pps.
 * - The IFG is included by default 0x18.
 * @param[in]	chip_id     The switch support chip id
 * @param[in]	egress_port	The specified egress port
 * @param[in]	type		The traffic type to be set
 * @param[in]	mode		The rate mode to be set
 * @param[out]	rate		Storm control rate
 * @return Set storm control rate successfully or not
 * @retval JL_ERR_OK		Ok
 * @retval Other	Fail
 */
JL_API jl_api_ret_t jl_storm_control_rate_set(const jl_uint32 chip_id,
					      const jl_port_t egress_port,
					      const jl_storm_type_t type,
					      const jl_storm_rate_mode_t mode, const jl_uint32 rate);

/**
 * @brief Get storm control rate configuration
 * @details This function can get storm control rate for each egress port each traffic type.
 * - Support two mode: Kbps/pps.\n
 * - The IFG is included by default 0x18.
 * @param[in]	chip_id     The switch support chip id
 * @param[in]	egress_port	The specified egress port
 * @param[in]	type		The traffic type to be set
 * @param[out]	pmode		The returned rate mode
 * @param[out]	prate		The returned storm control rate
 * @return Get storm control rate successfully or not
 * @retval JL_ERR_OK		Ok
 * @retval Other	Fail
 */
JL_API jl_api_ret_t jl_storm_control_rate_get(const jl_uint32 chip_id,
					      const jl_port_t egress_port,
					      const jl_storm_type_t type,
					      jl_storm_rate_mode_t *pmode, jl_uint32 *prate);

#ifdef __cplusplus
}
#endif
#endif				/* __JL_TEST_H__ */
/** @} Storm APIs*/
