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

#include "jl_kit.h"
#include "jl_device.h"

#if ((defined CONFIG_JL_CHIP_ID_6108) || (defined CONFIG_JL_CHIP_ID_6110))
extern jl_driver_t g_drv_61xx;
#endif
#if ((defined CONFIG_JL_CHIP_ID_6105) || (defined CONFIG_JL_CHIP_ID_6107) \
		|| (defined CONFIG_JL_CHIP_ID_6107S) \
		|| (defined CONFIG_JL_CHIP_ID_6107SC))
extern jl_driver_t g_drv_6107x;
#endif

/* jl_device_t g_dev_61xx_0 = { */
/* 	.compat_id = JL_CHIP_6108, */
/* 	.name = "jl61xx-device-0", */
/* 	.id = 0, /1* must be less than JL_MAX_CHIP_NUM *1/ */
/* 	.io_desc = { */
/* 		.type = JL_IO_SMI, */
/* 		.smi.mdio.bus_id = 0 */
/* 	} */
/* }; */

/* jl_device_t g_dev_61xx_1 = { */
/* 	.compat_id = JL_CHIP_6110, */
/* 	.name = "jl61xx-device-1", */
/* 	.id = 1, /1* must be less than JL_MAX_CHIP_NUM *1/ */
/* 	.io_desc = { */
/* 		.type = JL_IO_SMI_SPI_DEMO, */
/* 		.smi.mdio.bus_id = 0 */
/* 	} */
/* }; */

jl_device_t *gpp_devices[JL_MAX_CHIP_NUM] = {
	NULL
	/* &g_dev_61xx_0 */
	/* &g_dev_61xx_1 */
};

jl_driver_t *gpp_drivers[] = {
#if ((defined CONFIG_JL_CHIP_ID_6108) || (defined CONFIG_JL_CHIP_ID_6110))
	&g_drv_61xx,
#endif
#if ((defined CONFIG_JL_CHIP_ID_6105) || (defined CONFIG_JL_CHIP_ID_6107) \
		|| (defined CONFIG_JL_CHIP_ID_6107S) \
		|| (defined CONFIG_JL_CHIP_ID_6107SC))
	&g_drv_6107x,
#endif
};

jl_module_t g_module = {
	.drivers = gpp_drivers,
	.devices = gpp_devices,
	.driver_num = JL_ARRAY_SIZE(gpp_drivers),
	.device_num = JL_MAX_CHIP_NUM, /* initialized in __device_reorder */
	.stat = STAT_UNINIT
};
