/**
 * @file mqtt.c
 * @author Matheus Arcangelo Esperanca
 * @brief MQTT sensor data comunication and Led control
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

// MQTT include
#include "mqtt.h"

// mqtt event handler initizalization
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event);

/**
 * @brief MQTT service initialization
 * 
 */
void initMQTT(void)
{   
    // "connected" eventgroup creation
    mqttEventGroup = xEventGroupCreate();
    if(mqttEventGroup == NULL)
    {
        ESP_LOGE(TAG_MQTT,"Error creating MQTT EventGroup");
    }
    ESP_LOGI(TAG_MQTT, "Initializing MQTT");
    const esp_mqtt_client_config_t mqtt_cfg = {
        .uri = MQTT_USER_URI,
        .event_handle = mqtt_event_handler,
        .client_id = DEVICE_ID,
    };
    // MQTT client initialization
    mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_start(mqtt_client);

    ESP_LOGI(TAG_MQTT,"Waiting for broker connection");
    xEventGroupWaitBits(mqttEventGroup, CONNECTED_BIT, false, false, portMAX_DELAY);
}

/**
 * @brief MQTT communication event handler
 * 
 * @param event event parameters
 * @return esp_err_t ESP_OK ou ESP_FAIL
 */
static esp_err_t mqtt_event_handler(esp_mqtt_event_handle_t event)
{
    // recovering client identification
    esp_mqtt_client_handle_t client = event->client;
    // message id reference
    int msg_id;

    switch(event->event_id)
    {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG_MQTT,"Connected on broker");
            // update connection status
            xEventGroupSetBits(mqttEventGroup, CONNECTED_BIT);
            // subscribed topics
            msg_id = esp_mqtt_client_subscribe(client, TOPIC_MQTT_CMD, 0);
            ESP_LOGI(TAG_MQTT,"Subscribe msg ID: %d",msg_id);
            break;

        case MQTT_EVENT_DISCONNECTED:
            ESP_LOGI(TAG_MQTT,"Disconnected from broker");
            // update connection status
            xEventGroupClearBits(mqttEventGroup, CONNECTED_BIT);
            // treat reconnection here
            break;

        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG_MQTT,"Subscribe succesfully");
            break;

        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG_MQTT,"Publish successfully");
            break;
            
        case MQTT_EVENT_DATA:         
            ESP_LOGI(TAG_MQTT, "Data received from broker");

            // recovering received data 
            char *data = malloc(event->data_len + 1);
            if(!data)
            {
                ESP_LOGE(TAG_MQTT, "Malloc error");
                return ESP_FAIL;
            }
            memcpy(data, event->data, event->data_len);
            data[event->data_len] = '\0';

            // recovering received topic
            char *topic = malloc(event->topic_len + 1);
            if(!topic)
            {
                ESP_LOGE(TAG_MQTT, "Malloc error");
                return ESP_FAIL;
            }
            memcpy(topic, event->topic, event->topic_len);
            topic[event->topic_len] = '\0';
    
            // topic confirmation
            if(strcmp(topic, TOPIC_MQTT_CMD) == false)
            {
                CommandReceived_t cmdReceived; 

                // creating json objects
                cJSON *jdata;
                cJSON *jcode;
                cJSON *jcmd;

                // parsing data to json objects
                jdata = cJSON_Parse(data);
                jcode = cJSON_GetObjectItem(jdata, RECEIVED_CODE);
                jcmd = cJSON_GetObjectItem(jdata, RECEIVED_CMD);

                // reading cmd and code from json objects
                cmdReceived.code = (int)jcode->valuedouble;
                cmdReceived.cmd = (int)jcmd->valuedouble;

                cJSON_Delete(jdata); // releasing memory

                // validating received code
                if(cmdReceived.code == LED_CONTROL_CODE)
                {
                    // queue data send (led status value)
                    xQueueSend(fLedStatusQueue, &cmdReceived.cmd, pdMS_TO_TICKS(0));
                }
                else
                {
                    ESP_LOGE(TAG_MQTT, "Unknown code: %d", cmdReceived.code);
                }
            }
            else
            {
                ESP_LOGE(TAG_MQTT, "Unknown topic: %s", topic);
            }
            
            // releasing memory
            free(data);
            free(topic);
            break;

        default:
            break;
    }

    return ESP_OK; //ok
}