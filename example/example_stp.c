#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_stp.h"

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

void print_stp_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. ingress spanning tree test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. egress spanning tree test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. ingress multiple spanning tree test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. egress multiple spanning tree test\n");
}

void ingress_stp_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_stp_state_t stp_state = 0;
	jl_uint32 counter = 0;

	/* Configure ingress spanning tree state:blocking for UTP_PORT0 */
	ret = jl_stp_port_ingress_state_set(chip_id, UTP_PORT0, STP_STATE_BLOCKING);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_ingress_state_set UTP_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_ingress_state_set UTP_PORT0 success[%d]\n", ret);

	/* Get ingress spanning tree state for UTP_PORT0 */
	ret = jl_stp_port_ingress_state_get(chip_id, UTP_PORT0, &stp_state);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_ingress_state_get UTP_PORT0 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_ingress_state_get UTP_PORT0 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "stp_state : %u\n", stp_state);
	}

	/* Get drop counter due to the specified ingress stp state:blocking */
	ret = jl_stp_ingress_state_drop_get(chip_id, STP_STATE_BLOCKING, &counter);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_ingress_state_drop_get STP_STATE_BLOCKING error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_ingress_state_drop_get STP_STATE_BLOCKING success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "counter : %u\n", counter);
	}

	/* Clear drop counter due to the specified ingress stp state:blocking */
	ret = jl_stp_ingress_state_drop_clear(chip_id, STP_STATE_BLOCKING);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_ingress_state_drop_clear STP_STATE_BLOCKING error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_ingress_state_drop_clear STP_STATE_BLOCKING success[%d]\n", ret);
}

void egress_stp_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_stp_state_t stp_state = 0;
	jl_uint32 counter = 0;

	/* Configure egress spanning tree state:learning for UTP_PORT1 */
	ret = jl_stp_port_egress_state_set(chip_id, UTP_PORT1, STP_STATE_LEARNING);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_egress_state_set UTP_PORT1 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_egress_state_set UTP_PORT1 success[%d]\n", ret);

	/* Get egress spanning tree state for UTP_PORT1 */
	ret = jl_stp_port_egress_state_get(chip_id, UTP_PORT1, &stp_state);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_egress_state_get UTP_PORT1 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_egress_state_get UTP_PORT1 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "stp_state : %u\n", stp_state);
	}

	/* Get UTP_PORT1 drop counter due to egress spanning tree state */
	ret = jl_stp_port_egress_drop_get(chip_id, UTP_PORT1, &counter);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_egress_drop_get UTP_PORT1 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_egress_drop_get UTP_PORT1 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "counter : %u\n", counter);
	}

	/* Clear UTP_PORT1 drop counter due to egress spanning tree state */
	ret = jl_stp_port_egress_drop_clear(chip_id, UTP_PORT1);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_egress_drop_clear UTP_PORT1 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_egress_drop_clear UTP_PORT1 success[%d]\n", ret);
}

void ingress_mstp_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_mstp_state_t mstp_state = 0;
	jl_uint32 counter = 0;
	jl_mstp_msti_t msti = 1;
	jl_uint16 vid = 100;

	/* Configure multiple spanning tree mapping between vid:100 and msti:1 */
	ret = jl_mstp_vlan_msti_set(chip_id, vid, msti);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_vlan_msti_set error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_vlan_msti_set success[%d]\n", ret);

	/* Get multiple spanning tree msti from vid:100 */
	msti = 0;
	ret = jl_mstp_vlan_msti_get(chip_id, vid, &msti);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_vlan_msti_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_vlan_msti_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "vid: %u multiple spanning tree instance : %u\n", vid, msti);
	}

	/* Configure ingress multiple spanning tree state:discarding for UTP_PORT2 */
	ret = jl_mstp_port_ingress_state_set(chip_id, msti, UTP_PORT2, MSTP_STATE_DISCARDING);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_ingress_state_set UTP_PORT2 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_ingress_state_set UTP_PORT2 success[%d]\n", ret);

	/* Get ingress multiple spanning tree state for UTP_PORT2 */
	ret = jl_mstp_port_ingress_state_get(chip_id, msti, UTP_PORT2, &mstp_state);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_ingress_state_get UTP_PORT2 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_port_ingress_state_get UTP_PORT2 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "mstp_state : %u\n", mstp_state);
	}

	/* Get drop counter due to the specified ingress mstp state:discarding */
	ret = jl_mstp_ingress_state_drop_get(chip_id, MSTP_STATE_DISCARDING, &counter);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_ingress_state_drop_get MSTP_STATE_DISCARDING error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_stp_ingress_state_drop_get MSTP_STATE_DISCARDING success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "counter : %u\n", counter);
	}

	/* Clear drop counter due to the specified ingress mstp state:discarding */
	ret = jl_mstp_ingress_state_drop_clear(chip_id, MSTP_STATE_DISCARDING);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_ingress_state_drop_clear MSTP_STATE_DISCARDING error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_ingress_state_drop_clear MSTP_STATE_DISCARDING success[%d]\n", ret);
}

void egress_mstp_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_mstp_state_t mstp_state = 0;
	jl_uint32 counter = 0;
	jl_mstp_msti_t msti = 2;
	jl_uint16 vid = 200;

	/* Configure multiple spanning tree mapping between vid:200 and msti:2 */
	ret = jl_mstp_vlan_msti_set(chip_id, vid, msti);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_vlan_msti_set error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_vlan_msti_set success[%d]\n", ret);

	/* Get multiple spanning tree msti from vid:200 */
	msti = 0;
	ret = jl_mstp_vlan_msti_get(chip_id, vid, &msti);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_vlan_msti_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_vlan_msti_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "vid: %u multiple spanning tree instance : %u\n", vid, msti);
	}

	/* Configure egress multiple spanning tree state:learning for UTP_PORT3 */
	ret = jl_mstp_port_egress_state_set(chip_id, msti, UTP_PORT3, MSTP_STATE_LEARNING);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_egress_state_set UTP_PORT3 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_egress_state_set UTP_PORT3 success[%d]\n", ret);

	/* Get egress multiple spanning tree state for UTP_PORT3 */
	ret = jl_mstp_port_egress_state_get(chip_id, msti, UTP_PORT3, &mstp_state);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_egress_state_get UTP_PORT3 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_egress_state_get UTP_PORT3 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "mstp_state : %u\n", mstp_state);
	}

	/* Get UTP_PORT3 drop counter due to egress multiple spanning tree state */
	ret = jl_mstp_port_egress_drop_get(chip_id, UTP_PORT3, &counter);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_egress_drop_get UTP_PORT3 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_egress_drop_get UTP_PORT3 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "counter : %u\n", counter);
	}

	/* Clear UTP_PORT3 drop counter due to egress multiple spanning tree state */
	ret = jl_mstp_port_egress_drop_clear(chip_id, UTP_PORT3);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_egress_drop_clear UTP_PORT3 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_mstp_port_egress_drop_clear UTP_PORT3 success[%d]\n", ret);
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
		print_stp_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n ingress_stp_test start!\n");
			ingress_stp_test(chip_id);
			break;
		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n egress_stp_test start!\n");
			egress_stp_test(chip_id);
			break;
		case 3:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n ingress_mstp_test start!\n");
			ingress_mstp_test(chip_id);
			break;
		case 4:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n egress_mstp_test start!\n");
			egress_mstp_test(chip_id);
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
