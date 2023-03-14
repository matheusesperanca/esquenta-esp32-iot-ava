/**
 * @file system.h
 * @author Matheus Arcangelo Esperanca
 * @brief ESP32 system initializations
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLATFORM_DRIVERS_SYSTEM_INC_SYSTEM_H_
#define PLATFORM_DRIVERS_SYSTEM_INC_SYSTEM_H_

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

// Drivers includes
#include <driver/gpio.h>
  
// cJSON library include
#include <cJSON.h>

// MQTT library include
#include <mqtt_client.h>

// definitions 
#define LED_HEART_BEAT_PIN  2      
#define LED_HEART_BEAT_TON  500
#define LED_HEART_BEAT_TOFF 500

#define TAG_SYSTEM "SYSTEM"

// function prototypes
void initSystem(void);
void vHeartBeatTask(void *pvParameter); // tarefa pisca led (código "rodando")

#endif