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
	jl_dev_t dev_6108 = {
		.compat_id = JL_CHIP_6108,
		.name = "device-jl6108",
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

	ret = jl_switch_init();
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "smi init fail\n");
		return -1;
	}

	/* 1. create multiple devices 0&1 */
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_switch_device_create dev_6108\n");
	ret = jl_switch_device_create(&dev_6108);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create fail\n");
		//return -1;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_switch_device_create dev_6110\n");
	ret = jl_switch_device_create(&dev_6110);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create fail\n");
		//return -1;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_switch_device_create dev_6110_smi_id\n");
	ret = jl_switch_device_create(&dev_6110_smi_id);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create dev_6110_smi_id fail\n");
		//return -1;
	}

	/* 2. SDK is ready
	 * TODO call sdk APIs here
	 * to control the corresponding switch device
	 * jl_xx_api(chip_id, ...)
	 */

	/* 3. close device io temporarily(free device io) */
	ret = jl_switch_device_close(0);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device close fail\n");
		return -1;
	}

	/* 4. re-open device io(request device io again)
	 * control the corresponding switch device again
	 */
	ret = jl_switch_device_open(0);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device open fail\n");
		return -1;
	}

	/* 5. delete multiple devices 0&1 */
	ret = jl_switch_device_delete(0);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device delete fail\n");
		return -1;
	}

	ret = jl_switch_device_delete(1);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device delete fail\n");
		return -1;
	}

	ret = jl_switch_device_delete(2);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device delete fail\n");
		return -1;
	}

	jl_switch_deinit();

	return 0;
}
