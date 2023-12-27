/**
 * @file jl_stp.h
 * @brief Spanning Tree Protocol
 */

/**
 * @defgroup spanning_tree_protocol_group Spanning Tree Protocol
 * @{
 */

/**
 * @example example_stp.c
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

#ifndef __JL_API_STP_H__
#define __JL_API_STP_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum jl_stp_state_e
 * @brief Stp state.
 */
/**
 * @public typedef jl_stp_state_t
 */
typedef enum jl_stp_state_e {
	STP_STATE_DISABLED = 0,/**< 0 */
	STP_STATE_BLOCKING,/**< 1 */
	STP_STATE_LISTENING,/**< 2 */
	STP_STATE_LEARNING,/**< 3 */
	STP_STATE_FORWARDING,/**< 4 */
	STP_STATE_END,/**< Invalid */
} jl_stp_state_t;

/**
 * @public typedef jl_mstp_msti_t
 */
typedef jl_uint32 jl_mstp_msti_t;

/**
 * @enum jl_mstp_state_e
 * @brief Mstp state.
 */
/**
 * @public typedef jl_mstp_state_t
 */
typedef enum jl_mstp_state_e {
	MSTP_STATE_FORWARDING,/**< 0 */
	MSTP_STATE_DISCARDING,/**< 1 */
	MSTP_STATE_LEARNING,/**< 2 */
	MSTP_STATE_END,/**< Invalid */
} jl_mstp_state_t;

/**
 * @brief Configure ingress spanning tree state for the specified port
 * @param[in]	chip_id		Chip Id
 * @param[in]	port		Port id
 * @param[in]	stp_state	Ingress spanning tree state
 * @return Set ingress stp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports per-port ingress spanning tree state.\n
 *		There are five states supported by ASIC.
 *		- STP_STATE_DISABLED
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 *		- STP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_stp_port_ingress_state_set(jl_uint32 chip_id, jl_port_t port, jl_stp_state_t stp_state);

/**
 * @brief Get ingress spanning tree state for the specified port
 * @param[in]	chip_id		Chip Id
 * @param[in]	port		Port id
 * @param[out]	pstp_state	The returned ingress spanning tree state
 * @return Get ingress stp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports per-port ingress spanning tree state.\n
 *		There are five states supported by ASIC.
 *		- STP_STATE_DISABLED
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 *		- STP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_stp_port_ingress_state_get(jl_uint32 chip_id, jl_port_t port, jl_stp_state_t *pstp_state);

/**
 * @brief Configure egress spanning tree state for the specified port
 * @param[in]	chip_id		Chip Id
 * @param[in]	port		Port id
 * @param[in]	stp_state	Egress spanning tree state
 * @return Set egress stp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports per-port egress spanning tree state.\n
 *		There are five states supported by ASIC.
 *		- STP_STATE_DISABLED
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 *		- STP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_stp_port_egress_state_set(jl_uint32 chip_id, jl_port_t port, jl_stp_state_t stp_state);

/**
 * @brief Get egress spanning tree state for the specified port
 * @param[in]	chip_id		Chip Id
 * @param[in]	port		Port id
 * @param[out]	pstp_state	Egress spanning tree state
 * @return Get egress stp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports per-port egress spanning tree state.\n
 *		There are five states supported by ASIC.
 *		- STP_STATE_DISABLED
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 *		- STP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_stp_port_egress_state_get(jl_uint32 chip_id, jl_port_t port, jl_stp_state_t *pstp_state);

/**
 * @brief Get drop counters due to the specified ingress stp state
 * @param[in]	chip_id		Chip Id
 * @param[in] stp_state Ingress spanning tree state
 * @param[out] pcntr Drop counters
 * @return Get ingress stp drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * @note
 *		Drop counter will be calculated per ingress spanning tree state not per port.\n
 *		Packets will be dropped when stp state is set as the following three states:
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 * @note
 *		While the ingress spanning tree state is set as the following two states,
 *		packets will be forwarded as normal.
 *		- STP_STATE_FORWARDING
 *		- STP_STATE_DISABLED
 */
