/**
 * @file button.c
 * @author Matheus Arcangelo Esperanca
 * @brief 
 * @version 0.1
 * @date 2023-03-11
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "button.h"

void initButtonISR(void)
{
    buttonEventGroup = xEventGroupCreate();
    if(buttonEventGroup == NULL)
    {
        ESP_LOGE(TAG_BUTTON,"Falha ao criar o EventGroup Button");
    }

    gpio_config_t io_conf;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = 1ULL<<BUTTON_PIN;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.intr_type = GPIO_PIN_INTR_NEGEDGE;
    gpio_config(&io_conf);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_IRAM);
    gpio_isr_handler_add(BUTTON_PIN, GPIOISRHandler, (void*) BUTTON_PIN);
}

static void IRAM_ATTR GPIOISRHandler(void* arg)
{
    BaseType_t xHigherPriorityTaskWoken, xResult;

    /* xHigherPriorityTaskWoken must be initialised to pdFALSE. */
    xHigherPriorityTaskWoken = pdFALSE;

    /* Set bit in xEventGroup. */
    xResult = xEventGroupSetBitsFromISR(
        buttonEventGroup,   /* The event group being updated. */
        ISRBIT0, /* The bits being set. */
        &xHigherPriorityTaskWoken
    );


    /* Was the message posted successfully? */
    if( xResult )
    {
        /* If xHigherPriorityTaskWoken is now set to pdTRUE then a context
        switch should be requested.  The macro used is port specific and will
        be either portYIELD_FROM_ISR() or portEND_SWITCHING_ISR() - refer to
        the documentation page for the port being used. */
        portYIELD_FROM_ISR();
    }
}

