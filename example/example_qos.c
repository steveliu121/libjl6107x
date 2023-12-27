#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>
#include "jl_base.h"
#include "jl_switch.h"
#include "jl_qos.h"


#define QOS_EXAMPLE_SET 0
#define QOS_EXAMPLE_GET 1

#define QOS_COLOR_GREEN 0
#define QOS_COLOR_YELLOW 1
#define QOS_COLOR_RED 2

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

static void print_QoS_usage(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "======QoS TEST======\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Test commad: <index> <opt>\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "opt: 0 = set, 1 = get\n\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. QoS nonvlan map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. QoS unknown map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. QoS MAC map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. QoS IP map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "5. QoS VID map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "6. QoS EthType map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "7. QoS TOS map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "8. QoS EXP map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "9. QoS PCP map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "10. QoS L4 PORT map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "11. QoS L4 PROTOCOL map to queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "12. QoS nonvlan map to color test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "13. QoS unknown map to color test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "14. QoS TOS map to color test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "15. QoS EXP map to color test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "16. QoS PCP map to color test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "17. QoS remap vlan by eport test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "18. QoS remap vlan by mapping table test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "19. QoS color remap by eport test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "20. QoS color remap by mmp test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "21. QoS ingress admission control srTCM test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "22. QoS ingress admission control trTCM test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "23. QoS queue map trust l3 test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "24. QoS color map trust l3 test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "25. QoS queue shaper test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "26. QoS priority shaper test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "27. QoS queue dwrr weight test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "28. QoS enable queue test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "29. QoS port shaper test\n");
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "30. QoS schedule type test\n");
}

static jl_ret_t qos_nonvlan_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 queue;
	jl_uint32 port = UTP_PORT2;

	if (op == QOS_EXAMPLE_SET) {
		queue = 2;

		ret = jl_qos_nonvlan_2queue_set(chip_id, port, queue);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d Non-VLAN map to queue %d  success!\n",
		       port, queue);
	} else {
		ret = jl_qos_nonvlan_2queue_get(chip_id, port, &queue);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d Non-VLAN map to queue %d, success!\n",
		       port, queue);

		ret = jl_qos_nonvlan_2queue_del(chip_id, port);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d Non-VLAN map to queue sucess!\n", port);
	}
	return 0;
}

static jl_ret_t qos_unknown_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 enable;
	jl_uint8 queue;
	jl_uint32 port = UTP_PORT0;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_queue_map_trust_l3_set(chip_id, port, ENABLED);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d queue map trust L3!\n", port);

		queue = 2;

		ret = jl_qos_unknownl3_2queue_set(chip_id, port, queue);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d Unknown map to queue %d success!\n",
		       port, queue);
	} else {
		ret = jl_qos_queue_map_trust_l3_get(chip_id, port, &enable);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d queue map trust L3 enbale: %d!\n", port, enable);

		ret = jl_qos_unknownl3_2queue_get(chip_id, port, &queue);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d Unknown map to queue %d success!\n",
		       port, queue);

		ret = jl_qos_unknownl3_2queue_del(chip_id, port);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d Unknown map to queue sucess!\n", port);

		ret = jl_qos_queue_map_trust_l3_set(chip_id, port, DISABLED);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_mac_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 rule_idx = 0;
	jl_uint64 mac_addr = 0;
	jl_qos_mac_map_t map_info;
	jl_uint32 port = EXT_PORT0;

	if (op == QOS_EXAMPLE_SET) {
		map_info.queue = 2;
		map_info.mac.value.val = 0xaa2233445566;
		map_info.mac.mask.val = 0xffffffffffff;
		map_info.mac_type = QOS_DIR_SOURCE;

		ret = jl_qos_mac_2queue_set(chip_id, port, rule_idx++, &map_info);
		if (ret)
			return ret;
		mac_addr = map_info.mac.value.val;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d source mac 0x%llx map to queue %d success!\n",
		       port, mac_addr, map_info.queue);

		map_info.queue = 3;
		map_info.mac.value.val = 0xaabbccddeeff;
		map_info.mac.mask.val = 0xffffffffffff;
		map_info.mac_type = QOS_DIR_DEST;

		ret = jl_qos_mac_2queue_set(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;

		mac_addr = map_info.mac.value.val;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d dest mac 0x%llx map to queue %d success!\n",
		       port, mac_addr, map_info.queue);
	} else {
		rule_idx = 0;
		ret = jl_qos_mac_2queue_get(chip_id, port, rule_idx++, &map_info);
		if (ret)
			return ret;

		mac_addr = map_info.mac.value.val;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port source %d mac 0x%llx map to queue %d success!\n",
		       port, mac_addr, map_info.queue);

		ret = jl_qos_mac_2queue_get(chip_id, port, rule_idx++, &map_info);
		if (ret)
			return ret;

		mac_addr = map_info.mac.value.val;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port dest %d mac 0x%llx map to queue %d success!\n",
		       port, mac_addr, map_info.queue);

		rule_idx = 0;
		ret = jl_qos_mac_2queue_del(chip_id, port, rule_idx);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d source mac map to queue sucess!\n", port, rule_idx);
		ret = jl_qos_mac_2queue_del(chip_id, port, ++rule_idx);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d dest mac map to queue sucess!\n", port, rule_idx);
	}
	return 0;
}

