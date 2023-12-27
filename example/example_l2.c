#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
//#include <getopt.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_error.h"
#include "jl_l2.h"

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
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK L2 TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. jl_l2_table_set()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. jl_l2_table_search()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. jl_l2_table_clear()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. jl_l2_hash_col_table_set/get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "5. jl_l2_set/get_agetime()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "6. jl_l2_mc_table_set/get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "7. jl_l2_mc_handle_set()\n");
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char console_input[128] = {0};
	jl_uint32 cmd;
	jl_api_ret_t ret;
	jl_uint32 chip_id = 0;
	jl_uint32 index = 0;
	jl_uint32 time = 300000;
	jl_uint32 portmsk = 3;
	jl_uint32 idx = 0;
	jl_l2_mc_handle_t handle = JL_L2_MC_HANDLE_INC_L2MC;
	jl_bool flag = TRUE;

	jl_l2_sel_mode_t mode;
	jl_l2_tbl_t l2_tbl_cfg;

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
			l2_tbl_cfg.valid = TRUE;
			l2_tbl_cfg.stat = FALSE;
			l2_tbl_cfg.mac.addr[0] = 0x0;
			l2_tbl_cfg.mac.addr[1] = 0x11;
			l2_tbl_cfg.mac.addr[2] = 0x22;
			l2_tbl_cfg.mac.addr[3] = 0x33;
			l2_tbl_cfg.mac.addr[4] = 0x44;
			l2_tbl_cfg.mac.addr[5] = 0x55;
			l2_tbl_cfg.fid = 100;
			l2_tbl_cfg.type = JL_L2_TBL_TYPE_UC;
			l2_tbl_cfg.sportval = 0;

			ret = jl_l2_table_set(chip_id, &l2_tbl_cfg);
			if (JL_ERR_FULL == ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Table full.\n");
			else if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");
			break;

		case 2:
			mode = JL_L2_SEL_MODE_SPECIFIC;
			l2_tbl_cfg.mac.addr[0] = 0x0;
			l2_tbl_cfg.mac.addr[1] = 0x11;
			l2_tbl_cfg.mac.addr[2] = 0x22;
			l2_tbl_cfg.mac.addr[3] = 0x33;
			l2_tbl_cfg.mac.addr[4] = 0x44;
			l2_tbl_cfg.mac.addr[5] = 0x55;
			l2_tbl_cfg.fid = 100;

			ret = jl_l2_table_search(chip_id, mode, &l2_tbl_cfg);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Search end\n");
			else if (JL_ERR_EXISTS == ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get value\n");
			/*Add customer processing code*/
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 3:
			mode = JL_L2_SEL_MODE_SPECIFIC;
			l2_tbl_cfg.mac.addr[0] = 0x0;
			l2_tbl_cfg.mac.addr[1] = 0x11;
			l2_tbl_cfg.mac.addr[2] = 0x22;
			l2_tbl_cfg.mac.addr[3] = 0x33;
			l2_tbl_cfg.mac.addr[4] = 0x44;
			l2_tbl_cfg.mac.addr[5] = 0x55;
			l2_tbl_cfg.fid = 100;

			ret = jl_l2_table_clear(chip_id, mode, &l2_tbl_cfg);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 4:
			l2_tbl_cfg.valid = TRUE;
			l2_tbl_cfg.stat = TRUE;
			l2_tbl_cfg.mac.addr[0] = 0x01;
			l2_tbl_cfg.mac.addr[1] = 0x0;
			l2_tbl_cfg.mac.addr[2] = 0x5e;
			l2_tbl_cfg.mac.addr[3] = 0x01;
			l2_tbl_cfg.mac.addr[4] = 0x01;
			l2_tbl_cfg.mac.addr[5] = 0x01;
			l2_tbl_cfg.fid = 100;
			l2_tbl_cfg.type = JL_L2_TBL_TYPE_MC;
			l2_tbl_cfg.sportval = 0x3;

			ret = jl_l2_hash_col_table_set(chip_id, index, &l2_tbl_cfg);
			if (JL_ERR_FULL == ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Table full.\n");
			else if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_l2_hash_col_table_get(chip_id, index, &l2_tbl_cfg);

			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 5:
			ret = jl_l2_set_agetime(chip_id, time);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_l2_get_agetime(chip_id, &time);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 6:

			ret = jl_l2_mc_table_set(chip_id, idx, portmsk);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");


			ret = jl_l2_mc_table_get(chip_id, idx, &portmsk);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 7:

			ret = jl_l2_mc_handle_set(chip_id, handle, flag);
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
