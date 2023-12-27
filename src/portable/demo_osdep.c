#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include "jl_types.h"

#if defined CONFIG_JL_API_MUTEX || defined CONFIG_JL_IO_MUTEX
#if __MINGW32__ || __MINGW64__
#pragma message("Note: SDK not support mutex yet for MINGW toolchain!!!")
void *port_mutex_init(void)
{
	return NULL;
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
#else
#include <pthread.h>
void *port_mutex_init(void)
{
	pthread_mutex_t *mutex;

	mutex = malloc(sizeof(pthread_mutex_t));
	if (!mutex)
		return NULL;

	pthread_mutex_init(mutex, NULL);

	return mutex;
}

void port_mutex_deinit(void *arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;

	if (mutex) {
		pthread_mutex_destroy(mutex);
		free(mutex);
	}
}

void port_mutex_lock(void *arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;

	if (mutex)
		pthread_mutex_lock(mutex);
}

void port_mutex_unlock(void *arg)
{
	pthread_mutex_t *mutex = (pthread_mutex_t *)arg;

	if (mutex)
		pthread_mutex_unlock(mutex);
}
#endif
#endif

void port_ndelay(jl_uint32 ns)
{
	jl_uint32 i;
	jl_uint32 us = ns / 1000;

	for (i = 0; i < us;)
		i++;
}

void port_udelay(jl_uint32 us)
{
	usleep(us);
}

void *port_mem_malloc(jl_uint32 size)
{
	return malloc(size);
}

void port_mem_free(void *ptr)
{
	free(ptr);
}