/**
 * @file dht.c
 * @author Matheus Arcangelo Esperanca (adaptation of Andrey-m library)
 * @brief DHT22 sensor driver
 * @version 0.1
 * @date 2023-03-05
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "dht.h"

// definitions 
#define TAG_DHT "DHT"

#define DHT_OK              0
#define DHT_CHECKSUM_ERROR  -1
#define DHT_TIMEOUT_ERROR   -2
#define MAXDHTDATA          5       // to complete 40 = 5*8 Bits

static int pinDHT; 

// function prototypes
int   getSignalLevel(int usTimeOut, bool state);

void initDHT(void)
{
    pinDHT = 21; //GPIO21
}

/**
 * @brief Error handler from Andrey-m Github
 * 
 * @param response error code to show
 */
void errorHandlerDHT(int response)
{
    switch (response)
    {
        case DHT_TIMEOUT_ERROR:
            ESP_LOGE(TAG_DHT, "Sensor timeout");
            break;

        case DHT_CHECKSUM_ERROR:
            ESP_LOGE(TAG_DHT, "CheckSum error");
            break;

        case DHT_OK:
            break;

        default:
            ESP_LOGE(TAG_DHT, "Unknown error");
    }
}

/**
 * @brief Get the Signal Level object
 * 
 * @param usTimeOut 
 * @param state 
 * @return int 
 */
int getSignalLevel(int usTimeOut, bool state)
{
    int uSec = 0;
    while (gpio_get_level(pinDHT) == state)
    {
        if (uSec > usTimeOut)
            return -1;

        ++uSec;
        ets_delay_us(1); // uSec delay
    }
    return uSec;
}

/**
 * @brief temperature and humidity measurement function
 * 
 * @param temperature temperature float pointer
 * @param humidity humidity float pointer
 * @return see errorHandlerDHT()
 */
int readDHT(float *temperature, float *humidity)
{
    int uSec = 0;         

    uint8_t dhtData[MAXDHTDATA];
    uint8_t byteInx = 0;
    uint8_t bitInx = 7;

    for (int k = 0; k < MAXDHTDATA; k++)
        dhtData[k] = 0;

    // == Send start signal to DHT sensor ===========

    gpio_set_direction(pinDHT, GPIO_MODE_OUTPUT);

    // pull down for 3 ms for a smooth and nice wake up
    gpio_set_level(pinDHT, 0);
    ets_delay_us(3000);

    // pull up for 25 us for a gentile asking for data
    gpio_set_level(pinDHT, 1);
    ets_delay_us(25);

    gpio_set_direction(pinDHT, GPIO_MODE_INPUT); // change to input mode

    // == DHT will keep the line low for 80 us and then high for 80us ====

    uSec = getSignalLevel(85, 0);
    // ESP_LOGI(TAG, "Response = %d", uSec);
    if (uSec < 0)
        return DHT_TIMEOUT_ERROR;

    // -- 80us up ------------------------

    uSec = getSignalLevel(85, 1);
    // ESP_LOGI(TAG, "Response = %d", uSec);
    if (uSec < 0)
        return DHT_TIMEOUT_ERROR;

    // == No errors, read the 40 data bits ================

    for (int k = 0; k < 40; k++)
    {

        // -- starts new data transmission with >50us low signal

        uSec = getSignalLevel(56, 0);
        if (uSec < 0)
            return DHT_TIMEOUT_ERROR;

        // -- check to see if after >70us rx data is a 0 or a 1

        uSec = getSignalLevel(75, 1);
        if (uSec < 0)
            return DHT_TIMEOUT_ERROR;

        // add the current read to the output data
        // since all dhtData array where set to 0 at the start,
        // only look for "1" (>28us us)

        if (uSec > 40)
        {
            dhtData[byteInx] |= (1 << bitInx);
        }

        // index to next byte

        if (bitInx == 0)
        {
            bitInx = 7;
            ++byteInx;
        }
        else
            bitInx--;
    }

    // == get humidity from Data[0] and Data[1] ==========================

    *humidity = dhtData[0];
    *humidity *= 0x100; // >> 8
    *humidity += dhtData[1];
    *humidity /= 10; // get the decimal

    // == get temp from Data[2] and Data[3]

    *temperature = dhtData[2] & 0x7F;
    *temperature *= 0x100; // >> 8
    *temperature += dhtData[3];
    *temperature /= 10;

    if (dhtData[2] & 0x80) // negative temp, brrr it's freezing
        *temperature *= -1;

    // == verify if checksum is ok ===========================================
    // Checksum is the sum of Data 8 bits masked out 0xFF

    if (dhtData[4] == ((dhtData[0] + dhtData[1] + dhtData[2] + dhtData[3]) & 0xFF))
        return DHT_OK;

    else
        return DHT_CHECKSUM_ERROR;
}