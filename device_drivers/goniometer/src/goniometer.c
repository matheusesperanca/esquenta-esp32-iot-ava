/**
 * @file goniometer.c
 * @author Matheus Arcangelo Esperanca
 * @brief Goniometer driver
 * @version 0.1
 * @date 2023-03-10
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "goniometer.h"

/**
 * @brief Goniometer initialization
 * 
 */
void initGoniometer(void)
{
    // adc resolution configuration
    adc1_config_width(ADC_WIDTH_BIT_12);
}

/**
 * @brief Goniometer read function
 * 
 * @param angle angle float pointer
 * @return int 0 se ok, -1 - falha
 */
esp_err_t readGoniometer(float *ang)
{   
    int raw = adc1_get_raw(channel);
    *ang = (float)ANGLE_MAX * ( (float)raw / (float)ADC_WIDTH );
    if((*ang < 0) || (*ang > ANGLE_MAX))
    {
        return ESP_FAIL; // conversion error
    }
    return ESP_OK;
}
