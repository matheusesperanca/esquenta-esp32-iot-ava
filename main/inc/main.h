/**
 * @file main.h
 * @author Matheus Arcangelo Esperanca
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef MAIN_INC_MAIN_H_
#define MAIN_INC_MAIN_H_

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <freertos/semphr.h>
#include <freertos/queue.h>
#include <freertos/event_groups.h>
#include "driver/gpio.h"
#include "sdkconfig.h"

#include "system.h"
#include "wifi.h"
#include "mqtt.h"
#include "dht.h"
#include "goniometer.h"
#include "button.h"

// Biblioteca do cJSON
#include <cJSON.h>

#define STANDARD_WAIT_TIME 1000 // tempo atualizacao loop principal

#define TAG_SENSOR            "SENSOR" 
#define TAG_MAIN              "MAIN" 
#define SENSOR_REFRESH_TIME   3000 // tempo atualizacao leitura dos sensores
#define SENSORQUEUE_WAIT_TIME 5000
#define SENSOR_QUEUE_SIZE     3

#define LEDSTATUS_QUEUE_SIZE  1
#define LED_PIN               GPIO_NUM_19

static QueueHandle_t fSensorQueue;

// prototipo de funcao
void initApp(void); // funcao para inicializacao da aplicacao do usuario
void vSensorTask(void *pvParameter); // tarefa para leitura dos sensores
void vSendDataTask(void *pvParameter); // tarefa para envio dos dados dos sensores
void vLedControlTask(void *pvParameter); // tarefa para controle do led por comando recebido via mqtt

struct xSENSOR_MEASUREMENTS
{
    float temp;
    float hum;
    float angle;
};
typedef struct xSENSOR_MEASUREMENTS SensorMeasurements_t;

#endif