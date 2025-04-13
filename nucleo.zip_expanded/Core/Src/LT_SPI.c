/*
 * LT_SPI.c
 *
 *  Created on: Mar 30, 2025
 *      Author: verma
 */


#include "LT_SPI.h"

extern SPI_HandleTypeDef hspi2;

void cs_low(void){
    // Set chip select pin low.
    HAL_GPIO_WritePin(LTC6804_CHIP_SELECT_GPIO_PORT, LTC6804_CHIP_SELECT_GPIO_PIN, GPIO_PIN_RESET);
}

void cs_high(void){
    HAL_GPIO_WritePin(LTC6804_CHIP_SELECT_GPIO_PORT, LTC6804_CHIP_SELECT_GPIO_PIN, GPIO_PIN_SET);
}

void spi_write(uint8_t data){
		uint8_t ret_val;
		HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&data, &ret_val, 1, HAL_MAX_DELAY);
}

void spi_write_array(uint8_t len,uint8_t data[]){
    uint8_t ret_val;
    uint8_t i;

    for ( i = 0; i < len; i++ )
    {
        HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&data[i], &ret_val, 1, HAL_MAX_DELAY);
    }
}

// spi_write_read

void spi_write_read(uint8_t  tx_Data[],uint8_t  tx_len,uint8_t* rx_data,uint8_t  rx_len){
    uint8_t i;
    uint8_t data;

    // Transfer data to LTC6804
    for ( i = 0; i < tx_len; i++ )
    {
        // Transmit byte.
        HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)&tx_Data[i], &data, 1, HAL_MAX_DELAY);
    }
}

// spi_read_byte

uint8_t spi_read_byte(uint8_t tx_dat){
    uint8_t data;

    if ( HAL_SPI_TransmitReceive(&hspi2, (uint8_t*)0xFF, (uint8_t*)&data, 1, HAL_MAX_DELAY) == HAL_OK )
    {
        return(data);
    }
		return(-1);
}
