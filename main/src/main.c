/**
 * @file main.c
 * @author Matheus Arcangelo Esperanca
 * @brief  Class "Eletronica embarcada com iot, IA e robótica" (exercicio esquenta-esp32-iot AVA)
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "main.h"

/**
 * @brief Main function
 * 
 */
void app_main(void)
{
    // ESP32 system initialization
    initSystem();

    // Application initialization
    initApp();

    // main loop
    while(true){ vTaskDelay( STANDARD_WAIT_TIME / portTICK_PERIOD_MS ); }
}

/**
 * @brief Application initialization
 * 
 */
void initApp(void)
{
    // Wifi initialization
    initWifi();
    // mqtt initialization
    initMQTT();
    // Goniometer initialization
    initGoniometer();
    // DHT sensor initialization
    initDHT();
    // Button initialization
    initButton(BUTTON_PIN);

    // led control task initialization
    xTaskCreate(&vLedControlTask, "LedControlTask", 1024*2, NULL, 5, NULL);
    // sensor measurements task initialization
    xTaskCreate(&vSensorTask, "SensorTask", 1024*4, NULL, 5, NULL);
}

/**
 * @brief Sensors measurements (temperature, humidity and angle) task
 * 
 * @param pvParameter 
 */
void vSensorTask(void *pvParameter)
{
    ESP_LOGI(TAG_SENSOR, "Initializing SensorTask");
    static SensorMeasurements_t measurements;

    // sensor measurements queue
    fSensorQueue = xQueueCreate(SENSOR_QUEUE_SIZE,sizeof(SensorMeasurements_t));

    while(true)
    {
        ESP_LOGI(TAG_SENSOR, "Read sensors");
        int retDHT = 0;
        int retAngle = 0;
        retDHT = readDHT( &measurements.temp, &measurements.hum );
        if(retDHT != 0)
        {
            ESP_LOGE(TAG_SENSOR,"DHT read error");
            errorHandlerDHT(retDHT);
        }
        retAngle = readGoniometer( &measurements.angle );
        if(retAngle != 0)
        {
            ESP_LOGE(TAG_SENSOR,"Goniometer read error");
        }
        if((retDHT && retAngle) == 0)
        {
            xQueueSend(fSensorQueue, &measurements, pdMS_TO_TICKS(0));
        }
        else
        {
            ESP_LOGE(TAG_SENSOR,"Measurement error");
        }
        vTaskDelay(SENSOR_REFRESH_TIME / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Send data function (MQTT)
 * 
 */
void vSendData(void)
{
    ESP_LOGI(TAG_MAIN, "Sending data");
    static SensorMeasurements_t fDataToSend;
    if(xQueueReceive(fSensorQueue, &fDataToSend, pdMS_TO_TICKS(SENSORQUEUE_WAIT_TIME)) == true)
    {
        cJSON *json;
        char *json_str;
        char *conv_str;
        // json creating
        json = cJSON_CreateObject();
        cJSON_AddStringToObject(json, "ID", DEVICE_ID);
        asprintf(&conv_str, "%.1f", fDataToSend.temp);
        cJSON_AddStringToObject(json, "TEMP", conv_str);
        asprintf(&conv_str, "%.1f", fDataToSend.hum);
        cJSON_AddStringToObject(json, "UMID", conv_str);
        asprintf(&conv_str, "%.1f", fDataToSend.angle);
        cJSON_AddStringToObject(json, "ANG", conv_str);
        json_str = cJSON_Print(json);

        esp_mqtt_client_publish(mqtt_client, TOPIC_MQTT_DATA, json_str, strlen(json_str), 0, 0);

        ESP_LOGI(TAG_MAIN, "Data published");

        cJSON_Delete(json); // release memory
        free(json_str); 
        free(conv_str); 
    }
}

/**
 * @brief Led control task
 * 
 * @param pvParameter 
 */
void vLedControlTask(void *pvParameter)
{
    static int bLedStatus;
    // status receive queue creation
    fLedStatusQueue = xQueueCreate(LEDSTATUS_QUEUE_SIZE,sizeof(bool));\
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    while(true)
    {
        xQueueReceive(fLedStatusQueue, &bLedStatus, portMAX_DELAY);
        gpio_set_level(LED_PIN, (bool)bLedStatus);
        vTaskDelay(STANDARD_WAIT_TIME / portTICK_PERIOD_MS);
    }
}

/**
 * @brief IoT Button initialization
 * 
 */
void initButton(uint32_t gpio_pin_number)
{
    ESP_LOGI(TAG_MAIN,"Initialising pin %d" , gpio_pin_number);
    button_handle_t buttonHandle = iot_button_create(gpio_pin_number, BUTTON_ACTIVE_LOW);
    if (buttonHandle) {
        /* Register a callback for a button tap (short press) event */
        iot_button_set_evt_cb(buttonHandle, BUTTON_CB_PUSH, vButtonCallBack, (void *)gpio_pin_number);
    }
    else
    {
        ESP_LOGE(TAG_MAIN,"Failed to create pin %d handler", gpio_pin_number);
    }
}

/**
 * @brief Button pressed callback
 * 
 * @param arg 
 */
static void vButtonCallBack(void *arg)
{
    switch((uint32_t)arg)
    {
        case BUTTON_PIN:    ESP_LOGI(TAG_MAIN,"IoT button pressed");
                            vSendData();
        break;

        default:            ESP_LOGE(TAG_MAIN,"Unknown button");
        break;
    }
}