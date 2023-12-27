#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#ifdef FREERTOS_SYS
#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"
#include "timers.h"

#define DIV_ROUND_UP(n, d) (((n)+(d)-1) / (d))

void port_ndelay(uint32_t ns)
{
	vTaskDelay(pdMS_TO_TICKS(DIV_ROUND_UP(ns, 1000000)));
}

void port_udelay(uint32_t us)
{
	vTaskDelay(pdMS_TO_TICKS(DIV_ROUND_UP(us, 1000)));
}

void *port_mem_malloc(uint32_t size)
{
	return pvPortMalloc(size);
}

void port_mem_free(void *ptr)
{
	vPortFree(ptr);
}

#if defined CONFIG_JL_API_MUTEX || defined CONFIG_JL_IO_MUTEX
void *port_mutex_init(void)
{
	SemaphoreHandle_t *mutex = port_mem_malloc(sizeof(SemaphoreHandle_t));
	if (mutex == NULL)
		return NULL;

	*mutex = (SemaphoreHandle_t)xSemaphoreCreateMutex();
	if (*mutex == NULL)
		return NULL;

	return mutex;
}

void port_mutex_deinit(void *arg)
{
	SemaphoreHandle_t *mutex = (SemaphoreHandle_t *)arg;

	if (mutex == NULL) {
		return;
	}
	vSemaphoreDelete(*mutex);
	port_mem_free(mutex);
}

void port_mutex_lock(void *arg)
{
	SemaphoreHandle_t *mutex = (SemaphoreHandle_t *)arg;
	if (mutex == NULL)
		return;

	xSemaphoreTake(*mutex, portMAX_DELAY);
}

void port_mutex_unlock(void *arg)
{
	SemaphoreHandle_t *mutex = (SemaphoreHandle_t *)arg;
	if (mutex == NULL)
		return;

	xSemaphoreGive(*mutex);
}
#endif

#else
#if defined CONFIG_JL_API_MUTEX || defined CONFIG_JL_IO_MUTEX
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
#endif


void port_ndelay(uint32_t ns)
{
	(void)ns;
}

void port_udelay(uint32_t us)
{
	(void)us;
}

void *port_mem_malloc(uint32_t size)
{
	return malloc(size);
}

void port_mem_free(void *ptr)
{
	free(ptr);
}

#endif

