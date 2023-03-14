/**
 * @file dht.h
 * @author Matheus Arcangelo Esperanca (adaptation of Andrey-m library)
 * @brief DHT22 sensor driver
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DEVICE_DRIVERS_DHT_INC_DHT_H_
#define DEVICE_DRIVERS_DHT_INC_DHT_H_

// general library includes
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

// FreeRTOS library includes
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>

// Espressif library includes
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include <esp_wifi.h>
#include <esp_event.h>

// drivers includes
#include <driver/gpio.h>

// function prototypes
void  initDHT(void);
int   readDHT(float *temperature, float *humidity);
void  errorHandlerDHT(int response);

#endif