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

#ifndef __JL6107X_REG_H__
#define __JL6107X_REG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/jl_reg_io.h"
#include "jl6107x/reg_ccs.h"
#include "jl6107x/reg_cmac.h"
#include "jl6107x/reg_crg.h"
#include "jl6107x/reg_iop.h"
#include "jl6107x/reg_ladp_access.h"
#include "jl6107x/reg_led.h"
#include "jl6107x/reg_lmac.h"
#include "jl6107x/reg_lmac_adp.h"
#include "jl6107x/reg_mag.h"
#include "jl6107x/reg_swcore.h"
#include "jl6107x/reg_phg_afe_misc.h"
#include "jl6107x/reg_phg_afe_phypll.h"
#include "jl6107x/reg_phg_afe_sdspll.h"
#include "jl6107x/reg_phg_gephy.h"
#include "jl6107x/reg_phg_serdes.h"
#include "jl6107x/reg_phg_top.h"
#include "jl6107x/reg_pmx.h"
#include "jl6107x/reg_sag.h"
#include "jl6107x/reg_smac.h"
#include "jl6107x/reg_smac_adp.h"
#include "jl6107x/reg_swc.h"
#include "jl6107x/reg_top.h"
#include "jl6107x/jl6107x_chip.h"

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

#endif /* __JL6107X_REG_H__ */
