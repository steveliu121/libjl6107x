#include "jl_types.h"
#include "jl_error.h"
#include "jl_device.h"

#include "portable/jl_portable.h"

jl_ret_t port_gpio_request(jl_gpio_desc_t *gpio)
{
	(void) gpio;

	return JL_ERR_OK;
}

jl_ret_t port_gpio_free(jl_gpio_desc_t *gpio)
{
	(void) gpio;

	return JL_ERR_OK;
}

/*dir: 0: input, 1: output*/
void port_gpio_set_dir(jl_gpio_desc_t *gpio, jl_int32 dir)
{
	(void) gpio;
	(void) dir;
}

void port_gpio_set_value(jl_gpio_desc_t *gpio, jl_int32 val)
{
	(void) gpio;
	(void) val;
}

jl_int32 port_gpio_get_value(jl_gpio_desc_t *gpio)
{
	(void) gpio;

	return JL_ERR_OK;
}
