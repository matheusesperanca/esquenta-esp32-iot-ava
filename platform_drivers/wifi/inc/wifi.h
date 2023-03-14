/**
 * @file wifi.h
 * @author Matheus Arcangelo Esperanca
 * @brief  Wifi connection 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLATFORM_DRIVERS_WIFI_INC_WIFI_H_
#define PLATFORM_DRIVERS_WIFI_INC_WIFI_H_

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

// Espressif general library includes
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include <esp_wifi.h>
#include <esp_event.h>

// function prototypes
void initWifi(void); // inicializacao do wifi

#endif