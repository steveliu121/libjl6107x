/**
* @file jl_mirror.h
* @brief Mirror
*/

/**
* @defgroup mirror_group Mirror
* @{
*/

/**
 * @example example_mirror.c
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

#ifndef __JL_MIRROR_H__
#define __JL_MIRROR_H__

#include"jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Get input mirroring port for the specified mirrored port.
 * @param[in]	chip_id			The specified chip id
 * @param[in]	mirrored_port	Mirrored port
 * @param[out]	mirroring_port	Mirroring port
 * @return Get mirroring port successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_UNAVAIL		Unavailable Port id
 */
JL_API jl_api_ret_t jl_mirror_input_mr_get(jl_uint32 chip_id, jl_port_t mirrored_port,
					jl_port_t *mirroring_port);

/**
 * @brief Get output mirroring port for the specified mirrored port.
 * @param[in]	chip_id			The specified chip id
 * @param[in]	mirrored_port	Mirrored port
 * @param[out]	mirroring_port	Mirroring port
 * @return Get mirroring port successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_UNAVAIL		Unavailable Port id
 */
JL_API jl_api_ret_t jl_mirror_output_mr_get(jl_uint32 chip_id, jl_port_t mirrored_port,
					jl_port_t *mirroring_port);

/**
 * @brief Set input mirroring port for the specified mirrored port.
 * @param[in]	chip_id			The specified chip id
 * @param[in]	mirrored_port	Mirrored port
 * @param[out]	mirroring_port	Mirroring port
 * @return Set mirroring port successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_UNAVAIL		Unavailable Port id
 */
JL_API jl_api_ret_t jl_mirror_input_mr_set(jl_uint32 chip_id, jl_port_t mirrored_port,
					jl_port_t mirroring_port);


/**
 * @brief Set output mirroring port for the specified mirrored port.
 * @param[in]	chip_id			The specified chip id
 * @param[in]	mirrored_port	Mirrored port
 * @param[out]	mirroring_port	Mirroring port
 * @return Set mirroring port successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_UNAVAIL		Unavailable Port id
 */
JL_API jl_api_ret_t jl_mirror_output_mr_set(jl_uint32 chip_id, jl_port_t mirrored_port,
						jl_port_t mirroring_port);

/**
 * @brief Clear input mirroring ports for the specified mirrored port.
 * @param[in]	chip_id			The specified chip id
 * @param[in]	mirrored_port	Mirrored port
 * @return Clear mirrorming ports successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_UNAVAIL		Unavailable Port id
 */
JL_API jl_api_ret_t jl_mirror_input_mr_clear(jl_uint32 chip_id, jl_port_t mirrored_port);

/**
 * @brief Clear output mirrormirroing ports for the specified mirrored port.
 * @param[in]	chip_id			The specified chip id
 * @param[in]	mirrored_port	Mirrored port
 * @return Clear mirroring ports successfully or not.
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM	PARAM Fail
 *		@retval JL_ERR_TIMEOUT	Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 *		@retval JL_ERR_UNAVAIL		Unavailable Port id
 */
JL_API jl_api_ret_t jl_mirror_output_mr_clear(jl_uint32 chip_id, jl_port_t mirrored_port);

#ifdef __cplusplus
}
#endif

#endif /* __JL_MIRROR_H__*/
/** @} Mirror APIs*/
