#include "jl61xx/jl61xx_drv_led.h"
#include "jl61xx/jl61xx_drv_switch.h"
#include "jl61xx/jl61xx_chip.h"

jl_uint32 __led_config_to_mask(jl_led_config_t config)
{
	jl_uint32 src_mask = 0;

	switch (config) {
	case LED_CONFIG_LINK:
		src_mask = 0x2FF;
		break;
	case LED_CONFIG_ACT:
		src_mask = 0x3FC;
		break;
	case LED_CONFIG_LINK_ACT:
		src_mask = 0x2FC;
		break;
	case LED_CONFIG_SPD10:
		src_mask = 0x3E7;
		break;
	case LED_CONFIG_SPD100:
		src_mask = 0x3D7;
		break;
	case LED_CONFIG_SPD1000:
		src_mask = 0x3B7;
		break;
	case LED_CONFIG_SPD10_ACT:
		src_mask = 0x3EC;
		break;
	case LED_CONFIG_SPD100_ACT:
		src_mask = 0x3DC;
		break;
	case LED_CONFIG_SPD1000_ACT:
		src_mask = 0x3BC;
		break;
	case LED_CONFIG_SPD100_10_ACT:
		src_mask = 0x3CC;
		break;
	case LED_CONFIG_DUPLEX:
		src_mask = 0x37F;
		break;
	case LED_CONFIG_COL:
		src_mask = 0x3FB;
		break;
	case LED_CONFIG_DUP_COL:
		src_mask = 0x37B;
		break;
	case LED_CONFIG_LOOP:
		src_mask = 0x3F7;
		break;
	case LED_CONFIG_LINK_RX_ACT:
		src_mask = 0x2FD;
		break;
	case LED_CONFIG_LINK_TX_ACT:
		src_mask = 0x2FE;
		break;
	case LED_CONFIG_MASTER:
		src_mask = 0x1FF;
		break;
	case LED_CONFIG_OFF:
		src_mask = 0x3FF;
		break;
	default:
		src_mask = 0;
		break;
	}
	return src_mask;
}

jl_ret_t __mask_to_led_config(jl_uint32 src_mask, jl_led_config_t *pconfig)
{
	jl_ret_t ret = JL_ERR_OK;

	switch (src_mask) {
	case 0x2FF:
		*pconfig = LED_CONFIG_LINK;
		break;
	case 0x3FC:
		*pconfig = LED_CONFIG_ACT;
		break;
	case 0x2FC:
		*pconfig = LED_CONFIG_LINK_ACT;
		break;
	case 0x3E7:
		*pconfig = LED_CONFIG_SPD10;
		break;
	case 0x3D7:
		*pconfig = LED_CONFIG_SPD100;
		break;
	case 0x3B7:
		*pconfig = LED_CONFIG_SPD1000;
		break;
	case 0x3EC:
		*pconfig = LED_CONFIG_SPD10_ACT;
		break;
	case 0x3DC:
		*pconfig = LED_CONFIG_SPD100_ACT;
		break;
	case 0x3BC:
		*pconfig = LED_CONFIG_SPD1000_ACT;
		break;
	case 0x3CC:
		*pconfig = LED_CONFIG_SPD100_10_ACT;
		break;
	case 0x37F:
		*pconfig = LED_CONFIG_DUPLEX;
		break;
	case 0x3FB:
		*pconfig = LED_CONFIG_COL;
		break;
	case 0x37B:
		*pconfig = LED_CONFIG_DUP_COL;
		break;
	case 0x3F7:
		*pconfig = LED_CONFIG_LOOP;
		break;
	case 0x2FD:
		*pconfig = LED_CONFIG_LINK_RX_ACT;
		break;
	case 0x2FE:
		*pconfig = LED_CONFIG_LINK_TX_ACT;
		break;
	case 0x1FF:
		*pconfig = LED_CONFIG_MASTER;
		break;
	case 0x3FF:
		*pconfig = LED_CONFIG_OFF;
		break;
	default:
		ret = JL_ERR_PARAM;
		break;
	}
	return ret;
}

