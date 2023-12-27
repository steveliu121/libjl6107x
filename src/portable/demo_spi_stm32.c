#include <stdint.h>
#include <stdio.h>

#include "jl_types.h"
#include "jl_error.h"
#include "jl_device.h"
#include "portable/jl_portable.h"
#include "main.h"

extern SPI_HandleTypeDef hspi1;
jl_api_ret_t port_spi_init(jl_io_desc_t *io_desc)
{
	jl_ret_t ret = JL_ERR_OK;
	hspi1.Instance = SPI1;
	hspi1.Init.Mode = SPI_MODE_MASTER;
	hspi1.Init.Direction = SPI_DIRECTION_2LINES;
	hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
	hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
	hspi1.Init.NSS = SPI_NSS_SOFT;
	hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_128;
	hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi1.Init.CRCPolynomial = 7;
	HAL_SPI_Init(&hspi1);

	__HAL_SPI_ENABLE(&hspi1);

	return ret;
}

jl_api_ret_t port_spi_deinit(jl_io_desc_t *io_desc)
{
	jl_ret_t ret = JL_ERR_OK;
	return ret;
}

jl_ret_t port_spi_read(jl_io_desc_t *io_desc,
		jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size)

{
	HAL_GPIO_WritePin(SPI_CS1_PORT, SPI_CS1_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, size, 100);
	HAL_GPIO_WritePin(SPI_CS1_PORT, SPI_CS1_PIN, GPIO_PIN_SET);

	return JL_ERR_OK;
}
jl_ret_t port_spi_write(jl_io_desc_t *io_desc,
		jl_uint8 *tx_buf, jl_uint8 *rx_buf, jl_uint32 size)

{
	HAL_GPIO_WritePin(SPI_CS1_PORT, SPI_CS1_PIN, GPIO_PIN_RESET);
	HAL_SPI_TransmitReceive(&hspi1, tx_buf, rx_buf, size, 100);
	HAL_GPIO_WritePin(SPI_CS1_PORT, SPI_CS1_PIN, GPIO_PIN_SET);
}
