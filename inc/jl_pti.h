/**
 * @file jl_pti.h
 * @brief Port Isolation
 */

/**
 * @defgroup port_isolation_group Port Isolation
 * @{
 */

/**
 * @example example_pti.c
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

#ifndef __JL_PTI_H__
#define __JL_PTI_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum jl_pti_group_e
 * @brief Port isolation group.
 */
/**
 * @public typedef jl_pti_group_t
 */
typedef enum jl_pti_group_e {
	PTI_GROUP_0,/**< 0 */
	PTI_GROUP_1,/**< 1 */
	PTI_GROUP_2,/**< 2 */
	PTI_GROUP_3,/**< 3 */
	PTI_GROUP_4,/**< 4 */
	PTI_GROUP_5,/**< 5 */
	PTI_GROUP_6,/**< 6 */
	PTI_GROUP_7,/**< 7 */
	PTI_GROUP_8,/**< 8 */
	PTI_GROUP_9,/**< 9 */
	PTI_GROUP_10,/**< 10 */
	PTI_GROUP_END/**< Invalid */
} jl_pti_group_t;

/**
 * @brief Set the prevented portmask for the dest port
 * @details This function sets the port members which can't transimit packets to the specified dest port.
 * @param[in] chip_id		Chip Id
 * @param[in]	dstport		Index of dest port number
 * @param[in]	portmask	The prevented portmask
 * @return Set dest port and the prevented portmask successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_isolation_dst_set(jl_uint32 chip_id, jl_port_t dstport, jl_portmask_t portmask);

/**
 * @brief Get the prevented portmask for the dest port
 * @details This function gets the port members which can't transimit packets to the specified dest port.
 * @param[in] chip_id		Chip Id
 * @param[in]	dstport		Index of dest port number
 * @param[out]	pportmask	The pointer of the prevented portmask
 * @return Get the prevented portmask successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_isolation_dst_get(jl_uint32 chip_id, jl_port_t dstport, jl_portmask_t *pportmask);

/**
 * @brief Set the prevented portmask for the src port
 * @details This function sets the port members which can't receive packets from the specified src port.
 * @param[in] chip_id		Chip Id
 * @param[in]	srcport		Index of src port number
 * @param[in]	portmask	The prevented portmask
 * @return Set src port and the prevented portmask successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_isolation_src_set(jl_uint32 chip_id, jl_port_t srcport, jl_portmask_t portmask);

/**
 * @brief Get the prevented portmask for the src port
 * @details This function gets the port members which can't receive packets from the specified src port.
 * @param[in] chip_id		Chip Id
 * @param[in]	srcport		Index of src port number
 * @param[out]	pportmask	The pointer of the prevented portmask
 * @return Get the prevented portmask successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_isolation_src_get(jl_uint32 chip_id, jl_port_t srcport, jl_portmask_t *pportmask);

/**
 * @brief Set the prevented portmask for the specified port isolation group.
 * @details This function sets the prevented portmask for the port isolation group,
 * the ports members in the portmask can't transmit packets to each other.
 * @param[in] chip_id		Chip Id
 * @param[in]	group_id	The port isolation group id
 * @param[in]	portmask	The prevented portmask
 * @return Set the port isolation group id and the prevented portmask successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_isolation_group_set(jl_uint32 chip_id, jl_pti_group_t group_id, jl_portmask_t portmask);

/**
 * @brief Get the prevented portmask for the specified port isolation group.
 * @details This function gets the prevented portmask for the port isolation group,
 * the ports members in the portmask can't transmit packets to each other.
 * @param[in] chip_id		Chip Id
 * @param[in]	group_id	The port isolation group id
 * @param[out]	pportmask	The prevented portmask
 * @return Get the prevented portmask from the port isolation group successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_isolation_group_get(jl_uint32 chip_id, jl_pti_group_t group_id, jl_portmask_t *pportmask);

/**
 * @brief Clear the prevented portmask for the port isolation group.
 * @details This function clears the prevented portmask for the port isolation group.
 * @param[in] chip_id		Chip Id
 * @param[in]	group_id	The port isolation group id
 * @return Clear the prevented portmask for the port isolation group successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_isolation_group_clear(jl_uint32 chip_id, jl_pti_group_t group_id);

#ifdef __cplusplus
}
#endif

#endif /* __JL_PTI_H__ */
/** @} Port Isolation APIs*/

