#include <stdlib.h>
#include <string.h>

#include "jl_types.h"

void *port_mutex_init(void)
{
	return NULL;
	/* return mutex; */
}

void port_mutex_deinit(void *arg)
{
	(void) arg;
}

void port_mutex_lock(void *arg)
{
	(void) arg;
}

void port_mutex_unlock(void *arg)
{
	(void) arg;
}

void port_ndelay(jl_uint32 ns)
{
	(void) ns;
}

void port_udelay(jl_uint32 us)
{
	(void) us;
}

void *port_mem_malloc(jl_uint32 size)
{
	/* If user-defined memory used, plz change std lib to user-defined func */
	return malloc(size);
}

void port_mem_free(void *ptr)
{
	/* If user-defined memory used, plz change std lib to user-defined func */
	free(ptr);
}
