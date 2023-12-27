#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_lpd.h"

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

void print_lpd_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. Loop Detection configuration test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. Loop Detection status test\n");
}

void lpd_config_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_lpd_config_t lpd_config;
	jl_uint8 srcaddr[ETHER_ADDR_LEN] = {0x66, 0x55, 0x44, 0x33, 0x22, 0x11};
	jl_uint32 i = 0;
	jl_enable_t enable = 0;

	/* Set loop detection configuration with ethtype:0x9988, ttl:15 */
	/* timescale:2(default 2s) and trig_time:2(default 2s) */
	memset(&lpd_config, 0x00, sizeof(jl_lpd_config_t));
	lpd_config.eth_type = 0x9988;
	lpd_config.ttl = 15;
	lpd_config.timescale = 2;
	lpd_config.trig_time = 2;
	memcpy(lpd_config.src_mac.addr, srcaddr, ETHER_ADDR_LEN);
	ret = jl_lpd_config_set(chip_id, &lpd_config);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_config_set error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_config_set success[%d]\n", ret);

	/* Get loop detection configuration */
	memset(&lpd_config, 0x00, sizeof(jl_lpd_config_t));
	ret = jl_lpd_config_get(chip_id, &lpd_config);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_config_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_config_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "lpd_config eth_type : 0x%x\n", lpd_config.eth_type);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "lpd_config ttl : %u\n", lpd_config.ttl);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "lpd_config timescale : %u\n", lpd_config.timescale);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "lpd_config trig_time : %u\n", lpd_config.trig_time);
		for (i = 0; i < ETHER_ADDR_LEN; i++)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "lpd_config src_mac[%u] : 0x%x\n", i, lpd_config.src_mac.addr[i]);
	}

	/* Configure each port to support loop detection */
	for (i = 0; i < JL_PORT_MAX; i++) {
		ret = jl_lpd_port_enable_set(chip_id, i, ENABLED);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_port_enable_set port: %u error[%d]\n", i, ret);
		else
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_port_enable_set port: %u success[%d]\n", i, ret);
	}

	/* Check whether each port supports loop detection */
	for (i = 0; i < JL_PORT_MAX; i++) {
		ret = jl_lpd_port_enable_get(chip_id, i, &enable);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_port_enable_get port: %u error[%d]\n", i, ret);
		else
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port: %u support lpd status : %u\n", i, enable);
	}

	/* Enable the loop detection function */
	ret = jl_lpd_enable_set(chip_id, ENABLED);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_enable_set error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_enable_set success[%d]\n", ret);

	/* Check whether the loop detection function is enabled */
	ret = jl_lpd_enable_get(chip_id, &enable);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_enable_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_enable_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "lpd status : %u\n", enable);
	}
}

void lpd_status_get_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_lpd_port_status_t status = 0;
	jl_portmask_t maskstatus = 0;
	jl_uint32 i = 0;

	/* Get loop detection current status of the each port */
	for (i = 0; i < JL_PORT_MAX; i++) {
		ret = jl_lpd_port_status_get(chip_id, i, &status);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_port_status_get port: %u error[%d]\n", i, ret);
		else
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port: %u lpd portstatus : %u\n", i, status);
	}

	/* Get loop detection history status of the portmask */
	ret = jl_lpd_status_history_get(chip_id, &maskstatus);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_status_history_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_status_history_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "lpd history maskstatus : 0x%x\n", maskstatus);
	}
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char console_input[128] = {0};
	jl_uint32 cmd = 0;
	jl_api_ret_t ret = 0;
	jl_uint32 chip_id = 0;
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
		print_lpd_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n lpd_config_test start!\n");
			lpd_config_test(chip_id);
			break;
		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n lpd_status_get_test start!\n");
			lpd_status_get_test(chip_id);
			break;
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n Test commad: error!\n");
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
