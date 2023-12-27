/**
 * @file jl_fc.h
 * @brief Flow Control
 */

/**
 * @defgroup fc_group Flow Control
 * @{
 */

/**
 * @example example_fc.c
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


#ifndef __JL_API_FC_H__
#define __JL_API_FC_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Set port flow control to force mode
 * @details  This function can enable or disable flow control in the specified port to force mode.
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Port ID
 * @param[in]  enable    Enable or disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note   Users can use this api to set flow control to force mode.\n
 *		   If set enable, flow control enters force mode.
 *       Then using \ref jl_port_fc_force_enable_set() to turn on/off flow control function.\n
 *         If set disable, flow control is restored to automatic mode.
 *       Then whether flow control is enabled depends on the phy Auto-negotiation result.
 */
JL_API jl_ret_t jl_port_fc_force_mode_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable);

/**
 * @brief Get port flow control force mode status
 * @details  This function can get flow control force mode status in the specified port.
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Port ID
 * @param[out]  penable   Enabled status
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_port_fc_force_mode_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable);

/**
 * @brief Enable or disable port flow control in force mode
 * @details  This function enables or disables forced flow control for a specified port.
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Port ID
 * @param[in]  enable    Enable or disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note Using \ref jl_port_fc_force_mode_set() to enter force mode firstly, then using this api to turn on/off flow control function.
 */
JL_API jl_ret_t jl_port_fc_force_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable);

/**
 * @brief Get port flow control status in force mode
 * @details  This function can get forced flow control status for the specified port.
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Port ID
 * @param[out]  penable  Enabled status
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_port_fc_force_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable);

/**
 * @brief Set port flow control threshold
 * @details  This function can set flow control threshold in the specified port.
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Port ID
 * @param[in]  threshold_on   On threshold for the number of used free-for-all cells for this port,range[0,0x3ff]
 * @param[in]  threshold_off  Off threshold for the number of used free-for-all cells for this port,range[0,0x3ff]
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 * @note  The value of threshold_off should be greater than the value of threshold_on
 */
JL_API jl_ret_t jl_port_fc_threshold_set(jl_uint32 chip_id, jl_port_t port,
	jl_uint32 threshold_on, jl_uint32 threshold_off);

/**
 * @brief Get port flow control threshold
 * @details  This function can get flow control threshold in the specified port.
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Port ID
 * @param[out]  pthreshold_on   On threshold for this port
 * @param[out]  pthreshold_off  Off threshold for this port
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_port_fc_threshold_get(jl_uint32 chip_id, jl_port_t port,
	jl_uint32 *pthreshold_on, jl_uint32 *pthreshold_off);

/**
 * @brief Enable or disable tail drop of the specified port
 * @param[in]  chip_id   Chip ID
 * @param[in]  phyport  Port number
 * @param[in]  enable  1: enable tail drop; 0: disable tail drop
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_taildrop_enable_set(jl_uint32 chip_id, jl_port_t phyport, jl_enable_t enable);

/**
 * @brief Get tail drop enabled status of the specified port
 * @param[in]   chip_id   Chip ID
 * @param[in]   phyport  Port number
 * @param[out]  penable  The return enabled status, 1: enable tail drop; 0: disable tail drop
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_taildrop_enable_get(jl_uint32 chip_id, jl_port_t phyport, jl_enable_t *penable);

/**
 * @brief Get the drop counters due to taildrop
 * @param[in]  chip_id   Chip ID
 * @param[out]  pcntr  Drop counter
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval minus_value		Fail
 */
JL_API jl_ret_t jl_taildrop_counter_get(jl_uint32 chip_id, jl_uint32 *pcntr);


#ifdef __cplusplus
}
#endif

#endif /* __JL_API_FC_H__*/
/** @} Flow Control APIs*/

