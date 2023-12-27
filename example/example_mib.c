#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_mib.h"

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

void print_mib_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. one type mib test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. all types mib test\n");
}

void one_type_mib_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_uint64 counter = 0;

	/* Set UTP_PORT0 mib counter mode as not read-clear */
	ret = jl_mib_port_rc_mode_set(chip_id, UTP_PORT0, 0);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_rc_mode_set UTP_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_rc_mode_set UTP_PORT0 success[%d]\n", ret);

	/* Get number of received valid unicast packets of UTP_PORT0 */
	ret = jl_mib_port_get(chip_id, UTP_PORT0, PORT_MIB_RX_PKT_UC_CNT, &counter);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_get UTP_PORT0 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_get UTP_PORT0 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "type PORT_MIB_RX_PKT_UC_CNT counter : %llu\n", counter);
	}
	/* Clear number of received valid unicast packets of UTP_PORT0 */
	ret = jl_mib_port_clear(chip_id, UTP_PORT0, PORT_MIB_RX_PKT_UC_CNT);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_clear UTP_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_clear UTP_PORT0 success[%d]\n", ret);
	/* Get number of received valid unicast packets of UTP_PORT0 */
	ret = jl_mib_port_get(chip_id, UTP_PORT0, PORT_MIB_RX_PKT_UC_CNT, &counter);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_get UTP_PORT0 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_get UTP_PORT0 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "type rx_pkt_uc_cnt counter : %llu\n", counter);
	}
}

void all_types_mib_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_port_mib_cnt_t port_cnt;

	/* Set UTP_PORT1 mib counter mode as not read-clear */
	ret = jl_mib_port_rc_mode_set(chip_id, UTP_PORT1, 0);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_rc_mode_set UTP_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_rc_mode_set UTP_PORT0 success[%d]\n", ret);

	/* Get all kinds of MIB counters of UTP_PORT1 */
	memset(&port_cnt, 0x00, sizeof(jl_port_mib_cnt_t));
	ret = jl_mib_port_get_all(chip_id, UTP_PORT1, &port_cnt);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_get_all UTP_PORT1 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_get_all UTP_PORT1 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rc_enable : %u\n", port_cnt.rc_enable);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_cnt : %llu\n", port_cnt.tx_pkt_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_byte_cnt : %llu\n", port_cnt.tx_pkt_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_uc_cnt : %llu\n", port_cnt.tx_pkt_uc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_uc_byte_cnt : %llu\n", port_cnt.tx_pkt_uc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_mc_cnt : %llu\n", port_cnt.tx_pkt_mc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_mc_byte_cnt : %llu\n", port_cnt.tx_pkt_mc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_bc_cnt : %llu\n", port_cnt.tx_pkt_bc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_bc_byte_cnt : %llu\n", port_cnt.tx_pkt_bc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_drp_cnt : %u\n", port_cnt.tx_pkt_drp_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_err_cnt : %u\n", port_cnt.tx_pkt_err_cnt);

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_cnt : %llu\n", port_cnt.rx_pkt_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_byte_cnt : %llu\n", port_cnt.rx_pkt_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_uc_cnt : %llu\n", port_cnt.rx_pkt_uc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_uc_byte_cnt : %llu\n", port_cnt.rx_pkt_uc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_mc_cnt : %llu\n", port_cnt.rx_pkt_mc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_mc_byte_cnt : %llu\n", port_cnt.rx_pkt_mc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_bc_cnt : %llu\n", port_cnt.rx_pkt_bc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_bc_byte_cnt : %llu\n", port_cnt.rx_pkt_bc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_drp_cnt : %u\n", port_cnt.rx_pkt_drp_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_crc_err_cnt : %u\n", port_cnt.rx_pkt_crc_err_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_frag_err_cnt : %u\n", port_cnt.rx_frag_err_cnt);
	}
	/* Clear all kinds of MIB counters of UTP_PORT1 */
	ret = jl_mib_port_clear_all(chip_id, UTP_PORT1);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_clear_all UTP_PORT1 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_clear_all UTP_PORT1 success[%d]\n", ret);
	/* Get all kinds of MIB counters of UTP_PORT1 */
	ret = jl_mib_port_get_all(chip_id, UTP_PORT1, &port_cnt);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_get_all UTP_PORT1 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mib_port_get_all UTP_PORT1 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rc_enable : %u\n", port_cnt.rc_enable);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_cnt : %llu\n", port_cnt.tx_pkt_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_byte_cnt : %llu\n", port_cnt.tx_pkt_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_uc_cnt : %llu\n", port_cnt.tx_pkt_uc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_uc_byte_cnt : %llu\n", port_cnt.tx_pkt_uc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_mc_cnt : %llu\n", port_cnt.tx_pkt_mc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_mc_byte_cnt : %llu\n", port_cnt.tx_pkt_mc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_bc_cnt : %llu\n", port_cnt.tx_pkt_bc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_bc_byte_cnt : %llu\n", port_cnt.tx_pkt_bc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_drp_cnt : %u\n", port_cnt.tx_pkt_drp_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt tx_pkt_err_cnt : %u\n", port_cnt.tx_pkt_err_cnt);

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_cnt : %llu\n", port_cnt.rx_pkt_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_byte_cnt : %llu\n", port_cnt.rx_pkt_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_uc_cnt : %llu\n", port_cnt.rx_pkt_uc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_uc_byte_cnt : %llu\n", port_cnt.rx_pkt_uc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_mc_cnt : %llu\n", port_cnt.rx_pkt_mc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_mc_byte_cnt : %llu\n", port_cnt.rx_pkt_mc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_bc_cnt : %llu\n", port_cnt.rx_pkt_bc_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_bc_byte_cnt : %llu\n", port_cnt.rx_pkt_bc_byte_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_drp_cnt : %u\n", port_cnt.rx_pkt_drp_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_pkt_crc_err_cnt : %u\n", port_cnt.rx_pkt_crc_err_cnt);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port_cnt rx_frag_err_cnt : %u\n", port_cnt.rx_frag_err_cnt);
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
		print_mib_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n one_type_mib_test start!\n");
			one_type_mib_test(chip_id);
			break;
		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n all_types_mib_test start!\n");
			all_types_mib_test(chip_id);
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