JL_API jl_api_ret_t jl_stp_ingress_state_drop_get(jl_uint32 chip_id, jl_stp_state_t stp_state, jl_uint32 *pcntr);

/**
 * @brief Clear drop counters due to the specified ingress stp state
 * @param[in]	chip_id		Chip Id
 * @param[in] stp_state Ingress spanning tree state
 * @return Clear ingress stp drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * @note
 *		Drop counters will be calculated per ingress spanning tree state not per port.\n
 *		Packets will be dropped when stp state is set as the following three states:
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 * @note
 *		While the ingress spanning tree state is set as the following two states,
 *		packets will be forwarded as normal.
 *		- STP_STATE_FORWARDING
 *		- STP_STATE_DISABLED
 */
JL_API jl_api_ret_t jl_stp_ingress_state_drop_clear(jl_uint32 chip_id, jl_stp_state_t stp_state);

/**
 * @brief Get drop counters of the specified port due to egress spanning tree state.
 * @param[in]	chip_id		Chip Id
 * @param[in] port Port id
 * @param[out] pcntr Drop counter
 * @return Get egress stp drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * note The dropped counters are caculated based on the following three states:
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 */
JL_API jl_api_ret_t jl_stp_port_egress_drop_get(jl_uint32 chip_id, jl_port_t port, jl_uint32 *pcntr);

/**
 * @brief Clear drop counters of the specified port due to egress spanning tree state.
 * @param[in]	chip_id		Chip Id
 * @param[in] port Port id
 * @return Clear egress stp drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * note The dropped counters are caculated based on the following three states:
 *		- STP_STATE_BLOCKING
 *		- STP_STATE_LISTENING
 *		- STP_STATE_LEARNING
 */
JL_API jl_api_ret_t jl_stp_port_egress_drop_clear(jl_uint32 chip_id, jl_port_t port);

/**
 * @brief Configure ingress multiple spanning tree state of the specified port for the msti.
 * @param[in]	chip_id		Chip Id
 * @param[in]	msti		Multiple spanning tree instance, range:[0-15]
 * @param[in]	port		Port id
 * @param[in]	mstp_state	Ingress multiple spanning tree state
 * @return Set ingress mstp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports ingress per-port multiple spanning tree state for each msti.\n
 *		There are three states supported by ASIC.
 *		- MSTP_STATE_FORWARDING
 *		- MSTP_STATE_DISCARDING
 *		- MSTP_STATE_LEARNING
 */
JL_API jl_api_ret_t jl_mstp_port_ingress_state_set(jl_uint32 chip_id, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state);

/**
 * @brief Get ingress multiple spanning tree state of the specified port for the msti.
 * @param[in]	chip_id		Chip Id
 * @param[in]	msti		Multiple spanning tree instance,range:[0-15]
 * @param[in]	port		Port id
 * @param[out]	pmstp_state	The returned ingress multiple spanning tree state
 * @return Get ingress mstp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports ingress per-port multiple spanning tree state for each msti.\n
 *		There are three states supported by ASIC.
 *		- MSTP_STATE_FORWARDING
 *		- MSTP_STATE_DISCARDING
 *		- MSTP_STATE_LEARNING
 */
JL_API jl_api_ret_t jl_mstp_port_ingress_state_get(jl_uint32 chip_id, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state);

/**
 * @brief Configure egress multiple spanning tree state of the specified port for the msti.
 * @param[in]	chip_id		Chip Id
 * @param[in]	msti		Multiple spanning tree instance
 * @param[in]	port		Port id
 * @param[in]	mstp_state	Egress multiple spanning tree state
 * @return Set egress mstp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports egress per-port multiple spanning tree state for each msti
 *		There are three states supported by ASIC.
 *		- MSTP_STATE_FORWARDING
 *		- MSTP_STATE_DISCARDING
 *		- MSTP_STATE_LEARNING
 */
JL_API jl_api_ret_t jl_mstp_port_egress_state_set(jl_uint32 chip_id, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t mstp_state);

