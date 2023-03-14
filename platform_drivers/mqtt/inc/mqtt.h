/**
 * @file mqtt.h
 * @author Matheus Arcangelo Esperanca
 * @brief MQTT sensor data comunication and Led control
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef PLATFORM_DRIVERS_MQTT_INC_MQTT_H_
#define PLATFORM_DRIVERS_MQTT_INC_MQTT_H_

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

// drivers includes
#include <driver/gpio.h>
  
// cJSON library include
#include <cJSON.h>

// MQTT library include
#include <mqtt_client.h>

// MQTT definitions
#define DEVICE_ID       "MQTT_ESP32_Matheus"
#define TOPIC_MQTT_DATA "unisal/iot/matheusesperanca/alert"
#define TOPIC_MQTT_CMD  "unisal/iot/matheusesperanca/cmd"
#define MQTT_USER_URI   "mqtt://mqtt.eclipseprojects.io:1883"
#define TAG_MQTT        "MQTT"

// Led control command definitions
#define RECEIVED_CODE   "code"
#define RECEIVED_CMD    "cmd"
#define LED_CONTROL_CODE 42 

// Led status control queue declaration
QueueHandle_t fLedStatusQueue;

// Connected bit event group declarations
EventGroupHandle_t mqttEventGroup;
const static int CONNECTED_BIT = BIT0;

// MQTT client handler declarations
esp_mqtt_client_handle_t mqtt_client;

// general function prototypes
void initMQTT(void); // mqtt initialization

/**
 * @brief MQTT comand received struct
 * 
 */
struct xCMD_RECEIVED
{
    int cmd;    // command number (value)
    int code;   // code number (key)
};
typedef struct xCMD_RECEIVED CommandReceived_t;

#endif