#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_port.h"

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

void print_port_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. mac port test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. phy port test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. fiber port test\n");
}

void mac_port_test(jl_uint32 chip_id)
{
	jl_port_mac_ability_t macability;
	jl_mode_ext_t mode = 0;
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
	ret = jl_port_mac_force_link_ext_set(chip_id, EXT_PORT0, MODE_EXT_RMII_PHY, &macability);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_mac_force_link_ext_set EXT_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_mac_force_link_ext_set EXT_PORT0 success[%d]\n", ret);
	/* Get MAC ability of EXT_PORT0 */
	memset(&macability, 0x00, sizeof(jl_port_mac_ability_t));
	ret = jl_port_mac_force_link_ext_get(chip_id, EXT_PORT0, &mode, &macability);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_mac_force_link_ext_get EXT_PORT0 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_mac_force_link_ext_get EXT_PORT0 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "ext mode : %d\n", mode);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability force mode : %d\n", macability.force_mode);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability speed : %d\n", macability.speed);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability duplex : %d\n", macability.duplex);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability linkstatus : %d\n", macability.link);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability tx_pause : %d\n", macability.tx_pause);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "macability rx_pause : %d\n", macability.rx_pause);
	}
	/* Get MAC status of EXT_PORT0 */
	memset(&macability, 0x00, sizeof(jl_port_mac_ability_t));
	ret = jl_port_mac_status_get(chip_id, EXT_PORT0, &macability);
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

void phy_port_test(jl_uint32 chip_id)
{
	jl_port_phy_ability_t phyability;
	jl_port_speed_t speed = 0;
	jl_port_duplex_t duplex = 0;
	jl_port_link_status_t linkst = 0;
	jl_api_ret_t ret = 0;

	/* set UTP_PORT1 with Auto negotiation, 100F,10F */
	/* and Symmetric PAUSE flow control abilities */
	memset(&phyability, 0x00, sizeof(jl_port_phy_ability_t));
	phyability.auto_negotiation = 1;
	phyability.full_duplex_100 = 1;
	phyability.full_duplex_10 = 1;
	phyability.flow_control = 1;
	phyability.asym_flow_control = 1;
	ret = jl_port_phy_autoneg_ability_set(chip_id, UTP_PORT1, &phyability);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_autoneg_ability_set UTP_PORT1 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_autoneg_ability_set UTP_PORT1 success[%d]\n", ret);
	/* Get PHY ability of UTP_PORT1 */
	memset(&phyability, 0x00, sizeof(jl_port_phy_ability_t));
	ret = jl_port_phy_autoneg_ability_get(chip_id, UTP_PORT1, &phyability);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_autoneg_ability_get UTP_PORT1 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_autoneg_ability_get UTP_PORT1 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability auto_negotiation : %d\n", phyability.auto_negotiation);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability half_duplex_10 : %d\n", phyability.half_duplex_10);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability full_duplex_10 : %d\n", phyability.full_duplex_10);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability half_duplex_100 : %d\n", phyability.half_duplex_100);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability full_duplex_100 : %d\n", phyability.full_duplex_100);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability full_duplex_1000 : %d\n", phyability.full_duplex_1000);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability flow_control : %d\n", phyability.flow_control);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability asym_flow_control : %d\n", phyability.asym_flow_control);
	}

	/* set UTP_PORT2 with Force mode, 1000F, without flow control */
	memset(&phyability, 0x00, sizeof(jl_port_phy_ability_t));
	phyability.full_duplex_1000 = 1;
	ret = jl_port_phy_force_ability_set(chip_id, UTP_PORT2, &phyability);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_force_ability_set UTP_PORT2 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_force_ability_set UTP_PORT2 success[%d]\n", ret);

	/* Get PHY ability of UTP_PORT2 */
	memset(&phyability, 0x00, sizeof(jl_port_phy_ability_t));
	ret = jl_port_phy_force_ability_get(chip_id, UTP_PORT2, &phyability);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_force_ability_get UTP_PORT2 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_force_ability_get UTP_PORT2 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability auto_negotiation : %d\n", phyability.auto_negotiation);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability half_duplex_10 : %d\n", phyability.half_duplex_10);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability full_duplex_10 : %d\n", phyability.full_duplex_10);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability half_duplex_100 : %d\n", phyability.half_duplex_100);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability full_duplex_100 : %d\n", phyability.full_duplex_100);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability full_duplex_1000 : %d\n", phyability.full_duplex_1000);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability flow_control : %d\n", phyability.flow_control);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "phyability asym_flow_control : %d\n", phyability.asym_flow_control);
	}

	/* Get PHY status of UTP_PORT1 */
	ret = jl_port_phy_status_get(chip_id, UTP_PORT1, &linkst, &duplex, &speed);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_status_get UTP_PORT1 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_status_get UTP_PORT1 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "linkst : %d\n", linkst);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "duplex : %d\n", duplex);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "speed : %d\n", speed);
	}
	/* Get PHY status of UTP_PORT2 */
	ret = jl_port_phy_status_get(chip_id, UTP_PORT2, &linkst, &duplex, &speed);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_status_get UTP_PORT2 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_phy_status_get UTP_PORT2 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "linkst : %d\n", linkst);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "duplex : %d\n", duplex);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "speed : %d\n", speed);
	}
}

void fiber_port_test(jl_uint32 chip_id)
{
	jl_port_fiber_ability_t fiberability;
	jl_port_mac_ability_t macability;
	jl_api_ret_t ret = 0;

	/* set EXT_PORT0 with Auto negotiation, 1000F */
	/* and Symmetric PAUSE flow control abilities */
	memset(&fiberability, 0x00, sizeof(jl_port_fiber_ability_t));
	fiberability.auto_negotiation = 1;
	fiberability.full_duplex_100 = 0;
	fiberability.full_duplex_1000 = 1;
	fiberability.flow_control = 1;
	fiberability.asym_flow_control = 1;
	ret = jl_port_fiber_ability_set(chip_id, EXT_PORT0, &fiberability);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fiber_ability_set EXT_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fiber_ability_set EXT_PORT0 success[%d]\n", ret);
	/* Get Fiber ability of EXT_PORT0 */
	memset(&fiberability, 0x00, sizeof(jl_port_fiber_ability_t));
	ret = jl_port_fiber_ability_get(chip_id, EXT_PORT0, &fiberability);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fiber_ability_get EXT_PORT0 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_fiber_ability_get EXT_PORT0 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "fiberability auto_negotiation : %d\n", fiberability.auto_negotiation);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "fiberability full_duplex_100 : %d\n", fiberability.full_duplex_100);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "fiberability full_duplex_1000 : %d\n", fiberability.full_duplex_1000);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "fiberability flow_control : %d\n", fiberability.flow_control);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "fiberability asym_flow_control : %d\n", fiberability.asym_flow_control);
	}
	/* Get MAC status of EXT_PORT0 */
	memset(&macability, 0x00, sizeof(jl_port_mac_ability_t));
	ret = jl_port_mac_status_get(chip_id, EXT_PORT0, &macability);
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
		print_port_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n mac_port_test start!\n");
			mac_port_test(chip_id);
			break;
		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n phy_port_test start!\n");
			phy_port_test(chip_id);
			break;
		case 3:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n fiber_port_test start!\n");
			fiber_port_test(chip_id);
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
