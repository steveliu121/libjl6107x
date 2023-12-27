#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
//#include <getopt.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_types.h"
#include "jl_interrupt.h"
#include "jl_port.h"
#include "jl_lpd.h"

static int g_exit;

static void sig_handle(int sig)
{
	(void)sig;
	g_exit = 1;
}

static void clear_window(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\033c");
}

void testcase_int_show(void)
{
	jl_api_ret_t ret;
	jl_enable_t enable = DISABLED;
	jl_port_t port = 0;
	jl_int_polarity_t polarity = INT_POLAR_HIGH;
	jl_uint32 status = 0;
	jl_int_status_t statusmask = { 0 };
	jl_int_info_t info = { 0 };
	jl_int_type_t type = 0;

	ret = jl_interrupt_polarity_get(0, &polarity);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "int polarity.return err %d, polarity = %d\n", ret, polarity);

	for (port = 0; port < JL_PORT_MAX; port++) {
		enable = 0;
		ret = jl_interrupt_port_phy_int_enable_get(0, port, &enable);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "int_phy_enable return err %d, port %d, enable = %d\n", ret, port,
			   enable);
	}
	for (port = 0; port < JL_PORT_MAX; port++) {
		status = 0;
		ret = jl_interrupt_port_phy_int_status_get(0, port, &status);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "int_phy_status_get return err %d, port %d, status = %d\n", ret,
			   port, status);
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "type 0: INT_PORT_LINKDOWN; 1: INT_PORT_LINKUP; 2: INT_TYPE_LOOP_DETECT\n");
	for (type = 0; type < INT_TYPE_END; type++) {
		enable = 0;
		ret = jl_interrupt_enable_get(0, type, &enable);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_interrupt_enable_get ret %d,type %d, enable=%d\n", ret, type,
			   enable);
	}

	ret = jl_interrupt_status_get(0, &statusmask);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_interrupt_status_get ret err %d,statusmask= 0x%x.\n", ret,
		   statusmask.mask);

	for (type = 0; type < INT_TYPE_END; type++) {
		info.port_mask = 0;
		ret = jl_interrupt_detail_get(0, type, &info);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_interrupt_detail_get ret err %d,type %d,info.port_mask=0x%x\n",
			   ret, type, info.port_mask);
	}
}

void config_mac_port_extport0(void)
{
	jl_port_mac_ability_t macability;
	//jl_mode_ext_t mode = 0;
	jl_api_ret_t ret = 0;

	/* Force the MAC of EXT_PORT0 working with MODE_EXT_RMII_PHY 100F */
	/* and Symmetric PAUSE flow control abilities */
	memset(&macability, 0x00, sizeof(jl_port_mac_ability_t));
	macability.force_mode = 1;
	macability.speed = 1;
	macability.duplex = 1;
	macability.link = 1;
	macability.tx_pause = 1;
	macability.rx_pause = 1;
	ret = jl_port_mac_force_link_ext_set(0, EXT_PORT0, MODE_EXT_RMII_PHY, &macability);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_mac_force_link_ext_set EXT_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_mac_force_link_ext_set EXT_PORT0 success[%d]\n", ret);

	/* Get MAC status of EXT_PORT0 */
	memset(&macability, 0x00, sizeof(jl_port_mac_ability_t));
	ret = jl_port_mac_status_get(0, EXT_PORT0, &macability);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_mac_status_get EXT_PORT0 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_mac_status_get EXT_PORT0 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability force mode : %d\n", macability.force_mode);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability speed : %d\n", macability.speed);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability duplex : %d\n", macability.duplex);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability linkstatus : %d\n", macability.link);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability tx_pause : %d\n", macability.tx_pause);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability rx_pause : %d\n", macability.rx_pause);
	}
}

void config_loop_dect(void)
{
	jl_api_ret_t ret = 0;
	jl_lpd_config_t lpd_config;
	jl_uint8 srcaddr[ETHER_ADDR_LEN] = { 0x66, 0x55, 0x44, 0x33, 0x22, 0x11 };
	jl_uint32 i = 0;

	/* Set loop detection configuration with ethtype:0x9988, ttl:15 */
	/* timescale:2(default 2s) and trig_time:2(default 2s) */
	memset(&lpd_config, 0x00, sizeof(jl_lpd_config_t));
	lpd_config.eth_type = 0x9988;
	lpd_config.ttl = 15;
	lpd_config.timescale = 2;
	lpd_config.trig_time = 2;
	memcpy(lpd_config.src_mac.addr, srcaddr, ETHER_ADDR_LEN);
	ret = jl_lpd_config_set(0, &lpd_config);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_config_set error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_config_set success[%d]\n", ret);

	/* Configure each port to support loop detection */
	for (i = 0; i < JL_PORT_MAX; i++) {
		ret = jl_lpd_port_enable_set(0, i, ENABLED);
		if (ret)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_port_enable_set port: %u error[%d]\n", i, ret);
		else
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_port_enable_set port: %u success[%d]\n", i, ret);
	}

	/* Enable the loop detection function */
	ret = jl_lpd_enable_set(0, ENABLED);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_enable_set error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_lpd_enable_set success[%d]\n", ret);
}

void testcase_interrupt(void)
{
	jl_api_ret_t ret;
	jl_port_t port = 0;
	jl_int_type_t type = 0;

	config_mac_port_extport0();
	config_loop_dect();

	//open all ports int enable
	for (port = 0; port < JL_PORT_MAX; port++) {
		ret = jl_interrupt_port_phy_int_enable_set(0, port, ENABLED);
		if (ret != 0)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "int_phy_enable_set return err %d,port %d\n", ret, port);
	}
	for (type = 0; type < INT_TYPE_END; type++) {
		ret = jl_interrupt_enable_set(0, type, ENABLED);
		if (ret != 0)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_interrupt_enable_set return err %d,type %d\n", ret,
				   type);
	}

	testcase_int_show();
}

