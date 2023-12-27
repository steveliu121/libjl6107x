#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_mirror.h"

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

void print_mirror_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. mirror input test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. mirror output test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "q. exit test\n");
}

void mirror_input_test(jl_uint32 chip_id)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "====== set input mirror start ======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "==== mirrored port is 0, mirroring port is 1 ====\n");

	jl_mirror_input_mr_set(chip_id, UTP_PORT0, UTP_PORT1);
}

void mirror_output_test(jl_uint32 chip_id)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "====== set output mirror start ======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "==== mirrored port is 0, mirroring port is 1 ====\n");

	jl_mirror_output_mr_set(chip_id, UTP_PORT0, UTP_PORT1);
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_api_ret_t ret;
	jl_uint32 cmd = 0;
	char console_input[128] = {0};
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

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);

	while (!g_exit) {
		print_mirror_usage();
		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();
		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			mirror_input_test(chip_id);
			break;
		case 2:
			mirror_output_test(chip_id);
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

