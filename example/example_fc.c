#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
//#include <getopt.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_fc.h"

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
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK FC TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. taildrop_test()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. jl_taildrop_counter_get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. port_flow_control_force_enable_test()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. port_flow_control_auto_mode_test()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "5. port_flow_control_threshold_test()\n");
}

int taildrop_test(jl_uint32 chip_id)
{
	char console_input[128] = {0};
	jl_api_ret_t ret = 0;
	jl_uint32 port = 0;
	jl_uint32 value = 0;
	jl_enable_t enable =  DISABLED;

	do {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "tail drop enable:<port, enable>\n");

		fgets(console_input, 127, stdin);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");

		sscanf(console_input, "%u,%u", &port, &value);
	} while (0);

	ret = jl_taildrop_enable_set(chip_id, (jl_port_t)port, (jl_enable_t)value);
	if (ret != 0)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_taildrop_enable_set return err %d\n", ret);

	ret = jl_taildrop_enable_get(chip_id, (jl_port_t)port, &enable);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_taildrop_enable_get return err %d,port %d,enable = %d\n", ret, port, enable);
	return ret;

}

int port_flow_control_force_enable_test(jl_uint32 chip_id)
{
	char console_input[128] = {0};
	jl_api_ret_t ret = 0;
	jl_uint32 port = 0;
	jl_uint32 value = 0;
	jl_enable_t enable =  DISABLED;

	do {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "force port flow control enable or disable:<port, enable>\n");

		fgets(console_input, 127, stdin);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");

		sscanf(console_input, "%u,%u", &port, &value);
	} while (0);

	/*step1:set force mode*/
	ret = jl_port_fc_force_mode_set(chip_id, (jl_port_t)port, ENABLED);
	if (ret != 0)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fc_force_mode_set return err %d\n", ret);

	ret = jl_port_fc_force_mode_get(chip_id, (jl_port_t)port, &enable);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fc_force_mode_get return err %d,port %d,enable = %d\n", ret, port, enable);

	/*step2:set flow control enable or disable*/
	ret = jl_port_fc_force_enable_set(chip_id, (jl_port_t)port, (jl_enable_t)value);
	if (ret != 0)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fc_enable_set return err %d\n", ret);

	ret = jl_port_fc_force_enable_get(chip_id, (jl_port_t)port, &enable);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fc_enable_get return err %d,port %d,enable = %d\n", ret, port, enable);
	return ret;

}

int port_flow_control_auto_mode_test(jl_uint32 chip_id)
{
	char console_input[128] = {0};
	jl_api_ret_t ret = 0;
	jl_uint32 port = 0;
	jl_uint32 value = 0;
	jl_enable_t enable =  DISABLED;

	do {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "set port flow control auto mode,value: 0-disable,1-enable:<port,value>\n");

		fgets(console_input, 127, stdin);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");

		sscanf(console_input, "%u,%u", &port, &value);
	} while (0);

	/*enable/disable forced mode*/
	ret = jl_port_fc_force_mode_set(chip_id, (jl_port_t)port, value);
	if (ret != 0)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fc_force_mode_set return err %d\n", ret);

	ret = jl_port_fc_force_mode_get(chip_id, (jl_port_t)port, &enable);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fc_force_mode_get return err %d,port %d,enable = %d\n", ret, port, enable);
	return ret;

}

int port_flow_control_threshold_test(jl_uint32 chip_id)
{
	char console_input[128] = {0};
	jl_api_ret_t ret;
	jl_uint32 port = 0;
	jl_uint32 xon = 0;
	jl_uint32 xoff = 0;

	do {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port flow control threshold:<port, xon, xoff>\n");

		fgets(console_input, 127, stdin);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");

		sscanf(console_input, "%u,%u,%u", &port, &xon, &xoff);
	} while (0);

	ret = jl_port_fc_threshold_set(chip_id, (jl_port_t)port, xon, xoff);
	if (ret != 0)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fc_threshold_set return err %d\n", ret);
	xon = 0;
	xoff = 0;
	ret = jl_port_fc_threshold_get(chip_id, (jl_port_t)port, &xon, &xoff);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fc_threshold_get return err %d,port %d,xon = %d,xoff=%d.\n", ret, port, xon, xoff);
	return ret;

}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char console_input[128] = {0};
	jl_uint32 cmd = 0;
	jl_api_ret_t ret = 0;
	jl_uint32 cnt = 0;
	jl_uint32 chip_id = 0;
	char *pstr = NULL;

	jl_dev_t dev_6108 = {
		.compat_id = JL_CHIP_6107,
		.name = "device-jl6108",
		.id = chip_id, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI_SPI_DEMO,
			.smi.mdio.bus_id = 0
		}
	};

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);

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
		pstr = fgets(console_input, 127, stdin);
		if (pstr == NULL)
			continue;

		if (!strncmp(console_input, "q", 1))
			goto exit;
		else
			cmd = atoi(console_input);

		clear_window();

		switch (cmd) {
		case 1:
			ret = taildrop_test(chip_id);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "taildrop_test\n");
				return ret;
			}
			break;
		case 2:
			ret = jl_taildrop_counter_get(chip_id, &cnt);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_taildrop_counter_get\n");
				return ret;
			}
			break;
		case 3:
			ret = port_flow_control_force_enable_test(chip_id);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_flow_control_force_enable_test\n");
				return ret;
			}
			break;
		case 4:
			ret = port_flow_control_auto_mode_test(chip_id);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_flow_control_auto_mode_test\n");
				return ret;
			}
			break;
		case 5:
			ret = port_flow_control_threshold_test(chip_id);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_flow_control_threshold_test\n");
				return ret;
			}
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
