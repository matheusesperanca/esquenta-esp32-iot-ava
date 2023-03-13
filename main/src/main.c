/**
 * @file main.c
 * @author Matheus Arcangelo Esperanca
 * @brief  Aula eletronica embarcada com iot, IA e robótica (exercicio esquenta-esp32-iot AVA)
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"

/**
 * @brief Loop principal
 * 
 */
void app_main(void)
{
    // inicializa sistema da esp32
    initSystem();

    // inicializa aplicacoes do usuario
    initApp();

    // loop principal
    while(true){ vTaskDelay( STANDARD_WAIT_TIME / portTICK_PERIOD_MS ); }
}

/**
 * @brief Inicializacao aplicacao do usuario
 * 
 */
void initApp(void)
{
    // inicializa wifi
    initWifi();
    // inicaliza mqtt
    initMQTT();
    // inicializa Goniometro
    initGoniometer();
    // inicializa Sensor DHT
    initDHT();
    // inicializa tratamento do botao
    initButtonISR();

    ESP_LOGI(TAG_MAIN,"Aguardando conexao com o broker");
    xEventGroupWaitBits(mqttEventGroup, CONNECTED_BIT, false, false, portMAX_DELAY);

    //tarefas dependentes da conexao com o broker

    // tarefa controle led
    xTaskCreate(&vLedControlTask, "LedControlTask", 1024*2, NULL, 5, NULL);
    // tarefa de leitura dos sensores
    xTaskCreate(&vSensorTask, "SensorTask", 1024*4, NULL, 5, NULL);
    // tarefa envio dados
    xTaskCreate(&vSendDataTask, "SendDataTask", 1024*4, NULL, 5, NULL);
}

/**
 * @brief Tarefa para leitura periodica do sensor de temperatura e umidade DHT22
 * Vamos modificar o projeto "Esquenta ESP32 IoT" para operar da seguinte forma:
 * a. Modifique a tarefa DHT_task para ser uma tarefa mais ampla, como "Sensor_Task", a fim de realizar a leitura dos seguintes sinais a cada 5s:
 * - Temperatura (via DHT22)
 * - Umidade (via DHT22)
 * - Posição do Potenciômetro(convertida para 0 a 180 graus, via Potenciômetro)
 * 
 * @param pvParameter 
 */
void vSensorTask(void *pvParameter)
{
    ESP_LOGI(TAG_SENSOR, "Iniciando SensorTask");
    static SensorMeasurements_t measurements;

    // cria a fila para recebimento dos dados do sensor
    fSensorQueue = xQueueCreate(SENSOR_QUEUE_SIZE,sizeof(SensorMeasurements_t));

    while(true)
    {
        ESP_LOGI(TAG_SENSOR, "Lendo sensores");
        int retDHT = 0;
        int retAngle = 0;
        retDHT = readDHT( &measurements.temp, &measurements.hum );
        if(retDHT != 0)
        {
            ESP_LOGE(TAG_SENSOR,"Falha na leitura do DHT");
            errorHandlerDHT(retDHT);
        }
        retAngle = readGoniometer( &measurements.angle );
        if(retAngle != 0)
        {
            ESP_LOGE(TAG_SENSOR,"Falha na leitura do Goniometro");
        }
        if((retDHT && retAngle) == 0)
        {
            xQueueSend(fSensorQueue, &measurements, pdMS_TO_TICKS(0));
        }
        else
        {
            ESP_LOGE(TAG_SENSOR,"Erro - fila nao atualizada");
        }
        vTaskDelay(SENSOR_REFRESH_TIME / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Tarefa para envio dos dados medidos para o broker
 * 
 * @param parameters 
 */
void vSendDataTask(void *pvParameter)
{
    ESP_LOGI(TAG_SENSOR, "Iniciando SendDataTask");
    static SensorMeasurements_t fDataToSend;
    while(true)
    {
        xEventGroupWaitBits(buttonEventGroup, ISRBIT0, false, false, portMAX_DELAY);
        vTaskDelay(DEBOUNCE_TIME / portTICK_PERIOD_MS);
        if(gpio_get_level(BUTTON_PIN) == 0)
        {
            ESP_LOGI(TAG_MAIN,"Botao pressionado");
            if(xQueueReceive(fSensorQueue, &fDataToSend, pdMS_TO_TICKS(SENSORQUEUE_WAIT_TIME)) == true)
            {
                cJSON *json;
                char *json_str;
                char *conv_str;
                // Criamos o objeto json a ser utilizado
                json = cJSON_CreateObject();
                cJSON_AddStringToObject(json, "ID", DEVICE_ID);
                asprintf(&conv_str, "%.1f", fDataToSend.temp);
                cJSON_AddStringToObject(json, "TEMP", conv_str);
                asprintf(&conv_str, "%.1f", fDataToSend.hum);
                cJSON_AddStringToObject(json, "UMID", conv_str);
                asprintf(&conv_str, "%.1f", fDataToSend.angle);
                cJSON_AddStringToObject(json, "ANG", conv_str);
                json_str = cJSON_Print(json); //construimos a string a ser enviada via MQTT

                esp_mqtt_client_publish(mqtt_client, TOPIC_MQTT_DATA, json_str, strlen(json_str), 0, 0);

                ESP_LOGI(TAG_SENSOR, "Dados publicados");

                cJSON_Delete(json); // libera memoria do ponteiro
                free(json_str); 
                free(conv_str); 
            }
        }
        xEventGroupClearBits(buttonEventGroup, ISRBIT0);
        vTaskDelay(DEBOUNCE_TIME / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Tarefa para controle do led baseado no comando recebido via mqtt
 * 
 * @param pvParameter 
 */
void vLedControlTask(void *pvParameter)
{
    static int bLedStatus;
    // cria a fila para recebimento dos dados do sensor
    fLedStatusQueue = xQueueCreate(LEDSTATUS_QUEUE_SIZE,sizeof(bool));\
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while(true)
    {
        xQueueReceive(fLedStatusQueue, &bLedStatus, portMAX_DELAY);
        gpio_set_level(LED_PIN, (bool)bLedStatus);
        vTaskDelay(STANDARD_WAIT_TIME / portTICK_PERIOD_MS);
    }
}