jl_ret_t jl61xx_led_enable_set(jl_device_t *device, jl_led_group_t group, jl_portmask_t portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_portmask_t swc_port_mask = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_PORTMASK(device, portmask);

	ret = jlxx_portmask_l2c(device, portmask, &swc_port_mask);
	if (ret != JL_ERR_OK)
		return ret;

	FLIP_BITS(swc_port_mask, 0, (LED_LED_CFG_4_OFFSET_CFG_LED0_DISABLE_WIDTH-1));

	if (group == LED_GROUP0) {
		REGISTER_READ(device, LED, LED_CFG_4, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_led0_disable = swc_port_mask;
		REGISTER_WRITE(device, LED, LED_CFG_4, val, INDEX_ZERO, INDEX_ZERO);
	} else if (group == LED_GROUP1) {
		REGISTER_READ(device, LED, LED_CFG_5, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_led1_disable = swc_port_mask;
		REGISTER_WRITE(device, LED, LED_CFG_5, val, INDEX_ZERO, INDEX_ZERO);
	} else if (group == LED_GROUP2) {
		REGISTER_READ(device, LED, LED_CFG_6, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_led2_disable = swc_port_mask;
		REGISTER_WRITE(device, LED, LED_CFG_6, val, INDEX_ZERO, INDEX_ZERO);
	} else
		return JL_ERR_PARAM;

	return ret;
}

jl_ret_t jl61xx_led_enable_get(jl_device_t *device, jl_led_group_t group, jl_portmask_t *pportmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_portmask_t swc_port_mask = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pportmask);

	if (group == LED_GROUP0) {
		REGISTER_READ(device, LED, LED_CFG_4, val, INDEX_ZERO, INDEX_ZERO);
		swc_port_mask = val.BF.cfg_led0_disable;
	} else if (group == LED_GROUP1) {
		REGISTER_READ(device, LED, LED_CFG_5, val, INDEX_ZERO, INDEX_ZERO);
		swc_port_mask = val.BF.cfg_led1_disable;
	} else if (group == LED_GROUP2) {
		REGISTER_READ(device, LED, LED_CFG_6, val, INDEX_ZERO, INDEX_ZERO);
		swc_port_mask = val.BF.cfg_led2_disable;
	} else
		return JL_ERR_PARAM;

	FLIP_BITS(swc_port_mask, 0,  (LED_LED_CFG_4_OFFSET_CFG_LED0_DISABLE_WIDTH-1));

	ret = jlxx_portmask_c2l(device, swc_port_mask,  pportmask);
	return ret;
}

jl_ret_t jl61xx_led_group_config_set(jl_device_t *device, jl_led_group_t group, jl_led_config_t config)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 src_mask = 0;

	JL_CHECK_POINTER(device);

	src_mask = __led_config_to_mask(config);
	if (src_mask == 0)
		return JL_ERR_PARAM;

	if (group == LED_GROUP0) {
		REGISTER_READ(device, LED, LED_CFG_7, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_led0_src_mask = src_mask;
		REGISTER_WRITE(device, LED, LED_CFG_7, val, INDEX_ZERO, INDEX_ZERO);
	} else if (group == LED_GROUP1) {
		REGISTER_READ(device, LED, LED_CFG_8, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_led1_src_mask = src_mask;
		REGISTER_WRITE(device, LED, LED_CFG_8, val, INDEX_ZERO, INDEX_ZERO);
	} else if (group == LED_GROUP2) {
		REGISTER_READ(device, LED, LED_CFG_9, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_led2_src_mask = src_mask;
		REGISTER_WRITE(device, LED, LED_CFG_9, val, INDEX_ZERO, INDEX_ZERO);
	} else
		return JL_ERR_PARAM;

	return ret;
}

jl_ret_t jl61xx_led_group_config_get(jl_device_t *device, jl_led_group_t group, jl_led_config_t *pconfig)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 src_mask = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pconfig);

	if (group == LED_GROUP0) {
		REGISTER_READ(device, LED, LED_CFG_7, val, INDEX_ZERO, INDEX_ZERO);
		src_mask = val.BF.cfg_led0_src_mask;
	} else if (group == LED_GROUP1) {
		REGISTER_READ(device, LED, LED_CFG_8, val, INDEX_ZERO, INDEX_ZERO);
		src_mask = val.BF.cfg_led1_src_mask;
	} else if (group == LED_GROUP2) {
		REGISTER_READ(device, LED, LED_CFG_9, val, INDEX_ZERO, INDEX_ZERO);
		src_mask = val.BF.cfg_led2_src_mask;
	} else
		return JL_ERR_PARAM;

	ret = __mask_to_led_config(src_mask, pconfig);
	return ret;
}

jl_ret_t jl61xx_led_blink_rate_set(jl_device_t *device, jl_led_blink_rate_t blink_rate)
{
	jl_ret_t ret = JL_ERR_OK;

	JL_CHECK_POINTER(device);
	JL_ASSERT(blink_rate < LED_BLINKRATE_END);

	REGISTER_READ(device, LED, LED_CFG_3, val, INDEX_ZERO, INDEX_ZERO);
	val.BF.cfg_led_off_time = blink_rate;
	val.BF.cfg_led_on_time = blink_rate;

	REGISTER_WRITE(device, LED, LED_CFG_3, val, INDEX_ZERO, INDEX_ZERO);
	return ret;
}

jl_ret_t jl61xx_led_blink_rate_get(jl_device_t *device, jl_led_blink_rate_t *pblink_rate)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint8 on_time = 0, off_time = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pblink_rate);

	REGISTER_READ(device, LED, LED_CFG_3, val, INDEX_ZERO, INDEX_ZERO);

	on_time = val.BF.cfg_led_off_time;
	off_time = val.BF.cfg_led_on_time;

	if (on_time != off_time) {
		JL_DBG_MSG(JL_FLAG_LED, _DBG_ERROR,
				"LED blinkrate on_time != off_time!\n");
		return JL_ERR_CONFIG;
	}

	*pblink_rate = on_time;
	return ret;
}

