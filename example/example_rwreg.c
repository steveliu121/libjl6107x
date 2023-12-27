#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_rwreg.h"

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

void print_rwreg_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index,value>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. reg io write test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. reg io read test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. reg io burst write test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. reg io burst read test\n");
}

void reg_io_write_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_uint32 val = 0x12345678;

	/* Write top scratch field register value */
	ret = jl_reg_io_write(chip_id, (jl_uint32)0x002000E0U, val);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_reg_io_write value error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_reg_io_write value success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "core version write : 0x%x\n", val);
	}
}

void reg_io_read_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_uint32 val = 0;

	/* Read top scratch field register value */
	ret = jl_reg_io_read(chip_id, (jl_uint32)0x002000E0U, &val);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_reg_io_read value error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_reg_io_read value success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "core version read : 0x%x\n", val);
	}
}

void reg_io_burst_write_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_uint32 val[2] = {0x5a5a5a5a, 0x6b6b6b6b};
	jl_uint32 size = 2;

	/* Write sw core scratch field register value */
	ret = jl_reg_io_burst_write(chip_id, (jl_uint32)0x0010000CU, val, size);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_reg_io_burst_write value error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_reg_io_burst_write value success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "scratch field write [0]: 0x%x, scratch field write [1]: 0x%x\n", val[0], val[1]);
	}
}

void reg_io_burst_read_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_uint32 val[2] = {0};
	jl_uint32 size = 2;

	/* Read sw core scratch field register value */
	ret = jl_reg_io_burst_read(chip_id, (jl_uint32)0x0010000CU, val, size);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_reg_io_burst_read value error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_reg_io_burst_read value success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "scratch field read [0]: 0x%x, scratch field read [1]: 0x%x\n", val[0], val[1]);
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
		print_rwreg_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n reg_io_write_test start!\n");
			reg_io_write_test(chip_id);
			break;
		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n reg_io_read_test start!\n");
			reg_io_read_test(chip_id);
			break;
		case 3:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n reg_io_burst_write_test start!\n");
			reg_io_burst_write_test(chip_id);
			break;
		case 4:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n reg_io_burst_read_test start!\n");
			reg_io_burst_read_test(chip_id);
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
