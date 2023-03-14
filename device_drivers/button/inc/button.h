/**
 * @file button.h
 * @author Matheus Arcangelo Esperanca
 * @brief Button ISR driver
 * @version 0.1
 * @date 2023-03-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DEVICE_DRIVERS_BUTTON_INC_BUTTON_H_
#define DEVICE_DRIVERS_BUTTON_INC_BUTTON_H_

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
#include <freertos/portmacro.h>

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
#include "driver/periph_ctrl.h"

// definitions 
#define TAG_BUTTON      "BUTTON"

#define BUTTON_PIN		GPIO_NUM_22
#define DEBOUNCE_TIME   50

// event group creation
EventGroupHandle_t buttonEventGroup;
const static int BUTTON0 = BIT0;

void initButtonISR(void);

#endif