jl_ret_t jl61xx_led_group_active_high_set(jl_device_t *device, jl_led_group_t group, jl_portmask_t portmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_portmask_t swc_port_mask = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_PORTMASK(device, portmask);

	ret = jlxx_portmask_l2c(device, portmask, &swc_port_mask);
	if (ret != JL_ERR_OK)
		return ret;

	FLIP_BITS(swc_port_mask, 0, (LED_LED_CFG_11_OFFSET_CFG_PLED0_POLARITY_WIDTH-1));

	if (group == LED_GROUP0) {
		REGISTER_READ(device, LED, LED_CFG_11, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_pled0_polarity = swc_port_mask;
		REGISTER_WRITE(device, LED, LED_CFG_11, val, INDEX_ZERO, INDEX_ZERO);
	} else if (group == LED_GROUP1) {
		REGISTER_READ(device, LED, LED_CFG_12, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_pled1_polarity = swc_port_mask;
		REGISTER_WRITE(device, LED, LED_CFG_12, val, INDEX_ZERO, INDEX_ZERO);
	} else if (group == LED_GROUP2) {
		REGISTER_READ(device, LED, LED_CFG_13, val, INDEX_ZERO, INDEX_ZERO);
		val.BF.cfg_pled2_polarity = swc_port_mask;
		REGISTER_WRITE(device, LED, LED_CFG_13, val, INDEX_ZERO, INDEX_ZERO);
	} else
		return JL_ERR_PARAM;
	return ret;
}

jl_ret_t jl61xx_led_group_active_high_get(jl_device_t *device, jl_led_group_t group, jl_portmask_t *pportmask)
{
	jl_ret_t ret = JL_ERR_OK;
	jl_uint32 swc_port_mask = 0;

	JL_CHECK_POINTER(device);
	JL_CHECK_POINTER(pportmask);

	if (group == LED_GROUP0) {
		REGISTER_READ(device, LED, LED_CFG_11, val, INDEX_ZERO, INDEX_ZERO);
		swc_port_mask = val.BF.cfg_pled0_polarity;
	} else if (group == LED_GROUP1) {
		REGISTER_READ(device, LED, LED_CFG_12, val, INDEX_ZERO, INDEX_ZERO);
		swc_port_mask = val.BF.cfg_pled1_polarity;
	} else if (group == LED_GROUP2) {
		REGISTER_READ(device, LED, LED_CFG_13, val, INDEX_ZERO, INDEX_ZERO);
		swc_port_mask = val.BF.cfg_pled2_polarity;
	} else
		return JL_ERR_PARAM;

	FLIP_BITS(swc_port_mask, 0, (LED_LED_CFG_11_OFFSET_CFG_PLED0_POLARITY_WIDTH-1));

	ret = jlxx_portmask_c2l(device, swc_port_mask, pportmask);
	return ret;
}

