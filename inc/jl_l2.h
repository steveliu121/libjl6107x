/**
 * @file jl_l2.h
 * @brief l2
 */

/**
 * @defgroup l2_group L2
 * @{
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

#ifndef __JL_L2_H__
#define __JL_L2_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif	/*  */

/**
 * @example example_l2.c
 */

/****Macro Define****/

/**
 * @enum JL_L2_MC_HANDLE_E
 * @brief Select switch support multicast frame mode.
 */
/**
 * @public typedef jl_l2_mc_handle_t
 */
typedef enum JL_L2_MC_HANDLE_E {
	JL_L2_MC_HANDLE_EXC_IPV4, /**< 0 */
	JL_L2_MC_HANDLE_EXC_IPV6, /**< 1 */
	JL_L2_MC_HANDLE_INC_L2MC, /**< 2 */
	JL_L2_MC_HANDLE_INC_MULTIPORT, /**< 3 */
	JL_L2_MC_HANDLE_END, /**< 4 */
} jl_l2_mc_handle_t;

/**
 * @enum JL_L2_SEL_MODE_E
 * @brief Select l2 table multi entry action (search/delete) mode.
 * @note
 *       -# Note for hash table entry.
 *       -# JL_L2_SEL_MODE_SPORT only for unicast, JL_L2_SEL_MODE_MULPORTMSK only for multicast.
 */
/**
 * @public typedef jl_l2_sel_mode_t
 */
typedef enum JL_L2_SEL_MODE_E  {
	JL_L2_SEL_MODE_SPECIFIC, /**<0, specific mac&fid entry */
	JL_L2_SEL_MODE_STATIC, /**<1, static entry */
	JL_L2_SEL_MODE_FID, /**<2, specific fid entry */
	JL_L2_SEL_MODE_SPORT, /**<3, specific source port entry */
	JL_L2_SEL_MODE_MULPORTMSK, /**<4, specific multicast port mask entry */
	JL_L2_SEL_MODE_END, /**< 5 */
} jl_l2_sel_mode_t;

/****Type Define****/

/**
 * @struct jl_l2_tbl_s
 * @brief L2 entry tabel value setting.
 * @note
 *       -# Suggest setting fid value synchronize with VLAN table fid value.
 *       -# Support unicast/multicast mode in type value.
 *       -# Support input port index /port mask for unicast/multicast in sp field
 *       -# Support JL_L2_MAC_DROPPORT_UC/ JL_L2_MAC_DROPPORT_MC for frame drop
 */
/**
 * @public typedef jl_l2_tbl_t
 */
typedef struct jl_l2_tbl_s  {
	jl_bool valid; /**< 0, valid or not*/
	jl_bool stat; /**< 1, static or dynamic*/
	jl_mac_addr_t mac; /**< 2, mac address*/
	jl_uint32 fid; /**< 3, fid value*/

#define JL_L2_TBL_TYPE_UC          (1) /*!< L2 table type: unicast */
#define JL_L2_TBL_TYPE_MC          (0) /*!< L2 table type: multicast */
	jl_uint32 type; /**< 4,unicast:JL_L2_TBL_TYPE_UC, mulitcast:JL_L2_TBL_TYPE_MC*/

#define JL_L2_MAC_DROPPORT_UC      (0xffffffff) /*!< L2 table sportval: unicast drop */
#define JL_L2_MAC_DROPPORT_MC      (0)          /*!< L2 table sportval: multicast drop */
	jl_uint32 sportval; /**< 5, source port value, JL_L2_MAC_DROPPORT_UC for Unicast frame drop, JL_L2_MAC_DROPPORT_MC for mulitcast frame drop*/
} jl_l2_tbl_t;

/**
 * @brief Init L2
 * @details This function initialize the l2 module.
 * @param chip_id
 * @return Init vlan successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_l2_init(const jl_uint32 chip_id);

/**
 * @brief Deinit L2
 * @details This function will reset and disable l2 module.
 * @param chip_id
 * @return Deinit vlan successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_l2_deinit(const jl_uint32 chip_id);

/**
 * @brief Set a L2 table entry
 * @details This function is used to set a L2 table entry with the specified config value
 * @param chip_id
 * @param pcfg		l2 table config value
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval JL_ERR_FULL     Table full
 *      @retval Other           Fail
 * @note
 *      Setting static entry will auto overwrite hash field calculating with dyanmic entry.\n
 *      Return JL_ERR_FULL means both l2 table and hash collision table are full.
 */
