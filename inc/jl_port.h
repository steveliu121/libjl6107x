/**
 * @file jl_port.h
 * @brief Port
 */

/**
 * @defgroup port_group Port
 * @{
 */

/**
 * @example example_port.c
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

#ifndef __JL_API_PORT_H__
#define __JL_API_PORT_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @enum jl_port_link_status_e
 * @brief port link status.
 */
/**
 * @public typedef jl_port_link_status_t
 */
typedef enum jl_port_link_status_e {
	PORT_LINKDOWN = 0,/**< 0 */
	PORT_LINKUP,/**< 1 */
	PORT_LINKSTATUS_END/**< Invalid */
} jl_port_link_status_t;

/**
 * @enum jl_port_duplex_e
 * @brief port duplex.
 */
/**
 * @public typedef jl_port_duplex_t
 */
typedef enum jl_port_duplex_e {
	PORT_HALF_DUPLEX = 0,/**< 0 */
	PORT_FULL_DUPLEX,/**< 1 */
	PORT_DUPLEX_END/**< Invalid */
} jl_port_duplex_t;

/**
 * @enum jl_speed_e
 * @brief port speed.
 */
/**
 * @public typedef jl_port_speed_t
 */
typedef enum jl_speed_e {
	PORT_SPEED_10M = 0,/**< 0 */
	PORT_SPEED_100M,/**< 1 */
	PORT_SPEED_1000M,/**< 2 */
	PORT_SPEED_2500M,/**< 3 */
	PORT_SPEED_END/**< Invalid */
} jl_port_speed_t;

/**
 * @enum jl_mode_ext_e
 * @brief ext mode select.
 */
/**
 * @public typedef jl_mode_ext_t
 */
typedef enum jl_mode_ext_e {
    MODE_EXT_DISABLE = 0,/**< Indicates EXT MAC MODE DISABLE */
    MODE_EXT_RGMII,/**< Indicates EXT MAC MODE RGMII */
    MODE_EXT_MII_MAC,/**< Indicates EXT MAC MODE MII_MAC */
    MODE_EXT_MII_PHY,/**< Indicates EXT MAC MODE MII_PHY */
    MODE_EXT_RMII_MAC,/**< Indicates EXT MAC MODE RMII_MAC */
    MODE_EXT_RMII_PHY,/**< Indicates EXT MAC MODE RMII_PHY */
    MODE_EXT_SGMII_MAC,/**< Indicates EXT MAC MODE SGMII_MAC */
    MODE_EXT_SGMII_PHY,/**< Indicates EXT MAC MODE SGMII_PHY */
    MODE_EXT_HSGMII,/**< Indicates EXT MAC MODE HSGMII */
    MODE_EXT_FIBER,/**< Indicates EXT MAC MODE FIBER */
    MODE_EXT_END/**< Invalid */
} jl_mode_ext_t;

/**
 * @struct jl_port_mac_ability_s
 * @brief mac port ability.
 */
/**
 * @public typedef jl_port_mac_ability_t
 */
typedef struct jl_port_mac_ability_s {
	jl_uint32 force_mode;/**< Indicates whether the MAC's force link ability is disabled or not 1:ENABLED 0:DISABLED(mac will syc phy config)*/
	jl_uint32 speed;/**< Indicates the speed of MAC, 0: 10M, 1: 100M 2: 1000M 3: 2500M */
	jl_uint32 duplex;/**< Indicates the duplex mode of MAC, 0: Half, 1: Full */
	jl_uint32 link;/**< Indicates the link status of MAC */
	jl_uint32 tx_pause;/**< Indicates the tx flow control of MAC */
	jl_uint32 rx_pause;/**< Indicates the rx flow control of MAC */
} jl_port_mac_ability_t;

/**
 * @struct jl_port_phy_ability_s
 * @brief phy port ability.
 */
/**
 * @public typedef jl_port_phy_ability_t
 */
typedef struct jl_port_phy_ability_s {
	jl_uint32 auto_negotiation;/**< Indicates whether PHY operate in auto negotiation mode or not */
	jl_uint32 half_duplex_10;/**< Indicates PHY operate in half duplex 10M mode */
	jl_uint32 full_duplex_10;/**< Indicates PHY operate in full duplex 10M mode */
	jl_uint32 half_duplex_100;/**< Indicates PHY operate in half duplex 100M mode */
	jl_uint32 full_duplex_100;/**< Indicates PHY operate in full duplex 100M mode */
	jl_uint32 full_duplex_1000;/**< Indicates PHY operate in full duplex 1000M mode */
	jl_uint32 flow_control;/**< Indicates PHY support flow control */
	jl_uint32 asym_flow_control;/**< Indicates PHY support asymmetric flow control */
} jl_port_phy_ability_t;

/**
 * @struct jl_port_fiber_ability_s
 * @brief port fiber ability.
 */
