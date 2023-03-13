/**
 * @file wifi.c
 * @author Matheus Arcangelo Esperanca
 * @brief 
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "wifi.h"

/**
 * @brief Rotina para resposta a transicao de eventos de conexao Wifi
 * 
 * @param ctx Ponteiro de referencia para contexto do evento gerado
 * @param event Ponteiro para referencia do evento gerado
 * @return esp_err_t Status de retorno do erro
 */
static esp_err_t wifi_event_handler(void *ctx, system_event_t *event)
{
    switch (event->event_id)
    {
    case SYSTEM_EVENT_STA_START:
        // STA - STATION - Equipamento conecta a ponto de acesso sem fio
        esp_wifi_connect(); // manda conectar na rede
        break;

    case SYSTEM_EVENT_STA_GOT_IP:
        xEventGroupSetBits(wifi_event_group, CONNECTION_STATUS);
        break;

    case SYSTEM_EVENT_STA_DISCONNECTED:
        esp_wifi_connect(); // tenta conectar novamente
        xEventGroupClearBits(wifi_event_group, CONNECTION_STATUS);
        break;
    
    default:
        break;
    }
    return ESP_OK;
}

/**
 * @brief Rotina para inicializacao da pilha WiFi do ESP32
 * Não tem retorno
 * 
 */
void initWifi(void)
{
    ESP_LOGI(TAG_WIFI, "Initializing Wifi");
    // inicializacao da pilha tcp
    tcpip_adapter_init();
    // inicializacao do grupo de eventos
    wifi_event_group = xEventGroupCreate();
    // inicializacao do gerenciador de eventos
    ESP_ERROR_CHECK( esp_event_loop_init(wifi_event_handler, NULL));
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg)); // inicializa com a pilha padrao
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) ); // dados de conexao na ram
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = MYSSID, // ssid - nome da rede
            .password = MYPASSWORD, // password - senha da rede
        }
    };
    ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA)); // modo station
    ESP_ERROR_CHECK( esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config)); // 
    ESP_ERROR_CHECK( esp_wifi_start());
    //espera o evento para setar o bit de conexao:
    xEventGroupWaitBits(wifi_event_group, CONNECTION_STATUS, false, true, portMAX_DELAY);
}