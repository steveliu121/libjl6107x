#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
#include <string.h>

#include "jl_switch.h"
#include "jl_led.h"

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

static void print_led_config_list(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_LINK\t\t\t\t%u\n", LED_CONFIG_LINK);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_ACT\t\t\t\t%u\n", LED_CONFIG_ACT);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_LINK_ACT\t\t\t%u(default)\n", LED_CONFIG_LINK_ACT);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_SPD10\t\t\t%u\n", LED_CONFIG_SPD10);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_SPD100\t\t\t%u\n", LED_CONFIG_SPD100);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_SPD1000\t\t\t%u\n", LED_CONFIG_SPD1000);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_SPD10_ACT\t\t\t%u\n", LED_CONFIG_SPD10_ACT);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_SPD100_ACT\t\t\t%u\n", LED_CONFIG_SPD100_ACT);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_SPD1000_ACT\t\t\t%u\n", LED_CONFIG_SPD1000_ACT);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_SPD100_10_ACT\t\t\t%u\n", LED_CONFIG_SPD100_10_ACT);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_DUPLEX\t\t\t%u\n", LED_CONFIG_DUPLEX);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_COL\t\t\t\t%u\n", LED_CONFIG_COL);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_DUP_COL\t\t\t%u\n", LED_CONFIG_DUP_COL);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_LOOP\t\t\t\t%u\n", LED_CONFIG_LOOP);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_LINK_RX_ACT\t\t\t%u\n", LED_CONFIG_LINK_RX_ACT);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_LINK_TX_ACT\t\t\t%u\n", LED_CONFIG_LINK_TX_ACT);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_MASTER\t\t\t%u\n", LED_CONFIG_MASTER);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_CONFIG_OFF\t\t\t%u\n", LED_CONFIG_OFF);
}

static void print_led_blink_rate(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_BLINKRATE_32MS\t\t\t%u\n", LED_BLINKRATE_32MS);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_BLINKRATE_48MS\t\t\t%u\n", LED_BLINKRATE_48MS);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_BLINKRATE_64MS\t\t\t%u\n", LED_BLINKRATE_64MS);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_BLINKRATE_96MS\t\t\t%u\n", LED_BLINKRATE_96MS);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_BLINKRATE_128MS\t\t\t%u\n", LED_BLINKRATE_128MS);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_BLINKRATE_256MS\t\t\t%u(default)\n", LED_BLINKRATE_256MS);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_BLINKRATE_512MS\t\t\t%u\n", LED_BLINKRATE_512MS);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_BLINKRATE_1024MS\t\t\t%u\n", LED_BLINKRATE_1024MS);
}

static inline void print_led_polarity(void)
{
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_ACTIVE_HIGH\t\t\t\t%u(default)\n", LED_ACTIVE_HIGH);
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "LED_ACTIVE_LOW\t\t\t\t%u\n", LED_ACTIVE_LOW);
}

static int led_config_test(jl_uint32 chip_id)
{
	char console_input[128] = {0};
	jl_api_ret_t ret = 0;
	jl_uint32 led_group = 0;
	jl_led_config_t config = 0;
	jl_led_config_t	cur_config = 0;
	char *pstr = NULL;

	do {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led group config:<led_group, config>\n");
		print_led_config_list();
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\texample: you want configure led1 group as LED_CONFIG_LINK\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\t1, 0\n");
		pstr = fgets(console_input, 127, stdin);
		if (pstr == NULL) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "<<<<<<Wrong input>>>>>>\n");
			continue;
		}
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");

		sscanf(console_input, "%u,%u", &led_group, &config);
		break;
	} while (1);

	ret = jl_led_group_config_set(chip_id, led_group, config);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "set led_group: %u, config=0x%x fail\n",
				led_group, config);
		return ret;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "observe the led behavior..., press any key to continue\n");
	(void)getchar();

	ret = jl_led_group_config_get(chip_id, led_group, &cur_config);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "get led_group: %u fail\n", led_group);
		return ret;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led_group: %u, cur_config=%u\n",
			led_group, cur_config);
	return ret;
}

static int led_bink_rate_test(jl_uint32 chip_id)
{
	char console_input[128] = {0};
	jl_api_ret_t ret = 0;
	jl_led_blink_rate_t rate = 0;
	jl_led_blink_rate_t cur_rate = 0;
	char *pstr = NULL;

	do {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "set led blinkrate:<blinkrate>\n");
		print_led_blink_rate();
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\texample: you want to set led blinkrate as LED_BLINKRATE_256MS\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\t32\n");
		pstr = fgets(console_input, 127, stdin);
		if (pstr == NULL) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "<<<<<<Wrong input>>>>>>\n");
			continue;
		}
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");

		sscanf(console_input, "%u", &rate);
		break;
	} while (1);

	ret = jl_led_blink_rate_set(chip_id, rate);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "set led_blink_rate=0x%x fail\n", rate);
		return ret;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "observe the led behavior..., press any key to continue\n");
	(void)getchar();

	ret = jl_led_blink_rate_get(chip_id, &cur_rate);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "get current led blinkrate fail\n");
		return ret;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "current led blinkrate=%ums\n", cur_rate*8);

	return ret;
}

