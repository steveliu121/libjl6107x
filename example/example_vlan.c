#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
//#include <getopt.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_error.h"
#include "jl_vlan.h"

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
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======SDK VLAN TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. jl_vlan_ingressport_mode_set()/jl_vlan_ingressport_default_set()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. jl_vlan_egressport_mode_set()/jl_vlan_egressport_default_set()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. jl_vlan_preprocess_mac_set/get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. jl_vlan_preprocess_outvid_set/get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "5. jl_vlan_preprocess_invid_set/get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "6. jl_vlan_preprocess_ethtype_set/get()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "7. jl_vlan_tbl_set/get/del()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "8. jl_vlan_tbl_set/get_fid()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "9. jl_vlan_tbl_set/get_memberPortmsk()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "10. jl_vlan_set/get_operation_selection()\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "11. jl_vlan_set/get_tpid_val/()\n");
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	char console_input[128] = {0};
	jl_uint32 cmd;
	jl_api_ret_t ret;
	jl_uint32 chip_id = 0;
	jl_uint32 port_id = 0;

	jl_vlan_operation_t opt;
	jl_vlan_val_sel_t vidsel;
	jl_vlan_val_sel_t cfisel;
	jl_vlan_val_sel_t pcpsel;
	jl_vlan_tpid_sel_t ethtypesel;
	jl_sport_vlan_assign_t mode;

	jl_uint32 vid = 100;
	jl_uint32 cfi = 0;
	jl_uint32 pcp = 1;
	jl_uint8 order = 3;

	jl_uint32 idx = 0;
	jl_vlan_macbase_tbl_t maccfg;

	jl_vlan_outvidbase_tbl_t outcfg;

	jl_vlan_invidbase_tbl_t incfg;

	jl_vlan_ethertypebase_tbl_t ethcfg;

	jl_uint32 fid = 100;
	jl_uint32 portmsk = 3;

	jl_vlan_tbl_t vlancfg;

	jl_vlan_sel_operation_t selmode = JL_VLAN_SEL_OPERATION_INGRESS;
	jl_vlan_operation_t value = JL_VLAN_OPERATION_PUSH;

	jl_uint32 tpid_val = 0x8100;

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

			opt = JL_VLAN_OPERATION_PUSH;
			vidsel = JL_VLAN_VAL_SEL_OUTER;
			cfisel = JL_VLAN_VAL_SEL_OUTER;
			pcpsel = JL_VLAN_VAL_SEL_OUTER;
			ethtypesel = JL_VLAN_TPID_SEL_8100;
			mode = JL_SPORT_VLAN_ASSIGN_PORT;

			ret = jl_vlan_ingressport_mode_set(chip_id, port_id, opt, vidsel, cfisel, pcpsel, ethtypesel, mode);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			chip_id = 0;
			port_id = 0;
			vid = 100;
			cfi = 0;
			pcp = 1;
			order = 3;

			ret = jl_vlan_ingressport_default_set(chip_id, port_id, vid, cfi, pcp, order);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 2:
			opt = JL_VLAN_OPERATION_PUSH;
			vidsel = JL_VLAN_VAL_SEL_OUTER;
			cfisel = JL_VLAN_VAL_SEL_OUTER;
			pcpsel = JL_VLAN_VAL_SEL_OUTER;
			ethtypesel = JL_VLAN_TPID_SEL_8100;

			ret = jl_vlan_egressport_mode_set(chip_id, port_id, opt, vidsel, cfisel, pcpsel, ethtypesel);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			vid = 100;
			cfi = 0;
			pcp = 1;

			ret = jl_vlan_egressport_default_set(chip_id, port_id, vid, cfi, pcp);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 3:
			idx = 0;
			portmsk = 3;

			maccfg.mode = JL_VLAN_MACBASED_DMAC;
			maccfg.vid = 100;
			maccfg.order = 3;

			maccfg.mac_range_start.addr[0] = 0x01;
			maccfg.mac_range_start.addr[1] = 0x00;
			maccfg.mac_range_start.addr[2] = 0x5e;
			maccfg.mac_range_start.addr[3] = 0xff;
			maccfg.mac_range_start.addr[4] = 0xff;
			maccfg.mac_range_start.addr[5] = 0x00;

			maccfg.mac_range_end.addr[0] = 0x01;
			maccfg.mac_range_end.addr[1] = 0x00;
			maccfg.mac_range_end.addr[2] = 0x5e;
			maccfg.mac_range_end.addr[3] = 0xff;
			maccfg.mac_range_end.addr[4] = 0xff;
			maccfg.mac_range_end.addr[5] = 0xff;

			ret = jl_vlan_preprocess_mac_set(chip_id, idx, &maccfg);

			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");


			ret = jl_vlan_preprocess_mac_get(chip_id, idx, &maccfg);

			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 4:

			outcfg.portmsk = 3;
			outcfg.mode = JL_VLAN_OUTVID_STYPE;
			outcfg.checkvidstart = 2;
			outcfg.checkvidend = 4094;
			outcfg.vid = 100;
			outcfg.order = 3;

			ret = jl_vlan_preprocess_outvid_set(chip_id, idx, &outcfg);

			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_vlan_preprocess_outvid_get(chip_id, idx, &outcfg);

			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 5:
			incfg.portmsk = 3;
			incfg.mode = JL_VLAN_OUTVID_CTYPE;
			incfg.checkvidstart = 2;
			incfg.checkvidend = 4094;
			incfg.vid = 100;
			incfg.order = 3;

			ret = jl_vlan_preprocess_invid_set(chip_id, idx, &incfg);

			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_vlan_preprocess_invid_get(chip_id, idx, &incfg);

			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 6:
			ethcfg.portmsk = 3;
			ethcfg.ethertype_range_start = 0x8035;
			ethcfg.ethertype_range_end = 0x8035;
			ethcfg.vid = 100;
			ethcfg.order = 3;

			ret = jl_vlan_preprocess_ethtype_set(chip_id, idx, &ethcfg);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_vlan_preprocess_ethtype_get(chip_id, idx, &ethcfg);

			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 7:
			vlancfg.portmsk = 0x7;
			vlancfg.vlanopt = JL_VLAN_OPERATION_POP;

			vlancfg.vidsel = JL_VLAN_VAL_SEL_OUTER;
			vlancfg.cfisel = JL_VLAN_VAL_SEL_OUTER;
			vlancfg.pcpsel = JL_VLAN_VAL_SEL_OUTER;
			vlancfg.typesel = JL_VLAN_TPID_SEL_8100;

			vlancfg.fid = 100;

			vlancfg.vid = 100;
			vlancfg.cfi = 0;
			vlancfg.pcp = 1;

			vlancfg.msptptr = 1;

			vlancfg.mmpvalid = TRUE;
			vlancfg.mmpptr = 1;
			vlancfg.mmporder = 3;

			ret = jl_vlan_tbl_set(chip_id, vid, &vlancfg);

			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_vlan_tbl_get(chip_id, vid, &vlancfg);
			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_vlan_tbl_del(chip_id, vid);
			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;

		case 8:
			vid = 200;
			fid = 100;

			ret = jl_vlan_tbl_set_fid(chip_id, vid, fid);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_vlan_tbl_get_fid(chip_id, vid, &fid);
			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;
		case 9:
			vid = 200;
			portmsk = 3;

			ret = jl_vlan_tbl_set_memberPortmsk(chip_id, vid, portmsk);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			ret = jl_vlan_tbl_get_memberPortmsk(chip_id, vid, &portmsk);
			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;
		case 10:
			selmode = JL_VLAN_SEL_OPERATION_INGRESS;
			value = JL_VLAN_OPERATION_PUSH;
			port_id = 0;

			ret = jl_vlan_set_operation_selection(chip_id, port_id, selmode, value);
			if (!ret)
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			value = JL_VLAN_OPERATION_NONE;
			ret = jl_vlan_get_operation_selection(chip_id, port_id, selmode, &value);
			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			break;
		case 11:
			ret = jl_vlan_set_tpid_val(chip_id, tpid_val);
			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");

			tpid_val = 0xffff;
			ret = jl_vlan_get_tpid_val(chip_id, &tpid_val);
			if (!ret)
				/*do processing code*/
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Success.\n");
			else
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Fail.\n");


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
