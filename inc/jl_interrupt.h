/**
 * @file jl_interrupt.h
 * @brief interrupt
 */

/**
 * @defgroup interrupt_group Interrupt
 * @{
 */

/**
 * @example example_interrupt.c
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

#ifndef __JL_API_INT_H__
#define __JL_API_INT_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @struct jl_int_status_s
 * @brief interrupt status bit mask
 */
/**
 * @public typedef jl_int_status_t
 */
typedef struct jl_int_status_s {
	jl_uint32 mask;  /**< bitmap of jl_int_type_t*/
} jl_int_status_t;

/**
 * @struct jl_int_status_info_s
 * @brief interrupt status mask
 */
/**
 * @public typedef jl_int_status_info_t
 */
typedef struct jl_int_status_info_s {
	jl_portmask_t port_mask;
} jl_int_info_t;

/**
 * @enum jl_int_polarity_e
 * @brief interrupt polarity type.
 */
/**
 *@public typedef jl_int_polarity_t
 */
typedef enum jl_int_polarity_e {
    INT_POLAR_HIGH = 0,
    INT_POLAR_LOW,
    INT_POLAR_END
} jl_int_polarity_t;

/**
 * @enum jl_int_type_e
 * @brief interrupt type.
 */
/**
 *@public typedef jl_int_type_t
 */
typedef enum jl_int_type_e {
    INT_PORT_LINKDOWN = 0,    /**< trigger if port changed to link down at the mac layer*/
    INT_PORT_LINKUP,      /**< trigger if port changed to link up at the mac layer*/
    INT_TYPE_LOOP_DETECT, /**< trigger if loop detected*/
    INT_TYPE_END
} jl_int_type_t;

/**
 * @brief Set interrupt polarity configuration.
 * @param[in]  chip_id   Chip ID
 * @param[in]  polarity  Interrupt polarity type
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		 The API can set interrupt polarity configuration.
 */
jl_ret_t jl_interrupt_polarity_set(jl_uint32 chip_id, jl_int_polarity_t polarity);

/**
 * @brief Get interrupt polarity configuration.
 * @param[in]  chip_id   Chip ID
 * @param[out]  ppolarity  Interrupt polarity type
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		 The API can get interrupt polarity configuration.
 */
jl_ret_t jl_interrupt_polarity_get(jl_uint32 chip_id, jl_int_polarity_t *ppolarity);

/**
 * @brief Set interrupt status configuration on specified phy port
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Index of port number
 * @param[in]  enable   1:enable;0:disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		 The API can enable or disable the interrupt status configuration on specified phy port.
 */
jl_ret_t jl_interrupt_port_phy_int_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable);

/**
 * @brief Get interrupt status configuration on specified phy port
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Index of port number
 * @param[out]  penable  Enable status
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		The API can get interrupt status configuration on specified phy port.
 */
jl_ret_t jl_interrupt_port_phy_int_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable);

/**
 * @brief Get interrupt trigger status on specified phy port
 * @param[in]  chip_id   Chip ID
 * @param[in]  port      Index of port number
 * @param[out]  pstatus  Interrupt status
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		The API can get interrupt trigger status when the specified phy port link status happened.
 *      The status will be cleared after execute this API.
 */
jl_ret_t jl_interrupt_port_phy_int_status_get(jl_uint32 chip_id, jl_port_t port, jl_uint32 *pstatus);

/**
 * @brief Set interrupt trigger status configuration.
 * @param[in]  chip_id   Chip ID
 * @param[in]  type     Interrupt trigger type
 * @param[in]  enable   1:enable;0:disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *      The API can set enable or disable the specified interrupt type for all ports.
 */
jl_ret_t jl_interrupt_enable_set(jl_uint32 chip_id, jl_int_type_t type, jl_enable_t enable);

/**
 * @brief Get interrupt trigger status configuration.
 * @param[in]  chip_id   Chip ID
 * @param[in]  type      Interrupt trigger type
 * @param[out]  penable  Enable status
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *      The API can get interrupt status configuration.
 */
jl_ret_t jl_interrupt_enable_get(jl_uint32 chip_id, jl_int_type_t type, jl_enable_t *penable);

/**
 * @brief Set interrupt trigger status to clean
 * @param[in]  chip_id   Chip ID
 * @param[in]  type   Interrupt trigger type
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *      The API can clean the specified interrupt status after interrupt happened.
 *      The status will be cleared after execute this API.
 */
jl_ret_t jl_interrupt_status_clean(jl_uint32 chip_id, jl_int_type_t type);

/**
 * @brief Get interrupt trigger status
 * @param[in]  chip_id   Chip ID
 * @param[out]  pstatusmask   Interrupt status bit mask
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *      The API can get interrupt trigger status when interrupt happened on any port.
 *      Out para statusmask bit0 means type INT_PORT_LINKDOWN;
 *                 bit1 means type INT_PORT_LINKUP;
 *                 bit2 means type INT_TYPE_LOOP_DETECT.
 *      The status will not be cleared after execute this API.
 */
jl_ret_t jl_interrupt_status_get(jl_uint32 chip_id, jl_int_status_t *pstatusmask);

/**
 * @brief Get detail interrupt trigger status
 * @param[in]  chip_id   Chip ID
 * @param[in]  type   Interrupt trigger type
 * @param[out] pinfo   Interrupt status info
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *      The API can get detail interrupt status when the specified type interrupt happened.
 *      The status will not be cleared after execute this API.
 */
jl_ret_t jl_interrupt_detail_get(jl_uint32 chip_id, jl_int_type_t type, jl_int_info_t *pinfo);

#ifdef __cplusplus
}
#endif
#endif				/* __JL_API_INT_H__ */
/** @} INT APIs*/
