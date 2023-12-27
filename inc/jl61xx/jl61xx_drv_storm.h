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

#ifndef __JL61XX_DRV_STORM_H__
#define __JL61XX_DRV_STORM_H__

#include "jl_base.h"
#include "jl_storm.h"

#ifdef __cplusplus
extern "C" {
#endif

/****Macro Define****/
/*
 * core clk 25Mhz, clkDivider 25, master tick 1Mhz, refill tick = master tick * stepDivider(exp) tick level
 * tick level 0-4
 */
#define Jl_STORM_CTL_MAX_TICK_LEVEL     (5UL)

#define JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_PPS  (1)
#define JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_BPS  (10)
#define JL_STORMCTRL_BUCKET_BURSTSIZE_MULTIPLE_PPS  (2)
#define JL_STORMCTRL_BUCKET_BURSTSIZE_MULTIPLE_BPS  (2)

#define IFG_DEFAULT_LENGTH                          (20)
#define FCS_DEFAULT_LENGTH                          (4)

#define JL_STORMCTRL_BUCKET_MINDEPTH_VAL_BPS        \
((JL_SYSTEM_JUMBO_LENGTH) * (JL_STORMCTRL_BUCKET_THRESHOLD_MULTIPLE_BPS) / g_tick_freq_list[JL_SYSTIME_MAX_STEP-1])

/****Type Define****/
jl_ret_t jl61xx_stormctrl_enable_set(jl_device_t *pdevice, const jl_port_t egress_port, const jl_storm_type_t type,
				     const jl_bool enable);
jl_ret_t jl61xx_stormctrl_enable_get(jl_device_t *pdevice, const jl_port_t egress_port, const jl_storm_type_t type,
				     jl_bool *penable);

jl_ret_t jl61xx_stormctrl_rate_set(jl_device_t *pdevice, const jl_port_t egress_port, const jl_storm_type_t type,
				   const jl_storm_rate_mode_t mode, const jl_uint32 rate);
jl_ret_t jl61xx_stormctrl_rate_get(jl_device_t *pdevice, const jl_port_t egress_port, const jl_storm_type_t type,
				   jl_storm_rate_mode_t *pmode, jl_uint32 *prate);

#ifdef __cplusplus
}
#endif
#endif				/* __JL61XX_DRV_STORM_H__ */