static int led_polarity_test(jl_uint32 chip_id)
{
	char console_input[128] = {0};
	jl_api_ret_t ret = 0;
	jl_uint32 led_group = 0;
	jl_portmask_t portmask = 0;
	jl_portmask_t cur_portmask = 0;
	char *pstr = NULL;

	do {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led group polarity:<led_group, portmask>\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\texample: you want to set utp_port3,utp_port4's led0 as active high\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\t	       other ports led0 as active low\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\t0, 24\n");
		pstr = fgets(console_input, 127, stdin);
		if (pstr == NULL) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "<<<<<<Wrong input>>>>>>\n");
			continue;
		}
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");

		sscanf(console_input, "%u,%u", &led_group, &portmask);
		break;
	} while (1);

	ret = jl_led_group_active_high_set(chip_id, led_group, portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "set active_high led_group: %u, portmask=0x%x fail\n",
				led_group, portmask);
		return ret;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "observe the led behavior..., press any key to continue\n");
	(void)getchar();

	ret = jl_led_group_active_high_get(chip_id, led_group, &cur_portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "get active_high led_group: %ufail\n", led_group);
		return ret;
	}

	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led_group: %u, set as active high portmask=%u\n",
			led_group, cur_portmask);

	return ret;
}

static int led_group_enable_test(jl_uint32 chip_id)
{
	char console_input[128] = {0};
	jl_api_ret_t ret = 0;
	jl_uint32 led_group = 0;
	jl_portmask_t portmask = 0;
	jl_portmask_t cur_portmask = 0;
	char *pstr = NULL;

	do {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led group enable:<led_group, portmask>\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\texample: you want enable utp_port0,utp_port1's led0\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\t0, 3\n");
		pstr = fgets(console_input, 127, stdin);
		if (pstr == NULL) {
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "<<<<<<Wrong input>>>>>>\n");
			continue;
		}
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n");

		sscanf(console_input, "%u,%u", &led_group, &portmask);
		break;
	} while (1);

	ret = jl_led_enable_set(chip_id, led_group, portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "enable led_group: %u, portmask=0x%x fail\n",
				led_group, portmask);
		return ret;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "observe the led behavior..., press any key to continue\n");
	(void)getchar();

	ret = jl_led_enable_get(chip_id, led_group, &cur_portmask);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "get enable status led_group: %ufail\n", led_group);
		return ret;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "curent led_group:%u, cur_portmask=%u\n", led_group, cur_portmask);
	return ret;
}

int main(int argc, char *argv[])
{
	(void) argc;
	(void) argv;
	jl_ret_t ret = 0;
	char console_input[128] = {0};
	int cmd_index = -1;
	jl_uint32 chip_id = 0;
	char *pstr = NULL;

	signal(SIGINT, sig_handle);
	signal(SIGTERM, sig_handle);

	jl_dev_t dev_6108 = {
		.compat_id = JL_CHIP_6107,
		.name = "device-jl6107",
		.id = chip_id, /* must be less than JL_MAX_CHIP_NUM */
		.io = {
			.type = JL_IO_SMI_SPI_DEMO,
			.smi.mdio.bus_id = 0
		}
	};

	ret = jl_switch_init();
	if (ret != 0) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "smi init fail\n");
		return -1;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "after jl_switch_init \n");

	ret = jl_switch_device_create(&dev_6108);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device create fail\n");
		return -1;
	}
	JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "after jl_switch_device_create \n");

	while (!g_exit) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "\n======JL SWITCH: %s=======\n", argv[0]);
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "1. led group enable/disable test\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "2. led config test\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "3. led blink rate test\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "4. led polarity test\n");
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Type a number or 'q' to exit\n");

		pstr = fgets(console_input, 127, stdin);
		if (pstr == NULL)
			continue;
		if (!strncmp(console_input, "q", 1))
			goto exit;
		else
			cmd_index = atoi(console_input);

		clear_window();

		switch (cmd_index) {
		case 1:
			ret = led_group_enable_test(chip_id);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led_group_enable_test\n");
				return ret;
			}
			break;
		case 2:
			ret = led_config_test(chip_id);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led_config_test\n");
				return ret;
			}
			break;
		case 3:
			ret = led_bink_rate_test(chip_id);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led_bink_rate_test\n");
				return ret;
			}
			break;
		case 4:
			ret = led_polarity_test(chip_id);
			if (ret) {
				JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "led_polarity_test\n");
				return ret;
			}
			break;
		default:
			JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "Unknown input message\n");
			break;
		}

	};
exit:
	ret = jl_switch_device_delete(chip_id);
	if (ret) {
		JL_DBG_MSG(JL_FLAG_SYS, _DBG_INFO, "device delete fail\n");
		return -1;
	}
	jl_switch_deinit();
	return ret;
}


