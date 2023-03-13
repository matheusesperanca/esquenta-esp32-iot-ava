/**
 * @file wifi.h
 * @author Matheus Arcangelo Esperanca
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLATFORM_DRIVERS_WIFI_INC_WIFI_H_
#define PLATFORM_DRIVERS_WIFI_INC_WIFI_H_

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
  
// Biblioteca do cJSON
#include <cJSON.h>

// Biblioteca MQTT
#include <mqtt_client.h>

#define MYSSID              "ESPERANCA"
#define MYPASSWORD          "7*_!,chuP@c@Br@5"

#define TAG_WIFI "WIFI"

const static int CONNECTION_STATUS = BIT0;   // SE 0 - desconectado 1 - conectado


// Wifi - Referencias de status de conexao
static EventGroupHandle_t wifi_event_group;
// wifi "event_handler" cuidam de responder aos eventos de wifi
static esp_err_t wifi_event_handler(void *ctx, system_event_t *event);

// 3 - Prototipos de funcoes e tarefas
void initWifi(void); // inicializacao do wifi

#endif