static jl_ret_t qos_ip_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 rule_idx = 0;
	jl_uint8 i = 0;
	jl_uint32 port = UTP_PORT4;
	jl_qos_ip_map_t map_info;

	memset(&map_info, 0, sizeof(jl_qos_ip_map_t));

	if (op == QOS_EXAMPLE_SET) {
		map_info.queue = 2;
		map_info.v4_v6 = QOS_IP_V4;
		map_info.sa_da = QOS_DIR_SOURCE;
		map_info.ip.ipv4.addr[0] = 0xc0;  //192.168.1.2
		map_info.ip.ipv4.addr[1] = 0xa8;
		map_info.ip.ipv4.addr[2] = 0x1;
		map_info.ip.ipv4.addr[3] = 0x2;

		map_info.ip.ipv4.mask[0] = 0xff;
		map_info.ip.ipv4.mask[1] = 0xff;
		map_info.ip.ipv4.mask[2] = 0xff;
		map_info.ip.ipv4.mask[3] = 0xff;

		ret = jl_qos_ip_2queue_set(chip_id, port, rule_idx++, &map_info);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d source ipv4 %s map to queue %d success!\n",
		       port, "192.168.1.2", map_info.queue);

		memset(&map_info, 0, sizeof(jl_qos_ip_map_t));
		map_info.queue = 3;
		map_info.v4_v6 = QOS_IP_V6;
		map_info.sa_da = QOS_DIR_DEST;
		map_info.ip.ipv6.addr[0] = 0x1122;
		map_info.ip.ipv6.addr[1] = 0x3344;
		map_info.ip.ipv6.addr[2] = 0x5566;
		map_info.ip.ipv6.addr[5] = 0xaabb;
		map_info.ip.ipv6.addr[6] = 0xccdd;
		map_info.ip.ipv6.addr[7] = 0xeeff;

		for (i =0; i < 8; i++)
			map_info.ip.ipv6.mask[i] = 0xffff;

		ret = jl_qos_ip_2queue_set(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d dest ipv6 ", port);

		for (i = 8; i >= 1; i--) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "%x : ", map_info.ip.ipv6.addr[i-1]);
		}

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "map to queue %d success!\n", map_info.queue);
	} else {
		rule_idx = 0;

		ret = jl_qos_ip_2queue_get(chip_id, port, rule_idx++, &map_info);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d source ip map to queue %d success!\n",
		       port, map_info.queue);

		ret = jl_qos_ip_2queue_get(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d dest ipv6 ", port);

		for (i = 8; i >= 1; i--) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "%x : ", map_info.ip.ipv6.addr[i-1]);
		}

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "map to queue %d success!\n", map_info.queue);

		rule_idx = 0;
		ret = jl_qos_ip_2queue_del(chip_id, port, rule_idx);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d ip map to queue sucess!\n", port, rule_idx);
		ret = jl_qos_ip_2queue_del(chip_id, port, ++rule_idx);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d ip map to queue sucess!\n", port, rule_idx);
	}
	return 0;
}

static jl_ret_t qos_vid_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 rule_idx = 0;
	jl_uint32 port = UTP_PORT2;
	jl_qos_vid_map_t map_info;

	memset(&map_info, 0, sizeof(jl_qos_vid_map_t));

	if (op == QOS_EXAMPLE_SET) {
		map_info.queue = 2;
		map_info.vid = 100;
		map_info.mask = 0xfff;
		map_info.inner_outer = 0;
		map_info.cstag = 0;

		ret = jl_qos_vid_2queue_set(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d vid %d map to queue %d success!\n",
		       port, map_info.vid, map_info.queue);

	} else {
		ret = jl_qos_vid_2queue_get(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d vid %d map to queue %d success!\n",
		       port, map_info.vid, map_info.queue);

		ret = jl_qos_vid_2queue_del(chip_id, port, rule_idx);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d vid map to queue sucess!\n", port, rule_idx);
	}
	return 0;
}

static jl_ret_t qos_ethtype_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 rule_idx = 3;
	jl_uint32 port = UTP_PORT2;
	jl_qos_ethtype_map_t map_info;

	if (op == QOS_EXAMPLE_SET) {
		map_info.queue = 2;
		map_info.eth_type = 0x88a0;

		ret = jl_qos_ethtype_2queue_set(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d ethtype 0x%x map to queue %d! sucess\n",
		       port, map_info.eth_type, map_info.queue);
	} else {
		ret = jl_qos_ethtype_2queue_get(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d ethtype 0x%x map to queue %d!\n",
		       port, map_info.eth_type, map_info.queue);

		ret = jl_qos_ethtype_2queue_del(chip_id, port, rule_idx);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d ethtype map to queue sucess!\n", port, rule_idx);
	}
	return 0;
}

