/**
 * @file dht.h
 * @author Matheus Arcangelo Esperanca
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DEVICE_DRIVERS_DHT_INC_DHT_H_
#define DEVICE_DRIVERS_DHT_INC_DHT_H_

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

#define TAG_DHT "DHT"

#define DHT_OK              0
#define DHT_CHECKSUM_ERROR  -1
#define DHT_TIMEOUT_ERROR   -2
#define MAXDHTDATA          5       // to complete 40 = 5*8 Bits

//#define DHT_PIN             21 //GPIO21
static int pinDHT; 

// prototipos de funcao

int   readDHT(float *temperature, float *humidity);
int   getSignalLevel(int usTimeOut, bool state);
void  errorHandlerDHT(int response);
void  initDHT(void);

#endif