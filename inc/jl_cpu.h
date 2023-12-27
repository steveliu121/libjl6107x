/**
 * @file jl_cpu.h
 * @brief cpu
 */

/**
 * @defgroup cpu_group CPU
 * @{
 */

/**
 * @example example_cpu.c
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

#ifndef __JL_API_CPU_H__
#define __JL_API_CPU_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define MAX_RMA_NUM   (0x2F) /**< MAX_RMA_NUM */

/**
 * @enum jl_rma_action_e
 * @brief Rma action value.
 */
enum jl_rma_action_e {
	RMA_ACTION_FORWARD = 0, /**< 0 */
	RMA_ACTION_SEND2CPU,/**< 1 */
	RMA_ACTION_DROP,/**< 2 */
	RMA_ACTION_END/**< Invalid */
};

/**
 * @public typedef jl_led_active_t
 */
typedef enum jl_rma_action_e jl_rma_action_t;


/**
 * @brief Set allow bpdu packets send to CPU port or not
 * @param[in]  chip_id   Chip ID
 * @param[in]  enable   1:enable;0:disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to set allow bpdu packets send to CPU port or not.\n
 *		If set 1, then the packets that have the destination MAC address equal to
 *		01:80:C2:00:00:00 are sent to the CPU port. Otherwise, the packets
 *		that have the destination MAC address equal to 01:80:C2:00:00:00 are
 *		broadcast to all the ports.
 */
JL_API jl_api_ret_t jl_cpu_allow_bpdu_enable_set(jl_uint32 chip_id, jl_enable_t enable);

/**
 * @brief Get allow bpdu packets send to CPU port is enabled or not
 * @param[in]  chip_id   Chip ID
 * @param[out]  penable  Enable status
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to get allow bpdu packets send to CPU port or not.\n
 *		If get 1, then the packets that have the destination MAC address equal to
 *		01:80:C2:00:00:00 are sent to the CPU port. Otherwise, the packets
 *		that have the destination MAC address equal to 01:80:C2:00:00:00 are
 *		broadcast to all the ports.
 */
JL_API jl_api_ret_t jl_cpu_allow_bpdu_enable_get(jl_uint32 chip_id, jl_enable_t *penable);

/**
 * @brief Set allow rst-bpdu packets send to CPU port or not
 * @param[in]  chip_id   Chip ID
 * @param[in]  enable    1:enable;0:disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to set allow rst bpdu packets send to CPU port or not.\n
 *		If set 1, then packets that have the destination MAC address equal to
 *		01:00:0C:CC:CC:CD are sent to the CPU port.
 */
JL_API jl_api_ret_t jl_cpu_allow_rst_bpdu_enable_set(jl_uint32 chip_id, jl_enable_t enable);

/**
 * @brief Get allow rst-bpdu packets send to CPU port or not
 * @param[in]  chip_id   Chip ID
 * @param[out]  penable  Enable status
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to get allow rst bpdu packets send to CPU port or not.\n
 *		If get 1, then packets that have the destination MAC address equal to
 *		01:00:0C:CC:CC:CD are sent to the CPU port.
 */
JL_API jl_api_ret_t jl_cpu_allow_rst_bpdu_enable_get(jl_uint32 chip_id, jl_enable_t *penable);

/**
 * @brief Enable or disable To CPU Port Tag
 * @param[in]  chip_id   Chip ID
 * @param[in]  enable   1:enable;0:disable
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to enable the To CPU Port Tag.
 */
JL_API jl_api_ret_t jl_cpu_tag_enable_set(jl_uint32 chip_id, jl_enable_t enable);

/**
 * @brief Get To CPU Port Tag is enabled or not
 * @param[in]  chip_id   Chip ID
 * @param[out] penable   Enable status
 * @return Get successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can be used to get the To CPU Port Tag is enabled or not.
 */
JL_API jl_api_ret_t jl_cpu_tag_enable_get(jl_uint32 chip_id, jl_enable_t *penable);

/**
 * @brief Set allow lldp packets send to CPU port or not
 * @param[in]  chip_id   Chip ID
 * @param[in]  enable   1:send to cpu; 0:Transparent
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can allow lldp packets To CPU or Transparent.\n
 *		LLDP is as following definition:
 *		- DMAC = 01:80:c2:00:00:0e ethertype = 0x88CC
 *		- DMAC = 01:80:c2:00:00:03 ethertype = 0x88CC
 *		- DMAC = 01:80:c2:00:00:00 ethertype = 0x88CC
 */
