/**
 * @file main.h
 * @author Matheus Arcangelo Esperanca
 * @brief  Class "Eletronica embarcada com iot, IA e robótica" (exercicio esquenta-esp32-iot AVA) 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAIN_INC_MAIN_H_
#define MAIN_INC_MAIN_H_

// general library includes 
#include <stdio.h>

// FreeRTOS library includes
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>

// drivers includes
#include "driver/gpio.h"

// Espressif library includes
#include "sdkconfig.h"

// Application includes
#include "system.h"
#include "wifi.h"
#include "mqtt.h"
#include "dht.h"
#include "goniometer.h"
#include "button.h"

// cJSON library includes
#include <cJSON.h>

#define STANDARD_WAIT_TIME 1000 // main loop update time (ms)

#define TAG_SENSOR            "SENSOR" 
#define TAG_MAIN              "MAIN" 
#define SENSOR_REFRESH_TIME   3000 // sensor update time (ms)
#define SENSORQUEUE_WAIT_TIME 5000 
#define SENSOR_QUEUE_SIZE     3

#define LEDSTATUS_QUEUE_SIZE  1
#define LED_PIN               GPIO_NUM_19

// queue declaration
static QueueHandle_t fSensorQueue;

// function prototypes
void initApp(void); // user application initialization
void vSensorTask(void *pvParameter); // sensor measurements task
void vSendDataTask(void *pvParameter); // sensor measurements send data task
void vLedControlTask(void *pvParameter); // led control task

// sensor measurements struct
struct xSENSOR_MEASUREMENTS
{
    float temp;
    float hum;
    float angle;
};
typedef struct xSENSOR_MEASUREMENTS SensorMeasurements_t;

#endif