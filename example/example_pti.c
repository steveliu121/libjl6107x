#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_pti.h"

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

void print_pti_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. one-way dst port isolation test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. one-way src port isolation test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. two-way port isolation test\n");
}

void dst_port_pti_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_portmask_t portmask = 0;

	/* Set one-way port isolation: set the prevented portmask(port 1,2) to the dest port 0 */
	portmask = (1 << UTP_PORT1) | (1 << UTP_PORT2);
	ret = jl_port_isolation_dst_set(chip_id, UTP_PORT0, portmask);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_dst_set UTP_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_dst_set UTP_PORT0 success[%d]\n", ret);

	/* Get one-way port isolation: get the prevented portmask to the dest port 0 */
	portmask = 0;
	ret = jl_port_isolation_dst_get(chip_id, UTP_PORT0, &portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_dst_get UTP_PORT0 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_dst_get UTP_PORT0 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "the prevented portmask : 0x%x\n", portmask);
	}

	/* clear one-way port isolation: clear the prevented portmask to the dest port 0 */
	portmask = 0;
	ret = jl_port_isolation_dst_set(chip_id, UTP_PORT0, portmask);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_dst_set UTP_PORT0 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_dst_set UTP_PORT0 success[%d]\n", ret);

	/* Get one-way port isolation: get the prevented portmask to the dest port 0 */
	portmask = 0;
	ret = jl_port_isolation_dst_get(chip_id, UTP_PORT0, &portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_dst_get UTP_PORT0 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_dst_get UTP_PORT0 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "the prevented portmask : 0x%x\n", portmask);
	}
}

void src_port_pti_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_portmask_t portmask = 0;

	/* Set one-way port isolation: set the prevented portmask(port 4,5) from the src port 3 */
	portmask = (1 << UTP_PORT4) | (1 << UTP_PORT5);
	ret = jl_port_isolation_src_set(chip_id, UTP_PORT3, portmask);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_src_set UTP_PORT3 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_src_set UTP_PORT3 success[%d]\n", ret);

	/* Get one-way port isolation: get the prevented portmask from the src port 3 */
	portmask = 0;
	ret = jl_port_isolation_src_get(chip_id, UTP_PORT3, &portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_src_get UTP_PORT3 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_src_get UTP_PORT3 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "the prevented portmask : 0x%x\n", portmask);
	}

	/* Clear one-way port isolation: clear the prevented portmask from the src port 3 */
	portmask = 0;
	ret = jl_port_isolation_src_set(chip_id, UTP_PORT3, portmask);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_src_set UTP_PORT3 error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_src_set UTP_PORT3 success[%d]\n", ret);

	/* Get one-way port isolation: get the prevented portmask from the src port 3 */
	portmask = 0;
	ret = jl_port_isolation_src_get(chip_id, UTP_PORT3, &portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_src_get UTP_PORT3 error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_src_get UTP_PORT3 success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "the prevented portmask : 0x%x\n", portmask);
	}
}

void twoway_port_pti_test(jl_uint32 chip_id)
{
	jl_api_ret_t ret = 0;
	jl_portmask_t portmask = 0;
	jl_pti_group_t group_id = 0;

	/* Set two-way port isolation: set the port isolation group id 0 */
	group_id = 0;
	/* Set two-way port isolation: set the prevented portmask of the port(port 0,1,2) from the group id 0 */
	portmask = (1 << UTP_PORT0) | (1 << UTP_PORT1) | (1 << UTP_PORT2);
	ret = jl_port_isolation_group_set(chip_id, group_id, portmask);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_set error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_set success[%d]\n", ret);

	/* Get two-way port isolation: get the prevented portmask of the each port from the group id 0 */
	portmask = 0;
	ret = jl_port_isolation_group_get(chip_id, group_id, &portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "the group id : %d, the the prevented portmask : 0x%x\n", group_id, portmask);
	}
	/* Set two-way port isolation: set the port isolation group id 1 */
	group_id = 1;
	/* Set two-way port isolation: set the prevented portmask of the port(port 5,6,7) from the group id 1 */
	portmask = (1 << UTP_PORT5) | (1 << UTP_PORT6) | (1 << UTP_PORT7);
	ret = jl_port_isolation_group_set(chip_id, group_id, portmask);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_set error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_set success[%d]\n", ret);

	/* Get two-way port isolation: get the prevented portmask of the each port from the group id 1 */
	portmask = 0;
	ret = jl_port_isolation_group_get(chip_id, group_id, &portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "the group id : %d, the the prevented portmask : 0x%x\n", group_id, portmask);
	}

	/* Clear two-way port isolation: clear the prevented portmask of the each port from the group id 0 */
	group_id = 0;
	ret = jl_port_isolation_group_clear(chip_id, group_id);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_clear error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_clear success[%d]\n", ret);

	/* Get two-way port isolation: the prevented portmask of the each port from the group id 0 */
	portmask = 0;
	ret = jl_port_isolation_group_get(chip_id, group_id, &portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "the group id : %d, the the prevented portmask : 0x%x\n", group_id, portmask);
	}
	/* Clear two-way port isolation: clear the prevented portmask of the each port from the group id 1 */
	group_id = 1;
	ret = jl_port_isolation_group_clear(chip_id, group_id);
	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_clear error[%d]\n", ret);
	else
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_clear success[%d]\n", ret);

	/* Get two-way port isolation: the prevented portmask of the each port from the group id 1 */
	portmask = 0;
	ret = jl_port_isolation_group_get(chip_id, group_id, &portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_get error[%d]\n", ret);
	} else {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "jl_port_isolation_portmask_get success[%d]\n", ret);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "the group id : %d, the the prevented portmask : 0x%x\n", group_id, portmask);
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
		print_pti_usage();

		fgets(console_input, 127, stdin);
		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		sscanf(console_input, "%d", &cmd);

		switch (cmd) {
		case 1:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n dst_port_pti_test start!\n");
			dst_port_pti_test(chip_id);
			break;
		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n src_port_pti_test start!\n");
			src_port_pti_test(chip_id);
			break;
		case 3:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n twoway_port_pti_test start!\n");
			twoway_port_pti_test(chip_id);
			break;
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n Test commad: error\n");
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
