/**
 * @file jl_trunk.h
 * @brief Link Aggregation
 */

/**
 * @defgroup trunk_group Trunk
 * @{
 */

/**
 * @example example_trunk.c
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

#ifndef __JL_TRUNK_H__
#define __JL_TRUNK_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

#define JL_LA_MAX_HASH_INDEX		256 /**< define max hash index 256 */

/**
 * @struct jl_la_hash2ports_s
 * @brief The portmask for every hash index
 */
/**
 * @public typedef jl_la_hash2ports_t
 */
typedef struct jl_la_hash2ports_s {
	jl_uint16 value[JL_LA_MAX_HASH_INDEX];/**< 0, the portmasks array for all hash index*/
} jl_la_hash2ports_t;

/**
 * @brief Set link aggregation to be enabled or disabled.
 * @details This function can be used to control whether link aggregation is enabled.
 * @param[in]	chip_id		The specified chip id
 * @param[in]	enable		The link aggregatin enabled status
 * @return Set link aggregation enabled status successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @note
 *	 - Before enable the link aggregation, you need use API \ref jl_link_aggregation_portmask_set to setup link aggregation groups.
 *	 - Once disable the link aggregation, all the link aggregation configuration will be cleared, including portmask,hashfactor and hashmappingTable.
 */
JL_API jl_api_ret_t jl_link_aggregation_enable_set(jl_uint32 chip_id, jl_enable_t enable);

/**
 * @brief Get link aggregation enabled status.
 * @details This function can be used to get whether link aggregation is enabled.
 * @param[in]	chip_id		The specified chip id
 * @param[out]	penable		The link aggregatin enabled status
 * @return Get link aggregation enabled status successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_link_aggregation_enable_get(jl_uint32 chip_id, jl_enable_t *penable);

/**
 * @brief Set link aggregation group port members and assign equal amount of traffic for each port.
 * @details This function can be used to setup a link aggregation group and determine which physical ports are memebers of the specified link aggregation.\n
 * Meanwhile,this function sets each port member in the link aggregation group has equal amount of traffic.
 * @param[in]	chip_id		The specified chip id
 * @param[in]	la_id		The specified link aggregation number
 * @param[in]	la_portmask	The physical ports that are memebers of the link aggregation. One bit means one port. bit0->port0 and so on
 * @return Set link aggregation group successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @note
 *		You must set the la_id to one of the port's number in this link aggregation, such as you set port 1,2 into a link aggregation, you should set la_id as 1 or 2
 */
JL_API jl_api_ret_t jl_link_aggregation_portmask_set(jl_uint32 chip_id, jl_la_group_t la_id, jl_portmask_t la_portmask);

/**
 * @brief Get link aggregation group portmask.
 * @details This function can be used to get which physical ports are memebers of the specified link aggregation group.
 * @param[in]	chip_id		The specified chip id
 * @param[in]	la_id		The specified link aggregation number
 * @param[out]	pla_portmask	The physical ports that are memebers of the link aggregation. One bit means one port
 * @return Get link aggregation portmask successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_link_aggregation_portmask_get(jl_uint32 chip_id, jl_la_group_t la_id, jl_portmask_t *pla_portmask);

/**
 * @brief Delete the specified port members for the specified link aggregation group.
 * @details This function can be used to delete the specified port members for the specified la.
 * @param[in]	chip_id		The specified chip id
 * @param[in]	la_id		The specified link aggregation number
 * @param[in]	port_id		TThe physical port which you want to delete from the link aggregation
 * @return Delete link aggregation port successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @note
 *		When deleted port_id is equal to la_id, the la_id group will be deleted and a new la group with the rest port members will be setup whose la_id is the lowest port number in the rest port members.
 */
jl_api_ret_t jl_link_aggregation_port_del(jl_uint32 chip_id, jl_la_group_t la_id, jl_port_t port_id);

/**
 * @brief Delete the specified link aggregation group.
 * @details This function can be used to delete the specified link aggregation group.
 * @param[in]	chip_id		The specified chip id
 * @param[in]	la_id		The specified link aggregation number
 * @return Delete link aggregation group successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */

JL_API jl_api_ret_t jl_link_aggregation_group_del(jl_uint32 chip_id, jl_la_group_t la_id);


/**
 * @brief Set link aggregation hash factor.
 * @details This function can be used to determine the hash factors which are used to calculated the link aggregation hash value.
 * @param[in]	chip_id					The specified chip id
 * @param[in]	la_hashfactor_bitmask	Bitmask of the hash factors
 * @return Set link aggregation hash factor successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @note
 *		One bit mask presents one hash factor, the total bit masks are 6:
 *		- 0b000001: SMAC, the packets destination MAC address
 *		- 0b000010: DMAC, the packets destination MAC address
 *		- 0b000100: IP, the packets IP source and destination address
 *		- 0b001000: L4, the packets L4 SP/DP and L4 protocol byte
 *		- 0b010000: TOS, the incoming packets TOS byte
 *		- 0b100000: VLAN, tThe packets vlan tag\n
 *		eg: 0b000011: SMAC & DMAC
 */
JL_API jl_api_ret_t jl_link_aggregation_hashfactor_set(jl_uint32 chip_id, jl_uint32 la_hashfactor_bitmask);

/**
 * @brief Get link aggregation hash factor.
 * @details This function can be used to get the hash factors which are used to calculate the hash value.
 * @param[in]	chip_id					The specified chip id
 * @param[out]	pla_hashfactor_bitmask	The bitmask of hash factors
 * @return Get link aggregation hash factor successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_link_aggregation_hashfactor_get(jl_uint32 chip_id, jl_uint32 *pla_hashfactor_bitmask);

/**
 * @brief Set equal amount of traffic for the each port in portmask.
 * @details This function can be used to reset the hash map when active ports changed in the specified link aggregation group.
 * @param[in]	chip_id		The specified chip id
 * @param[in]	la_id		The specified link aggregation number
 * @param[in]	la_portmask		The port members which are linkup in the specified link aggregation group
 * @return Set link aggregation weight successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @note
 *		Before using this API to reset hash map, must use the API \ref jl_link_aggregation_hash_map_clear to clear the old hash map.
 */
JL_API jl_api_ret_t jl_link_aggregation_hash_map_set(jl_uint32 chip_id, jl_la_group_t la_id, jl_portmask_t la_portmask);

/**
 * @brief Get link aggregation hash mapping table.
 * @details This function can be used to get the port members for all hash index.
 * @param[in]	chip_id			The specified chip id
 * @param[out]	la_hash2ports	The port members for all hash index
 * @return Get link aggregation hash mapping table successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 */
JL_API jl_api_ret_t jl_link_aggregation_hash_map_get(jl_uint32 chip_id, jl_la_hash2ports_t *la_hash2ports);

/**
 * @brief clear the hash map for the specified portmask that had been set before.
 * @details This function can be used to clear the old hash map before reset a new hash map.
 * @param[in]	chip_id		The specified chip id
 * @param[in]	la_id		The specified link aggregation number
 * @param[in]	la_portmask		The port members which need to clear hash map.
 * @return Set link aggregation weight successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 * @note
 *		Before using API \ref jl_link_aggregation_hash_map_set to reset hash map, must use this api to clear the old hash map.
 */
JL_API jl_api_ret_t jl_link_aggregation_hash_map_clear(jl_uint32 chip_id, jl_la_group_t la_id, jl_portmask_t la_portmask);

#ifdef __cplusplus
extern }
#endif

#endif /* __JL_TRUNK_H__*/
/** @} Trunk APIs*/