static jl_ret_t qos_tos_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 tos = 0xc0;
	jl_uint8 queue;
	jl_uint32 port = UTP_PORT2;

	if (op == QOS_EXAMPLE_SET) {
		queue = 3;
		ret = jl_qos_queue_map_trust_l3_set(chip_id, port, ENABLED);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d queue map trust L3!\n", port);

		ret = jl_qos_tos_2queue_set(chip_id, port, tos, queue);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set tos %d map to queue %d on port %d success!\n",
		       tos, queue, port);
	} else {
		ret = jl_qos_tos_2queue_get(chip_id, port, tos, &queue);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get tos %d map to queue %d on port %d success!\n",
		       tos, queue, port);

		ret = jl_qos_tos_2queue_del(chip_id, port, tos);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del tos %d map to queue sucess!\n", tos);

		ret = jl_qos_queue_map_trust_l3_set(chip_id, port, DISABLED);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_exp_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 exp = 7;
	jl_uint8 queue;
	jl_uint32 port = UTP_PORT2;

	if (op == QOS_EXAMPLE_SET) {
		queue = 7;

		ret = jl_qos_queue_map_trust_l3_set(chip_id, port, ENABLED);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d queue map trust L3!\n", port);

		ret = jl_qos_exp_2queue_set(chip_id, port, exp, queue);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set exp %d map to queue %d on port %d success!\n",
		       exp, queue, port);
	} else {
		ret = jl_qos_exp_2queue_get(chip_id, port, exp, &queue);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get exp %d map to queue %d on port %d success!\n",
		       exp, queue, port);

		ret = jl_qos_exp_2queue_del(chip_id, port, exp);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del exp %d map to queue sucess!\n", exp);

		ret = jl_qos_queue_map_trust_l3_set(chip_id, port, DISABLED);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_pcp_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 pcp = 6;
	jl_uint8 queue;
	jl_uint32 port = UTP_PORT0;

	if (op == QOS_EXAMPLE_SET) {
		queue = 7;
		ret = jl_qos_pcp_2queue_set(chip_id, port, pcp, queue);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set pcp %d map to queue %d success!\n", pcp, queue);
	} else {
		ret = jl_qos_pcp_2queue_get(chip_id, port, pcp, &queue);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get pcp %d map to queue %d success!\n", pcp, queue);

		ret = jl_qos_pcp_2queue_del(chip_id, port, pcp);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del pcp %d map to queue sucess!\n", pcp);
	}
	return 0;
}

static jl_ret_t qos_l4_port_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 rule_idx = 0;
	jl_uint32 port = UTP_PORT0;
	jl_qos_l4_port_range_map_t map_info;

	if (op == QOS_EXAMPLE_SET) {
		map_info.queue = 0;
		map_info.source_dest = QOS_DIR_SOURCE;
		map_info.udp_tcp = QOS_L4_TCP;

		map_info.start_port = 10001;
		map_info.end_port = 10021;
		ret = jl_qos_l4port_2queue_set(chip_id, port, rule_idx++, &map_info);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set tcp stream source l4-port %d - %d map to queue %d success!\n",
		       map_info.start_port, map_info.end_port,map_info.queue);

		map_info.queue = 4;
		map_info.source_dest = QOS_DIR_DEST;
		map_info.udp_tcp = QOS_L4_UDP;

		map_info.start_port = 10030;
		map_info.end_port = 10040;
		ret = jl_qos_l4port_2queue_set(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set udp stream dest l4-port %d - %d map to queue %d success!\n",
		       map_info.start_port, map_info.end_port,map_info.queue);
	} else {
		rule_idx = 0;
		ret = jl_qos_l4port_2queue_get(chip_id, port, rule_idx++, &map_info);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set source l4-port %d - %d map to queue %d success!\n",
		       map_info.start_port, map_info.end_port,map_info.queue);

		ret = jl_qos_l4port_2queue_get(chip_id, port, rule_idx++, &map_info);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set dest l4-port %d - %d map to queue %d success!\n",
		       map_info.start_port, map_info.end_port,map_info.queue);

		rule_idx = 0;
		ret = jl_qos_l4port_2queue_del(chip_id, port, rule_idx);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d l4-port map to queue sucess!\n", port, rule_idx);

		ret = jl_qos_l4port_2queue_del(chip_id, port, ++rule_idx);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d l4-port map to queue sucess!\n", port, rule_idx);
	}
	return 0;
}

static jl_ret_t qos_l4_protocol_map2q_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 rule_idx = 0;
	jl_uint32 port = UTP_PORT2;
	jl_qos_l4_protocol_map_t map_info;

	if (op == QOS_EXAMPLE_SET) {
		map_info.queue = 0;
		map_info.protocol = 6; //tcp

		ret = jl_qos_l4protocol_2queue_set(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set l4-protocol %d map to queue %d success!\n",
		       map_info.protocol, map_info.queue);
	} else {
		ret = jl_qos_l4protocol_2queue_get(chip_id, port, rule_idx, &map_info);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get l4-protocol %d map to queue %d success!\n",
		       map_info.protocol, map_info.queue);

		ret = jl_qos_l4protocol_2queue_del(chip_id, port, rule_idx);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Del port %d rule %d l4-protocol map to queue sucess!\n", port, rule_idx);
	}
	return 0;
}

