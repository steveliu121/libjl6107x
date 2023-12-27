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

#ifndef __JL61XX_REG_H__
#define __JL61XX_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/jl_reg_io.h"
#include "jl61xx/reg_ccs.h"
#include "jl61xx/reg_cmac.h"
#include "jl61xx/reg_crg.h"
#include "jl61xx/reg_iop.h"
#include "jl61xx/reg_led.h"
#include "jl61xx/reg_lmac.h"
#include "jl61xx/reg_lmac_adp.h"
#include "jl61xx/reg_mag.h"
#include "jl61xx/reg_swcore.h"
#include "jl61xx/reg_phg_afe_misc.h"
#include "jl61xx/reg_phg_afe_phypll.h"
#include "jl61xx/reg_phg_afe_sdspll.h"
#include "jl61xx/reg_phg_gephy.h"
#include "jl61xx/reg_phg_serdes.h"
#include "jl61xx/reg_phg_top.h"
#include "jl61xx/reg_pmx.h"
#include "jl61xx/reg_sag.h"
#include "jl61xx/reg_smac.h"
#include "jl61xx/reg_smac_adp.h"
#include "jl61xx/reg_swc.h"
#include "jl61xx/reg_top.h"
#include "jl61xx/jl61xx_chip.h"

#define LMAC_BLOCK_SIZE                                                                        ((jl_uint32)0x00000200U)
#define LMAC_ADP_BLOCK_SIZE                                                                    ((jl_uint32)0x00000400U)
#define PHG_AFE_PHYPLL_BLOCK_SIZE                                                              ((jl_uint32)0x00000040U)
#define PHG_GEPHY_BLOCK_SIZE                                                                   ((jl_uint32)0x00004000U)
#define PHG_SERDES_BLOCK_SIZE                                                                  ((jl_uint32)0x00004000U)
#define SMAC_BLOCK_SIZE                                                                        ((jl_uint32)0x00000200U)
#define SMAC_ADP_BLOCK_SIZE                                                                    ((jl_uint32)0x00000400U)


#ifdef __cplusplus
}
#endif

#endif /* __JL61XX_REG_H__ */