JL_API jl_api_ret_t jl_cpu_allow_lldp_enable_set(jl_uint32 chip_id, jl_enable_t enable);

/**
 * @brief Get lldp packets send to CPU port is enabled or not
 * @param[in]  chip_id   Chip ID
 * @param[out]  penable  Enable status, 1:send to cpu; 0:Transparent
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *		This API can get lldp packets To CPU is enabled or not.\n
 *		LLDP is as following definition:
 *		- DMAC = 01:80:c2:00:00:0e ethertype = 0x88CC
 *		- DMAC = 01:80:c2:00:00:03 ethertype = 0x88CC
 *		- DMAC = 01:80:c2:00:00:00 ethertype = 0x88CC
 */
JL_API jl_api_ret_t jl_cpu_allow_lldp_enable_get(jl_uint32 chip_id, jl_enable_t *penable);

/**
 * @brief Set Reserved multicast address action
 * @param[in]  chip_id   Chip ID
 * @param[in]  rma_mac_L8   [0,47].Lowest 8 bits of 802.1 reserved ethernet multicast address are used as RMA Action table index.
 * @param[in]  rma_action   Set rma action
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *       There are 48 types of Reserved Multicast Address frame for application usage: 01-80-C2-00-00-00 to 01-80-C2-00-00-2F.\n
 *		DMAC = 01:80:c2:00:00:01 Full Duplex PAUSE will always be dropped. This API can not change its action.
 */
JL_API jl_api_ret_t jl_cpu_rma_action_set(jl_uint32 chip_id, jl_uint32 rma_mac_L8, jl_rma_action_t rma_action);

/**
 * @brief Get Reserved multicast address action
 * @param[in]  chip_id   Chip ID
 * @param[in]  rma_mac_L8   [0,47].Lowest 8 bits of 802.1 reserved ethernet multicast address are used as RMA Action table index.
 * @param[out] *prma_action   get rma action
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL		Fail
 * @note
 *     There are 48 types of Reserved Multicast Address frame for application usage: 01-80-C2-00-00-00 to 01-80-C2-00-00-2F.
 */
JL_API jl_api_ret_t jl_cpu_rma_action_get(jl_uint32 chip_id, jl_uint32 rma_mac_L8, jl_rma_action_t *prma_action);

/**
 * @brief Set cpu MAC address
 * @param[in]  chip_id	 Chip ID
 * @param[in]  cpu_mac	 specific MAC address
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL 	Fail
 * @note
 *	   Packets with this destination MAC address will be sent to the CPU.
 */
JL_API jl_api_ret_t jl_cpu_cpumac_set(jl_uint32 chip_id, jl_mac_addr_t cpu_mac);

/**
 * @brief Get cpu MAC address
 * @param[in]  chip_id	 Chip ID
 * @param[out]  pcpu_mac	 specific MAC address
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL 	Fail
 * @note
 *	   Packets with this destination MAC address will be sent to the CPU.
 */
JL_API jl_api_ret_t jl_cpu_cpumac_get(jl_uint32 chip_id, jl_mac_addr_t *pcpu_mac);

/**
 * @brief Set enable detecting special destination mac to send to CPU port or not
 * @param[in]  chip_id	 Chip ID
 * @param[in]  enable	 1:detect special destination mac; 0:not detect
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL 	Fail
 * @note
 *	   This API enables detection of a specific MAC address which sets by API \ref jl_cpu_cpumac_set, that will be sent to the CPU.
 */
JL_API jl_api_ret_t jl_cpu_detection_cpumac_enable_set(jl_uint32 chip_id, jl_enable_t enable);

/**
 * @brief Get enable detecting special destination mac to send to CPU port or not
 * @param[in]  chip_id	 Chip ID
 * @param[out]  penable	 get enable status
 * @return Set successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_SMI		SMI Operation Fail
 *		@retval JL_ERR_FAIL 	Fail
 * @note
 *
 */
JL_API jl_api_ret_t jl_cpu_detection_cpumac_enable_get(jl_uint32 chip_id, jl_enable_t *penable);


#ifdef __cplusplus
}
#endif
#endif /* __JL_API_CPU_H__*/
/** @} CPU APIs*/