JL_API jl_api_ret_t jl_l2_table_set(const jl_uint32 chip_id, jl_l2_tbl_t *pcfg);

/**
 * @brief Search L2 table entry with the specified mode
 * @details This function is used to search all l2 tables with a specified mode.
 * @param	chip_id
 * @param	mode	 select mode
 * @param	pcfg	 l2 table config value
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Search end
 *      @retval JL_ERR_EXISTS   Get specified entry
 *      @retval Other           Fail
 * @note
 *      Please input relative values in pcfg for the specified mode search.\n
 *      Repeat this function with same value until it returns JL_ERR_OK means you have searched to the end of the table.
 */
JL_API jl_api_ret_t jl_l2_table_search(const jl_uint32 chip_id, const jl_l2_sel_mode_t mode,
						jl_l2_tbl_t *pcfg);

/**
 * @brief Clear L2 table entry with specified mode
 * @details This function is used to clear ALL L2 table entry with the specified mode in l2 table.
 * @param  chip_id
 * @param  mode  select mode
 * @param  pcfg  l2 table config value
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note
 *      Please input relative values in pcfg for specified mode clear.
 */
JL_API jl_api_ret_t jl_l2_table_clear(const jl_uint32 chip_id, const jl_l2_sel_mode_t mode,
					       jl_l2_tbl_t *pcfg);

/**
 * @brief Set a specified hash table entry
 * @details This function is used to set a specified hash entry with the specified index.
 * @param chip_id
 * @param idx		hash entry index
 * @param pcfg		l2 table entry value
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_l2_hash_col_table_set(const jl_uint32 chip_id, const jl_uint32 idx,
						  jl_l2_tbl_t *pcfg);

/**
 * @brief Get a specified hash table entry
 * @details This function is used to get a specified hash entry with index.
 * @param chip_id
 * @param idx       hash entry index
 * @param pcfg      l2 table entry value
 * @return Set successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_l2_hash_col_table_get(const jl_uint32 chip_id, const jl_uint32 idx,
						  jl_l2_tbl_t *pcfg);

/**
 * @brief Set L2 aging time
 * @details This function is used to configure the L2 MAC table hardware aging period.
 * @param chip_id
 * @param time (unit    ms)
 * @return Set L2 aging time successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note Age time will have deviation (round up)
 */
JL_API jl_api_ret_t jl_l2_set_agetime(const jl_uint32 chip_id, const jl_uint32 time);

/**
 * @brief Get L2 aging time
 * @details This function is used to get the L2 MAC table hardware aging period.
 * @param chip_id
 * @param ptime    (unit    ms)
 * @return Get L2 aging time successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_l2_get_agetime(const jl_uint32 chip_id, jl_uint32 *ptime);

/**
 * @brief Set L2 multicast port group table
 * @details This function is used to set the L2 multicast port group table.
 * @param chip_id
 * @param idx    entry id
 * @param portmask    port group(port mask)
 * @return Set multicast port  successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_l2_mc_table_set(const jl_uint32 chip_id, const jl_uint32 idx, jl_uint32 portmask);

/**
 * @brief Get L2 multicast port group table
 * @details This function is used to get the L2 multicast port group table.
 * @param chip_id
 * @param idx    entry id
 * @param pportmask    port group(port mask)
 * @return Get multicast port  successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 */
JL_API jl_api_ret_t jl_l2_mc_table_get(const jl_uint32 chip_id, const jl_uint32 idx,
						jl_uint32 *pportmask);

/**
 * @brief Set L2 multicast mode
 * @details This function is used to determine which types of packets can be treated as multicast by the switch.
 * @param chip_id
 * @param mcHandle    selected multicast mode
 * @param enable    mode
 * @return Set multicast mode successfully or not
 *      @retval JL_ERR_OK       Ok
 *      @retval Other           Fail
 * @note By default, L2 multicast table & all frame mode (IPv4/IPv6) are enabled, unicast multiport action is disabled.
 */
JL_API jl_api_ret_t jl_l2_mc_handle_set(const jl_uint32 chip_id, jl_l2_mc_handle_t mcHandle,
						 jl_bool enable);

#ifdef __cplusplus
}
#endif	/*  */

#endif	/*  */
/** @} l2 APIs*/