static jl_ret_t qos_nonvlan_map2color_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 color = QOS_COLOR_RED;
	jl_uint8 enable = 0;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_nonvlan_2color_set(chip_id, color, TRUE);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set nonvlan map to color %d success!\n", color);
	} else {
		ret = jl_qos_nonvlan_2color_get(chip_id, &color, &enable);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get nonvlan map to color %d success!\n", color);

		ret = jl_qos_nonvlan_2color_set(chip_id, color, FALSE);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_unknown_map2color_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 color = QOS_COLOR_RED;
	jl_uint8 enable = 0;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_color_map_trust_l3_set(chip_id, UTP_PORT2, ENABLED);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d color map trust l3 success!\n", UTP_PORT2);

		ret = jl_qos_unknownl3_2color_set(chip_id, color, TRUE);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set unknown map to color %d success!\n", color);
	} else {
		ret = jl_qos_color_map_trust_l3_get(chip_id, UTP_PORT2, &enable);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d color map trust l3 success, enable %d!\n", UTP_PORT2, enable);

		ret = jl_qos_unknownl3_2color_get(chip_id, &color, &enable);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get unknown map to color %d success!\n", color);

		ret = jl_qos_unknownl3_2color_set(chip_id, color, FALSE);
		if (ret)
			return ret;

		ret = jl_qos_color_map_trust_l3_set(chip_id, UTP_PORT2, DISABLED);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_tos_map2color_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 color = QOS_COLOR_RED;
	jl_uint8 tos = 0x11;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_color_map_trust_l3_set(chip_id, UTP_PORT3, ENABLED);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d color map trust l3 success!\n", UTP_PORT3);

		ret = jl_qos_tos_2color_set(chip_id, tos, color);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set tos %d map to color %d success!\n", tos, color);
	} else {
		ret = jl_qos_tos_2color_get(chip_id, tos, &color);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get tos %d map to color %d success!\n", tos, color);

		ret = jl_qos_tos_2color_set(chip_id, tos, QOS_COLOR_GREEN);
		if (ret)
			return ret;

		ret = jl_qos_color_map_trust_l3_set(chip_id, UTP_PORT3, DISABLED);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_exp_map2color_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 color = QOS_COLOR_RED;
	jl_uint8 exp = 7;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_color_map_trust_l3_set(chip_id, UTP_PORT3, ENABLED);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d color map trust l3 success!\n", UTP_PORT3);

		ret = jl_qos_exp_2color_set(chip_id, exp, color);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set exp %d map to color %d success!\n", exp, color);
	} else {
		ret = jl_qos_exp_2color_get(chip_id, exp, &color);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get exp %d map to color %d success!\n", exp, color);

		ret = jl_qos_exp_2color_set(chip_id, exp, QOS_COLOR_GREEN);
		if (ret)
			return ret;

		ret = jl_qos_color_map_trust_l3_set(chip_id, UTP_PORT3, DISABLED);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_pcp_map2color_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 color = QOS_COLOR_RED;
	jl_uint8 pcp = 7;
	jl_uint8 dei = 1;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_pcpdei_2color_set(chip_id, pcp, dei, color);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set pcp %d dei %d map to color %d success!\n", pcp, dei, color);
	} else {
		ret = jl_qos_pcpdei_2color_get(chip_id, pcp, dei, &color);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get pcp %d dei %d map to color %d success!\n", pcp, dei, color);

		ret = jl_qos_pcpdei_2color_set(chip_id, pcp, dei, QOS_COLOR_GREEN);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_remap_vlan_by_port_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_qos_remap_vlan_t remap;

	memset(&remap, 0, sizeof(jl_qos_remap_vlan_t));

	if (op == QOS_EXAMPLE_SET) {
		remap.vlan_single_opt_valid = TRUE;
		remap.vlan_single_opt = QOS_VLAN_OPT_PUSH;

		remap.vid_sel_valid = TRUE;
		remap.vid_sel = QOS_REMAP_VID_FROM_EPORT;
		remap.vlan_port.vid = 111;
		remap.pcp_sel_valid = TRUE;
		remap.pcp_sel = QOS_REMAP_PCP_FROM_EPORT;
		remap.vlan_port.valid = TRUE;
		remap.vlan_port.pcp = 5;
		remap.cfi_dei_sel_valid = TRUE;
		remap.cfi_dei_sel = QOS_REMAP_PCP_FROM_EPORT;
		remap.vlan_port.cfi_dei = 1;

		ret = jl_qos_remap_vlan_set(chip_id, port, &remap);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d remap pcp %d from egress port success!\n", port, remap.vlan_port.pcp);
	} else {
		memset(&remap, 0, sizeof(jl_qos_remap_vlan_t));

		ret = jl_qos_remap_vlan_get(chip_id, port, &remap);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d remap pcp from egress port success!\n", port);

		remap.vlan_single_opt_valid = TRUE;
		remap.vlan_single_opt = QOS_VLAN_OPT_NO_OPT;

		ret = jl_qos_remap_vlan_set(chip_id, port, &remap);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_remap_vlan_by_queue_mapping_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 idx = 0;
	jl_qos_remap_vlan_t remap;

	memset(&remap, 0, sizeof(jl_qos_remap_vlan_t));

	if (op == QOS_EXAMPLE_SET) {
		remap.vlan_single_opt_valid = TRUE;
		remap.vlan_single_opt = QOS_VLAN_OPT_PUSH;
		remap.pcp_sel_valid = TRUE;
		remap.pcp_sel = QOS_REMAP_PCP_FROM_QUEUE_MAPPING;

		remap.cfi_dei_sel_valid = TRUE;
		remap.cfi_dei_sel = QOS_REMAP_PCP_FROM_QUEUE_MAPPING;

		for (idx = 0; idx < 8; idx++) {
			remap.queue_mapping[idx].pcp = 3;
			remap.queue_mapping[idx].cfi_dei = 1;
			remap.queue_mapping[idx].valid = TRUE;
		}

		ret = jl_qos_remap_vlan_set(chip_id, port, &remap);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d remap pcp from queue mapping tbl success!\n", port);
	} else {
		memset(&remap, 0, sizeof(jl_qos_remap_vlan_t));

		ret = jl_qos_remap_vlan_get(chip_id, port, &remap);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d remap pcp from egress port success!\n", port);

		remap.vlan_single_opt_valid = TRUE;
		remap.vlan_single_opt = QOS_VLAN_OPT_NO_OPT;

		ret = jl_qos_remap_vlan_set(chip_id, port, &remap);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_color_remap_by_port_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT3;
	jl_qos_color_remap_t remap;

	memset(&remap, 0, sizeof(jl_qos_color_remap_t));

	if (op == QOS_EXAMPLE_SET) {
		remap.enable = TRUE;
		remap.remap_mode = QOS_MMP_REMAP_TO_L2_L3;
		remap.green_tos = 15;
		remap.yellow_tos = 100;
		remap.red_tos = 255;
		remap.tos_mask = 0b11111111;

		remap.green_dei = 0;
		remap.yellow_dei = 0;
		remap.red_dei = 1;

		ret = jl_qos_color_remap_set(chip_id, port, &remap);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d remap color from egress port success!\n", port);
	} else {
		memset(&remap, 0, sizeof(jl_qos_color_remap_t));

		ret = jl_qos_color_remap_get(chip_id, port, &remap);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d remap color from egress port success!\n", port);

		remap.enable = FALSE;

		ret = jl_qos_color_remap_set(chip_id, port, &remap);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_color_remap_by_mmp_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 pcp = 0;
	jl_uint8 pointer = 0;
	jl_qos_mmp_cfg_t mmp_cfg;
	jl_qos_mmp_pointer_t mmp;

	memset(&mmp_cfg, 0, sizeof(jl_qos_mmp_cfg_t));

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_mmp_unused_pointer_get(chip_id, &pointer);
		if (ret)
			return ret;

		mmp_cfg.policy_valid = TRUE;
		mmp_cfg.policy.bucket_mode = QOS_MMP_MODE_SR;
		mmp_cfg.policy.rate.s_rate.CIR = 50;
		mmp_cfg.policy.rate.s_rate.CBS = 1000;
		mmp_cfg.policy.rate.s_rate.EBS = 1500;
		mmp_cfg.policy.rate.s_rate.cir_unit = QOS_RATE_UNIT_MBPS;
		mmp_cfg.policy.green_drop = FALSE;
		mmp_cfg.policy.yellow_drop = FALSE;
		mmp_cfg.policy.red_drop = FALSE;
		mmp_cfg.color_blind_valid = TRUE;
		mmp_cfg.color_blind = QOS_COLOR_BLIND;

		mmp_cfg.color_remap_valid = TRUE;
		mmp_cfg.color_remap.enable = TRUE;
		mmp_cfg.color_remap.remap_mode = QOS_MMP_REMAP_TO_L2_L3;
		mmp_cfg.color_remap.tos_mask = 0Xff;
		mmp_cfg.color_remap.green_tos = 14;
		mmp_cfg.color_remap.yellow_tos = 100;
		mmp_cfg.color_remap.red_tos = 255;

		mmp_cfg.color_remap.green_dei = 1;
		mmp_cfg.color_remap.yellow_dei = 1;
		mmp_cfg.color_remap.red_dei = 1;

		ret = jl_qos_mmp_cfg_set(chip_id, pointer, &mmp_cfg);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set mmp %d cfg success!\n", pointer);

		mmp.mmp_valid = TRUE;
		mmp.mmp_pointer = pointer;
		mmp.mmp_order = QOS_MMP_ORDER_LV0;

		ret = jl_qos_ingress_admission_ctl_set(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d pcp %d ingress admission cfg success!\n", port, pcp);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "green to tos %d, yellow to tos %d, red to tos %d\n",
		       mmp_cfg.color_remap.green_tos, mmp_cfg.color_remap.yellow_tos, mmp_cfg.color_remap.red_tos);
	} else {
		ret = jl_qos_ingress_admission_ctl_get(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d ingress admission cfg success, mmp pointer index: %d order: %d !\n",
		       port, mmp.mmp_pointer, mmp.mmp_order);

		ret = jl_qos_mmp_cfg_get(chip_id, pointer, &mmp_cfg);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get mmp pointer %d cfg success, bucket_mode: %d !\n",
		       pointer, mmp_cfg.policy.bucket_mode);

		mmp.mmp_valid = FALSE;
		ret = jl_qos_ingress_admission_ctl_set(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;

		mmp_cfg.color_remap_valid = TRUE;
		mmp_cfg.color_remap.enable = FALSE;

		ret = jl_qos_mmp_cfg_set(chip_id, pointer, &mmp_cfg);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_ingress_admission_control_test_srTCM(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 pcp = 7;
	jl_uint8 pointer = 0;
	jl_qos_mmp_pointer_t mmp;
	jl_qos_mmp_cfg_t mmp_cfg;

	memset(&mmp_cfg, 0, sizeof(jl_qos_mmp_cfg_t));

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_mmp_unused_pointer_get(chip_id, &pointer);
		if (ret)
			return ret;

		mmp_cfg.policy_valid = TRUE;
		mmp_cfg.policy.bucket_mode = QOS_MMP_MODE_SR;
		mmp_cfg.policy.rate.s_rate.CIR = 30;
		mmp_cfg.policy.rate.s_rate.CBS = 10000;
		mmp_cfg.policy.rate.s_rate.EBS = 15000;
		mmp_cfg.policy.rate.s_rate.cir_unit = QOS_RATE_UNIT_MBPS;
		mmp_cfg.policy.green_drop = FALSE;
		mmp_cfg.policy.yellow_drop = FALSE;
		mmp_cfg.policy.red_drop = TRUE;
		mmp_cfg.color_blind_valid = TRUE;
		mmp_cfg.color_blind = QOS_COLOR_AWARE;

		ret = jl_qos_mmp_cfg_set(chip_id, pointer, &mmp_cfg);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set mmp %d cfg success!\n", pointer);

		mmp.mmp_valid = TRUE;
		mmp.mmp_pointer = pointer;
		mmp.mmp_order = QOS_MMP_ORDER_LV3;

		ret = jl_qos_ingress_admission_ctl_set(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d pcp %d CIR %d ingress admission cfg success!\n", port, pcp, mmp_cfg.policy.rate.s_rate.CIR);
	} else {
		ret = jl_qos_ingress_admission_ctl_get(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d ingress admission cfg success, mmp pointer index: %d order: %d !\n",
		       port, mmp.mmp_pointer, mmp.mmp_order);

		ret = jl_qos_mmp_cfg_get(chip_id, pointer, &mmp_cfg);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get mmp pointer %d cfg success, bucket_mode: %d !\n",
		       pointer, mmp_cfg.policy.bucket_mode);

		mmp.mmp_valid = FALSE;
		ret = jl_qos_ingress_admission_ctl_set(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;
	}
	return 0;
}


static jl_ret_t qos_ingress_admission_control_test_trTCM(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 pcp = 7;
	jl_uint8 pointer = 0;
	jl_qos_mmp_pointer_t mmp;
	jl_qos_mmp_cfg_t mmp_cfg;

	memset(&mmp_cfg, 0, sizeof(jl_qos_mmp_cfg_t));

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_mmp_unused_pointer_get(chip_id, &pointer);
		if (ret)
			return ret;

		mmp_cfg.policy_valid = TRUE;
		mmp_cfg.policy.bucket_mode = QOS_MMP_MODE_TR;
		mmp_cfg.policy.rate.t_rate.CIR = 40;
		mmp_cfg.policy.rate.t_rate.CBS = 10000;
		mmp_cfg.policy.rate.t_rate.PIR = 80;
		mmp_cfg.policy.rate.t_rate.PBS = 20000;

		mmp_cfg.policy.rate.t_rate.cir_unit = QOS_RATE_UNIT_MBPS;
		mmp_cfg.policy.rate.t_rate.pir_unit = QOS_RATE_UNIT_MBPS;

		mmp_cfg.policy.green_drop = FALSE;
		mmp_cfg.policy.yellow_drop = FALSE;
		mmp_cfg.policy.red_drop = TRUE;
		mmp_cfg.color_blind_valid = TRUE;
		mmp_cfg.color_blind = QOS_COLOR_AWARE;

		ret = jl_qos_mmp_cfg_set(chip_id, pointer, &mmp_cfg);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set mmp %d cfg success!\n", pointer);

		mmp.mmp_valid = TRUE;
		mmp.mmp_pointer = pointer;
		mmp.mmp_order = QOS_MMP_ORDER_LV3;

		ret = jl_qos_ingress_admission_ctl_set(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d pcp %d ingress admission cfg success!\n", port, pcp);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "CIR %d PIR %d\n", mmp_cfg.policy.rate.t_rate.CIR, mmp_cfg.policy.rate.t_rate.PIR);
	} else {
		ret = jl_qos_ingress_admission_ctl_get(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d ingress admission cfg success, mmp pointer index: %d order: %d !\n",
		       port, mmp.mmp_pointer, mmp.mmp_order);

		ret = jl_qos_mmp_cfg_get(chip_id, pointer, &mmp_cfg);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get mmp pointer %d cfg success, bucket_mode: %d !\n",
		       pointer, mmp_cfg.policy.bucket_mode);

		mmp.mmp_valid = FALSE;
		ret = jl_qos_ingress_admission_ctl_set(chip_id, port, pcp, &mmp);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_queue_map_trust_l3_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 enable = TRUE;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_queue_map_trust_l3_set(chip_id, port, enable);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d queue map trust l3 success!\n", port);
	} else {
		ret = jl_qos_queue_map_trust_l3_get(chip_id, port, &enable);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d queue map trust l3 success, enable %d!\n", port, enable);

		ret = jl_qos_queue_map_trust_l3_set(chip_id, port, DISABLED);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_color_map_trust_l3_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 enable = TRUE;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_color_map_trust_l3_set(chip_id, port, enable);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d color map trust l3 success!\n", port);
	} else {
		ret = jl_qos_color_map_trust_l3_get(chip_id, port, &enable);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d color map trust l3 success, enable %d!\n", port, enable);

		ret = jl_qos_color_map_trust_l3_set(chip_id, port, DISABLED);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_queue_shaper_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 idx = 0;
	jl_qos_queue_shaper_t shaper;

	memset(&shaper, 0, sizeof(jl_qos_queue_shaper_t));

	if (op == QOS_EXAMPLE_SET) {
		for (idx = 0; idx < 8; idx++) {
			shaper.queue[idx].enable = ENABLED;
			shaper.queue[idx].rate = 10000;
			shaper.queue[idx].unit = QOS_RATE_UNIT_PPS;
			shaper.queue[idx].valid = TRUE;
		}

		ret = jl_qos_queue_shaper_set(chip_id, port, &shaper);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d queue shaper rate %d unit %d success!\n",
		       port, shaper.queue[0].rate, shaper.queue[0].unit);
	} else {
		ret = jl_qos_queue_shaper_get(chip_id, port, &shaper);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d queue shaper success! rate %d unit %d\n",
		       port, shaper.queue[0].rate, shaper.queue[0].unit);

		for (idx = 0; idx < 8; idx++) {
			shaper.queue[idx].enable = DISABLED;
			shaper.queue[idx].valid = TRUE;
		}
		ret = jl_qos_queue_shaper_set(chip_id, port, &shaper);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_priority_shaper_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 idx = 0;
	jl_qos_priority_shaper_t shaper;

	memset(&shaper, 0, sizeof(jl_qos_priority_shaper_t));

	if (op == QOS_EXAMPLE_SET) {
		for (idx = 0; idx < 8; idx++) {
			shaper.priority[idx].enable = ENABLED;
			shaper.priority[idx].rate = 30;
			shaper.priority[idx].unit =  QOS_RATE_UNIT_MBPS;
			shaper.priority[idx].valid = TRUE;
		}

		ret = jl_qos_priority_shaper_set(chip_id, port, &shaper);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d priority shaper success! rate %d unit %d\n",
		       port, shaper.priority[0].rate, shaper.priority[0].unit);
	} else {
		ret = jl_qos_priority_shaper_get(chip_id, port, &shaper);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d priority shaper success! rate %d unit %d\n",
		       port, shaper.priority[0].rate, shaper.priority[0].unit);

		for (idx = 0; idx < 8; idx++) {
			shaper.priority[idx].enable = DISABLED;
			shaper.priority[idx].valid = TRUE;
		}
		ret = jl_qos_priority_shaper_set(chip_id, port, &shaper);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_port_shaper_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT0;
	jl_qos_port_shaper_t shaper;

	memset(&shaper, 0, sizeof(jl_qos_port_shaper_t));

	shaper.enable = ENABLED;
	shaper.rate = 40;
	shaper.unit =  QOS_RATE_UNIT_MBPS;

	if (op == QOS_EXAMPLE_SET) {
		ret = jl_qos_port_shaper_set(chip_id, port, &shaper);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d  shaper success! rate %d unit %d\n",
		       port, shaper.rate, shaper.unit);
	} else {
		ret = jl_qos_port_shaper_get(chip_id, port, &shaper);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d shaper success! rate %d unit %d\n",
		       port, shaper.rate, shaper.unit);

		shaper.enable = DISABLED;

		ret = jl_qos_port_shaper_set(chip_id, port, &shaper);
		if (ret)
			return ret;
	}
	return 0;
}

