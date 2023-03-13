/**
 * @file button.h
 * @author Matheus Arcangelo Esperanca
 * @brief 
 * @version 0.1
 * @date 2023-03-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DEVICE_DRIVERS_BUTTON_INC_BUTTON_H_
#define DEVICE_DRIVERS_BUTTON_INC_BUTTON_H_

// 1 - Includes de bibl1otecas de suporte geral
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>

// Bibliotecas do FreeRTOS
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>
#include <freertos/portmacro.h>

// Bibliotecas gerais da Espressif
#include <esp_system.h>
#include <esp_spi_flash.h>
#include <nvs_flash.h>
#include <esp_log.h>
#include <sdkconfig.h>
#include <esp_wifi.h>
#include <esp_event.h>

// Inclusao de driver
#include <driver/gpio.h>
#include "driver/periph_ctrl.h"

#define TAG_BUTTON      "BUTTON"

#define BUTTON_PIN		GPIO_NUM_22
#define DEBOUNCE_TIME   50

void initButtonISR(void);
static void IRAM_ATTR GPIOISRHandler(void* arg);

EventGroupHandle_t buttonEventGroup;
const static int ISRBIT0 = BIT0;

#endif