/**
 * @public typedef jl_port_fiber_ability_t
 */
typedef struct jl_port_fiber_ability_s {
	jl_uint32 auto_negotiation;/**< Indicates whether fiber operate in auto negotiation mode or not */
	jl_uint32 full_duplex_100;/**< Indicates fiber operate in full duplex 100M mode */
	jl_uint32 full_duplex_1000;/**< Indicates fiber operate in full duplex 1000M mode */
	jl_uint32 flow_control;/**< Indicates fiber support flow control */
	jl_uint32 asym_flow_control;/**< Indicates fiber support asymmetric flow control */
} jl_port_fiber_ability_t;

/**
 * @brief Set ethernet PHY auto-negotiation ability
 * @details This function configures PHY capability of specified port.\n
 * If Full_1000 bit is set to 1, the auto negotiation will be automatically set to 1.\n
 * While both auto negotiation and Full_1000 are set to 0, the PHY speed and duplex
 * selection will be set as following 1000F > 100F > 100H > 10F > 10H priority sequence.
 * @param[in] chip_id		Chip Id
 * @param[in] port	Index of port number
 * @param[in] pability	PHY ability structure
 * @return Set PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_autoneg_ability_set(jl_uint32 chip_id, jl_port_t port, jl_port_phy_ability_t *pability);

/**
 * @brief Get ethernet PHY auto-negotiation ability
 * @details This function gets PHY auto negotiation capability of specified port.
 * @param[in] chip_id		Chip Id
 * @param[in]	port		Index of port number
 * @param[out]	pability	PHY ablility structure
 * @return Get PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_autoneg_ability_get(jl_uint32 chip_id, jl_port_t port, jl_port_phy_ability_t *pability);

/**
 * @brief Set ethernet PHY force-mode ability
 * @details While auto negotiation is set to 0, the PHY speed and duplex selection will be
 * set as following 1000F > 100F > 100H > 10F > 10H priority sequence.
 * @param[in] chip_id		Chip Id
 * @param[in] port	Index of port number
 * @param[in] pability	PHY ability structure
 * @return Set PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_force_ability_set(jl_uint32 chip_id, jl_port_t port, jl_port_phy_ability_t *pability);

/**
 * @brief Get ethernet PHY auto-negotiation ability
 * @details This function gets the force-mode PHY capability of specified port.
 * @param[in] chip_id		Chip Id
 * @param[in]	port		Index of port number
 * @param[out]	pability	PHY ablility structure
 * @return Get PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_force_ability_get(jl_uint32 chip_id, jl_port_t port, jl_port_phy_ability_t *pability);

/**
 * @brief Get ethernet PHY link status
 * @details This function gets PHY linking status of specified port.
 * @param[in]	chip_id		Chip Id
 * @param[in]	port	Index of port number
 * @param[out]	pstatus	PHY Status
 * @param[out]	pduplex	PHY Duplex mode
 * @param[out]	pspeed	PHY Speed
 * @return Get PHY link status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_status_get(jl_uint32 chip_id, jl_port_t port, jl_port_link_status_t *pstatus, jl_port_duplex_t *pduplex, jl_port_speed_t *pspeed);

/**
 * @brief Set PHY isolate status on specific port
 * @details This function sets PHY electrically isolate from MII/RMII of specified port.
 * @param[in] chip_id		Chip Id
 * @param[in]	port	Index of port numb
 * @param[in]	isolate	Isolate status
 * @return set PHY isolate function successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can enable or disable PHY isolate status on specific port
 */
JL_API jl_api_ret_t jl_port_phy_isolate_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t isolate);