static jl_ret_t qos_dwrr_weight_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_uint8 idx = 0;
	jl_qos_queue_dwrr_t dwrr;

	memset(&dwrr, 0, sizeof(jl_qos_queue_dwrr_t));

	if (op == QOS_EXAMPLE_SET) {
		for (idx = 0; idx < 8; idx++) {
			dwrr.queue[idx].weight = idx + 1;
			dwrr.queue[idx].valid = TRUE;

			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "queue %d weight %d\n", idx, dwrr.queue[idx].weight);
		}

		ret = jl_qos_dwrr_weight_set(chip_id, port, &dwrr);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d dwrr success!\n", port);
	} else {
		ret = jl_qos_dwrr_weight_get(chip_id, port, &dwrr);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d dwrr success!\n", port);

		for (idx = 0; idx < 8; idx++) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "queue %d weight %d\n", idx, dwrr.queue[idx].weight);
		}

		for (idx = 0; idx < 8; idx++) {
			dwrr.queue[idx].weight = 1;
			dwrr.queue[idx].valid = TRUE;
		}
		ret = jl_qos_dwrr_weight_set(chip_id, port, &dwrr);
		if (ret)
			return ret;
	}
	return 0;
}


static jl_ret_t qos_enqueue_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret;
	jl_uint8 port = UTP_PORT2;
	jl_qos_enqueue_state_t queue_mask;

	memset(&queue_mask, 0, sizeof(jl_qos_enqueue_state_t));

	if (op == QOS_EXAMPLE_SET) {
		queue_mask.enable_mask = 0b11110000;
//		ret = jl_qos_enqueue_enable_set(chip_id, port, &queue_mask);
//		if (ret)
//			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d queue enable 0x%x success!\n", port, queue_mask.enable_mask);
	} else {
		ret = jl_qos_enqueue_enable_get(chip_id, port, &queue_mask);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d queue enable 0x%x success!\n", port, queue_mask.enable_mask);

		queue_mask.enable_mask = 0xff;

//		ret = jl_qos_enqueue_enable_set(chip_id, port, &queue_mask);
//		if (ret)
//			return ret;
	}
	return 0;
}

