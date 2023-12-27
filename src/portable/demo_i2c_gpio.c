#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "jl_types.h"
#include "jl_error.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
//"A GPIO (over STM32 F4) porting example. Do not release this part, just for demo&debug using"
#include "gpio.h"

static GPIO_TypeDef *scl_gpio_port = GPIO_SCL_GPIO_Port;
static GPIO_TypeDef *sda_gpio_port = GPIO_SDA_GPIO_Port;

jl_ret_t port_gpio_request(jl_gpio_desc_t *gpio)
{
	if (gpio->pin == GPIO_SCL_Pin)
		gpio->priv = scl_gpio_port;
	if (gpio->pin == GPIO_SDA_Pin)
		gpio->priv = sda_gpio_port;

	return JL_ERR_OK;
}

jl_ret_t port_gpio_free(jl_gpio_desc_t *gpio)
{
	gpio->pin = 0xFF;
	gpio->priv = NULL;

	return JL_ERR_OK;
}

/*dir: 0: input, 1: output*/
void port_gpio_set_dir(jl_gpio_desc_t *gpio, jl_int32 dir)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	if (dir) {
		GPIO_InitStruct.Pin = gpio->pin;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		HAL_GPIO_Init((GPIO_TypeDef *)gpio->priv, &GPIO_InitStruct);
	} else {
		GPIO_InitStruct.Pin = gpio->pin;
		GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
		HAL_GPIO_Init((GPIO_TypeDef *)gpio->priv, &GPIO_InitStruct);
	}
}

void port_gpio_set_value(jl_gpio_desc_t *gpio, jl_int32 val)
{
	HAL_GPIO_WritePin((GPIO_TypeDef *)gpio->priv, gpio->pin, (val ? GPIO_PIN_SET : GPIO_PIN_RESET));
}

jl_int32 port_gpio_get_value(jl_gpio_desc_t *gpio)
{
	jl_int32 ret;

	ret = HAL_GPIO_ReadPin((GPIO_TypeDef *)gpio->priv, gpio->pin);
	return (ret == GPIO_PIN_SET) ? 1 : 0;
}

