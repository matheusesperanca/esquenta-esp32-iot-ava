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

// definitions
#define STANDARD_WAIT_TIME 1000 // main loop update time (ms)

#define TAG_SENSOR            "SENSOR" 
#define TAG_MAIN              "MAIN" 
#define SENSOR_REFRESH_TIME   2500 // sensor update time (ms)
#define SENSOR_QUEUE_SIZE     10

#define LEDSTATUS_QUEUE_SIZE  1
#define LED_PIN               GPIO_NUM_19
#define BUTTON_PIN            GPIO_NUM_22

// queue declaration
static QueueHandle_t fSensorQueue;

// function prototypes
void initApp(void); // user application initialization
void vSensorTask(void *pvParameter); // sensor measurements task
static void vSendData(void); // sensor measurements send data
void vLedControlTask(void *pvParameter); // led control task
static void initButton(uint32_t gpio_pin_number); // button initialization
static void vButtonCallBack(void *arg); // callback initialization

// sensor measurements struct
struct xSENSOR_MEASUREMENTS
{
    float temp;
    float hum;
    float angle;
};
typedef struct xSENSOR_MEASUREMENTS SensorMeasurements_t;

#endif