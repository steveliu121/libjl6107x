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

#ifndef __JL61XX_CHIP_H__
#define __JL61XX_CHIP_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "jl_error.h"
#include "jl_types.h"
#include "jl_debug.h"
#include "jl_register.h"
#include "drv_common/jl_drv_common.h"
#include "jl61xx/jl61xx_reg.h"

enum chip_id_e {
	CHIP_ID_JL6110 = 0,
	CHIP_ID_JL6108,
	CHIP_ID_END
};

extern struct jl_switch_dev_s g_switch_dev_6108;
extern struct jl_switch_dev_s g_switch_dev_6110;

#endif /* __JL61XX_CHIP_H__ */
