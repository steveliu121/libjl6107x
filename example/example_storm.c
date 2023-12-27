#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
//#include <getopt.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_error.h"
#include "jl_storm.h"

static int g_exit;

static void sig_handle(int sig)
{
	(void) sig;
	g_exit = 1;
}

static void clear_window(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\033c");
}

void print_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK STORM TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. jl_storm_control_enable_set/get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. jl_storm_control_rate_set/get()\n");
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char console_input[128] = {0};
	jl_uint32 cmd;

	jl_api_ret_t ret;
	jl_uint32 chip_id = 0;
	jl_uint32 port_id = 0;
	jl_storm_type_t type = JL_STORM_TYPE_BROADCAST;
	jl_bool flag = TRUE;


	jl_storm_rate_mode_t mode = JL_STORM_RATE_MODE_PPS;
	jl_uint32 rate = 5000;

	jl_dev_t dev_6108 = {
		.compat_id = JL_CHIP_6108,
		.name = "device-jl6108",
		.id = chip_id, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI,
			.smi.mdio.bus_id = 0
		}
	};

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);

	/* ret = jl_reg_io_init(); */
	ret = jl_switch_init();
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "smi init fail\n");
		return -1;
	}

	ret = jl_switch_device_create(&dev_6108);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create fail\n");
		return -1;
	}

	while (!g_exit) {
		print_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;
		else
			cmd = atoi(console_input);
		clear_window();


		switch (cmd) {
		case 1:
			ret = jl_storm_control_enable_set(chip_id, port_id, type, flag);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");


			type = JL_STORM_TYPE_MULTICAST;
			ret = jl_storm_control_enable_get(chip_id, port_id, type, &flag);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 2:
			ret = jl_storm_control_rate_set(chip_id, port_id, type, mode, rate);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			mode = JL_STORM_RATE_MODE_KBPS;
			rate = 2000;
			ret = jl_storm_control_rate_set(chip_id, port_id, type, mode, rate);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");


			mode = JL_STORM_RATE_MODE_END;
			rate = 0;

			ret = jl_storm_control_rate_get(chip_id, port_id, type, &mode, &rate);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;
		default:
			break;

		}

}

exit:
	ret = jl_switch_device_delete(chip_id);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device delete fail\n");
		return -1;
	}
	jl_switch_deinit();

	return 0;
}