/**
 * @brief Get egress multiple spanning tree state of the specified port for the msti.
 * @param[in]	chip_id		Chip Id
 * @param[in]	msti		Multiple spanning tree instance
 * @param[in]	port		Port id
 * @param[out]	pmstp_state	Egress multiple spanning tree state
 * @return Get egress mstp state successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *		System supports egress per-port multiple spanning tree state for each msti.\n
 *		There are three states supported by ASIC.
 *		- MSTP_STATE_FORWARDING
 *		- MSTP_STATE_DISCARDING
 *		- MSTP_STATE_LEARNING
 */
JL_API jl_api_ret_t jl_mstp_port_egress_state_get(jl_uint32 chip_id, jl_mstp_msti_t msti, jl_port_t port, jl_mstp_state_t *pmstp_state);

/**
 * @brief Get drop counter due to the specified ingress mstp state
 * @param[in]	chip_id		Chip Id
 * @param[in]	mstp_state	Ingress multiple spanning tree state
 * @param[out]	pcntr		Drop counters
 * @return Get ingress mstp drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * @note
 *		Drop counters will be calculated per ingress multiple spanning tree state not per port.\n
 *		Packets will be dropped when mstp state is set as the following two states:
 *		- MSTP_STATE_DISCARDING
 *		- MSTP_STATE_LEARNING
 * @note
 *		While the ingress multiple spanning tree state is set as the following one state,
 *		packets will be forwarded as normal.
 *		- MSTP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_mstp_ingress_state_drop_get(jl_uint32 chip_id, jl_mstp_state_t mstp_state, jl_uint32 *pcntr);

/**
 * @brief Clear drop counters due to the specified ingress mstp state
 * @param[in]	chip_id		Chip Id
 * @param[in]	mstp_state	Ingress multiple spanning tree state
 * @return Clear ingress mstp drop counter successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * @note
 *		Drop counters will be calculated per ingress multiple spanning tree state not per port.\n
 *		Packets will be dropped when mstp state is set as the following two states:
 *		- MSTP_STATE_DISCARDING
 *		- MSTP_STATE_LEARNING
 * @note
 *		While the ingress multiple spanning tree state is set as the following one state,
 *		packets will be forwarded as normal.
 *		- MSTP_STATE_FORWARDING
 */
JL_API jl_api_ret_t jl_mstp_ingress_state_drop_clear(jl_uint32 chip_id, jl_mstp_state_t mstp_state);

/**
 * @brief Get drop counters of the specified port.
 * @param[in]	chip_id		Chip Id
 * @param[in]	port	port id
 * @param[out]	pcntr	Drop counters
 * @return Get egress mstp drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * @note	The dropped counters are caculated based on the following two states:
 *		- MSTP_STATE_DISCARDING
 *		- MSTP_STATE_LEARNING
 */
JL_API jl_api_ret_t jl_mstp_port_egress_drop_get(jl_uint32 chip_id, jl_port_t port, jl_uint32 *pcntr);

/**
 * @brief Clear drop counters of the specified port due to egress multiple spanning tree state.
 * @param[in]	chip_id		Chip Id
 * @param[in]	port	Port id
 * @return Clear egress mstp drop counters successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 * @note	The dropped counters are caculated based on the following two states:
 *		- MSTP_STATE_DISCARDING
 *		- MSTP_STATE_LEARNING
 */
JL_API jl_api_ret_t jl_mstp_port_egress_drop_clear(jl_uint32 chip_id, jl_port_t port);

/**
 * @brief Configure multiple spanning tree mapping between vid and msti
 * @param[in]	chip_id		Chip Id
 * @param[in]	vid		Vlan id
 * @param[in]	msti		Multiple spanning tree instance
 * @return Set mstp mapping between vid and msti successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mstp_vlan_msti_set(jl_uint32 chip_id, jl_uint16 vid, jl_mstp_msti_t msti);

/**
 * @brief Get multiple spanning tree msti from vlan id
 * @param[in]	chip_id		Chip Id
 * @param[in]	vid		Vlan id
 * @param[out]	pmsti		Pointer of multiple spanning tree instance
 * @return Get mstp msti from vlan id successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_mstp_vlan_msti_get(jl_uint32 chip_id, jl_uint16 vid, jl_mstp_msti_t *pmsti);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_STP_H__*/
/** @} Spanning Tree Protocol APIs*/

