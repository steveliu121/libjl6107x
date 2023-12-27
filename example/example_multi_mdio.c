#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_api_ret_t ret;
	jl_dev_t dev_6107 = {
		.compat_id = JL_CHIP_6107,
		.name = "device-jl6107",
		.id = 0, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI,
			.smi.mdio.bus_id = (MDIO_DEVID_0<<24)+0 /*bit24-31 is for mido phy address,bit0-23 is for bus id*/
		}
	};

	jl_dev_t dev_6110 = {
		.compat_id = JL_CHIP_6110,
		.name = "device-jl6110",
		.id = 1, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI,
			.smi.mdio.bus_id = (MDIO_DEVID_0<<24)+0 /*bit24-31 is for mido phy address,bit0-23 is for bus id*/
		}
	};

	jl_dev_t dev_6110_smi_id = {
		.compat_id = JL_CHIP_6110,
		.name = "device-jl6110-smi29",
		.id = 2, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI,
			.smi.mdio.bus_id = (MDIO_DEVID_29<<24)+0 /*bit24-31 is for mido phy address,bit0-23 is for bus id*/
		}
	};

	jl_dev_t dev_6107_smi_29 = {
		.compat_id = JL_CHIP_6107,
		.name = "device-jl6107-smi29",
		.id = 3, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI,
			.smi.mdio.bus_id = (MDIO_DEVID_29<<24)+0 /*bit24-31 is for mido phy address,bit0-23 is for bus id*/
		}
	};

	ret = jl_switch_init();
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "smi init fail\n");
		return -1;
	}

	/* 1. create multiple devices  */
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_switch_device_create dev_6107_smi_29\n");
	ret = jl_switch_device_create(&dev_6107_smi_29);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create dev_6107_smi_29 fail\n");
		//return -1;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_switch_device_create dev_6107\n");
	ret = jl_switch_device_create(&dev_6107);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create dev_6107 fail\n");
		//return -1;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_switch_device_create dev_6110\n");
	ret = jl_switch_device_create(&dev_6110);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create fail\n");
		//return -1;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_switch_device_create dev_6110_smi_29\n");
	ret = jl_switch_device_create(&dev_6110_smi_id);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create dev_6110_smi_id fail\n");
		//return -1;
	}

	return 0;
}
