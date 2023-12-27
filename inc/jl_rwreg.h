/**
 * @file jl_rwreg.h
 * @brief Read or Write Reg
 */

/**
 * @defgroup rwreg_group Read Or Write Reg
 * @{
 */

/**
 * @example example_rwreg.c
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

#ifndef __JL_RWREG_H__
#define __JL_RWREG_H__

#include "jl_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Write the 32-bit value to the register with the specified address
 * @details This function is used to directly write the 32-bit value to the register with the specified address
 * @param[in]	chip_id		Chip Id
 * @param[in]	addr		Address of the register
 * @param[in]	val		32-bit value
 * @return  Write the 32-bit value to the register successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_reg_io_write(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 val);

/**
 * @brief Read the 32-bit value from the register with the specified address
 * @details This function is used to directly read the 32-bit value from the register with the specified address
 * @param[in]	chip_id		Chip Id
 * @param[in]	addr		Address of the register
 * @param[out]	pval		The pointer of value
 * @return  Read the 32-bit value from the register successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_reg_io_read(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 *pval);

/**
 * @brief Write the array values to the multiple registers in a row.
 * @details This function is used to directly write the values to the continuous registers with the specified size and initial address.
 * @param[in]	chip_id		Chip Id
 * @param[in]	addr		The initial address of the continuous registers
 * @param[in]	pval		The pointer of values
 * @param[in]	size        Count of the registers which need to write
 * @return  Write the values to the continuous registers successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_reg_io_burst_write(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size);

/**
 * @brief Read the array values from the multiple registers in a row.
 * @details This function is used to directly get the values from the continuous registers with the specified size and initial address.
 * @param[in]	chip_id		Chip Id
 * @param[in]	addr		The initial address of the continuous registers
 * @param[out]	pval		The pointer of values
 * @param[in]	size        Count of the registers which need to write
 * @return  Read the values from the continuous registers successfully or not
 *		@retval JL_ERR_OK		Ok
 *		@retval JL_ERR_INIT		Check Switch State Fail
 *		@retval JL_ERR_PARAM		PARAM Fail
 *		@retval JL_ERR_TIMEOUT		Read/Write Reg Timeout
 *		@retval JL_ERR_FAIL		Read/Write Reg Fail
 *		@retval JL_ERR_PORT		Error Port id
 */
JL_API jl_api_ret_t jl_reg_io_burst_read(jl_uint32 chip_id, jl_uint32 addr, jl_uint32 *pval, jl_uint32 size);

#ifdef __cplusplus
}
#endif

#endif /* __JL_RWREG_H__ */
/** @} Read or Write Reg APIs*/

