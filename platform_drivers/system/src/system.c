/**
 * @file system.c
 * @author Matheus Arcangelo Esperanca
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "system.h"

/**
 * @brief Inicializa sistema
 * 
 */
void initSystem(void)
{
    // inicializa a flash da esp
    ESP_ERROR_CHECK( nvs_flash_init());
    ESP_LOGI(TAG_SYSTEM,"Initializing System");
    
    xTaskCreate(&vHeartBeatTask, "heartBeatTask", 1024*2, NULL, 7, NULL);
}

/**
 * @brief Tarefa auxiliar para controle de LED a operacao normal
 * 
 * @param pvParameter - Parametros para inicializacao da tarega
*/
void vHeartBeatTask(void *pvParameter)
{
    ESP_LOGI(TAG_SYSTEM, "Iniciando HeartBeatTask");
    gpio_pad_select_gpio(LED_HEART_BEAT_PIN);
    gpio_set_direction(LED_HEART_BEAT_PIN, GPIO_MODE_OUTPUT);
    while(1)
    {
        gpio_set_level(LED_HEART_BEAT_PIN, false);
        vTaskDelay(LED_HEART_BEAT_TOFF / portTICK_PERIOD_MS); // delay em ms
        gpio_set_level(LED_HEART_BEAT_PIN, true);
        vTaskDelay(LED_HEART_BEAT_TON / portTICK_PERIOD_MS); // delay em ms
    }
}