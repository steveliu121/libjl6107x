#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
//#include <getopt.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_types.h"
#include "jl_cpu.h"

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
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK CPU TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index,value>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. jl_cpu_allow_bpdu_enable_set(value)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. jl_cpu_allow_rst_bpdu_enable_set(value)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. jl_cpu_tag_enable_set(value)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. jl_cpu_allow_lldp_enable_set(value)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "5. jl_cpu_rma_action_set(value,forward)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "6. jl_cpu_rma_action_set(value,drop)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "7. jl_cpu_rma_action_set(value,send to cpu)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "8. jl_cpu_cpumac_set(value)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "9. jl_cpu_detection_cpumac_enable_set(value)\n");
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char console_input[128] = {0};
	jl_uint32 cmd;
	jl_uint32 value;
	char value_str[128] = {0};
	jl_api_ret_t ret;
	jl_enable_t enable =  DISABLED;
	jl_uint32 chip_id = 0;
	char *pstr = NULL;
	jl_mac_addr_t cpu_port_mac_addr = {0};

	jl_dev_t dev_6107 = {
		.compat_id = JL_CHIP_6107,
		.name = "device-jl6107",
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

	ret = jl_switch_device_create(&dev_6107);
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

		clear_window();

		sscanf(console_input, "%d,%s", &cmd, value_str);
		if (!strncmp(value_str, "0x", 2))
			value = strtoul(value_str, NULL, 16);
		else
			value = strtoul(value_str, NULL, 10);

		switch (cmd) {
		case 1:
			ret = jl_cpu_allow_bpdu_enable_set(0, (jl_enable_t)value);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_allow_bpdu_enable_set return err %d\n", ret);
			else {
				ret = jl_cpu_allow_bpdu_enable_get(0, &enable);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_allow_bpdu_enable_get return err %d,enable = %d\n", ret, enable);
			}
			break;

		case 2:
			ret = jl_cpu_allow_rst_bpdu_enable_set(0, (jl_enable_t)value);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_allow_rst_bpdu_enable_set return err %d\n", ret);
			else {
				ret = jl_cpu_allow_bpdu_enable_get(0, &enable);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_allow_rst_bpdu_enable_get return err %d,enable = %d\n", ret, enable);
			}
			break;

		case 3:
			ret = jl_cpu_tag_enable_set(0, (jl_enable_t)value);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_tag_enable_set return err %d\n", ret);
			else {
				ret = jl_cpu_tag_enable_get(0, &enable);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_tag_enable_get return err %d,enable = %d\n", ret, enable);
			}
			break;

		case 4:
			ret = jl_cpu_allow_lldp_enable_set(0, (jl_enable_t)value);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_allow_lldp_enable_set return err %d\n", ret);
			else {
				ret = jl_cpu_allow_lldp_enable_get(0, &enable);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_allow_lldp_enable_get return err %d,enable = %d\n", ret, enable);
			}
			break;

		case 5:
			ret = jl_cpu_rma_action_set(0, value, RMA_ACTION_FORWARD);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_rma_action_set return err %d\n", ret);
			else {
				jl_rma_action_t action = {0};

				ret = jl_cpu_rma_action_get(0, value, &action);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_rma_action_get return err %d,action = %d\n", ret, action);
			}
			break;

		case 6:
			ret = jl_cpu_rma_action_set(0, value, RMA_ACTION_DROP);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_rma_action_set return err %d\n", ret);
			else {
				jl_rma_action_t action = {0};

				ret = jl_cpu_rma_action_get(0, value, &action);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_rma_action_get return err %d,action = %d\n", ret, action);
			}
			break;

		case 7:
			ret = jl_cpu_rma_action_set(0, value, RMA_ACTION_SEND2CPU);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_rma_action_set return err %d\n", ret);
			else {
				jl_rma_action_t action = {0};

				ret = jl_cpu_rma_action_get(0, value, &action);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_rma_action_get return err %d,action = %d\n", ret, action);
			}
			break;

		case 8:
			/*set cpu mac address = 12:34:56:78:9a:bc*/

			cpu_port_mac_addr.addr[0] = 0x12;
			cpu_port_mac_addr.addr[1] = 0x34;
			cpu_port_mac_addr.addr[2] = 0x56;
			cpu_port_mac_addr.addr[3] = 0x78;
			cpu_port_mac_addr.addr[4] = 0x9a;
			cpu_port_mac_addr.addr[5] = 0xbc;

			ret = jl_cpu_cpumac_set(0, cpu_port_mac_addr);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_cpumac_set return err %d\n", ret);
			else {
				jl_mac_addr_t cpu_mac_addr = {0};

				ret = jl_cpu_cpumac_get(0, &cpu_mac_addr);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
					"jl_cpu_cpumac_get return err %d,cpu_mac_addr:%x:%x:%x:%x:%x:%x\n",
					ret, cpu_mac_addr.addr[0], cpu_mac_addr.addr[1],
					cpu_mac_addr.addr[2], cpu_mac_addr.addr[3],
					cpu_mac_addr.addr[4], cpu_mac_addr.addr[5]);
			}
			break;
		case 9:
			ret = jl_cpu_detection_cpumac_enable_set(0, value);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_detection_cpumac_enable_set return err %d\n", ret);
			else {
				jl_enable_t enable = 0;

				ret = jl_cpu_detection_cpumac_enable_get(0, &enable);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_cpu_detection_cpumac_enable_get return err %d,enable = %d\n", ret, enable);
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
