#include <stdio.h>
#include <signal.h>
#include <string.h>
#include "jl_switch.h"
#include "jl_trunk.h"

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

void print_trunk_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. add link aggregation group test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. reset link aggregation hash map test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. reset link aggregation group test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. clear link aggregation groups and disable link aggregation test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "q. exit test\n");
}

static int trunk_add_test(jl_uint32 chip_id)
{
	jl_la_group_t la_id;
	jl_portmask_t lportmask;
	jl_uint32 la_hashfactor_bitmask;
	jl_api_ret_t ret;

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "===== set link aggregation start ===== \n");
	/*clear la configuration*/
	jl_link_aggregation_enable_set(chip_id, DISABLED);

	/*set la 0 portmask*/
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "set logic port 0,1,2 to link aggregation group 0\n");
	la_id = 0;
	lportmask = 0x07;

	ret = jl_link_aggregation_portmask_set(chip_id, la_id, lportmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation portmask set fail\n");
		return -1;
	}

	ret = jl_link_aggregation_portmask_get(chip_id, la_id, &lportmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation portmask get fail\n");
		return -1;
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation: %d portmask: 0x%x\n", la_id, lportmask);
	}

	/*set la 3 portmask*/
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "set logic port 3,4,5 to link aggregation group 3\n");
	la_id = 3;
	lportmask = 0x38;

	ret = jl_link_aggregation_portmask_set(chip_id, la_id, lportmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation portmask set fail\n");
		return -1;
	}

	ret = jl_link_aggregation_portmask_get(chip_id, la_id, &lportmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation portmask get fail\n");
		return -1;
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation: %d portmask: 0x%x\n", la_id, lportmask);
	}

	/* set hash factor */
	la_hashfactor_bitmask = 0x3f;
	ret = jl_link_aggregation_hashfactor_set(chip_id, la_hashfactor_bitmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation hashfactor set fail\n");
		return -1;
	}
	ret = jl_link_aggregation_hashfactor_get(chip_id, &la_hashfactor_bitmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation hashfactor get fail\n");
		return -1;
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation: %d hashfactor: 0x%x\n", la_id, la_hashfactor_bitmask);
	}

	/* enable link aggregation */
	ret = jl_link_aggregation_enable_set(chip_id, ENABLED);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation enable set fail\n");
		return -1;
	}
	ret = jl_link_aggregation_portmask_get(chip_id, la_id, &lportmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation portmask get fail\n");
		return -1;
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation: %d portmask: 0x%x\n", la_id, lportmask);
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "=== Confirm the one to one mapping result for port6,7 after enabling link aggregation  ==\n");

	la_id = 6;
	lportmask = 0;
	ret = jl_link_aggregation_portmask_get(chip_id, la_id, &lportmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation portmask get fail\n");
		return -1;
	} else {
		if ((jl_int32)lportmask != 1 << la_id)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "one to one mapping error: %d haven't been bet set to the %d link aggregation!\n", la_id, la_id);
		else
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "one to one mapping ok: port %d have been bet set to link aggregation %d!\n", la_id, la_id);
	}

	la_id = 7;
	lportmask = 0;
	ret = jl_link_aggregation_portmask_get(chip_id, la_id, &lportmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation portmask get fail\n");
		return -1;
	} else {
		if ((jl_int32)lportmask != 1 << la_id)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "one to one mapping error: la id %d portmask is %d\n", la_id, lportmask);
		else
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "one to one mapping ok: port %d have been bet set to link aggregation %d!\n", la_id, la_id);
	}

	return 0;
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_api_ret_t ret;
	jl_uint32 cmd = 0;
	jl_uint32 chip_id = 0;
	jl_portmask_t la_portmask;
	jl_la_group_t la_id;
	char console_input[128] = {0};
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
		print_trunk_usage();
		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			trunk_add_test(chip_id);
			break;

		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "reset la 0 hash map from portmask 0x07 to portmask 0x03\n");
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port0,1 will share the traffic equally, port2 won't receive any traffic\n");
			la_portmask = 0x07;
			ret = jl_link_aggregation_hash_map_clear(chip_id, 0, la_portmask);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_link_aggregation_hash_map_clear fail\n");
				return -1;
			}
			la_portmask = 0x03;
			ret = jl_link_aggregation_hash_map_set(chip_id, 0, la_portmask);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_link_aggregation_hash_map_set fail\n");
				return -1;
			}
			break;

		case 3:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "delete port 5 in la 3\n");
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "port3,4 will share the traffic equally, port5 will receive all traffic\n");
			/*clear la_id 3*/
			la_id = 3;
			la_portmask = 0x18;
			ret = jl_link_aggregation_portmask_set(chip_id, la_id, la_portmask);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_link_aggregation_port_del fail\n");
				return -1;
			}
			ret = jl_link_aggregation_portmask_get(chip_id, la_id, &la_portmask);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "link aggregation portmask get fail\n");
				return -1;
			} else if (la_portmask != 0x18) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Error! link aggregation: %d portmask is 0x%x, it should be 0x18\n", la_id, la_portmask);
				return -1;
			} else {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Correct! link aggregation: %d portmask: 0x%x\n", la_id, la_portmask);
			}
			break;

		case 4:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "disable link aggregation and clear all link aggregation configuration\n");
			ret = jl_link_aggregation_enable_set(chip_id, DISABLED);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_link_aggregation_enable_set fail\n");
				return -1;
			}
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