static jl_ret_t qos_schedule_test(jl_uint32 chip_id, jl_uint8 op)
{
	jl_api_ret_t ret = 0;
	jl_uint8 port = UTP_PORT2, idx;
	jl_qos_schedule_t t_sche;

	if (op == QOS_EXAMPLE_SET) {

		t_sche.type[0] = QOS_SCHE_SP;
		t_sche.type[1] = QOS_SCHE_DWRR;
		t_sche.type[2] = QOS_SCHE_SP;
		t_sche.type[3] = QOS_SCHE_DWRR;
		t_sche.type[4] = QOS_SCHE_SP;
		t_sche.type[5] = QOS_SCHE_DWRR;
		t_sche.type[6] = QOS_SCHE_SP;
		t_sche.type[7] = QOS_SCHE_SP;

		ret = jl_qos_schedule_set(chip_id, port, &t_sche);
		if (ret)
			return ret;

		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Set port %d schedule success!\n", port);
	}
	else {

		ret = jl_qos_schedule_get(chip_id, port, &t_sche);
		if (ret)
			return ret;
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Get port %d schedule success!\n", port);
		for (idx = 0; idx < QOS_QUEUE_NUM; idx++)
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Queue %d schedule type: %d!\n", idx, t_sche.type[idx]);

		t_sche.type[0] = QOS_SCHE_SP;
		t_sche.type[1] = QOS_SCHE_SP;
		t_sche.type[2] = QOS_SCHE_SP;
		t_sche.type[3] = QOS_SCHE_SP;
		t_sche.type[4] = QOS_SCHE_SP;
		t_sche.type[5] = QOS_SCHE_SP;
		t_sche.type[6] = QOS_SCHE_SP;
		t_sche.type[7] = QOS_SCHE_SP;

		ret = jl_qos_schedule_set(chip_id, port, &t_sche);
		if (ret)
			return ret;
	}
	return 0;
}