void print_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK INTERRUPT TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index,value,value>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. jl_interrupt_polarity_set(polarity)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. jl_interrupt_port_phy_int_enable_set(port, enable)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. jl_interrupt_port_phy_int_status_get(port)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. jl_interrupt_enable_set(type, enable)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "5. jl_interrupt_status_get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "6. jl_interrupt_detail_get(type)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "7. jl_interrupt_status_clean(type)\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "8. show all interrupt info\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "9. testcase_interrupt()\n");

}

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	char console_input[128] = { 0 };
	jl_uint32 cmd;
	jl_uint32 value;
	jl_uint32 value2;
	//char value_str[128] = {0};
	//char value_str2[128] = {0};
	jl_api_ret_t ret;
	jl_enable_t enable = DISABLED;
	jl_uint32 chip_id = 0;
	char *pstr = NULL;
	jl_int_polarity_t polarity = INT_POLAR_HIGH;
	jl_uint32 int_status = 0;
	jl_int_status_t statusmask = { 0 };
	jl_int_info_t info = { 0 };

	jl_dev_t dev_6107 = {
		.compat_id = JL_CHIP_6107S,
		.name = "device-jl6107S",
		.id = chip_id,	/* must be less than JL_MAX_CHIP_NUM */
		.io = {
		       .type = JL_IO_SMI,
		       .smi.mdio.bus_id = 0}
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
		sscanf(console_input, "%d,%d,%d,", &cmd, &value, &value2);

		switch (cmd) {
		case 1:
			ret = jl_interrupt_polarity_get(0, &polarity);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "get default int polarity.return err %d, polarity = %d\n",
				   ret, polarity);

			ret = jl_interrupt_polarity_set(0, (jl_int_polarity_t) value);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_interrupt_polarity_set return err %d\n", ret);
			else {
				ret = jl_interrupt_polarity_get(0, &polarity);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
					   "jl_interrupt_polarity_get return err %d,polarity = %d\n", ret, polarity);
			}
			break;

		case 2:
			ret = jl_interrupt_port_phy_int_enable_get(0, (jl_port_t) value, &enable);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
				   "Default,int_phy_enable_get return err %d, port %d, enable %d.\n", ret, value,
				   enable);
			enable = 0;
			ret = jl_interrupt_port_phy_int_enable_set(0, (jl_port_t) value, (jl_enable_t) value2);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "int_phy_enable_set return err %d,port %d\n", ret,
					   value);
			else {
				ret = jl_interrupt_port_phy_int_enable_get(0, (jl_port_t) value, &enable);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
					   "after set,int_phy_enable_get return err %d,port %d, enable %d\n", ret,
					   value, enable);
			}

			break;

		case 3:
			ret = jl_interrupt_port_phy_int_status_get(0, (jl_port_t) value, &int_status);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
				   "first read,jl_interrupt_port_phy_int_status_get return err %d,port %d,interrupt status=%d\n",
				   ret, value, int_status);

			int_status = 0;
			ret = jl_interrupt_port_phy_int_status_get(0, (jl_port_t) value, &int_status);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "second read,should be clear!\n");
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
				   "jl_interrupt_port_phy_int_status_get return err %d, port %d, status=%d\n", ret,
				   value, int_status);
			break;

		case 4:
			enable = 0;
			ret = jl_interrupt_enable_get(0, (jl_int_type_t) value, &enable);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Default,jl_interrupt_enable_get return err %d,type %d, enable = %d\n", ret, value, enable);
			enable = 0;
			ret = jl_interrupt_enable_set(0, (jl_int_type_t) value, (jl_enable_t) value2);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_interrupt_enable_set return err %d,type = %d\n",
					   ret, value);
			else {
				ret = jl_interrupt_enable_get(0, (jl_int_type_t) value, &enable);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "After set,jl_interrupt_enable_get return err %d, \
				type %d, enable = %d\n", ret, value, enable);
			}
			break;

		case 5:
			ret = jl_interrupt_status_get(0, &statusmask);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "first read, jl_interrupt_status_get return err %d,\
				statusmask = %d\n", ret, statusmask.mask);
			statusmask.mask = 0;
			ret = jl_interrupt_status_get(0, &statusmask);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "second read,should be the same as the first read");
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "return err %d,statusmask = %d\n", ret, statusmask.mask);
			break;

		case 6:
			ret = jl_interrupt_detail_get(0, (jl_int_type_t) value, &info);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "first read, jl_interrupt_detail_get return err %d,value %d,\
				port_mask=%d\n", ret, value, info.port_mask);
			info.port_mask = 0;
			ret = jl_interrupt_detail_get(0, (jl_int_type_t) value, &info);
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "second read,should be the same as the first read.\n");
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "return err %d, value %d,port_mask=%d\n", ret, value,
				   info.port_mask);
			break;
		case 7:
			ret = jl_interrupt_status_clean(0, (jl_int_type_t) value);
			if (ret != 0)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO,
					   "jl_interrupt_status_clean return err %d,type = %d\n", ret, value);
			else {
				info.port_mask = 0;
				ret = jl_interrupt_detail_get(0, (jl_int_type_t) value, &info);
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "after clean, jl_interrupt_detail_get return err %d,value %d,\
					port_mask=%d\n", ret, value, info.port_mask);
			}
			break;
		case 8:
			testcase_int_show();
			break;
		case 9:
			testcase_interrupt();
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