/**
 * @brief Get PHY isolate status on specific port
 * @details This function gets PHY isolate status of specified port
 * @param[in] chip_id		Chip Id
 * @param[in]	port	Index of port number
 * @param[out]	pisolate	Isolate status
 * @return get PHY isolate status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_isolate_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *pisolate);

/**
 * @brief Set ethernet PHY enable status
 * @details This function enables or disables the PHY on specified port
 * @param[in] chip_id		Chip Id
 * @param[in]	port	Index of port number
 * @param[in]	enable	PHY enable state
 * @return Set PHY enable status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_enable_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable);

/**
 * @brief Get ethernet PHY enable status
 * @details This function gets PHY enable status on specified port.
 * @param[in] chip_id		Chip Id
 * @param[in]	port	Index of port number
 * @param[out]	penable	PHY enable status
 * @return Get PHY enable status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_enable_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable);

/**
 * @brief Set ethernet all PHY enable status
 * @details This function enables or disables all the PHYs
 * @param[in] chip_id		Chip Id
 * @param[in]	enable	PHY enable state
 * @return Set all PHY enable status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_phy_all_enable_set(jl_uint32 chip_id, jl_enable_t enable);

/**
 * @brief Set port force link ability
 * @details This function sets MAC force-link capability of specified port.
 * @param[in] chip_id		Chip Id
 * @param[in] port	Index of port number
 * @param[in] pability	MAC ability structure
 * @return Set port MAC force link ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_mac_force_link_set(jl_uint32 chip_id, jl_port_t port, jl_port_mac_ability_t *pability);

/**
 * @brief Get port force link ability
 * @details This function gets MAC force-link capability of specified port.
 * @param[in] chip_id		Chip Id
 * @param[in]	port		Index of port number
 * @param[out]	pability	MAC ability structure
 * @return Get port MAC force link ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_mac_force_link_get(jl_uint32 chip_id, jl_port_t port, jl_port_mac_ability_t *pability);

/**
 * @brief Set external port force link ability
 * @details This function sets MAC force-link capability of specified Extension port
 * @param[in] chip_id		Chip Id
 * @param[in] port	Index of port number
 * @param[in] mode	external interface force mode
 * @param[in] pability	Ext MAC ability structure
 * @return Set external port MAC force link ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_NOT_FOUND	Fiber ability config from jl_port_fiber_ability_set
 * @note
 *	- RGMII:1000M/100M/10M
 *	- MII_MAC/MII_PHY:100M/10M
 *	- RMII_MAC/RMII_PHY:100M/10M
 *	- SGMII_MAC/SGMII_PHY:1000M/100M/10M
 *	- HSGMII:2500M
 *	- FIBER:1000M/100M (FIBER ability config from jl_port_fiber_ability_set API)
 */
JL_API jl_api_ret_t jl_port_mac_force_link_ext_set(jl_uint32 chip_id, jl_port_t port, jl_mode_ext_t mode, jl_port_mac_ability_t *pability);

/**
 * @brief Get external port force link ability
 * @details This function gets MAC force-link capability of specified Extension port
 * @param[in] chip_id		Chip Id
 * @param[in]	port		Index of port number
 * @param[out]	pmode		external interface force mode
 * @param[out]	pability	Ext MAC ability structure
 * @return Get external port MAC force link ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_mac_force_link_ext_get(jl_uint32 chip_id, jl_port_t port, jl_mode_ext_t *pmode, jl_port_mac_ability_t *pability);

/**
 * @brief Get port link status
 * @details This function gets force MAC link parameters of specified port.
 * @param[in] chip_id		Chip Id
 * @param[in]	port		Index of port number
 * @param[out]	pability	Current mac Status
 * @return Get port link status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_mac_status_get(jl_uint32 chip_id, jl_port_t port, jl_port_mac_ability_t *pability);

/**
 * @brief Set port local loopback (redirect tx to rx)
 * @details This function sets MAC loopback status of specified port
 * @param[in] chip_id		Chip Id
 * @param[in]	port	portid
 * @param[in]	enable	enable MAC loopback status
 * @return Enable/Disable local loopback of a specific port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_mac_loopback_set(jl_uint32 chip_id, jl_port_t port, jl_enable_t enable);

/**
 * @brief Get port local loopback status
 * @details This function gets MAC loopback status of specified port.
 * @param[in] chip_id		Chip Id
 * @param[in]	port	Index of port number
 * @param[out]	penable	enable MAC loopback status
 * @return Get local loopback status of a specific port successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note
 *      This API can get local loopback is enabled or not of a specific port
 */
JL_API jl_api_ret_t jl_port_mac_loopback_get(jl_uint32 chip_id, jl_port_t port, jl_enable_t *penable);

/**
 * @brief Set fiber phy port ability
 * @details Set auto negotiation, speed and fc to the fiber phy port.\n The PHY speed and duplex selection will be
 * set as following 1000F > 100F priority sequence.\n Only 1000F can support auto negotiation.
 * @param[in] chip_id		Chip Id
 * @param[in] port	Index of port number
 * @param[in] pability	fiber PHY ability structure
 * @return Set PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 * @note FIBER:1000M/100M
 */
JL_API jl_api_ret_t jl_port_fiber_ability_set(jl_uint32 chip_id, jl_port_t port, jl_port_fiber_ability_t *pability);

/**
 * @brief Get fiber phy port ability
 * @details This function gets the fiber PHY capability of specified port.
 * @param[in] chip_id		Chip Id
 * @param[in]	port		Index of port number
 * @param[out]	pability	fiber PHY ability structure
 * @return Get fiber PHY ability successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read Reg Timeout
 *		@retval JL_ERR_FAIL		Read Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_fiber_ability_get(jl_uint32 chip_id, jl_port_t port, jl_port_fiber_ability_t *pability);

/**
 * @brief Set serdes reset status
 * @param[in] chip_id		Chip Id
 * @param[in] port	Index of port number
 * @return Set serdes reset status successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_port_sds_reset(jl_uint32 chip_id, jl_port_t port);

#ifdef __cplusplus
}
#endif

#endif /* __JL_API_PORT_H__ */
/** @} Port APIs*/