int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;

	jl_api_ret_t ret = 0;
	char console_input[128] = {0};
	jl_uint32 chip_id = 0;
	jl_uint32 cmd = 0;
	jl_uint32 op = 0;
	jl_uint32 param_num = 0;
	char *pstr = NULL;

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

	/* init sdk */
	ret = jl_switch_init();
	if (ret)
		return ret;

	ret = jl_switch_device_create(&dev_6108);
	if (ret)
		return ret;

	ret = jl_qos_res_init(chip_id, 8);
	if (ret)
		return ret;

	while (!g_exit) {

		print_QoS_usage();

		pstr = fgets(console_input, 127, stdin);
		if (pstr == NULL)
			continue;

		if (!strncmp(console_input, "q", 1))
			goto exit;

		clear_window();

		param_num = sscanf(console_input, "%d%*[ ]%d", &cmd, &op);

		if (param_num != 2) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\nInvalid input cmd!\nShould be <cmd> <opt>\n");
			continue;
		}

		switch (cmd) {
		case 1:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS nonvlan map to queue test start!\n");
			ret = qos_nonvlan_map2q_test(chip_id, op);
			break;
		case 2:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS unknown map to queue test start!\n");
			ret = qos_unknown_map2q_test(chip_id, op);
			break;
		case 3:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS MAC map to queue test start!\n");
			ret = qos_mac_map2q_test(chip_id, op);
			break;
		case 4:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS IP map to queue test start!\n");
			ret = qos_ip_map2q_test(chip_id, op);
			break;
		case 5:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS VID map to queue test start!\n");
			ret = qos_vid_map2q_test(chip_id, op);
			break;
		case 6:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS EthType map to queue test start!\n");
			ret = qos_ethtype_map2q_test(chip_id, op);
			break;
		case 7:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS TOS map to queue test start!\n");
			ret = qos_tos_map2q_test(chip_id, op);
			break;
		case 8:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS EXP map to queue test start!\n");
			ret = qos_exp_map2q_test(chip_id, op);
			break;
		case 9:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS PCP map to queue test start!\n");
			ret = qos_pcp_map2q_test(chip_id, op);
			break;
		case 10:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS L4 PORT map to queue test start!\n");
			ret = qos_l4_port_map2q_test(chip_id, op);
			break;
		case 11:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS L4 PROTOCOL map to queue test start!\n");
			ret = qos_l4_protocol_map2q_test(chip_id, op);
			break;
		case 12:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS nonvlan map to color test start!\n");
			ret = qos_nonvlan_map2color_test(chip_id, op);
			break;
		case 13:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS unknown map to color test start!\n");
			ret = qos_unknown_map2color_test(chip_id, op);
			break;
		case 14:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS TOS map to color test start!\n");
			ret = qos_tos_map2color_test(chip_id, op);
			break;
		case 15:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS EXP map to color test start!\n");
			ret = qos_exp_map2color_test(chip_id, op);
			break;
		case 16:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS PCP map to color test start!\n");
			ret = qos_pcp_map2color_test(chip_id, op);
			break;
		case 17:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS remap vlan by port test start!\n");
			ret = qos_remap_vlan_by_port_test(chip_id, op);
			break;
		case 18:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS remap vlan by queue mapping test start!\n");
			ret = qos_remap_vlan_by_queue_mapping_test(chip_id, op);
			break;
		case 19:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS color remap by port test start!\n");
			ret = qos_color_remap_by_port_test(chip_id, op);
			break;
		case 20:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS color remap color by mmp test start!\n");
			ret = qos_color_remap_by_mmp_test(chip_id, op);
			break;
		case 21:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS ingress admission control srTCM test start!\n");
			ret = qos_ingress_admission_control_test_srTCM(chip_id, op);
			break;
		case 22:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS ingress admission control trTCM start!\n");
			ret = qos_ingress_admission_control_test_trTCM(chip_id, op);
			break;
		case 23:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS queue map trust l3 test start!\n");
			ret = qos_queue_map_trust_l3_test(chip_id, op);
			break;
		case 24:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS color map trust l3 test start!\n");
			ret = qos_color_map_trust_l3_test(chip_id, op);
			break;
		case 25:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS queue shaper test start!\n");
			ret = qos_queue_shaper_test(chip_id, op);
			break;
		case 26:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS priority shaper test start!\n");
			ret = qos_priority_shaper_test(chip_id, op);
			break;
		case 27:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS queue dwrr test start!\n");
			ret = qos_dwrr_weight_test(chip_id, op);
			break;
		case 28:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS enable queue test start!\n");
			ret = qos_enqueue_test(chip_id, op);
			break;
		case 29:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS port shaper test start!\n");
			ret = qos_port_shaper_test(chip_id, op);
			break;
		case 30:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n QoS schedule type test start!\n");
			ret = qos_schedule_test(chip_id, op);
			break;
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n Test commad: error!\n");
			break;
		}
	}

	if (ret)
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Cmd %d option %d test execution failed!\n\n", cmd, op);

exit:
	ret = jl_qos_res_deinit(chip_id);
	if (ret)
		return ret;
		
	ret = jl_switch_device_delete(chip_id);
	if (ret)
		return ret;

	/* deinit sdk */
	ret = jl_switch_deinit();
	if (ret)
		return ret;

	return JL_ERR_OK;
}



