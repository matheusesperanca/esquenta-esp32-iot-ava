/**
 * @file goniometer.h
 * @author Matheus Arcangelo Esperança
 * @brief  Fonte para leitura de angulo do potenciometro
 * @version 0.1
 * @date 2023-03-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef DEVICE_DRIVERS_GONIOMETER_INC_GONIOMETER_H_
#define DEVICE_DRIVERS_GONIOMETER_INC_GONIOMETER_H_

#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"

#define ANGLE_MAX	180
#define ADC_WIDTH	4095
#define TAG_GON     "GONIOMETRO"

void initGoniometer(void);
int readGoniometer( float *ang ); // método para leitura do goniometro

static const adc_channel_t channel = ADC_CHANNEL_0;  // GPIO36

#endif