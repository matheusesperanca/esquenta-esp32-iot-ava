/**
 * @file wifi.c
 * @author Matheus Arcangelo Esperanca
 * @brief Wifi connection 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "wifi.h"

// wifi ssid and password configuration
#define MYSSID              "MYSSID"
#define MYPASSWORD          "MYPASSWORD"

#define TAG_WIFI "WIFI"

const static int CONNECTION_STATUS = BIT0;

// Wifi event and group handlers
static EventGroupHandle_t wifi_event_group;
static esp_err_t wifi_event_handler(void *ctx, system_event_t *event);


/**
 * @brief Wifi event handler
 * 
 * @param ctx Reference context pointer
 * @param event Reference event pointer
 * @return esp_err_t ESP_FAIL or ESP_OK 
 */
static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        esp_wifi_connect();
        break;

    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTION_STATUS);
        break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect();
        xEventGroupClearBits(wifi_event_group, CONNECTION_STATUS);
        break;
    
    default:
        break;
    }
    return ESP_OK;
}

/**
 * @brief Wifi connection initialization
 * 
 */
void initWifi(void)
{
    ESP_LOGI(TAG_WIFI, "Initializing Wifi");
    // initializing pcp stack
    tcpip_adapter_init();
    // initializing event group
    wifi_event_group = xEventGroupCreate();
    // initializing event handlers
    ESP_ERROR_CHECK( esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg));
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = MYSSID, // ssid
            .password = MYPASSWORD, // password
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA)); //station mode
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config)); 
    ESP_ERROR_CHECK( esp_wifi_start());
    // wait for connection
    xEventGroupWaitBits(wifi_event_group, CONNECTION_STATUS, false, true, portMAX_DELAY);
}